# include "acActionBBVI.h"

//---------------Distribution densities------------------------
double log_normal(double x, double mu, double sd){
  return 0.0 - 0.5 * log(2 * M_PI * sd * sd) - (x - mu) * (x - mu) / (2 * sd * sd);
};

double log_gamma(double x, double shape, double rate){
  return shape * log(rate) - lgamma(shape) + (shape - 1) * log(x) - rate * x;
};

double log_beta(double x, double a, double b){
  return (a - 1) * log(x) + (b - 1) * log(1 - x) + lgamma(a + b) - lgamma(a) - lgamma(b);
};
//------------------Digamma-------------------------------------
double digamma(double x, int& ifault){
  static double c = 8.5;
  static double euler_mascheroni = 0.57721566490153286060;
  double r;
  double value;
  double x2;
  //  Check the input.
  if ( x <= 0.0 )
  {
    value = 0.0;
    ifault = 1;
    return value;
  }
  //  Initialize.
  ifault = 0;
  //  Use approximation for small argument.
  if ( x <= 0.000001 )
  {
    value = - euler_mascheroni - 1.0 / x + 1.6449340668482264365 * x;
    return value;
  }
  //  Reduce to DIGAMA(X + N).
  value = 0.0;
  x2 = x;
  while ( x2 < c )
  {
    value = value - 1.0 / x2;
    x2 = x2 + 1.0;
  }
  //  Use Stirling's (actually de Moivre's) expansion.
  r = 1.0 / x2;
  value = value + log ( x2 ) - 0.5 * r;
  r = r * r;
  value = value 
    - r * ( 1.0 / 12.0 
    - r * ( 1.0 / 120.0 
    - r * ( 1.0 / 252.0 
    - r * ( 1.0 / 240.0
    - r * ( 1.0 / 132.0 ) ) ) ) );
  return value;
};

//------------------Gradients of densities---------------------
void g_log_normal(double x, double mu, double sd, stdVec& grad){
  grad[0] = (x - mu) / (sd * sd);
  grad[1] = (x - mu) * (x - mu) / (sd * sd * sd) - 1.0 / sd;
};

void g_log_gamma(double x, double shape, double rate, stdVec& grad){
  int ifault = 0;
  grad[0] = log(rate) - digamma(shape, ifault) + log(x);
  grad[1] = shape / rate - x;
};

void g_log_beta(double x, double a, double b, stdVec& grad){
  int ifault = 0;
  grad[0] = log(x) - digamma(a, ifault) + digamma(a + b, ifault);
  grad[1] = log(1.0 - x) - digamma(b, ifault) + digamma(a + b, ifault);
};

double acActionBBVI::log_prior(const stdVec& params){
  double result = 0.0;
  // Get Parameter Limits
  stdVec limits;
  model->getParameterLimits(limits);
  // Loop over the parameters
  for(int loopA=0;loopA<model->getParameterTotal();loopA++){
    if((params[loopA] >= limits[loopA*2+1]) || (params[loopA] <= limits[loopA*2])){
      return (0.0 - (double) 10000);   
    }
    result += - log(limits[loopA*2+1] - limits[loopA*2]);
  }
  return result;
}


// Joint Distribution: Prior * Likelihood
double acActionBBVI::log_joint(const stdVec& params){
  stdVec outputs;
  stdIntVec errorCode;
  //double log_joint = log_prior(params) - model->evalModelError(params,outputs,errorCode);
  double log_like = -model->evalModelError(params,outputs,errorCode);
  std::cout << log_like << std::endl;
  double log_joint = log_prior(params) + log_like;
  //printf("k : %f\n",params[0]);
  //printf("log_prior: %f\n",log_prior(params));
  //printf("log_like: %f\n",log_like);
  //getchar();
  return log_joint;
}


// Constructor
acActionBBVI::acActionBBVI(){

}

acActionBBVI::~acActionBBVI(){

}

void acActionBBVI::initParameters(){
  // --- Parameters and variatinal parameters

  // Allocate and set initial guess for the variational parameters
  lam.resize(2*model->getParameterTotal());
  for(int loopA=0;loopA<2*model->getParameterTotal();loopA++){
    lam[loopA] = 1.0;
  }

  //! Vector with parameter distributions: each component can be "normal", "gamma", "beta"
  paramDist.resize(model->getParameterTotal());
  for(int loopA=0;loopA<model->getParameterTotal();loopA++){
    paramDist[loopA] = string("normal");
  }

  // --- Optimization options
  numSaves = 100;
  opt_method = string("adam");

  // Initialization: Do Nothing
  adj.resize(2*model->getParameterTotal());
  a.resize(model->getParameterTotal());
  b.resize(model->getParameterTotal());
  for(int loopA=0;loopA<2*model->getParameterTotal();loopA++){
    adj[loopA] = 1.0;
  }
  for(int loopA=0;loopA<model->getParameterTotal();loopA++){
    a[loopA] = 1.0;
    b[loopA] = 0.0;
  }

  // Allocate record to store variational parameter realization
  record.resize(numSaves);
  for(int loopA=0;loopA<numSaves;loopA++){
    record[loopA].resize(2*model->getParameterTotal());
  }

  // Set Default parameters
  // For total iterations
  totIt = 10000;
  batchSize = 25;

}

int acActionBBVI::go(){
  // totIt: totIt iteration number
  // batchSize: batch size for esimating gradient
  // totParams: number of parameters (double size of lam)
  // lam: vector of parameters
  // paramDist: vector of string, indicating variational families (Normal, Lognormal, Gamma, Beta)
  // Record: trace of the optimizor
  // opt_method: name of the optimizor (adadelta, adam)
  // Get the totIt number of parames in the model
  int totParams = model->getParameterTotal();

  std::default_random_engine generator;
  int recordIP = 0;

  // Adadelta hyperparameter vectors
  stdVec Egsq(2 * totParams,0.0);
  stdVec Edxsq(2 * totParams,0.0);
  stdVec Dx(2 * totParams,0.0);
  
  // Adam hyperparameter vectors
  stdVec m(2 * totParams,0.0);
  stdVec v(2 * totParams,0.0);
  stdVec m_hat(2 * totParams,0.0);
  stdVec v_hat(2 * totParams,0.0);
  
  // Loop on each iteration of the optimizor
  for (long t = 0; t < totIt; t++){
    
    // ff --> f vector in Alg 2 in BBVI paper
    // hh --> h vector in Alg 2 in BBVI paper
    // varf --> variance of f
    // varh --> variance of h
    // cov --> covariance of f and h
    // grad --> gradient estimated
    // ffb --> averaged f vector across batches
    // hhb --> averaged h vector across batches

    stdVec ff(2 * totParams,0.0);
    stdVec hh(2 * totParams,0.0);
    //stdVec varf(2 * totParams,0.0);
    stdVec varh(2 * totParams,0.0);
    stdVec cov(2 * totParams,0.0);
    stdVec grad(2 * totParams,0.0);
    stdVec ffb(2 * totParams,0.0);
    stdVec hhb(2 * totParams,0.0);

    // Loop on batch iteration
    stdVec sample(totParams,0.0);
    stdVec temp(2,0.0);
    double temp1 = 0.0;
    double L = 0.0;
    double lq = 0.0;

    for (int i = 0; i < batchSize; i++){

      // Loop on the dimension of the data
      for (int j = 0; j < totParams; j++){

        // Generate samples w.r.t variational families
        if(paramDist[j] == "normal"){
          sample[j] = -1.0;
          while(sample[j] < 0){
            std::normal_distribution<double> distribution(lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1]);
            sample[j] = distribution(generator) * a[j] + b[j];
          }
        }else if (paramDist[j] == "gamma"){
          std::gamma_distribution<double> distribution(lam[2 * j] / adj[2 * j], 1.0);
          sample[j] = distribution(generator) / (lam[2 * j + 1] / adj[2 * j + 1]) * a[j] + b[j];
        }else if (paramDist[j] == "beta"){
          std::gamma_distribution<double> distribution_B1(lam[2 * j] / adj[2 * j], 1.0);
          std::gamma_distribution<double> distribution_B2(lam[2 * j + 1] / adj[2 * j + 1], 1.0);
          double g1 = distribution_B1(generator);
          double g2 = distribution_B2(generator);
          sample[j] = g1 / (g1 + g2) * a[j] + b[j];
        }else if (paramDist[j] == "const"){
          sample[j] = lam[2 * j];
        }else{
          throw acException("ERROR: Invalid variational family.\n");
        }
      }
      // Calculate the joint probability of seeing the data given the sample
      double lj = log_joint(sample);

      // Loop on dimension of the data again (which is necessary, or lj would be recalculated multiple times)
      for (int j = 0; j < totParams; j++){
        
        // Calculate h vector in Alg 2
        if (paramDist[j] == "normal"){          
          g_log_normal((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1], temp);
          hh[2 * j] = temp[0] / adj[2 * j];
          hh[2 * j + 1] = temp[1] / adj[2 * j + 1];
          temp1 = log_normal((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1]);
        }else if (paramDist[j] == "gamma"){
          g_log_gamma((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1], temp);
          hh[2 * j] = temp[0] / adj[2 * j];
          hh[2 * j + 1] = temp[1] / adj[2 * j + 1];
          temp1 = log_gamma((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1]);
        }else if (paramDist[j] == "beta"){
          g_log_beta((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1], temp);
          hh[2 * j] = temp[0] / adj[2 * j];
          hh[2 * j + 1] = temp[1] / adj[2 * j + 1];
          temp1 = log_gamma((sample[j] - b[j]) / a[j],lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1]);
        }else if (paramDist[j] == "const"){
          varh[2 * j] = 1.0;
          varh[2 * j + 1] = 1.0;
          temp1 = 0.0;
        }else{
          throw acException("ERROR: Invalid variational family.\n");
        }

        // Given h, calculate f in Alg 2
        ff[2 * j] = hh[2 * j] * (lj - temp1);
        ff[2 * j + 1] = hh[2 * j + 1] * (lj - temp1);

        // Sequential version of average
        hhb[2 * j] = (double)i / ((double)i + 1.0) * hhb[2 * j] + hh[2 * j] / ((double)i + 1.0);
        hhb[2 * j + 1] = (double)i / ((double)i + 1.0) * hhb[2 * j + 1] + hh[2 * j + 1] / ((double)i + 1.0);
        ffb[2 * j] = (double)i / ((double)i + 1.0) * ffb[2 * j] + ff[2 * j] / ((double)i + 1.0);
        ffb[2 * j + 1] = (double)i / ((double)i + 1.0) * ffb[2 * j + 1] + ff[2 * j + 1] / ((double)i + 1.0);
        if(i>0){
          // Sequential version of variance of h, f, and covariance.
          varh[2 * j] = (double)i / ((double)i + 1.0) * varh[2 * j] + 1.0 / (double)i * (hh[2 * j] - hhb[2 * j]) * (hh[2 * j] - hhb[2 * j]);
          varh[2 * j + 1] = (double)i / ((double)i + 1.0) * varh[2 * j + 1] + 1.0 / (double)i * (hh[2 * j + 1] - hhb[2 * j + 1]) * (hh[2 * j + 1] - hhb[2 * j + 1]);
          cov[2 * j] = (double)i / ((double)i + 1.0) * cov[2 * j] + 1.0 / (double)i * (hh[2 * j] - hhb[2 * j]) * (ff[2 * j] - ffb[2 * j]);
          cov[2 * j + 1] = (double)i / ((double)i + 1.0) * cov[2 * j + 1] + 1.0 / (double)i * (hh[2 * j + 1] - hhb[2 * j + 1]) * (ff[2 * j + 1] - ffb[2 * j + 1]);
        }
        lq = lq + temp1;
      }
      L = (double) i / ((double) i + 1.0) * L  + (lj - lq) / ((double) i + 1.0);
    }

    // Calculate gradient as shown in Alg 2
    for (int j = 0; j < 2 * totParams; j++){
      grad[j] = ffb[j] - cov[j] / varh[j] * hhb[j];
    }

    // Given gradient, use adadelta or adam to search for next Parameters of Variational Family.
    if (opt_method == "adadelta"){
      for (int i = 0; i < 2 * totParams; i++){
        Egsq[i] = ggamma * Egsq[i] + (1.0 - ggamma) * grad[i] * grad[i];
        Dx[i] = sqrt((Edxsq[i] + eps) / (Egsq[i] + eps)) * grad[i];
        Edxsq[i] = ggamma * Edxsq[i] + (1.0 - ggamma) * Dx[i] * Dx[i];
        lam[i] += Dx[i];
      }
    }else if (opt_method == "adam"){
      for (int i = 0; i < 2 * totParams; i++){
        m[i] = beta1 * m[i] + (1 - beta1) * grad[i];
        v[i] = beta2 * v[i] + (1 - beta2) * grad[i] * grad[i];
        m_hat[i] = m[i] / (1 - pow(beta1, t + 1));
        v_hat[i] = v[i] / (1 - pow(beta2, t + 1));
        lam[i] += eta * m_hat[i] / (sqrt(v_hat[i]) + eps);
      }
    }else{
      throw acException("ERROR: Invalid optimization algorithm.\n");
    }
    
    // Take a record of the trace
    if((t + 1) % (totIt / 100) == 0){
      std::cout << recordIP << '%' << ' ';
      for (int i = 0; i < 2 * totParams; i++){
        record[recordIP][i] = lam[i];
        std::cout << lam[i] << ' ';
      }
      std::cout << "L= " << L << std::endl;
      recordIP += 1;
    }
  }
  return 0;
}

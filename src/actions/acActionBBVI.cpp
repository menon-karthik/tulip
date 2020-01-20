# include "acActionBBVI.h"

double log_prior(const stdVec& params){
  double result = 0.0
  // Get Parameter Limits
  stdVec limits;
  model.getParameterLimits(limits);
  // Loop over the parameters
  for(int loopA=0;loopA<model.getParameterTotal();loopA++){
    if((params[loopA] >= limits[loopA*2+1]) && (params[loopA] <= limits[loopA*2])){
      return (0.0 - (double) 10000);   
    }
    result += - log(limits[loopA*2+1] - limits[loopA*2]);
  }
  return result;
}


// Joint Distribution: Prior * Likelihood
double log_joint(const stdVec& params){
  stdVec outputs;
  stdIntVec errorCode;
  return log_prior() + model.evalModelError(params,outputs,errorCode);
}


// Constructor
acActionBBVI::acActionBBVI(){

  // --- Parameters and variatinal parameters

  // Allocate and set initial guess for the variational parameters
  lam.resize(2*model.getParameterTotal());
  for(int loopA=0;loopA<2*model.getParameterTotal();loopA++){
  	lam[loopA] = 1.0;
  }
  //! Vector with parameter distributions: each component can be "normal", "gamma", "beta"
  paramDist.resize(model.getParameterTotal());
  for(int loopA=0;loopA<model.getParameterTotal();loopA++){
  	paramDist[loopA] = string("normal");
  }

  // --- Optimization options
  numSaves = 100;
  opt_method = string("adam");

  // Initialization: Do Nothing
  adj.resize(2*model.getParameterTotal());
  a.resize(2*model.getParameterTotal());
  b.resize(2*model.getParameterTotal());
  for(int loopA=0;loopA<2*model.getParameterTotal();loopA++){
  	adj[loopA] = 1.0;
  	a[loopA] = 1.0;
  	b[loopA] = 0.0;
  }

  // Allocate record to store variational parameter realization
  record.resize(numSaves);
  for(int loopA=0;loopA<numSaves;loopA++){
  	record[loopA].resize(2*model.getParameterTotal());
  }

}

acActionBBVI::~acActionBBVI(){

}

int acActionBBVI::go(long totIt, int batchSize){
  // totIt: totIt iteration number
  // batchSize: batch size for esimating gradient
  // totParams: number of parameters (double size of lam)
  // lam: vector of parameters
  // Dist: vector of string, indicating variational families (Normal, Lognormal, Gamma, Beta)
  // Record: trace of the optimizor
  // opt_method: name of the optimizor (adadelta, adam)

  // Get the totIt number of parames in the model
  totParams = model.getParameterTotal();

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
    stdVec varf(2 * totParams,0.0);
    stdVec varh(2 * totParams,0.0);
    stdVec cov(2 * totParams,0.0);
    stdVec grad(2 * totParams,0.0);
    stdVec ffb(2 * totParams,0.0);
    stdVec hhb(2 * totParams,0.0);

    // Loop on batch iteration
    stdVec sample(totParams,0.0);
    for (int i = 0; i < batchSize; i++){

      // Loop on the dimension of the data
      for (int j = 0; j < totParams; j++){

        // Generate samples w.r.t variational families
        if(dist[j] == "normal"){
          sample[j] = -1.0;
          while(sample[j] < 0){
            std::normal_distribution<double> distribution(lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1]);
            sample[j] = distribution(generator) * a[j] + b[j];
          }
        }else if (dist[j] == "gamma"){
          std::gamma_distribution<double> distribution(lam[2 * j] / adj[2 * j], 1.0);
          sample[j] = distribution(generator) / (lam[2 * j + 1] / adj[2 * j + 1]) * a[j] + b[j];
        }else if (dist[j] == "beta"){
          std::gamma_distribution<double> distribution_B1(lam[2 * j] / adj[2 * j], 1.0);
          std::gamma_distribution<double> distribution_B2(lam[2 * j + 1] / adj[2 * j + 1], 1.0);
          double g1 = distribution_B1(generator);
          double g2 = distribution_B2(generator);
          sample[j] = g1 / (g1 + g2) * a[j] + b[j];
        }else{
          throw acException("ERROR: Invalid variational family.\n")
        }
      }

      // Calculate the joint probability of seeing the data given the sample
      double lj = log_joint(data, sample);

      // Loop on dimension of the data again (which is necessary, or lj would be recalculated multiple times)
      for (int j = 0; j < totParams; j++){
        double temp1;

        // Calculate h vector in Alg 2
        if (dist[j] == "Normal"){
          double temp[2];
          g_log_normal((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1], temp);
          hh[2 * j] = temp[0] / adj[2 * j];
          hh[2 * j + 1] = temp[1] / adj[2 * j + 1];
          temp1 = log_normal((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1]);
        }
        else if (dist[j] == "Gamma"){
          double temp[2];
          g_log_gamma((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1], temp);
          hh[2 * j] = temp[0] / adj[2 * j];
          hh[2 * j + 1] = temp[1] / adj[2 * j + 1];
          temp1 = log_gamma((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1]);
        }
        else{
          double temp[2];
          g_log_beta((sample[j] - b[j]) / a[j], lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1], temp);
          hh[2 * j] = temp[0] / adj[2 * j];
          hh[2 * j + 1] = temp[1] / adj[2 * j + 1];
          temp1 = log_gamma((sample[j] - b[j]) / a[j],lam[2 * j] / adj[2 * j], lam[2 * j + 1] / adj[2 * j + 1]);
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
          varf[2 * j] = (double)i / ((double)i + 1.0) * varf[2 * j] + 1.0 / (double)i * (ff[2 * j] - ffb[2 * j]) * (ff[2 * j] - ffb[2 * j]);
          varf[2 * j + 1] = (double)i / ((double)i + 1.0) * varf[2 * j + 1] + 1.0 / (double)i * (ff[2 * j + 1] - ffb[2 * j + 1]) * (ff[2 * j + 1] - ffb[2 * j + 1]);
          cov[2 * j] = (double)i / ((double)i + 1.0) * cov[2 * j] + 1.0 / (double)i * (hh[2 * j] - hhb[2 * j]) * (ff[2 * j] - ffb[2 * j]);
          cov[2 * j + 1] = (double)i / ((double)i + 1.0) * cov[2 * j + 1] + 1.0 / (double)i * (hh[2 * j + 1] - hhb[2 * j + 1]) * (ff[2 * j + 1] - ffb[2 * j + 1]);
        }
      }
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
    }
    else{
      for (int i = 0; i < 2 * totParams; i++){
        m[i] = beta1 * m[i] + (1 - beta1) * grad[i];
        v[i] = beta2 * v[i] + (1 - beta2) * grad[i] * grad[i];
        m_hat[i] = m[i] / (1 - pow(beta1, t + 1));
        v_hat[i] = v[i] / (1 - pow(beta2, t + 1));
        lam[i] += eta * m_hat[i] / (sqrt(v_hat[i]) + eps);
      }
    }
    
    // Take a record of the trace
    if((t + 1) % (totIt / 100) == 0){
      for (int i = 0; i < 2 * totParams; i++){
        record[rec][i] = lam[i];
        std::cout << lam[i] << ' ';
      }
      std::cout << rec << '%' << std::endl;
      rec += 1;
    }
  }
}


}

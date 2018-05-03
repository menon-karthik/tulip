# Imports
import sys
sys.path.insert(0, '../../../py')
import tulipUQ as uq
import numpy as np

if __name__ == "__main__":

  # Write Header
  print('--- Tulip Testing')
  print('One Dimensional Quadrature')
  print()

  # Set Quadrature Order and type of support
  quadOrder = 9
  suppType = uq.kLegendreRange

  # Contruct the three rules
  firstRule  = uq.uq1DQuadratureRule_CC(quadOrder,suppType);
  secondRule = uq.uq1DQuadratureRule_CCDouble(quadOrder,suppType);
  thirdRule  = uq.uq1DQuadratureRule_Regular(quadOrder,suppType);

  # Generate Points and Weights
  firstRule.generatePointsAndWeights();
  secondRule.generatePointsAndWeights();
  thirdRule.generatePointsAndWeights();

  # Get Points and Weights
  point1  = np.array(firstRule.getPoints());
  weight1 = np.array(firstRule.getWeights());
  point2  = np.array(secondRule.getPoints());
  weight2 = np.array(secondRule.getWeights());
  point3  = np.array(thirdRule.getPoints());
  weight3 = np.array(thirdRule.getWeights());

  print('Clenshaw-Curtis quadrature rule on [-1,1]')
  for loopA in range(len(point1)):
    print('%10f %10f' % (point1[loopA],weight1[loopA]))
  print('Weight sum: ',np.sum(weight1))
  print()

  print('Double Clenshaw-Curtis quadrature rule on [-1,1]')
  for loopA in range(len(point2)):
    print('%10f %10f' % (point2[loopA],weight2[loopA]))
  print('Weight sum: ',np.sum(weight2))
  print()

  print('Regular quadrature rule on [-1,1]')
  for loopA in range(len(point3)):
    print('%10f %10f' % (point3[loopA],weight3[loopA]))
  print('Weight sum: ',np.sum(weight3))
  print()



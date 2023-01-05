import sys
sys.path.insert(0, '../../../../bin/py')
import numpy as np
import matplotlib.pyplot as plt
import tulipUQ as uq

sampler = uq.uqGaussianPDF()

test = sampler.sample(0.0, 1.0)
test = sampler.sample(0.0, 1.0)

print(test)
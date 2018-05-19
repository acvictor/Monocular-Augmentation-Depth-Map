# used to read numpy file 
# push output into a text file
# putput requires some reformatting of spaces before it can be pushed

import numpy as np
np.set_printoptions(threshold='nan')
data = np.load('000000_10_disp.npy')
print data.shape
print data

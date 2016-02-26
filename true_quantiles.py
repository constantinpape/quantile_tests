import numpy as np

# generate true quantiles for test_exact.cpp

if __name__ == '__main__':

    arrays = [np.arange(0,10), np.arange(0,11), np.arange(0,1000), # simple
            np.arange(0,1e6), np.array([2.3,4.7,1.8,3.3,6.7,7.8,9.4,17.8,3.5])]
    for x in arrays:
        #print x
        print np.percentile(x,10), np.percentile(x,25), np.percentile(x,50), np.percentile(x,75), np.percentile(x,90)


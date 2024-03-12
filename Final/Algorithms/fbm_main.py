##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## simple fBM computing
## References:
############## https://doi.org/10.1145/358523.358553
##################################################

import io,sys,random,math,numpy
import matplotlib.pyplot as plt
from PIL import Image

m_samples = None

def set_random_seed(seed):
    random.seed(int(seed))

def get_random_float(range):
    rnd = random.random()
    x = ((rnd*2.0)-1.0)*range
    return x  

def subdivide(first,last,ratio,mstd):
    global m_samples
    n = int(last-first)
    if n>1:
        mn = int(math.floor(n/2))
        midpoint = mn+first
        std   = mstd*ratio
        value = ((m_samples[first]+m_samples[last])/2.0)+get_random_float(std)
        m_samples[midpoint]=value
        subdivide(midpoint,midpoint+mn,ratio,std)
        subdivide(midpoint-mn,midpoint,ratio,std)
    
    
def generateFbm(n,h,seed):
    global m_samples
    set_random_seed(seed)
    size = int(math.pow(2,n)+1)
    std = 1.0
    ratio = math.pow(2,-h)*std
    m_samples = [None]*size
    m_samples[0] = get_random_float(std)
    m_samples[size-1] = get_random_float(std)
    subdivide(0,size-1,ratio,std)
    return m_samples

if __name__ == "__main__":
    h08 = generateFbm(10,0.8,10)
    h05 = generateFbm(10,0.5,10)
    h03 = generateFbm(10,0.3,10)
    plt.plot(h03, 'r')
    plt.plot(h05, 'g')
    plt.plot(h08, 'b')
    plt.ylabel('fBM')
    plt.show()
    

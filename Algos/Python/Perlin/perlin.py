##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## Multifractal perlin-noise
## References:
##############
##############
##################################################

import os,sys,math,numpy,random,cv2
import perlin_noise

def _fdiv(a,b):
    return float(float(a)/float(b))

def _float2byte(array):
    img_int        = numpy.zeros(shape=(array.shape[0],array.shape[1]), dtype=numpy.short)
    img_byte       = numpy.zeros(shape=(array.shape[0],array.shape[1]), dtype=numpy.ubyte)
    img_int[:,:]   = array[:,:]*2048
    _max           = img_int.max()
    _min           = img_int.min()
    _base          = (_max-_min)
    img_byte[:,:]  = ((img_int[:,:]-_min)/(_base))*255
    return img_byte

def perlin_noise_gen(w,h,_seed,_octaves,lac):
    img      = numpy.zeros(shape=(w,h), dtype=numpy.float32)
    noise    = perlin_noise.PerlinNoise(seed=_seed,octaves=1)
    for i in range(w):
        for j in range(h):
            val = 0
            for p in range(0,_octaves,1):
                f   = math.pow(lac,p)
                val = val+(math.pow(2,-p) * noise([_fdiv(i*f,w),_fdiv(j*f,h)]))
            img[i,j]=val
    return _float2byte(img)
    

if __name__ == "__main__":
    img = perlin_noise_gen(512,512,128,8,3)
    cv2.imwrite('noise.bmp', img)


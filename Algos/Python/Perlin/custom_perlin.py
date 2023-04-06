import os,sys,math,numpy,random,cv2

def _perlin(x, y, seed=0):
    numpy.random.seed(seed)
    ptable = numpy.arange(256, dtype=int)
    numpy.random.shuffle(ptable)
    ptable = numpy.stack([ptable, ptable]).flatten()
    xi, yi = x.astype(int), y.astype(int)
    xg, yg = x - xi, y - yi
    xf, yf = _fade(xg), _fade(yg)
    n00 = _gradient(ptable[ptable[xi] + yi], xg, yg)
    n01 = _gradient(ptable[ptable[xi] + yi + 1], xg, yg - 1)
    n11 = _gradient(ptable[ptable[xi + 1] + yi + 1], xg - 1, yg - 1)
    n10 = _gradient(ptable[ptable[xi + 1] + yi], xg - 1, yg)
    x1 = _lerp(n00, n10, xf)
    x2 = _lerp(n01, n11, xf)  
    return _lerp(x1, x2, yf)  

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

def _lerp(a, b, x):
    return a+x*(b-a)

def _fade(f):
    return 6 * f**5 - 15 * f**4 + 10 * f**3

def _gradient(c, x, y):
    vectors = numpy.array([[0, 1], [0, -1], [1, 0], [-1, 0]])
    gradient_co = vectors[c % 4]
    return gradient_co[:, :, 0] * x + gradient_co[:, :, 1] * y

def generate_perlin(w,h,space,seed,roll):
    lin_array_w = numpy.linspace(space[0], space[1], w, endpoint=False)
    lin_array_h = numpy.linspace(space[0], space[1], w, endpoint=False)
    x, y = numpy.meshgrid(lin_array_w, lin_array_h)
    ar   = numpy.roll(_perlin(x, y, seed=seed),roll)
    return ar

def perlin_noise_gen(w,h,seed,_octaves,lac,ridge):
    noise_fractal = numpy.zeros(shape=(w,h), dtype=numpy.float32)
    noise = [generate_perlin(w,h,[lac*x,(lac*x)+(lac*x)],seed,lac*lac*x) for x in range(1,_octaves,1)]
    for i in range(w):
        for j in range(h):
            val = 0
            for p in range(len(noise)):
                sample = (math.pow(2,-p) * noise[p][i,j])
                val    = (val+math.fabs(sample)) if ridge==True else (val+sample)
            noise_fractal[i,j]   = val
    return _float2byte(noise_fractal)

if __name__ == "__main__": 
    img = perlin_noise_gen(1024,1024,128,8,6,False)
    cv2.imwrite('noise.bmp', img)

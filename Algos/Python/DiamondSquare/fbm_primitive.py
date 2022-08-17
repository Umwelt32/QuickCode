##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## simple fBM computing
## References:
############## https://doi.org/10.1145/358523.358553
##################################################

import io,sys,random,math,numpy
import matplotlib.pyplot as plt
from PIL import Image,ImageDraw

m_samples = None

def set_random_seed(seed):
    random.seed(int(seed))

def get_random_float(range):
    rnd = random.random()
    x = ((rnd*2.0)-1.0)*range
    return x  
    
def euclideanDist(p1,p2):
    return math.sqrt(math.pow(p1[0]-p2[0],2)+math.pow(p1[1]-p2[1],2))
def delta(p1,p2):
    return math.sqrt(math.pow(p1-p2,2))
    
def subdivide(first,last,ratio,mstd):
    global m_samples
    n = int(last-first)
    if n>1:
        mn = int(math.floor(n/2))
        midpoint = mn+first
        std   = mstd*ratio
        value = (((m_samples[first]+m_samples[last])/2.0)+get_random_float(std))
        m_samples[midpoint]=value
        subdivide(midpoint,midpoint+mn,ratio,std)
        subdivide(midpoint-mn,midpoint,ratio,std)
    
    
def generateFbm(n,h,seed,s,e,scale):
    global m_samples
    set_random_seed(seed)
    size = int(math.pow(2,n)+1)
    std = scale
    ratio = math.pow(2,-h)*std
    m_samples = [None]*size
    m_samples[0] = s
    m_samples[size-1] = e
    subdivide(0,size-1,ratio,std)
    return m_samples
    
def convToPoint(x,y,r,o1,o2,close):
    lst = []
    s1 = len(x)
    s2 = len(y)
    ml = min(s1,s2)
    for c in range(ml):
        lst.append([(x[c]*r)+o1,(y[c]*r)+o2])
    if close:
        lst.append(lst[0])
    return lst
    
def convToFbmPoints(points,h,scale):
    new_points = []
    last_point = None
    for point in points:
        if last_point:
            x2 = int(point[0])
            y2 = int(point[1])
            x1 = int(last_point[0])
            y1 = int(last_point[1])
            dst = euclideanDist(last_point,point)
            dx  = delta(x1,x2)
            dy  = delta(y1,y2)
            nds = int(math.sqrt((int(math.sqrt(dst)))))
            xfs = generateFbm(nds,h,10,x1,x2,(dy/dst)*scale)
            yfs = generateFbm(nds,h,10,y1,y2,(dx/dst)*scale)
            fbms = convToPoint(xfs,yfs,1.0,0,0,False)
            new_points.append(last_point)
            for fbm in fbms:
                new_points.append(fbm)
        last_point = point
    return new_points

def drawPrimitiveFromPoints(points,cx,cy):
    im = Image.new(mode="RGB", size=(cx, cy))
    draw = ImageDraw.Draw(im)
    last_point = None
    for point in points:
        x2 = int(point[0])
        y2 = int(point[1])
        im.putpixel((x2,y2), (255,255,255,255))
        if last_point:
            x1 = int(last_point[0])
            y1 = int(last_point[1])
            draw.line((x1,y1,x2,y2), fill=(255,255,255,255), width=0)
        last_point = point
    return im

if __name__ == "__main__":
    x = numpy.arange(0,2*numpy.pi-1,0.75)   # start,stop,step
    y = numpy.sin(x)
    z = numpy.cos(x)
    circle = convToPoint(y,z,512,1024,1024,True)
    circle1 = convToFbmPoints(circle,0.5,2.5)
    im=drawPrimitiveFromPoints(circle1,2048,2048)
    im.save('out.png')
    im.show()
    h08 = generateFbm(10,0.8,10,5,4,1.0)
    h05 = generateFbm(10,0.5,10,5,4,1.0)
    h03 = generateFbm(10,0.3,10,5,4,1.0)
    plt.plot(h03, 'r')
    plt.plot(h05, 'g')
    plt.plot(h08, 'b')
    plt.ylabel('fBM')
    plt.show()
    
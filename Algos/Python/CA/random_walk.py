##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## Random walk
## References:
##############
##############
##################################################
import os,sys,math,numpy,cv2
m_map=None

def rnd_rand_int(s,e):
    return int(numpy.random.randint(s,e))

def rnd_init(w,h,seed):
    global m_map
    numpy.random.seed(seed)
    m_map = numpy.zeros(shape=(w,h), dtype=numpy.uint8)

def rnd_set_path(x,y,v):
    global m_map
    success=False
    try:
        m_map[x][y] = v
    except:
        success=False
    else:
        success=True
    return success

def rnd_iterate(x,y,n):
    global m_map
    directions = [[1,0],[1,1],[0,1],[-1,0],[-1,-1],[0,-1],[1,-1],[-1,1]]
    position   = [x,y]
    for x in range(n):
        r = directions[rnd_rand_int(0,8)]
        p = [int(position[0]+r[0]),int(position[1]+r[1])]
        s = rnd_set_path(p[0],p[1],1)
        if s==True: position=p

def rnd_save_im(filename):
    global m_map
    m_map[m_map==1]=255
    cv2.imwrite(filename, m_map)
    
if __name__ == "__main__":
    rnd_init(1024,1024,128)
    rnd_iterate(512,512,1024*128)
    rnd_save_im('o1.bmp')
    exit(0x1)

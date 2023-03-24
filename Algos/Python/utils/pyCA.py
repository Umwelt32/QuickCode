
##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## Cellural automata
## References:
##############
##############
##################################################
import os,sys,math,numpy,random,cv2

grid = None

def ca_set_random_seed(val):
    random.seed(val)

def ca_random_int(v1,v2):
    return int(random.randint(v1,v2))

def ca_random_01(r):
    return 1 if (ca_random_int(0,100)>int(r*100)) else 0

def ca_init(seed,r,w,h):
    global grid
    ca_set_random_seed(seed)
    basic_grid = numpy.zeros(shape=(w,h), dtype=[('type', 'i4'), ('nb_value', 'i4'),('fl_value', 'i4')])
    basic_grid[:,:]['type'] = numpy.array([ca_random_01(r if r<1.0 else 1.0) for x in range(h*w)]).reshape(w,h)
    grid = basic_grid

def ca_calc_nb(x,y,m,type):
    global grid
    nb_value=0
    for i in range(x-m,x+m+1):
        for j in range(y-m,y+m+1):
            if i==x and j==y:
                continue
            else:
                try:
                    if grid[i,j]['type']==type: nb_value=nb_value+1
                except:
                    continue
    return nb_value

def ca_recal_nb_values(m):
    global grid
    for x in range(grid.shape[0]):
        for y in range(grid.shape[1]):
            grid[x,y]['nb_value']=ca_calc_nb(x,y,m,1)
            grid[x,y]['fl_value']=ca_calc_nb(x,y,m,0)

def ca_recal_types(T):
    global grid
    for x in range(grid.shape[0]):
        for y in range(grid.shape[1]):
            grid[x,y]['type']=1 if grid[x,y]['nb_value']>=T else 0

def ca_iterate_once(t,m):
    global grid
    ca_recal_nb_values(m)
    ca_recal_types(t)

def ca_iterate(n,t,m):
    global grid
    for x in range(n):
        ca_iterate_once(t,m)

def ca_save_grid_im(filename):
    global grid
    img = numpy.zeros(shape=(grid.shape[0],grid.shape[1]), dtype=numpy.uint8)
    img[:,:]=grid[:,:]['type']*255
    cv2.imwrite(filename, img)

if __name__ == "__main__":
    ca_init(128,0.25,256,256)
    ca_iterate(8,5,1)
    ca_save_grid_im('out.bmp')
    exit(0x1)

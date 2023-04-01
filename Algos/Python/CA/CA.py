
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

p_params = {"seed": 128,"r": 0.75,"w": 256,"h": 256,"i": 16,"t": 16,"m": 2,"file": 'out.bmp'}

def ca_set_random_seed(val):
    random.seed(val)

def ca_random_int(v1,v2):
    return int(random.randint(v1,v2))

def ca_random_01(r):
    return 1 if (ca_random_int(0,100)<=int(r*100)) else 0

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

def ca_recal_rocks(T):
    global grid
    for x in range(grid.shape[0]):
        for y in range(grid.shape[1]):
            grid[x,y]['type']=1 if grid[x,y]['nb_value']>=T else 0

def ca_recal_walls():
    global grid
    for x in range(grid.shape[0]):
        for y in range(grid.shape[1]):
            if grid[x,y]['fl_value']>0 and grid[x,y]['type']==1: grid[x,y]['type']=2

def ca_iterate_once(t,m):
    global grid
    ca_recal_nb_values(m)
    ca_recal_rocks(t)

def ca_subdivide():
    global grid
    new_grid = numpy.copy(grid)
    new_grid.resize(grid.shape[0]*2,grid.shape[1]*2)
    for x in range(grid.shape[0]):
        for y in range(grid.shape[1]):
            new_grid[(2*x)+0,(2*y)+0]=grid[x,y]
            new_grid[(2*x)+0,(2*y)+1]=grid[x,y]
            new_grid[(2*x)+1,(2*y)+0]=grid[x,y]
            new_grid[(2*x)+1,(2*y)+1]=grid[x,y]
    grid=new_grid

def ca_iterate(n,t,m,sub=False,n1=0):
    global grid
    for x in range(n):ca_iterate_once(t,m)
    if sub==True:
        ca_subdivide()
        for x in range(n1):ca_iterate_once(t,m)
    ca_recal_walls()

def ca_save_grid_im(filename):
    global grid
    colors = [0,128,255]
    img = numpy.zeros(shape=(grid.shape[0],grid.shape[1]), dtype=numpy.uint8)
    img[:,:]=grid[:,:]['type']
    img[img==0]=colors[0]
    img[img==1]=colors[1]
    img[img==2]=colors[2]
    cv2.imwrite(filename, img)

def ca_parse_params(arg):
    for a in arg[1:]:
        ap = a.split('=')
        p_params[ap[0]]=ap[1]
        
if __name__ == "__main__":
    ca_parse_params(sys.argv)
    ca_init(int(p_params['seed']),float(p_params['r']),int(p_params['w']),int(p_params['h']))
    ca_iterate(int(p_params['i']),int(p_params['t']),int(p_params['m']),True,4)
    ca_save_grid_im(p_params['file'])
    exit(0x1)

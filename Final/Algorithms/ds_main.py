##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## References:
############## https://en.wikipedia.org/wiki/Diamond-square_algorithm
############## https://doi.org/10.1145/358523.358553
##################################################

import io,sys,random,math,numpy
import matplotlib.pyplot as plt
from PIL import Image

def get_color_key(value,map):
    v = [value,value,value]
    if map != None:
        for x in map:
            if int(x[0])>=v[0]:
                v=[x[1][0],x[1][1],x[1][2]]
                break
    return v
    
def generate_PIL_img(seed,n,h,map):
    set_random_seed(seed)
    data = generateDsMap(n)
    size = len(data)
    do_ds_algo_it(data,h,1.0)
    im = Image.new(mode="RGB", size=(size, size))
    for x in range(size):
        for y in range(size):
            col = data[x][y][2]
            col = (col+1.0)/2.0
            col = col*255
            col = max( col,0 )
            col = min( col,255 )
            col = int(col)
            col = get_color_key(col,map)
            im.putpixel( (x, y), (col[0], col[1], col[2], 255) )
    return im

def generate_PIL_GIF(path,n,h_arr,seed_arr,dur,map):
    im = []
    iter = min(len(h_arr),len(seed_arr))
    for x in range(iter):
        im.append(generate_PIL_img(seed_arr[x],n,h_arr[x],map))
    im[0].save(path,save_all=True, append_images=im[1:], optimize=False, duration=dur, loop=0)

def set_random_seed(seed):
    random.seed(int(seed))

def get_random_float(range):
    rnd = random.random()
    x = ((rnd*2.0)-1.0)*range
    return x

def get_point_value(data,x,y):
    value = None
    lx = len(data)
    if lx > x:
        ly = len(data[x])
        if ly > y:
            value = data[x][y]
    return value

def set_point_value(data,x,y,f_val):
    lx = len(data)
    if lx > x:
        ly = len(data[x])
        if ly > y:
            data[x][y][2]=f_val
    
def generateDsMap(n):
    data = []
    size = int(math.pow(2,n)+1)
    for x in range(size):
        row = []
        for y in range(size):
            element = [x,y,None]
            row.append(element)
        data.append(row)
    data[0][0][2]=get_random_float(1.0)
    data[size-1][0][2]=get_random_float(1.0)
    data[0][size-1][2]=get_random_float(1.0)
    data[size-1][size-1][2]=get_random_float(1.0)
    return data

def calculate_midpoint(data,x,y,n,m_mid_point,ratio,diamond):
    calculated = False
    counter = 0
    f_value = 0.0
    lst = []
    point = get_point_value(data,x,y)
    if point!=None:
        if point[2]==None:
            mid_point = m_mid_point
            if diamond==True:
                lst.append(get_point_value(data, x-mid_point,y-mid_point))
                lst.append(get_point_value(data, x+mid_point,y+mid_point))
                lst.append(get_point_value(data, x-mid_point,y+mid_point))
                lst.append(get_point_value(data, x+mid_point,y-mid_point))
            else:
                lst.append(get_point_value(data, x-mid_point,y))
                lst.append(get_point_value(data, x+mid_point,y))
                lst.append(get_point_value(data, x,y+mid_point))
                lst.append(get_point_value(data, x,y-mid_point))
            for e in lst:
                if e != None:
                    v = e[2]
                    if v != None:
                        f_value = f_value+v
                        counter = counter+1
            if counter>0:
                f_value=float((float(f_value))/float(counter))+get_random_float(ratio)
                set_point_value(data,x,y,f_value)
                calculated=True
    return calculated
        

def do_diamond_step(data,x,y,n,m_mid_point,ratio):
    calculate_midpoint(data,x+m_mid_point,y+m_mid_point,n,m_mid_point,ratio,True)

def do_square_step(data,x,y,n,m_mid_point,ratio):
    calculate_midpoint(data,x,y-m_mid_point,n,m_mid_point,ratio,False)
    calculate_midpoint(data,x,y+m_mid_point,n,m_mid_point,ratio,False)
    calculate_midpoint(data,x-m_mid_point,y,n,m_mid_point,ratio,False)
    calculate_midpoint(data,x+m_mid_point,y,n,m_mid_point,ratio,False)

def do_diamond_square_step(data,x,y,n,m_mid_point,ratio,ds,ss):
    if ds:
        do_diamond_step(data,x,y,n,m_mid_point,ratio)
    if ss:
        do_square_step(data,x+m_mid_point,y+m_mid_point,n,m_mid_point,ratio)

def do_ds_algo_recurse(data,x,y,n,step,ratio,std):
    if n>1:
        m_mid_point = int(n/2)
        new_ratio = ratio*std
        new_step  = step+1
        do_diamond_square_step(data,x,y,n,m_mid_point,ratio,True,False)
        do_ds_algo_recurse(data,x,y,m_mid_point,new_step,new_ratio,std)
        do_ds_algo_recurse(data,x+m_mid_point,y,m_mid_point,new_step,new_ratio,std)
        do_ds_algo_recurse(data,x,y+m_mid_point,m_mid_point,new_step,new_ratio,std)
        do_ds_algo_recurse(data,x+m_mid_point,y+m_mid_point,m_mid_point,new_step,new_ratio,std)
        do_diamond_square_step(data,x,y,n,m_mid_point,ratio,False,True)
        
def do_ds_algo_rec(data,h,scale):
    size = len(data)
    ratio = math.pow(2,-h)
    std = scale*ratio
    do_ds_algo_recurse(data,0,0,size,1,1.0,std)

def do_ds_algo_it(data,h,scale):
    n = len(data)
    ratio = math.pow(2,-h)
    steps = 1
    std = scale*ratio
    while int(n)>1:
        half = int(n/2)
        for i in range(steps):
            for j in range(steps):
                do_diamond_square_step(data,i*n,j*n,n,half,std,True,False)
        for i in range(steps):
            for j in range(steps):
                do_diamond_square_step(data,i*n,j*n,n,half,std,False,True)
        steps=steps*2
        std=std*ratio
        n=half

def generate_output_by_data(data):
    _new = []
    for x1 in data:
        x1l = []
        for x2 in x1:
            val = x2[2]
            if val == None:
                val = 0
            x1l.append(val)
        _new.append(x1l)
    return _new

if __name__ == "__main__":
    set_random_seed(10)
    data = generateDsMap(8)
    do_ds_algo_rec(data,0.6,1.0)
    #do_ds_algo_it(data,0.6,1.0)
    outp = generate_output_by_data(data);
    #generate_PIL_img(11,8,0.5)
    color_map = [[50,[50,50,255]], [75,[100,100,200]], [100,[200,150,50]], [120,[200,150,0]], [160,[0,150,50]], [200,[0,64,10]],[220,[255,255,200]],[240,[255,255,255]]]
    arr_h =    [ 0.3+(x*0.01) for x in range(1, 20) ]
    arr_seed = [ x for x in range(1, 20) ]
    print(str(arr_h))
    print(str(arr_seed))
    generate_PIL_GIF('o.gif',10,arr_h,arr_seed,1000,color_map)
    plt.imshow(outp, cmap='gray')
    plt.show()
    exit(0x00)


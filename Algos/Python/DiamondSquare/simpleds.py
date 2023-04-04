##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## References:
############## https://en.wikipedia.org/wiki/Diamond-square_algorithm
############## https://doi.org/10.1145/358523.358553
##################################################

import os,sys,math,numpy,random,cv2

m_params = {"data": None, "n": 0,"size": 0,"level": 0,"current_half": 0}

def set_random_seed(seed):
    random.seed(int(seed))

def get_random_uniform(range):
    return float(((random.random()*2.0)-1.0)*range)

def set_point_value(pos,v):
    global m_params
    try:
        m_params['data'][pos[0],pos[1]]=float(v)
    except:
        return False
    else:
        return True

def get_point_value(pos):
    global m_params
    value = None
    try:
        value = float(m_params['data'][pos[0],pos[1]])
    except:
        return value
    else:
        return value

def generateDsMap(n):
    global m_params
    m_params['level']        = int(0)
    m_params['n']            = int(n)
    m_params['size']         = int(math.floor(math.pow(2,m_params['n'])+1))
    m_params['current_half'] = int(math.floor(m_params['size']/2))
    m_params['data']         = numpy.zeros(shape=(m_params['size'],m_params['size']), dtype=numpy.float32)
    m_params['data'][0,  0]  = get_random_uniform(1.0)
    m_params['data'][-1, 0]  = get_random_uniform(1.0)
    m_params['data'][0, -1]  = get_random_uniform(1.0)
    m_params['data'][-1,-1]  = get_random_uniform(1.0)

def calculate_mid(x,y,half,level,diamond):
    global m_params
    points_d_xy = [[x-half,y-half],[x+half,y+half],[x-half,y+half],[x+half,y-half]]
    points_s_xy = [[x,y-half],[x,y+half],[x+half,y],[x-half,y]]
    points_xy   = points_d_xy if diamond==True else points_s_xy
    value_list  = [get_point_value(p) for p in points_xy if get_point_value(p)!=None]
    point_value = numpy.average(value_list)+get_random_uniform(math.pow(2,-level))
    set_point_value([x,y],point_value)

def do_iteration():
    global m_params
    while  m_params['current_half'] > 0:
        size   = m_params['size']
        half   = m_params['current_half']
        _2half = int(half*2)
        level = m_params['level']
        m_params['level']=level+1
        print(m_params['current_half'])
        m_params['current_half']=int(math.floor(half/2))
        for i in range(0,size,half):
            for j in range(0,size,half):
                calculate_mid(i+half,j+half,      half,level,True )
                calculate_mid(i+half,j,           half,level,False)
                calculate_mid(i,j+half,           half,level,False)
                calculate_mid(i+_2half,j+half,  half,level,False)
                calculate_mid(i+half,  j+_2half,half,level,False)
                

def _float2byte(array):
    img_int        = numpy.zeros(shape=(array.shape[0],array.shape[1]), dtype=numpy.short)
    img_byte       = numpy.zeros(shape=(array.shape[0],array.shape[1]), dtype=numpy.ubyte)
    img_int[:,:]   = array[:,:]*2048
    _max           = img_int.max()
    _min           = img_int.min()
    _base          = (_max-_min)
    img_byte[:,:]  = ((img_int[:,:]-_min)/(_base))*255
    return img_byte

def save_img(filename):
    global m_params
    cv2.imwrite(filename, _float2byte(m_params['data']))

if __name__ == "__main__":
    set_random_seed(10)
    generateDsMap(8)
    do_iteration()
    save_img('out.bmp')
    exit(0x00)
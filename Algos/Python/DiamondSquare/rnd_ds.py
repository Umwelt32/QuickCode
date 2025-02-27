##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## References:
############## https://en.wikipedia.org/wiki/Diamond-square_algorithm
############## https://doi.org/10.1145/358523.358553
##################################################

import os,sys,math,numpy,cv2

m_params = {"data": None,"size": 0,"h_factor": 0,"current_half": 0,"current_size": 0,"current_ratio": 0}
m_rnd_distribution = None

def _get_rnd_distribution_from_img(path):
    img = cv2.imread(path, 0)
    img = cv2.normalize(img, img, 0, 255, cv2.NORM_MINMAX)
    img_array    = numpy.asarray(img)
    img_array_1D = img_array.reshape((img_array.shape[0]*img_array.shape[1]))
    histogram    = numpy.histogram(img_array_1D,density=True, bins=[x for x in range(0,256)])
    h_p_f        = histogram[0]
    h_p_f        = numpy.append(h_p_f, 0)
    h_values     = histogram[1]
    h_values_f   = numpy.interp(h_values, (h_values.min(), h_values.max()), (-1, 1)).round(8)
    return [h_values_f,h_p_f]
    
def _set_random_seed(seed):
    numpy.random.seed(int(seed))


def _get_random_uniform1(range):
    return float(((numpy.random.random()*2.0)-1.0)*range)

def _get_random_uniform2(range):
    global m_rnd_distribution
    return float(numpy.random.choice(m_rnd_distribution[0], p=m_rnd_distribution[1])*range)

def _get_random_uniform(range):
    global m_rnd_distribution
    if m_rnd_distribution!=None:
        return (_get_random_uniform1(1.0-range)+_get_random_uniform2(range))*range
    else:
        return _get_random_uniform1(range)

def _set_point_value(pos,v):
    global m_params
    try:
        m_params['data'][pos[0],pos[1]]=float(v)
    except:
        return False
    else:
        return True

def _get_point_value(pos):
    global m_params
    value = None
    try:
        value = float(m_params['data'][pos[0],pos[1]])
    except:
        return value
    else:
        return value

def _generateDsMap(n,h):
    global m_params
    m_params['h_factor']      = float(math.pow(2,-h))
    m_params['size']          = int(math.floor(math.pow(2,int(n))+1))
    m_params['current_ratio'] = float(1)
    m_params['current_half']  = int(math.floor(m_params['size']/2))
    m_params['current_size']  = int(m_params['size'])
    m_params['data']          = numpy.zeros(shape=(m_params['size'],m_params['size']), dtype=numpy.float32)
    m_params['data'][0,  0]   = _get_random_uniform(1.0)
    m_params['data'][-1, 0]   = _get_random_uniform(1.0)
    m_params['data'][0, -1]   = _get_random_uniform(1.0)
    m_params['data'][-1,-1]   = _get_random_uniform(1.0)

def _calculate_mid(x,y,half,ratio,diamond):
    global m_params
    points_d_xy = [[x-half,y-half],[x+half,y+half],[x-half,y+half],[x+half,y-half]]
    points_s_xy = [[x,y-half],[x,y+half],[x+half,y],[x-half,y]]
    points_xy   = points_d_xy if diamond==True else points_s_xy
    value_list  = [_get_point_value(p) for p in points_xy if _get_point_value(p)!=None]
    point_value = numpy.average(value_list)+_get_random_uniform(float(ratio))
    _set_point_value([x,y],point_value)

def _do_iteration():
    global m_params
    size  = m_params['size']
    while  m_params['current_half'] > 0:
        half         = m_params['current_half']
        current_size = m_params['current_size']
        ratio        = m_params['current_ratio']
        m_params['current_ratio'] = float(m_params['current_ratio']*m_params['h_factor'])
        m_params['current_size']  = half
        m_params['current_half']  = int(math.floor(half/2))
        for diamond in [True,False]:
            for i in range(0,size,current_size):
                for j in range(0,size,current_size):
                    if diamond:
                        _calculate_mid(i+half,j+half,half,ratio,    True )
                    else:
                        _calculate_mid(i+half,j,half,ratio,         False)
                        _calculate_mid(i,j+half,half,ratio,         False)
                        _calculate_mid(i+(2*half),j+half,half,ratio,False)
                        _calculate_mid(i+half,j+(2*half),half,ratio,False)

def _float2byte(array):
    img_int        = numpy.zeros(shape=(array.shape[0],array.shape[1]), dtype=numpy.short)
    img_byte       = numpy.zeros(shape=(array.shape[0],array.shape[1]), dtype=numpy.ubyte)
    img_int[:,:]   = array[:,:]*2048
    _max           = img_int.max()
    _min           = img_int.min()
    _base          = (_max-_min)
    img_byte[:,:]  = ((img_int[:,:]-_min)/(_base))*255
    return img_byte

def _save_img(filename):
    global m_params
    cv2.imwrite(filename, _float2byte(m_params['data']))

def DS_run(seed,n,h,file):
    _set_random_seed(seed)
    _generateDsMap(n,h)
    _do_iteration()
    _save_img(file)

def DS_run_bmp(input,seed,n,h,file):
    global m_rnd_distribution
    m_rnd_distribution = _get_rnd_distribution_from_img(input)
    DS_run(seed,n,h,file)

if __name__ == "__main__":
    DS_run_bmp('input.bmp',128,10,0.7,'out.bmp')
    exit(0x00)

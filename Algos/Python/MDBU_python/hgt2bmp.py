##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert .hgt file's to .bmp format
## References:
############## https://en.wikipedia.org/wiki/Shuttle_Radar_Topography_Mission
##############
##################################################

import cv2,os,sys,numpy,math

def readHgtFile(path):
    print('Load file: '+str(path))
    binary_data = numpy.fromfile(path,dtype=numpy.int16)
    return binary_data

def replaceInvalidSamples(x,v):
    x[x==(-32768)]=v
    new_data = x
    return new_data

def NormalizeU16(x):
    min_value_u16 = numpy.min(x)
    x=x-min_value_u16
    max_value_16b = numpy.max(x)
    binary_data = (x/max_value_16b)
    binary_data = numpy.clip(binary_data, 0, 1.0)
    return binary_data

def NormalizeData(data):
    binary_data = numpy.array([(x+0x7FFF) for x in data], dtype=numpy.ushort)
    binary_data = NormalizeU16(binary_data)
    return binary_data
    
def convBinToBmp(data,out):
    max_value_16b = numpy.max(data)
    min_value_16b = numpy.min(data)
    n = int(math.sqrt(len(data)))
    print('max_sample: '+str(max_value_16b))
    print('min_sample: '+str(min_value_16b))
    print('n: '+str(n))
    data = replaceInvalidSamples(data,0)
    data = NormalizeData(data)*255
    print('max_sample: '+str(numpy.max(data)))
    print('min_sample: '+str(numpy.min(data)))
    cv_img = data.reshape((n, n,1))
    cv2.imwrite(out, cv_img)

def convHgtToBmp(in_path,ou_path):
    print("[ "+str(in_path)+" ][ "+str(ou_path)+" ] ")
    bin = readHgtFile(in_path)
    convBinToBmp(bin,ou_path)

def convHgtToBmp1(in_path):
    file = in_path
    out = file
    out = out.replace(".","_")
    out = out+'.bmp'
    convHgtToBmp(file,out)
    
def convHgtToBmpRecurse(in_path):
     list = [f for f in os.listdir(in_path) if f.endswith('.hgt')]
     for file in list:
        convHgtToBmp1(file)

if __name__ == "__main__":
    if len(sys.argv)>1:
        convHgtToBmp1(sys.argv[1])
        exit(0x00)
    else:
        print("Missing input argument! - path to .hgt file.")
        cwd = os.getcwd()
        convHgtToBmpRecurse(cwd)
        exit(0xAA)

import cv2,os,sys,numpy,math

def readHgtFile(path):
    print('Load file: '+str(path))
    binary_data = numpy.fromfile(path,dtype=numpy.int16)
    return binary_data

def replaceInvalidSamples(x,v):
    x[x==(-32768)]=v
    new_data = x
    return new_data

def NormalizeData(data):
    binary_data = numpy.array([(x+0x7FFF) for x in data], dtype=numpy.ushort)
    binary_data = (binary_data/0xFFFF)
    binary_data = numpy.clip(binary_data, 0, 1.0)
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
    bin = readHgtFile(in_path)
    convBinToBmp(bin,ou_path)

if __name__ == "__main__":
    if len(sys.argv)>1:
        file = sys.argv[1]
        out = file
        out = out.replace(".","_")
        out = out+'.bmp'
        convHgtToBmp(file,out)
        exit(0x00)
    else:
        print("Missing input argument! - path to .hgt file.")
        exit(0xAA)
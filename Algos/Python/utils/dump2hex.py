
##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## save file data as byte-hex values
## References:
##############
##############
##################################################
import os,sys,math,numpy

def save_file(path,data,nlines):
    idx=0
    f = open(path, "w")
    for c in data:
        f.write(str(c))
        if nlines>0 and idx>=nlines:
            f.write('\n')
            idx=0
        idx=idx+1
    f.close()

def int2hex(v):
    s = str(hex(int(v)))
    if len(s)<4:
        s=s.replace('0x','0x0')
    return str(str(str(s).upper()).replace('X','x'))

def load2hex(path):
    data = numpy.fromfile(str(path),dtype=numpy.ubyte)
    data_list = [str(int2hex(x)) for x in data]
    data_list_comma = ','.join(data_list)
    return data_list_comma
    
if __name__ == "__main__":
    if (len(sys.argv) > 1):
        input_file_path = str(sys.argv[1])
        try:
            save_file(input_file_path.replace('.','_')+'.txt',load2hex(input_file_path),256)
        except:
            print('error: exception occurs!')
            exit(0x1)
        else:
            exit(0x0)
    else:
        print('no input file in script parameter!')
        exit(0x1)

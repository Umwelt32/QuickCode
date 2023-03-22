import os,sys,math,numpy

def save_file(path,data):
    f = open(path, "w")
    for line in data:
        f.write(str(line))
    f.close()
    
def load2hex(path):
    data = numpy.fromfile(str(path),dtype=numpy.ubyte)
    data_list = [str(hex(x)) for x in data]
    data_list_comma = ','.join(data_list)
    return data_list_comma
    
if __name__ == "__main__":
    input_file_path = str(sys.argv[1])
    save_file(input_file_path.replace('.','_')+'.txt',load2hex(input_file_path))
    exit(0x0)

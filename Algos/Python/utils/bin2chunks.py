##################################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## bin2chunks
## References:
##############
##############
##################################################################

import os,sys,math,numpy

def bin2rec_save_file(path,out_dir='./bin_chunks',chunk_size=0x40000):
    result=True
    try:
        print('input file: '+str(path))
        print('output directory: '+str(out_dir))
        file_data   = numpy.fromfile(str(path),dtype=numpy.ubyte)
        output_chunks = _bin2rec_split2chunks(file_data,chunk_size)
        for idx in range(len(output_chunks)): _bin2rec_save_file(out_dir+'/'+str(idx)+'.BIN',output_chunks[idx])
    except:
        print('error occures!')
        result=False
    else:
        print('done!')
        result=True
    return result

def _bin2rec_split2chunks(file_data,chunk_size):
    data_len    = int(len(file_data))
    output_data = [numpy.array(file_data[x:(x+chunk_size)],dtype=numpy.uint8) for x in range(0,data_len,chunk_size)]
    return output_data

def _bin2rec_save_file(path,data):
    print('write: '+str(path))
    f = open(str(path), "wb")
    f.write(data)
    f.close()
    
if __name__ == "__main__":
    if (len(sys.argv) > 1):
        input_file_path = str(sys.argv[1])
        result = bin2rec_save_file(input_file_path,'./bin_chunks',0x40000)
        exit(0x00) if result==True else exit(0x01)
    else:
        print('no input file in script parameter!')
        exit(0x1)

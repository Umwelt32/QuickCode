##################################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## bin2chunks
## References:
##############
##############
##################################################################

import os,sys,math,numpy

def chunks2bin_mergeChunks2Bin(txt_file,chunks_dir,output_file):
    txt_file  = open(txt_file, "r")
    txt_file_data = [line.replace('\n','') for line in txt_file]
    txt_file.close()
    chunk_size    = int(txt_file_data[0],base=16)
    txt_file_size = int(txt_file_data[1],base=16)
    chunk_count   = int(txt_file_data[2],base=16)
    data_poll     = numpy.zeros(txt_file_size,dtype=numpy.ubyte)
    ########################################
    for node in txt_file_data[3:]:
        start_addr=int(node.split(';')[0],base=16)
        file_name =str(node.split(';')[1])
        chunk_data=numpy.fromfile(str(chunks_dir+'/'+file_name),dtype=numpy.ubyte)
        data_poll[start_addr:(start_addr+chunk_size)]=chunk_data
    ########################################
    MEST_BIN_FILE = open(output_file, "wb")
    MEST_BIN_FILE.write(data_poll)
    MEST_BIN_FILE.close()
    return True

if __name__ == "__main__":
    if (len(sys.argv) > 3):
        #result = chunks2bin_mergeChunks2Bin('./bin_chunks/chunks.txt','./bin_chunks','./OUT_MEST.bin')
        result = chunks2bin_mergeChunks2Bin(str(sys.argv[1]),str(sys.argv[2]),str(sys.argv[3]))
        exit(0x00) if result==True else exit(0x01)
    else:
        print('no input file in script parameter!')
        exit(0x1)

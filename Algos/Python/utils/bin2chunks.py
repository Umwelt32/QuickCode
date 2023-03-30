##################################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## bin2chunks
## References:
##############
##############
##################################################################

import os,sys,math,numpy

m_file_size = 0;

def bin2rec_save_file(path,out_dir='./bin_chunks',list_out_file='',chunk_size=0x40000):
    global m_file_size
    result=True
    try:
        print('input file: '+str(path))
        print('output directory: '+str(out_dir))
        list_file     = list_out_file if len(list_out_file)>0 else out_dir+'/'+path.replace('\\','/').split('/')[-1].replace('.','_')+'.txt'
        file_data     = numpy.fromfile(str(path),dtype=numpy.ubyte)
        m_file_size   = len(file_data)
        output_chunks = _bin2rec_split2chunks(file_data,chunk_size)
        _bin2rec_save_chunks(list_file,out_dir,output_chunks,chunk_size)
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

def _bin2rec_save_bin_file(path,data):
    f = open(str(path), "wb")
    f.write(data)
    f.close()

def _bin2rec_save_txt_file(path,data):
    f = open(str(path), "w")
    for line in data: f.write(line+'\n')
    f.close()

def _bin2rec_save_chunks(list_file_name,directory,chunks_data,chunk_size):
    global m_file_size
    files2addr = []
    for idx in range(len(chunks_data)): 
        filename = str(idx)+'.BIN'
        _bin2rec_save_bin_file(directory+'/'+filename,chunks_data[idx])
        files2addr.append(str(hex(idx*chunk_size))+';'+filename)
    files2addr.insert(0,str(hex(len(files2addr))))
    files2addr.insert(0,str(hex(m_file_size)))
    files2addr.insert(0,str(hex(chunk_size)))
    _bin2rec_save_txt_file(list_file_name,files2addr)

if __name__ == "__main__":
    if (len(sys.argv) > 1):
        input_file_path = str(sys.argv[1])
        result = bin2rec_save_file(input_file_path,str(sys.argv[2]),str(sys.argv[3]),0x40000)
        exit(0x00) if result==True else exit(0x01)
    else:
        print('no input file in script parameter!')
        exit(0x1)

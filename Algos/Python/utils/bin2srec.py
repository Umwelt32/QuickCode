import os,sys,math,numpy

def srec_bin2rec_save_file(path,out_file='',addr_offset=0x0000,line_size=32,addr_width=2,file_offset=0):
    result=True
    output_file = str(out_file) if len(str(out_file))>0 else path.replace('.','_')+'.srec'
    print('input file: '+str(path))
    print('output file: '+str(output_file))
    data = srec_bin2rec(path,addr_offset,line_size,addr_width,file_offset)
    _srec_save_file(output_file,data)
    try:
        output_file = str(out_file) if len(str(out_file))>0 else path.replace('.','_')+'.srec'
        print('input file: '+str(path))
        print('output file: '+str(output_file))
        data = srec_bin2rec(path,addr_offset,line_size,addr_width,file_offset)
        _srec_save_file(output_file,data)
    except:
        print('error occures!')
        result=False
    else:
        print('done!')
        result=True
    return result

def srec_bin2rec(path,addr_offset=0x0000,line_size=32,addr_width=2,file_offset=0):
    header_data = 'S00F000068656C6C6F202020202000003C'
    file_data   = numpy.fromfile(str(path),offset=file_offset,dtype=numpy.ubyte)
    line_size   = min(line_size,0xFF)
    line_size   = max(line_size,0x00)
    file_chunks = _srec_split2chunks(file_data,line_size)
    print(file_chunks)
    file_chunks_count = len(file_chunks)
    srec_lines  = [str(_srec_get_srec_reg(addr_width,addr_offset+(idx*line_size),file_chunks[idx])) for idx in range(file_chunks_count)]
    srec_count  = _srec_gen_count_reg(file_chunks_count)
    srec_terminator  = _srec_gen_terminator_reg(addr_width)
    output_data      = [header_data]
    for line in srec_lines: output_data.append(line)
    output_data.append(srec_count)
    output_data.append(srec_terminator)
    return output_data

def _srec_split2chunks(file_data,chunk_size):
    data_len    = len(file_data)
    output_data = numpy.array([0],dtype=numpy.uint8)
    equal_size  = math.floor(math.floor(data_len/chunk_size)*chunk_size)
    try:
        output_data = numpy.split(file_data[0:equal_size], chunk_size)
    except:
        print('error in data handling!')
    else:
        print('data handling OK')
    return output_data

def _srec_save_file(path,data):
    f = open(str(path), "w")
    for line in data: f.write(str(line)+'\n')
    f.close()
    
def _srec_get_srec_reg(addr_width,base_addr,data):
    byte_count      = len(data)
    addr_width      = min(addr_width,0x04)
    addr_width      = max(addr_width,0x02)
    header          = ['XX','XX','S1','S2','S3','XX','XX']
    byte_count_u8   = numpy.uint8(byte_count+addr_width+0x01)
    base_addr_u32   = numpy.uint32(base_addr)
    base_addr_au32  = _srec_int2array(base_addr_u32,addr_width)
    complement_data = numpy.array([byte_count_u8],dtype=numpy.uint8)
    complement_data = numpy.append(complement_data,base_addr_au32)
    complement_data = numpy.append(complement_data,numpy.array(data,dtype=numpy.uint8))
    checksum_u8     = _srec_calc_checksum(complement_data)
    checksum_au8    = numpy.array([checksum_u8],dtype=numpy.uint8)
    complement_data = numpy.append(complement_data,checksum_au8)
    srec_complete   = str(header[addr_width])+''+str(_srec_array2string(complement_data))
    return srec_complete

def _srec_calc_checksum(data_array):
    data_array_au8 = numpy.array(data_array,dtype=numpy.uint8)
    checksum_u16   = data_array_au8.sum(dtype=numpy.uint16)
    checksum_u16   = numpy.uint16(numpy.bitwise_and(checksum_u16,0xFFFF))
    checksum_u8    = numpy.uint8(0xFF - numpy.uint8(numpy.bitwise_and(checksum_u16,0xFF)))
    return checksum_u8
    
def _srec_gen_count_reg(lines_count):
    header = ['S5','S5','S5','S6','S6','S6']
    lines_count_u32 = numpy.uint32(lines_count)
    addr_width = 0x02 if lines_count_u32>0xFFFF else 0x03
    byte_count_au8  = numpy.array([numpy.uint8(addr_width+0x01)])
    lines_count_au8 = _srec_int2array(lines_count_u32,addr_width)
    complement_data = numpy.append(byte_count_au8,lines_count_au8)
    checksum_u8     = _srec_calc_checksum(complement_data)
    checksum_au8    = numpy.array([checksum_u8],dtype=numpy.uint8)
    complement_data = numpy.append(complement_data,checksum_au8)
    srec_complete   = str(header[addr_width])+''+str(_srec_array2string(complement_data))
    return srec_complete

def _srec_gen_terminator_reg(addr_width):
    header = ['S9','S9','S9','S8','S7','S7']
    addr_width  = min(addr_width,0x04)
    addr_width  = max(addr_width,0x02)
    addr_width_u32 = numpy.uint32(addr_width)
    terminator = '030000FC'
    srec_complete = str(header[addr_width])+str(terminator)
    return srec_complete

def _srec_array2string(data):
    srec=''
    for x in data:srec=srec+str(_srec_int2hex(x)).replace('0x','')
    return srec

def _srec_get_byte(value,byte):
    value_u32  = numpy.uint32(value)
    return numpy.uint8(numpy.bitwise_and(numpy.right_shift(value_u32,byte*8),0xFF))
    
def _srec_int2array(value,size):
    value_u32  = numpy.uint32(value)
    size = min(size,4)
    size = max(size,1)
    int2array = numpy.array([_srec_get_byte(value,x) for x in range(size)], dtype=numpy.uint8)
    int2array_flip = numpy.flip(int2array)
    return int2array_flip

def _srec_int2hex(v):
    s = str(hex(int(v)))
    if len(s)<4:
        s=s.replace('0x','0x0')
    return str(str(str(s).upper()).replace('X','x'))
    
if __name__ == "__main__":
    if (len(sys.argv) > 1):
        input_file_path = str(sys.argv[1])
        result = srec_bin2rec_save_file(input_file_path,'',0x0000,0x8,0x02,0x00)
        exit(0x00) if result==True else exit(0x01)
    else:
        print('no input file in script parameter!')
        exit(0x1)

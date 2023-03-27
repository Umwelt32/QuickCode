import os,sys,math,numpy

m_file_data = None

def srec_load_data(path):
    global m_file_data
    m_file_data = numpy.fromfile(str(path),dtype=numpy.ubyte)

def srec_get_srec(addr_width,base_addr,data):
    byte_count      = len(data)
    addr_width      = min(addr_width,4)
    addr_width      = max(addr_width,2)
    header          = ['XX','XX','S1','S2','S3','XX','XX']
    byte_count_u8   = numpy.uint8(byte_count+addr_width+1)
    base_addr_u32   = numpy.uint32(base_addr)
    base_addr_au32  = srec_int2array(base_addr_u32,addr_width)
    checksum_u16    = numpy.uint16(0)
    complement_data = numpy.array([byte_count_u8],dtype=numpy.uint8)
    complement_data = numpy.append(complement_data,base_addr_au32)
    complement_data = numpy.append(complement_data,numpy.array(data,dtype=numpy.uint8))
    for x in complement_data: checksum_u16=checksum_u16+x
    checksum_u16   = numpy.uint16(numpy.bitwise_and(checksum_u16,0xFFFF))
    checksum_u8    = numpy.uint8(0xFF - numpy.uint8(numpy.bitwise_and(checksum_u16,0xFF)))
    checksum_au8   = numpy.array([checksum_u8],dtype=numpy.uint8)
    complement_data = numpy.append(complement_data,checksum_au8)
    srec_complete = str(header[addr_width])+''+str(srec_array2string(complement_data))
    return srec_complete

def srec_array2string(data):
    srec=''
    for x in data:srec=srec+str(srec_int2hex(x)).replace('0x','')
    return srec

def srec_get_byte(value,byte):
    value_u32  = numpy.uint32(value)
    return numpy.uint8(numpy.bitwise_and(numpy.right_shift(value_u32,byte*8),0xFF))
    
def srec_int2array(value,size):
    value_u32  = numpy.uint32(value)
    size = min(size,4)
    size = max(size,1)
    int2array = numpy.array([srec_get_byte(value,x) for x in range(size)], dtype=numpy.uint8)
    int2array_flip = numpy.flip(int2array)
    return int2array_flip

def srec_int2hex(v):
    s = str(hex(int(v)))
    if len(s)<4:
        s=s.replace('0x','0x0')
    return str(str(str(s).upper()).replace('X','x'))
    
if __name__ == "__main__":
    print('=>'+str(srec_get_srec(2,0x01C,[0x4B,0xFF,0xFF,0xE5,0x39,0x80,0x00,0x00,0x7D,0x83,0x63,0x78,0x80,0x01,0x00,0x14,0x38,0x21,0x00,0x10,0x7C,0x08,0x03,0xA6,0x4E,0x80,0x00,0x20])))
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

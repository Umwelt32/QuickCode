##################################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## srec_edit
## References:
############## https://en.wikipedia.org/wiki/SREC_(file_format)
##############
##################################################################

import os,sys,math,numpy

def _srec_decode_record(line):
    p_params = {"S0": 2,"S1": 2,"S2": 3,"S3": 4,"S4": 0,"S5": 2,"S6": 3,"S7": 4,"S8": 3,"S9": 2}
    S          = str(line[0:2])
    Scount     = str(line[2:4])
    record_b   = [int(str(line[x:x+2]),16) for x in range(2,len(line),2)]
    Scount_x   = int(Scount, 16)
    Scount_o   = (p_params[S]*2)+4
    Address_s  = str(line[4:Scount_o])
    Address_x  = int(Address_s,16)
    Address_b  = [int(str(Address_s[x:x+2]),16) for x in range(0,len(Address_s),2)]
    Data_s     = str(line[(Scount_o):-2])
    Data_b     = [int(str(Data_s[x:x+2]),16) for x in range(0,len(Data_s),2)]
    CRC        = int(str(line[-2:]),16)
    CRC_c      = _srec_calc_checksum(record_b[:-1])
    return [S,Scount_x,Address_x,Address_b,Data_b,CRC,CRC_c]

def _srec_calc_checksum(data_array):
    data_array_au8 = numpy.array(data_array,dtype=numpy.uint8)
    checksum_u16   = data_array_au8.sum(dtype=numpy.uint16)
    checksum_u16   = numpy.uint16(numpy.bitwise_and(checksum_u16,0xFFFF))
    checksum_u8    = numpy.uint8(0xFF - numpy.uint8(numpy.bitwise_and(checksum_u16,0xFF)))
    return checksum_u8

def _srec_calc_record_checksum(line):
    record_b = [int(str(line[x:x+2]),16) for x in range(2,len(line),2)]
    return _srec_calc_checksum(record_b[:-1])

def _srec_int2hex(v):
    s = str(hex(int(v)))
    if len(s)<4:
        s=s.replace('0x','0x0')
    return str(str(str(s).upper()).replace('X','x'))
    
def _srec_combine_record(Address_b,Data_b):
    S = ['XX','XX','S1','S2','S3','XX','XX']
    S_s = S[len(Address_b)]
    L_s = _srec_int2hex(len(Address_b)+len(Data_b)+1).replace('0x','')
    A_s = ''.join([_srec_int2hex(x).replace('0x','') for x in Address_b])
    D_s = ''.join([_srec_int2hex(x).replace('0x','') for x in Data_b])
    rec_s = S_s+L_s+A_s+D_s
    Crc_b = _srec_calc_record_checksum(rec_s+'00')
    rec_s = rec_s+_srec_int2hex(Crc_b).replace('0x','')
    return rec_s

def _srec_gen_Header():
    return 'S00600004844521B'

def _srec_gen_terminator_reg(addr_width):
    termination_str = ['','','S9030000FC','S804000000FB','S70500000000FA','']
    addr_width    = min(addr_width,0x04)
    addr_width    = max(addr_width,0x02)
    srec_complete = str(termination_str[addr_width])
    return srec_complete

def _srec_gen_count_reg(lines_count):
    header = ['S5','S5','S5','S6','S6','S6']
    addrs  = [[0,0],[0,0],[0,0],[0,0,0],[0,0,0,0]]
    lines_count_u32 = numpy.uint32(lines_count)
    addr_width      = 0x03 if lines_count_u32>0xFFFF else 0x02
    srec_complete   = _srec_combine_record(addrs[addr_width],[])
    srec_complete   = srec_complete.replace(srec_complete[0:2],header[addr_width])
    return srec_complete

if __name__ == "__main__":
    r = _srec_decode_record('S325800501600D0000024D00E04F02F5DA00CD0FE00F0D00C0041D018453000000000000000007')
    s = _srec_combine_record(r[3],r[4])
    l = _srec_gen_count_reg(0xFFFFF)
    t = _srec_gen_terminator_reg(3)
    print(str(r))
    print(str(s))
    print(str(l))
    print(str(t))
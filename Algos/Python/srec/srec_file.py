##################################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## srec_edit
## References:
############## https://en.wikipedia.org/wiki/SREC_(file_format)
##############
##################################################################

import os,sys,math,numpy
import srec_loader

m_srec_records = None
m_srec_addr_records = None

def _srec_file_load(path):
    global m_srec_records
    global m_srec_addr_records
    m_srec_records = {}
    m_srec_addr_records = []
    file = open(path, "r")
    for f in file:
        r = srec_loader._srec_decode_record(f.replace('\n',''))
        if r[0]=='S1' or r[0]=='S2' or r[0]=='S3':
            m_srec_records[r[2]]=r
            m_srec_addr_records.append(r[2])
    file.close()

def _srec_getRecordByAddr(addr):
    global m_srec_records
    try:
        record = m_srec_records[int(addr)]
    except:
        record = None
    return record

def _srec_setRecordData(addr,data):
    r = _srec_getRecordByAddr(addr)
    if r:
        size = len(r[4])
        size_i = min(size,len(data))
        new_data = [0]*size
        for x in range(size_i):new_data[x]=data[x]
        m_srec_records[int(addr)][4]=new_data
        return True
    else:
        return False

def _srec_write_file(path):
    global m_srec_records
    global m_srec_addr_records
    f = open(path, "w")
    f.write(srec_loader._srec_gen_Header()+'\n')
    for x in m_srec_addr_records:
        r = _srec_getRecordByAddr(x)
        if r: f.write(srec_loader._srec_combine_record(r[3],r[4])+'\n')
    f.write(srec_loader._srec_gen_terminator_reg(4)+'\n')
    f.close()

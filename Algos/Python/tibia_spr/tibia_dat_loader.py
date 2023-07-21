##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert .dat loader
## References:
##############
##############
##################################################

import cv2,os,sys,numpy,math

m_file_ver    = None
m_file_handle = None
m_items_count = None

def load_file(path):
    global m_file_ver
    global m_items_count
    global m_file_handle
    m_file_handle = open(path, "rb")
    m_file_ver    = numpy.fromfile(m_file_handle, dtype=numpy.uint32,count=1)[0] #file_ver - 4bytes
    m_items_count = numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=4)    #objects count - 2*4bytes
    bytes = numpy.fromfile(m_file_handle, dtype=numpy.uint8,count=32)
    print(str(m_items_count))
    print(str(bytes))
    m_file_handle.close()


if __name__ == "__main__":
    load_file('Tibia.dat')
    
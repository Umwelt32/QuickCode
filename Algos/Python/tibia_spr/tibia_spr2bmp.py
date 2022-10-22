##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert .spr file's to .bmp format
## References:
##############
##############
##################################################

import cv2,os,sys,numpy,math

m_file_ver        = None
m_sprites_count   = None
m_sprites_offsets = None

def load_file(path):
    global m_file_ver
    global m_sprites_count
    global m_sprites_offsets
    dt_header = numpy.dtype({'names': ['r','g','b','a'],'formats': [numpy.uint8, numpy.uint8, numpy.uint8, numpy.uint8]})
    f = open(path, "rb")
    m_file_ver      = numpy.fromfile(f, dtype=numpy.uint32,count=1)[0]
    m_sprites_count = numpy.fromfile(f, dtype=numpy.uint16,count=1)[0]
    print("ver: "+str(m_file_ver)+"\nsprites_count: "+str(m_sprites_count))
    m_sprites_offsets = numpy.fromfile(f, dtype=numpy.uint32,count=m_sprites_count)
    f.close()



if __name__ == "__main__":
    load_file('Tibia.spr')
    exit(0)
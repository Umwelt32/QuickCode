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
m_file_handle     = None
m_sprites         = None
m_sprites_data    = None

def load_file(path):
    global m_file_ver
    global m_sprites_count
    global m_sprites_offsets
    global m_file_handle
    global m_sprites
    global m_sprites_data
    m_sprites_data = []
    m_file_handle     = open(path, "rb")
    m_file_ver        = numpy.fromfile(m_file_handle, dtype=numpy.uint32,count=1)[0]
    m_sprites_count   = numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0]
    m_sprites_offsets = numpy.fromfile(m_file_handle, dtype=numpy.uint32,count=m_sprites_count)
    m_sprites         = [_load_sprite(x) for x in range(m_sprites_count)]
    m_file_handle.close()

def _load_sprite(sprite_idx):
    global m_file_handle
    global m_sprites_offsets
    global m_sprites_data
    m_file_handle.seek(m_sprites_offsets[5], 0)
    transparent_key = numpy.fromfile(m_file_handle, dtype=numpy.uint8, count=3)
    sprite_size     = numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0]
    sprite_data     = numpy.fromfile(m_file_handle, dtype=numpy.uint8, count=sprite_size)
    print(str(sprite_size))
    m_sprites_data.append(_sprite_data2img(sprite_data,32,[255,0,255]))

def _sprite_data2img(sprite_data,n,transparent_color):
    raw_data = numpy.zeros(n*n*3,dtype=numpy.uint8)
    offset = 0
    while offset < len(sprite_data):
        pix_offsets         = numpy.frombuffer(sprite_data, dtype = numpy.uint16,count=2,offset=offset)
        color_bytes_to_read = numpy.uint16(pix_offsets[1]*3)
        color_data          = numpy.frombuffer(sprite_data, dtype = numpy.uint8,count=color_bytes_to_read,offset=offset+4)
        raw_data=_memcpy(raw_data,transparent_color*pix_offsets[0],offset)
        offset=offset+color_bytes_to_read+4
        raw_data=_memcpy(raw_data,color_data,offset)
    return raw_data.reshape((n,n,3))

def _memcpy(dst,src,dst_offset):
    for idx in range(len(src)):
        if len(dst) > (dst_offset+idx): dst[(dst_offset+idx)] = numpy.uint8(src[idx])
    return dst

def ex_sprites_to_dir(path):
    global m_sprites_data
    idx=0
    for x in m_sprites_data:
        cv2.imwrite(path+'/'+str(idx)+'.bmp', x)
        idx=idx+1

if __name__ == "__main__":
    load_file('Tibia.spr')
    ex_sprites_to_dir('./out')
    exit(0)

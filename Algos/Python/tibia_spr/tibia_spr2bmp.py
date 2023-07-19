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
    m_file_handle.seek(m_sprites_offsets[sprite_idx], 0)
    transparent_key = numpy.fromfile(m_file_handle, dtype=numpy.uint8, count=3)
    sprite_size     = numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0]
    sprite_data     = numpy.fromfile(m_file_handle, dtype=numpy.uint8, count=sprite_size)
    m_sprites_data.append(_sprite_data2img(sprite_data,32,transparent_key))

def _sprite_data2img(sprite_data,n,transparent_color):
    raw_data = numpy.zeros(n*n*3,dtype=numpy.uint8)
    data_offset= 0
    pixel_byte = 0
    while data_offset < len(sprite_data):
        try:
            pix_offsets         = numpy.frombuffer(sprite_data, dtype = numpy.uint16,count=2,offset=data_offset)
            color_bytes_to_read = numpy.uint16(pix_offsets[1]*3)
            color_data          = numpy.frombuffer(sprite_data, dtype = numpy.uint8,count=color_bytes_to_read,offset=(data_offset+4))
            raw_data=_memcpy(raw_data,transparent_color*pix_offsets[0],pixel_byte)
            pixel_byte=pixel_byte+(pix_offsets[0]*3)
            raw_data=_memcpy(raw_data,color_data,pixel_byte)
            pixel_byte=pixel_byte+color_bytes_to_read
            data_offset=(data_offset+color_bytes_to_read+4)
        except:
            break
    return raw_data.reshape((n,n,3))

def _memcpy(dst,src,dst_offset):
    for idx in range(len(src)):
        if len(dst) > (dst_offset+idx): dst[(dst_offset+idx)] = numpy.uint8(src[idx])
    return dst

def ex_sprites_to_dir(path):
    global m_sprites_data
    idx=0
    for x in m_sprites_data:
        _x = cv2.cvtColor(x, cv2.COLOR_BGR2RGB)
        cv2.imwrite(path+'/'+str(idx)+'.bmp', _x)
        idx=idx+1

def ex_sprites_to_bmp(n,m,h,path):
    global m_sprites_data
    blank_image = numpy.zeros((n*m, n*m, 3), dtype=numpy.uint8)
    idx=0
    file_idx=0
    while idx < len(m_sprites_data):
        for x in range(0,n*m,m*h):
            for y in range(0,n*m,m*h):
                for z in range(h):
                    for w in range(h):
                        if idx < len(m_sprites_data):
                            x1 = x+((z)*m)
                            y1 = y+((w)*m)
                            blank_image[x1:(x1+m),y1:(y1+m)]=m_sprites_data[idx]
                            idx=idx+1
        file_idx=file_idx+1
        cv2.imwrite(path+'/'+str(file_idx)+'.bmp', cv2.cvtColor(blank_image, cv2.COLOR_BGR2RGB))
        blank_image = numpy.zeros((n*m, n*m, 3), dtype=numpy.uint8)

if __name__ == "__main__":
    load_file('Tibia.spr')
    #ex_sprites_to_dir('./out')
    ex_sprites_to_bmp(32,32,1,'./out2')
    exit(0)

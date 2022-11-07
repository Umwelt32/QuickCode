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

def load_file(path):
    global m_file_ver
    global m_sprites_count
    global m_sprites_offsets
    global m_file_handle
    global m_sprites
    m_file_handle     = open(path, "rb")
    m_file_ver        = int(numpy.fromfile(m_file_handle, dtype=numpy.uint32,count=1)[0])
    m_sprites_count   = int(numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0])
    m_sprites_offsets = numpy.fromfile(m_file_handle, dtype=numpy.uint32,count=m_sprites_count)
    m_sprites         = [load_sprite(x) for x in range(m_sprites_count)]
    m_file_handle.close()

def load_sprite(sprite_idx):
    global m_file_handle
    global m_sprites_offsets
    if sprite_idx  != None: m_file_handle.seek(m_sprites_offsets[sprite_idx], 0)
    transparent_key = numpy.fromfile(m_file_handle, dtype=numpy.uint8, count=3)
    sprite_size     = int(numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0])
    t_pixels_number = int(numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0])
    c_pixels_number = int(numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0])
    sprite_data     = numpy.fromfile(m_file_handle, dtype=numpy.uint8, count=sprite_size)
    total_pixels    = int(t_pixels_number+c_pixels_number)
    sprite_n        = int(math.floor(math.sqrt(total_pixels)))
    package = {'transparent_key':transparent_key,'sprite_size':sprite_size,'total_pixels':total_pixels,'sprite_n':sprite_n,'sprite_data':sprite_data}
    return package

def sprite_data(idx):
    global m_sprites
    i=0
    sprite = m_sprites[idx]
    n = sprite['sprite_n']
    data = numpy.array(sprite['sprite_data'])
    blank_image = numpy.zeros((n,n,3), numpy.uint8)
    for y in range(n):
        for x in range(n):
            if len(data)>(i+2):
                pixel = (data[i+2],data[i+1],data[i])
                blank_image[x,y]=pixel
                i=i+3
    return blank_image

def ex_sprites_to_dir(path):
    global m_sprites_count
    [cv2.imwrite(path+'/'+str(x)+'.bmp', sprite_data(x)) for x in range(m_sprites_count)]

if __name__ == "__main__":
    load_file('Tibia.spr')
    ex_sprites_to_dir('./out')
    exit(0)

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
    m_file_ver        = numpy.fromfile(m_file_handle, dtype=numpy.uint32,count=1)[0] #file_ver      - 4bytes
    m_sprites_count   = numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0] #sprites_count - 2bytes
    m_sprites_offsets = numpy.fromfile(m_file_handle, dtype=numpy.uint32,count=m_sprites_count) #sprites_offsets - (sprites_count*4) bytes
    m_sprites         = [_load_sprite(x) for x in range(m_sprites_count)]
    m_file_handle.close()

def _load_sprite(sprite_idx):
    global m_file_handle
    global m_sprites_offsets
    global m_sprites_data
    m_file_handle.seek(m_sprites_offsets[sprite_idx], 0) #set file position to sprite offset
    transparent_key = numpy.fromfile(m_file_handle, dtype=numpy.uint8, count=3)           #read transparent color - 3bytes
    sprite_size     = numpy.fromfile(m_file_handle, dtype=numpy.uint16,count=1)[0]        #read current sprite size
    sprite_data     = numpy.fromfile(m_file_handle, dtype=numpy.uint8, count=sprite_size) #read whole data of current sprite
    m_sprites_data.append(_sprite_data2img(sprite_data,32,transparent_key))

def _sprite_data2img(sprite_data,n,transparent_color):
    raw_data = numpy.zeros(n*n*3,dtype=numpy.uint8)
    data_offset= 0
    pixel_byte_offset = 0
    while data_offset < len(sprite_data): #repeat until current sprite data ends
        try:
            pix_offsets         = numpy.frombuffer(sprite_data, dtype = numpy.uint16,count=2,offset=data_offset) #read num of transparent and color pixels, (2+2)bytes
            color_bytes_to_read = numpy.uint16(pix_offsets[1]*3) #calculate amount of color bytes
            color_data          = numpy.frombuffer(sprite_data, dtype = numpy.uint8,count=color_bytes_to_read,offset=(data_offset+4)) #read color pixels data
            raw_data=_memcpy(raw_data,transparent_color*pix_offsets[0],pixel_byte_offset) #flush transparent pixels data
            pixel_byte_offset=pixel_byte_offset+(pix_offsets[0]*3)     #move over pixel buffer
            raw_data=_memcpy(raw_data,color_data,pixel_byte_offset)    #flush color pixel data
            pixel_byte_offset=pixel_byte_offset+color_bytes_to_read    #move over pixel buffer
            data_offset=(data_offset+color_bytes_to_read+4)            #move over sprite buffer
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

def ex_sprites_to_bmp(n,m,h,file_path):
    global m_sprites_data
    file_path=file_path.replace('\\','/')
    base_filename = file_path.split('/')[-1]
    idx=0
    file_idx=0
    while idx < len(m_sprites_data):
        blank_image = numpy.zeros((n*m, n*m, 3), dtype=numpy.uint8)
        for x in range(0,n*m,m*h):
            for y in range(0,n*m,m*h):
                for z in range(h):
                    for w in range(h):
                        if idx < len(m_sprites_data):
                            x1 = x+((z)*m)
                            y1 = y+((w)*m)
                            blank_image[x1:(x1+m),y1:(y1+m)]=m_sprites_data[idx]
                            idx=idx+1
        cv2.imwrite(file_path.replace(base_filename,str(file_idx)+'_'+base_filename), cv2.cvtColor(blank_image, cv2.COLOR_BGR2RGB))
        file_idx=file_idx+1

if __name__ == "__main__":
    load_file('Tibia.spr')
    #ex_sprites_to_dir('./out')
    ex_sprites_to_bmp(128,32,1,'./output.bmp')
    exit(0)

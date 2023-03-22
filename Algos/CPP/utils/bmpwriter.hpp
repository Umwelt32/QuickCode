      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2023                                         */
   /* bmp file writer.                                        */
  /* References:                                             */
 /* ---------------------------------------                 */
/*---------------------------------------------------------*/
#ifndef __BMP_WRITER__
#define __BMP_WRITER__
#include <stdio.h>
#include <string.h>

#define _TS_MEMSET_(dst,val,size)   {memset(dst,val,size);}
#define _TS_MALLOC_(ptr,size)       {void *data=(void*)malloc(size);void **_dst=(void**)ptr;(*_dst)=data;}
#define _TS_MALLOC_FREE_(ptr,size)  {free(ptr);(void)size;}
#define _TS_MEMCPY_(dst,src,size)   {memcpy(dst,src,size);}

static unsigned char *_BGR_Buffer_p;
static int _BGR_buffer_size_u32;
static int _BMP_W;
static int _BMP_H;

static unsigned char _bmp_header[]=
{
    /*0  1    2    3    4    5    6    7    8    9    10   11   12   13   14   15*/
    0x42,0x4d,0x36,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,/* 0 - 16*/
    0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,/*17 - 32*/
    0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*33 - 48*/
    0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff /*49 - 64*/
};

static void _bmpwriter_write_int(int value,void *buffer,int byte_offset,int data_size);
static int  _bmpwriter_read_int(const void *buffer,int byte_offset,int data_size);

void bmpwriter_init(int w,int h)
{
    _BMP_W=w;
    _BMP_H=h;
    _BGR_buffer_size_u32=(w*h*3);
    _TS_MALLOC_(&_BGR_Buffer_p,_BGR_buffer_size_u32);
}

void bmpwriter_clear(unsigned char gray_scale)
{
    _TS_MEMSET_(&_BGR_Buffer_p[0],gray_scale,_BGR_buffer_size_u32);
}

void bmpwriter_setPixel(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
    int offset;
    if (x<0)return;
    if (y<0)return;
    offset=(_BMP_W*y+x)*3;
    if (offset<_BGR_buffer_size_u32)
    {
        _BGR_Buffer_p[offset+0]=b;
        _BGR_Buffer_p[offset+1]=g;
        _BGR_Buffer_p[offset+2]=r;
    }
}

void bmpwriter_save(const char *path)
{
    FILE* file_handle;
    void *header_p   = (void*)&_bmp_header[0];
    void *bgr_data_p = (void*)&_BGR_Buffer_p[0];
    int bgr_offset=_bmpwriter_read_int(header_p,10,4);          /*bgr_offset_pu32:10bytes*/
    _bmpwriter_write_int(_BGR_buffer_size_u32,header_p,2,4);   /*file_size_pu32:2bytes  */
    _bmpwriter_write_int(_BMP_W,header_p,18,4);               /*bmp_w_p32:      18bytes*/
    _bmpwriter_write_int(_BMP_H,header_p,22,4);              /*bmp_h_p32:      22bytes*/
    file_handle = fopen(path,"wb");
    fwrite(header_p,1,bgr_offset,file_handle);
    fwrite(bgr_data_p,1,_BGR_buffer_size_u32,file_handle);
    fclose(file_handle);
}

void bmpwriter_deinit(void)
{
    _TS_MALLOC_FREE_(_BGR_Buffer_p,_BGR_buffer_size_u32);
}

static void _bmpwriter_write_int(int value,void *buffer,int byte_offset,int data_size)
{
    int pvbuffer[4]={value,0,0,0};
    unsigned char *c_buffer =  (unsigned char*)buffer;
    unsigned char *co_buffer=  (unsigned char*)&c_buffer[byte_offset];
    unsigned char *value_pu8 = (unsigned char*)&pvbuffer[0];
    unsigned char value_new_data[]={value_pu8[0],value_pu8[1],value_pu8[2],value_pu8[3]};
    _TS_MEMCPY_(&co_buffer[0],&value_new_data[0],data_size);
}

static int _bmpwriter_read_int(const void *buffer,int byte_offset,int data_size)
{
    int value[4]={0,0,0,0};
    const unsigned char *c_buffer = (const unsigned char*)buffer;
    const unsigned char *co_buffer= (const unsigned char*)&c_buffer[byte_offset];
    unsigned char value_new_data[]={co_buffer[0],co_buffer[1],co_buffer[2],co_buffer[3]};
    _TS_MEMCPY_(&value[0],&value_new_data[0],data_size);
    return value[0];
}

#endif

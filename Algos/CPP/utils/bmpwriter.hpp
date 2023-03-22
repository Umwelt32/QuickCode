#ifndef __BMP_WRITER__
#define __BMP_WRITER__
#include <stdio.h>
#include <string.h>

#define _TS_MEMSET_(dst,val,size)   {memset(dst,val,size);}
#define _TS_MALLOC_(ptr,size)      {void *data=(void*)malloc(size);void **_dst=(void**)ptr;(*_dst)=data;}
#define _TS_MALLOC_FREE_(ptr,size)  {free(ptr);(void)size;}

typedef unsigned char _uint8;
typedef unsigned int  _uint32;
typedef int  _int32;

static _uint8 *_BGR_Buffer_p;
static _uint32 _BGR_buffer_size_u32;
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
    _uint32 *file_size_pu32 =(_uint32*)&_bmp_header[ 2];    /*file_size_pu32:2bytes  */
    _uint32 *bgr_offset_pu32=(_uint32*)&_bmp_header[10];   /*bgr_offset_pu32:10bytes*/
    _int32  *bmp_w_p32      =( _int32*)&_bmp_header[18];  /*bmp_w_p32:      18bytes*/
    _int32  *bmp_h_p32      =( _int32*)&_bmp_header[22]; /*bmp_h_p32:      22bytes*/
    (*file_size_pu32)=_BGR_buffer_size_u32;
    (*bmp_w_p32)=_BMP_W;
    (*bmp_h_p32)=_BMP_H;
    file_handle = fopen(path,"wb");
    fwrite(&_bmp_header[0],1,(*bgr_offset_pu32),file_handle);
    fwrite(&_BGR_Buffer_p[0],1,_BGR_buffer_size_u32,file_handle);
    fclose(file_handle);
}

void bmpwriter_deinit(void)
{
    _TS_MALLOC_FREE_(_BGR_Buffer_p,_BGR_buffer_size_u32);
}

#endif

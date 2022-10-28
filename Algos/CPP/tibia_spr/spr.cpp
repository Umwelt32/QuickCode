#include "spr.hpp"
#include "bitmap_image.hpp"
#include <cmath>

tibia_spr::tibia_spr()
{
    ;
}

tibia_spr::~tibia_spr()
{
    m_data.clear();
}

void tibia_spr::load_spr(const std::string &path)
{
    load_to_memory(path);
    load_header();
}

U8 *tibia_spr::getSpriteRawData(const U16 &idx,U16 &size)
{
    U32 offset1=0;
    U32 offset2=0;
    U8 *data=nullptr;
    U16 spr_count = (*m_header.spr_count_2B);
    size=0;
    if (spr_count>idx)
    {
        offset1=m_header.spr_offsets_4B[idx];
        offset2=m_header.spr_offsets_4B[idx+1];
        size=(offset2-offset1);
        data=(U8*)&m_data[offset1];
    }
    return data;
}

void tibia_spr::load_to_memory(const std::string &path)
{
    U8 buffer[128];
    m_data.clear();
    std::ifstream f(path,std::ios::binary);
    while(f.good()&&(!f.eof()))
    {
        f.read((char*)&buffer[0],128);
        int read = f.gcount();
        for (int i=0;i<read;++i)m_data.push_back(buffer[i]);
        if(read<=0)break;
    }
    f.close();
}

void tibia_spr::load_header(void)
{
    U8 *m_base_addr         = static_cast<U8*>(&m_data[0]);
    m_header.magic_4B       = (U32*)&m_base_addr[0];
    m_header.spr_count_2B   = (U16*)&m_base_addr[4];
    m_header.spr_offsets_4B = (U32*)&m_base_addr[6];
    m_header.sprites.clear();
    for (U32 i=0u;i<(*m_header.spr_count_2B);++i){load_sprite(i);}
}

tibia_spr_header &tibia_spr::getHeader(void)
{
    return m_header;
}

void tibia_spr::load_sprite(const U16 &idx)
{
    U32 offset;
    U8 *sprite_data;
    U8 *sprite_pointer;
    tibia_sprite s;
    U16 spr_count = (*m_header.spr_count_2B);
    if (spr_count>idx)
    {
        offset                  =  m_header.spr_offsets_4B[idx];
        sprite_data             = ((U8*) &m_data[offset]);
        s.transparent_rgb_3B    = ((U8*)(&sprite_data[0]));
        s.sprite_size_2B        = ((U16*)&sprite_data[3]);
        s.number_of_t_pixels_2B = ((U16*)&sprite_data[5]);
        s.number_of_c_pixels_2B = ((U16*)&sprite_data[7]);
        s.spr_data              = &sprite_data[9];
        m_header.sprites.push_back(s);
    }
}

void tibia_spr::save_spr(const std::string &path,const U16 &idx)
{
    tibia_sprite *s=&(this->getHeader().sprites[idx]);
    U16 t_pixels_count = (*s->number_of_t_pixels_2B);
    U16 c_pixels_count = (*s->number_of_c_pixels_2B);
    U16 sprite_size_bytes = (*s->sprite_size_2B);;
    U16 pixels_count   = (t_pixels_count+c_pixels_count);
    U16 square_size = std::floor(std::sqrt(pixels_count));
    bitmap_image image(square_size,square_size);
    image.set_all_channels(s->transparent_rgb_3B[0], s->transparent_rgb_3B[1], s->transparent_rgb_3B[2]);
    std::cout<<"tpc: "<<t_pixels_count<<std::endl;
    std::cout<<"cpc: "<<c_pixels_count<<std::endl;
    std::cout<<"pc: "<<pixels_count<<std::endl;
    std::cout<<"sq: "<<square_size<<std::endl;
    std::cout<<"ss: "<<sprite_size_bytes<<std::endl;
    std::cout<<"ss/pc: "<<(sprite_size_bytes/pixels_count)<<std::endl;
    for (U16 i=0;i<pixels_count;++i)
    {
        U8 R=s->spr_data[3*i];
        U8 G=s->spr_data[3*i+1];
        U8 B=s->spr_data[3*i+2];
        U16 X = std::floor(i%square_size);
        U16 Y = std::floor(i/square_size);
        image.set_pixel(X,Y,R,G,B);
    }
    image.save_image(path);
}

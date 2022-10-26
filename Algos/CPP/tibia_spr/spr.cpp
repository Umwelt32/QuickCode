#include "spr.hpp"

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
        offset               =  m_header.spr_offsets_4B[idx];
        sprite_data          =  (U8*)&m_data[offset];
        s.transparent_rgb[0] = (*(&sprite_data[0]));
        s.transparent_rgb[1] = (*(&sprite_data[1]));
        s.transparent_rgb[2] = (*(&sprite_data[2]));
        s.width              = (*(&sprite_data[3]));
        s.heigh              = (*(&sprite_data[4]));
        s.spr_data           = &sprite_data[5];
        m_header.sprites.push_back(s);
    }
}

#pragma once
#include "utils.hpp"

typedef struct
{
    U8 transparent_rgb[3];
    U8 width;
    U8 heigh;
    U16 data_size;
    U8 *spr_data;
}tibia_sprite;

typedef struct
{
    U32 *magic_4B;
    U16 *spr_count_2B;
    U32 *spr_offsets_4B;
    std::vector<tibia_sprite> sprites;
}tibia_spr_header;

class tibia_spr
{
public:
    tibia_spr();
    ~tibia_spr();
    void load_spr(const std::string &path);
    U8 *getSpriteRawData(const U16 &idx,U16 &size);
    tibia_spr_header &getHeader(void);
private:
    void load_to_memory(const std::string &path);
    void load_header(void);
    void load_sprite(const U16 &idx);
    tibia_spr_header m_header;
    std::vector<U8> m_data;
};

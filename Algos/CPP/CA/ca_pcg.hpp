#pragma once

#include "defs.hpp"
#include "array2d.hpp"

typedef struct
{
    U8 type;
    U16 neightborhood_value;
    U16 floor_value;
}ca_node_t;

class ca_pcg
{
    public:
        ca_pcg();
        ~ca_pcg();
        void init(const U16 &w,const U16 &h);
        void generate(const U16 &seed,const F32 &r,const U16 &N,const S16 &T  ,const S16 &M);
        void generate_f(const U16 &seed,const F32 &r,const U16 &N,const F32 &M_F,const S16 &M);
        void generate_sub(const U16 &seed,const F32 &r,const U16 &N,const S16 &T,const S16 &M,const U8 &sub,const U8 &n1);
        void generate_sub_f(const U16 &seed,const F32 &r,const U16 &N,const F32 &M_F,const S16 &M,const U8 &sub,const U8 &n1);
        void saveToIo(std::ostream &s);
        void saveToFile(const std::string &path);
        void saveToBmp(const std::string &path);
    private:
        S16 FT_T(const S16 &M, const F32 &M_F);
        void subdivide(void);
        void setSeed(const U16 &seed);
        void iterate_n_epoch(const U16 &N,const S16 &T,const S16 &M);
        void reset(const F32 &r);
        void recalculate_walls(void);
        void recalculate_rocks(const U16 &T);
        void recalculate_nv_for_node(const S32 &x,const S32 &y,const S16 &M);
        void random_set(const F32 &r);
        void recalculate_all_nv_value(const S16 &M);
        void iterate_once(const S16 &T,const S16 &M);
        F32 getUniformRandomValue(void);
        array2d<ca_node_t> m_cells;
};

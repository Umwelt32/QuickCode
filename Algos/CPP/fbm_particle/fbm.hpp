#pragma once
#include "utils.hpp"
#include "array2d.hpp"

namespace algo
{
    class fbm
    {
        public:
            static void generate(std::vector<F32> &out,const U32 &n,const F32 &h,const U32 &seed,const F32 *s1=nullptr,const F32 *s2=nullptr);
            static void fill_array_with_fbm(algo::array2d<F32> &a,const F32 &h,const U32 &seed,const bool &join);
        private:
            static void do_step(std::vector<F32> &out,const U32 &mid_idx,const U32 &half,const F32 &ratio_level);
            static F32 m_step_ratio;
    };
}

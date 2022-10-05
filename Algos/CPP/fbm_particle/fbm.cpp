#include "fbm.hpp"
#include "utils.hpp"
#include "algo_utils.hpp"

namespace algo
{
    F32 fbm::m_step_ratio=0;

    void fbm::generate(std::vector<F32> &out,const U32 &n,const F32 &h,const U32 &seed,const F32 *s1,const F32 *s2)
    {
        U32 size_u32 = (std::pow(2,n)+1);
        U32 half_u32 = std::floor(size_u32/2);
        F32 s1_v = (s1==nullptr)?(utils::randomFloat(1.0f,true)):(*s1);
        F32 s2_v = (s2==nullptr)?(utils::randomFloat(1.0f,true)):(*s2);
        out.clear();
        out.resize(size_u32);
        out[0]=s1_v;
        out[size_u32-1]=s2_v;
        m_step_ratio=std::pow(2,-h);
        utils::setRandomSeed(seed);
        do_step(out,half_u32,half_u32,1.0f);
    }

    void fbm::do_step(std::vector<F32> &out,const U32 &mid_idx,const U32 &half,const F32 &ratio_level)
    {
        if (half<=0)return;
        F32 new_step_ratio = m_step_ratio*ratio_level;
        U32 new_half = std::floor(half/2);
        S32 first_idx = mid_idx-half;
        S32 second_idx = mid_idx+half;
        if ((first_idx<0) || (second_idx<0))return;
        if ((first_idx>=out.size()) || (second_idx>=out.size()))return;
        out[mid_idx]=(((out[first_idx]+out[second_idx])+utils::randomFloat(ratio_level,true))/(F32)2u);
        do_step(out,mid_idx+new_half,new_half,new_step_ratio);
        do_step(out,mid_idx-new_half,new_half,new_step_ratio);
    }

    void fbm::fill_array_with_fbm(algo::array2d<F32> &a,const F32 &h,const U32 &seed,const bool &join)
    {
        U32 n,m,p,s;
        S32 px,py;
        F32 vcc;
        std::vector<F32> x,y,v,w;
        a.getSize(n,m);
        p=utils::getNearPowerOf2(n);
        s=seed;
        fbm::generate(x,p,h,s++);
        fbm::generate(y,p,h,s++);
        fbm::generate(v,p,h,s++);
        fbm::generate(w,p,h,s++);
        utils::normalize_vector(x,false);
        utils::normalize_vector(y,false);
        utils::normalize_vector(v,false);
        utils::normalize_vector(w,false);
        for (U32 j=0;j<y.size();++j)
        {
            S32 xc = std::floor(x[j]*n);
            S32 yc = std::floor(y[j]*m);
            F32 *vc = a.getPtr(xc,yc);
            if (vc==nullptr)continue;
            (*vc)=v[j];
            if (join && j>0)
            {
                utils::draw_line(a,w,xc,yc,px,py);
            }
            px=xc;py=yc;
            vcc=*vc;
        }
    }
}

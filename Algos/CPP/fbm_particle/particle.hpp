#pragma once
#include "utils.hpp"
#include "array2d.hpp"

namespace algo
{
    class river_particle
    {
        public:
            river_particle();
            ~river_particle();
            void init(array2d<F32> *bmp,const F32 &h,const U32 &n,const U32 &seed);
            void setParticle(const U32 &x,const U32 &y,const F32 &a,const S32 &r,const F32 &d);
            void do_steps(const U32 &s);
            void do_steps_until_flows(void);
            void randomize(const F32 &h,const U32 &n,const U32 &seed);
        private:
            void do_step(void);
            void draw_particle(void);
            bool m_on_run;
            F32 m_particle_attitude;
            F32 m_r;
            F32 m_dec;
            S32 m_current_x;
            S32 m_current_y;
            S32 m_start_pos_x;
            S32 m_start_pos_y;
            U32 m_current_idx;
            F32 m_current_fbm_value;
            std::vector<F32> m_fbm;
            array2d<F32> *m_bmp_ptr;

    };
}

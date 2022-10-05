#include "fbm.hpp"
#include "algo_utils.hpp"
#include "particle.hpp"
#define M_PI (3.14f)

namespace algo
{
    river_particle::river_particle()
    {
        ;
    }

    river_particle::~river_particle()
    {
        ;
    }

    void river_particle::init(array2d<F32> *bmp,const F32 &h,const U32 &n,const U32 &seed)
    {
        randomize(h,n,seed);
        m_bmp_ptr=bmp;
    }

    void river_particle::randomize(const F32 &h,const U32 &n,const U32 &seed)
    {
        fbm::generate(m_fbm,n,h,seed);
        algo::utils::normalize_vector(m_fbm,false);
        m_current_idx=0;
    }

    void river_particle::setParticle(const U32 &x,const U32 &y,const F32 &a,const S32 &r,const F32 &d)
    {
        m_current_x=x;
        m_current_y=y;
        m_start_pos_x=x;
        m_start_pos_y=y;
        m_particle_attitude=a;
        m_r=r;
        m_dec=d;
        m_current_fbm_value=0;
        m_on_run=true;
    }

    void river_particle::do_step(void)
    {
        if (m_bmp_ptr==nullptr)return;
        if (m_on_run==false)return;
        F32 vc = m_fbm[m_current_idx%m_fbm.size()];
        m_current_fbm_value=vc;
        F32 rads=vc*2*M_PI;
        F32 r=2.0f;
        S32 step_x=std::floor(r*std::sin(rads));
        S32 step_y=std::floor(r*std::cos(rads));
        draw_particle();
        m_current_x=(m_current_x+step_x);
        m_current_y=(m_current_y+step_y);
        ++m_current_idx;
    }

    void river_particle::draw_particle(void)
    {
        S32 r=m_r;
        for (S32 i=-r;i<r;++i)
        {
            for (S32 j=-r;j<r;++j)
            {
                F32 g = utils::getGaussianDistro(i,j);
                S32 px1=m_current_x+i;
                S32 py1=m_current_y+j;
                F32 *v =(*m_bmp_ptr).getPtr(px1,py1);
                if (v==nullptr){m_on_run=false;continue;}
                F32 particle=m_particle_attitude*g;
                if (particle>1.0f)particle=1.0f;
                if (particle<0)particle=0.0f;
                (*v)=particle;
            }
        }
        m_particle_attitude-=(m_dec*utils::getGaussianDistro(m_current_x-m_start_pos_x,m_current_y-m_start_pos_y));
        if(m_particle_attitude<0){m_particle_attitude=0;m_on_run=false;}
    }

    void river_particle::do_steps_until_flows(void)
    {
        while(m_on_run)
        {
            do_step();
        }
    }

    void river_particle::do_steps(const U32 &s)
    {
        for (U32 i = 0; i <s;++i)do_step();
    }

}

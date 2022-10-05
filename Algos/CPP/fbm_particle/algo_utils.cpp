      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2022                                         */
   /* MDBU algorithm.                                         */
  /* References:                                             */
 /* https://doi.org/10.1145/1294685.1294717                 */
/*---------------------------------------------------------*/
#include "algo_utils.hpp"
#include "bitmap_image.hpp"
#include "fbm.hpp"

#define BYTE_MAX (255u)
#define RGB24_SUM (765u)
#define RAND_RANGE (8096u)
#define HALF_RAND_RANGE (4048u)
#define FI_I_VALUE (1)

namespace algo
{
    static S32 I_exponent = FI_I_VALUE;
    void utils::setRandomSeed(const U32 &seed)
    {
        std::srand(seed);
    }

    F32 utils::randomFloat(const F32 &range,const bool &sign)
    {
        F32 v;
        S32 value = (std::rand()%RAND_RANGE);
        value-=HALF_RAND_RANGE;
        v = ((F32)(value)/(F32)(HALF_RAND_RANGE));
        if (v>1.0f){v=1.0f;}else if(v<-1.0f){v=-1.0f;}else{;}
        if (sign==false)v=std::fabs(v);
        v=v*range;
        return v;
    }

    F32 utils::normalizeF32(const F32 &_min,const F32 &_max,const F32 &v)
    {
        F32 vc;
        F32 factor = (_max-_min);
        vc=((F32)(v-_min))/factor;
        vc=std::fabs(vc);
        if (vc>1.0f)vc=1.0f;
        return vc;
    }

    F32 utils::clamp(const F32 &_min,const F32 &_max,const F32 &p)
    {
        F32 diff=(_max-_min);
        F32 clamp = (diff*p);
        clamp+=_min;
        return clamp;
    }

    F32 utils::euclideanDistance(const F32 &x1,const F32 &y1,const F32 &x2,const F32 &y2)
    {
        F32 dist = std::pow((x1-x2),2)+std::pow((y1-y2),2);
        dist = std::sqrt(dist);
        return std::fabs(dist);
    }

    F32 utils::dBU(const F32 &e,const F32 &d,const F32 &dmax)
    {
        S32 I = I_exponent;
        F32 dd = (d/dmax);
        F32 v1 = (1.0f-std::pow((1.0f-dd),std::fabs(I)));
        F32 v2 = 1-(dFi(I)*v1);
        F32 v3 = e*(v2);
        return v3;
    }

    F32 utils::dFi(const S32 &i)
    {
        return (i>=0)?1.0f:-1.0f;
    }

    void utils::loadBmpFile(array2d<F32> &out,const std::string &path)
    {
        bitmap_image image(path);
        out.clear();
        if (!image)return;
        const U32 height = image.height();
        const U32 width  = image.width();
        out.init(width,height);
        for (std::size_t y = 0; y < height; ++y)
        {
            for (std::size_t x = 0; x < width; ++x)
            {
                F32 v;
                rgb_t colour;
                image.get_pixel(x, y, colour);
                v=colour.red+colour.green+colour.blue;
                v=(F32)v/((F32)(RGB24_SUM));
                out.get(x,y)=v;
            }
        }
    }

    void utils::saveBmpFile(array2d<F32> &in,const std::string &path)
    {
        U32 w,h;
        in.getSize(w,h);
        bitmap_image image(w,h);
        image.set_all_channels(0, 0, 0);

        for (U32 i=0;i<w;++i)
        {
            for(U32 j=0;j<h;++j)
            {
                U32 tv = (U32)(in.get(i,j)*BYTE_MAX);
                U8 v=std::min(tv,BYTE_MAX);
                image.set_pixel(i,j,v,v,v);
            }
        }
        image.save_image(path);
    }

    void utils::saveTxtFile(array2d<F32> &in,const std::string &path)
    {
        U32 w,h;
        std::ofstream f(path);
        if(f.bad())return;
        in.getSize(w,h);
        for (U32 i=0;i<w;++i)
        {
            for(U32 j=0;j<h;++j)
            {
                f<<std::fixed<<std::setprecision(5)<<in.get(i,j)<<" ";
            }
            f<<std::endl;
        }
        f.close();
    }

    U32 utils::getNearPowerOf2(const U32 &v)
    {
        U32 power=0;
        U32 value=1;
        while(value<v)
        {
            value*=2;
            if (value>=v)break;
            ++power;
        }
        return power;
    }

    void utils::getMinMaxArrayElement(array2d<F32> &in,F32 &_min,F32 &_max)
    {
        if (in.getElementsCount()>0)
        {
            F32 min = in.get(0,0);
            F32 max = min;
            for (U32 i=0;i<in.getElementsCount();++i)
            {
                F32 v=(*in.getElementPtr(i));
                if (min>v)min=v;
                if (max<v)max=v;
            }
            _max=max;
            _min=min;
        }
    }

    void utils::normalizeArray(array2d<F32> &in)
    {
        F32 min,max;
        utils::getMinMaxArrayElement(in,min,max);
        for (U32 i=0;i<in.getElementsCount();++i)
        {
            F32 *v=in.getElementPtr(i);
            (*v)=normalizeF32(min,max,*v);
        }
    }

    void utils::setIConst(const S32 &v)
    {
        I_exponent=v;
    }

    void utils::print_vector(std::vector<F32> &ic)
    {
        std::cout<<"\n--------------------------------------------------------\n";
        for (U32 i=0;i<ic.size();++i){std::cout<<(F32)ic[i]<<", ";}
        std::cout<<"\n--------------------------------------------------------\n";
    }

    void utils::normalize_vector(std::vector<F32> &ic,const bool &sign)
    {
        F32 _min=ic[0];
        F32 _max=_min;
        for (U32 i=0;i<ic.size();++i){if(ic[i]<_min)_min=ic[i];if(ic[i]>_max)_max=ic[i];}
        for (U32 i=0;i<ic.size();++i){ic[i]=normalizeF32(_min,_max,ic[i]);}
        if(sign)
        {
            for (U32 i=0;i<ic.size();++i){ic[i]=(ic[i]-0.5f)/0.5f;}
        }
    }

    void utils::draw_line(array2d<F32> &in,std::vector<F32> &f,const S32 &x,const S32 &y,const S32 &x1,const S32 &y1)
    {
        S32 cx=x;
        S32 cy=y;
        static int counter;
        while (cx!=x1 || cy!=y1)
        {
            if(cx>x1)--cx;else if(cx<x1)++cx;
            if(cy>y1)--cy;else if(cy<y1)++cy;
            if (in.getPtr(cx,cy)==nullptr)continue;
            (*in.getPtr(cx,cy))=f[counter++%f.size()];
        }
    }

    F32 utils::getGaussianDistro(const S32 &center_x,const S32 &center_y)
    {
        U32 dist = std::floor(euclideanDistance(0,0,center_x,center_y))+1u;
        F32 c = ((F32)1.0f)/((F32)(dist));
        return c;
    }

}

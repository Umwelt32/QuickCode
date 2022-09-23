      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2022                                         */
   /* MDBU algorithm.                                         */
  /* References:                                             */
 /* https://doi.org/10.1145/1294685.1294717                 */
/*---------------------------------------------------------*/
#include "ds_mdbu.hpp"
#include "algo_utils.hpp"

namespace algo
{
    ds_mdbu::ds_mdbu()
    {
        clear();
    }

    ds_mdbu::ds_mdbu(const U32 &n)
    {
        clear();
        init(n);
    }

    ds_mdbu::~ds_mdbu()
    {
        clear();
    }

    void ds_mdbu::clear(void)
    {
        m_data.clear();
        m_data_value.clear();
    }

    void ds_mdbu::init(const U32 &n)
    {
        U32 size_n = (U32)(std::pow(2,n)+1);
        m_data.init(size_n,size_n);
        m_data_value.init(size_n,size_n);
        for (U32 i=0;i<size_n;++i){for (U32 j=0;j<size_n;++j){m_data.get(i,j).setXY(i,j);}}
    }

    void ds_mdbu::set_midpoint(ds_point *point,ds_point *parent1,ds_point *parent2,ds_point *parent3,ds_point *parent4,const U32 &level,const F32 &ratio)
    {
        if (point==nullptr)return;
        ds_point *points[] = {parent1,parent2,parent3,parent4};
        m_point_order.push_back(point);
        point->setNestedLevel(level);
        point->setRatio(ratio);
        for (U32 i=0;i<4;++i)
        {
            if (points[i]==nullptr)continue;
            points[i]->addChild(point);
            point->addParent(points[i]);
        }
    }

    void ds_mdbu::getMidpointParentList(std::vector<ds_point*> &points,ds_point *point,const S32 &half,const bool &diamond)
    {
        S32 mid_x;
        S32 mid_y;
        points.clear();
        points.resize(4,nullptr);
        if (point==nullptr)return;
        mid_x = point->getX();
        mid_y = point->getY();
        if (diamond)
        {
            points[0]=m_data.getPtr(mid_x-half,mid_y-half);
            points[1]=m_data.getPtr(mid_x+half,mid_y-half);
            points[2]=m_data.getPtr(mid_x-half,mid_y+half);
            points[3]=m_data.getPtr(mid_x+half,mid_y+half);
        }
        else
        {
            points[0]=m_data.getPtr(mid_x-half,mid_y);
            points[1]=m_data.getPtr(mid_x+half,mid_y);
            points[2]=m_data.getPtr(mid_x,mid_y-half);
            points[3]=m_data.getPtr(mid_x,mid_y+half);
        }
    }

    void ds_mdbu::calculate_midpoint(ds_point *point,const S32 &half,const bool &diamond,const U32 &level,const F32 &ratio)
    {
        std::vector<ds_point*> points;
        if (point==nullptr)return;
        getMidpointParentList(points,point,half,diamond);
        set_midpoint(point,points[0],points[1],points[2],points[3],level,ratio);
    }

    void ds_mdbu::do_midpoint(const S32 &mid_x,const S32 &mid_y,const S32 &half,const bool &diamond,const U32 &level,const F32 &ratio)
    {
        ds_point *point=m_data.getPtr(mid_x,mid_y);
        if (point==nullptr)return;
        if (diamond)
        {
            calculate_midpoint(point,half,diamond,level,ratio);
        }
        else
        {
            std::vector<ds_point*> points;
            getMidpointParentList(points,point,half,diamond);
            for (U32 i = 0;i<points.size();++i)calculate_midpoint(points[i],half,diamond,level,ratio);
        }
    }

    void ds_mdbu::do_ds_iteration(void)
    {
        U32 n;
        U32 half;
        U32 level = 0;
        F32 ratio = 1.0;
        m_data.getSize(n,n);
        U32 step=n;
        half=std::floor(n/2);

        do
        {
            for (U32 diamond=0u;diamond<2u;++diamond)
            {
                for (U32 i = half;i<n;i+=step)
                {
                    for (U32 j = half;j<n;j+=step)
                    {
                        do_midpoint(i,j,half,(diamond==0)?true:false,level,ratio);
                    }
                }
            }
            ++level;
            ratio=ratio*m_prescaler;
            step=half;
            half=std::floor(half/2);
        }while(half>0);
    }

    void ds_mdbu::do_ds_recurse(void)
    {
        U32 n;
        U32 half;
        m_data.getSize(n,n);
        half=std::floor(n/2);
        do_recals_recurse(half,half,n,half,0,(1.0f*m_prescaler));
    }

    void ds_mdbu::do_recals_recurse(const S32 &mid_x,const S32 &mid_y,const U32 &n,const U32 &half,const U32 &level,const F32 &ratio)
    {
        if (half==0)return;
        U32 new_half = std::floor(half/2);
        do_midpoint(mid_x,mid_y,half,true,level,ratio);
        do_midpoint(mid_x,mid_y,half,false,level,ratio);
        if (new_half==0)return;
        F32 nratio=ratio*m_prescaler;
        U32 nlevel = level+1;
        do_recals_recurse(mid_x+new_half,mid_y+new_half,half,new_half,nlevel,nratio);
        do_recals_recurse(mid_x-new_half,mid_y+new_half,half,new_half,nlevel,nratio);
        do_recals_recurse(mid_x-new_half,mid_y-new_half,half,new_half,nlevel,nratio);
        do_recals_recurse(mid_x+new_half,mid_y-new_half,half,new_half,nlevel,nratio);
    }

    void ds_mdbu::do_ds(const U32 &seed,const bool &mdbu,const bool &recurse,const F32 &h,array2d<F32> *in,array2d<F32> *e,array2d<F32> *out)
    {
        utils::setRandomSeed(seed);
        m_prescaler=std::pow(2,-h);
        m_point_order.clear();
        if(recurse){do_ds_recurse();}else{do_ds_iteration();}
        copyToDataValue();
        if(in!=NULL_PTR){in->copyFrom(getPoints());}
        if(mdbu)
        {
            do_mdbu();
            copyToDataValue();
            if(e!=NULL_PTR){e->copyFrom(getPoints());}
        }
        do_ds_calc();
        copyToDataValue();
        if(out!=NULL_PTR){out->copyFrom(getPoints());}
    }

    void ds_mdbu::do_mdbu(void)
    {
        U32 n,m;
        F32 dmax;
        std::map<ds_point*, std::vector<ds_point*>> T;
        std::queue<ds_point*> FQ;
        m_data.getSize(n,m);
        dmax = utils::euclideanDistance(0,0,n,m);
        for (U32 i=0;i<n;++i){for (U32 j=0;j<m;++j){if(m_data.getPtr(i,j)->isValid()){FQ.push(m_data.getPtr(i,j));}}}
        while(FQ.size()>0)
        {
            ds_point *E = FQ.front();
            FQ.pop();
            for(U32 i=0;(E->getParent(i)!=nullptr);++i)
            {
                ds_point *A=E->getParent(i);
                if(A->isValid()==false)
                {
                    if (E->isValid())
                    {
                        T[A].push_back(E);
                    }
                }
            }

            for (auto A = T.begin(); A != T.end();)
            {
                F32 e = 0;
                U32 n = 0;
                for (U32 i=0;A->second.size()>i;++i)
                {
                    ds_point *C = A->second[i];
                    e=e+utils::dBU(C->getValue(),utils::euclideanDistance(A->first->getX(),A->first->getY(),C->getX(),C->getY()),dmax);
                    ++n;
                }
                A->first->setValue(e/n);
                FQ.push(A->first);
                A = T.erase(A);
            }
        }
    }

    void ds_mdbu::setPoint(const S32 &x,const S32 &y,const F32 &v)
    {
        if (m_data.getPtr(x,y)!=nullptr)
        {
            m_data.getPtr(x,y)->setValue(v);
        }
    }

    void ds_mdbu::do_ds_calc(void)
    {
        for (U32 i=0; i < m_point_order.size();++i){m_point_order[i]->calculate_by_parents(true);}
    }

    void ds_mdbu::setInitialPoints(array2d<F32> &a,const F32 &th)
    {
        U32 w,h,w2,h2;
        a.getSize(w,h);
        U32 n = utils::getNearPowerOf2((w+h)/2);
        m_data.clear();
        init(n);
        m_data.getSize(w2,h2);
        for (U32 i=0;(i<w)&&(i<w2);++i)for (U32 j=0;(j<w)&&(j<h2);++j)
        {
            F32 v =a.get(i,j);
            if (v>=th)setPoint(i,j,v);
        }
    }

    void ds_mdbu::copyToDataValue(void)
    {
        U32 w,h;
        m_data.getSize(w,h);
        for (U32 i=0;(i<w);++i)for (U32 j=0;(j<w);++j){m_data_value.get(i,j)=m_data.get(i,j).getValue();}
    }

    array2d<F32> &ds_mdbu::getPoints(void)
    {
        return m_data_value;
    }
}

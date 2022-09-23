      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2022                                         */
   /* MDBU algorithm.                                         */
  /* References:                                             */
 /* https://doi.org/10.1145/1294685.1294717                 */
/*---------------------------------------------------------*/
#include "ds_point.hpp"
#include "algo_utils.hpp"


namespace algo
{
    ds_point::ds_point()
    {
        clear();
    }

    ds_point::~ds_point()
    {
        clear();
    }

    void ds_point::clear(void)
    {
        m_value=0;
        m_isValid=false;
        m_childs.clear();
        m_parent.clear();
    }

    const F32 &ds_point::getValue(void) const
    {
        return m_value;
    }

    void ds_point::setValue(const F32 &v)
    {
        m_isValid=true;
        m_value=v;
    }

    const bool &ds_point::isValid(void) const
    {
        return m_isValid;
    }

    void ds_point::addParent(const ds_point *point)
    {
        if (point!=NULL_PTR){m_parent.push_back(point);}
    }

    void ds_point::addChild(const ds_point *point)
    {
        if (point!=NULL_PTR){m_childs.push_back(point);}
    }

    ds_point* ds_point::getParent(const U32 &idx)
    {
        ds_point* p = nullptr;
        if(m_parent.size()>idx)p=(ds_point*)m_parent[idx];
        return p;
    }

    ds_point* ds_point::getChild(const U32 &idx)
    {
        ds_point* p = nullptr;
        if(m_childs.size()>idx)p=(ds_point*)m_parent[idx];
        return p;
    }

    void ds_point::setXY(const U32 &x,const U32 &y)
    {
        m_x=x;
        m_y=y;
    }

    const U32 &ds_point::getX(void)const
    {
        return m_x;
    }

    const U32 &ds_point::getY(void)const
    {
        return m_y;
    }

    void ds_point::setNestedLevel(const U32 &n)
    {
        m_level=n;
    }

    const U32 &ds_point::getNestedLevel(void)const
    {
        return m_level;
    }

    void ds_point::calculate_by_parents(const bool &only_invalid)
    {
        U32 num=m_parent.size();
        F32 value = 0;
        if (((isValid()==false)||(only_invalid==false))&&(num>0))
        {
            for (U32 i = 0;i<num;++i){value+=m_parent[i]->getValue();}
            value=((F32)value/(F32)num);
            value+=utils::randomFloat(getRatio(),true);
            setValue(value);
        }
    }

    void ds_point::setRatio(const F32 &n)
    {
        m_ratio=n;
    }

    const F32 &ds_point::getRatio(void)const
    {
        return m_ratio;
    }

}

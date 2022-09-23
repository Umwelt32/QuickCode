      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2022                                         */
   /* MDBU algorithm.                                         */
  /* References:                                             */
 /* https://doi.org/10.1145/1294685.1294717                 */
/*---------------------------------------------------------*/
#pragma once
#include "utils.hpp"

namespace algo
{
    class ds_point
    {
        public:
            ds_point();
            ~ds_point();
            void clear(void);
            const F32 &getValue(void) const;
            const bool &isValid(void) const;
            void setValue(const F32 &v);
            void addParent(const ds_point *point);
            void addChild(const ds_point *point);
            void setXY(const U32 &x,const U32 &y);
            void setNestedLevel(const U32 &n);
            void setRatio(const F32 &n);
            const F32 &getRatio(void)const;
            const U32 &getNestedLevel(void)const;
            const U32 &getX(void)const;
            const U32 &getY(void)const;
            ds_point* getParent(const U32 &idx);
            ds_point* getChild(const U32 &idx);
            void calculate_by_parents(const bool &only_invalid);

        private:
            U32 m_x;
            U32 m_y;
            U32 m_level;
            F32 m_ratio;
            F32 m_value;
            bool m_isValid;
            std::vector<const ds_point*> m_childs;
            std::vector<const ds_point*> m_parent;
    };
}

      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2022                                         */
   /* MDBU algorithm.                                         */
  /* References:                                             */
 /* https://doi.org/10.1145/1294685.1294717                 */
/*---------------------------------------------------------*/
#pragma once
#include "utils.hpp"
#include "ds_point.hpp"
#include "array2d.hpp"

namespace algo
{
    class ds_mdbu
    {
        public:
            ds_mdbu();
            ds_mdbu(const U32 &n);
            ~ds_mdbu();
            void clear(void);
            void init(const U32 &n);
            void do_ds(const U32 &seed,const bool &mdbu,const bool &recurse,const F32 &h,array2d<F32> *in,array2d<F32> *e,array2d<F32> *out);
            void setPoint(const S32 &x,const S32 &y,const F32 &v);
            void setInitialPoints(array2d<F32> &a,const F32 &th);
            array2d<F32> &getPoints(void);
        private:
            void set_midpoint(ds_point *point,ds_point *parent1,ds_point *parent2,ds_point *parent3,ds_point *parent4,const U32 &level,const F32 &ratio);
            void calculate_midpoint(ds_point *point,const S32 &half,const bool &diamond,const U32 &level,const F32 &ratio);
            void getMidpointParentList(std::vector<ds_point*> &points,ds_point *point,const S32 &half,const bool &diamond);
            void do_midpoint(const S32 &mid_x,const S32 &mid_y,const S32 &half,const bool &diamond,const U32 &level,const F32 &ratio);
            void do_mdbu(void);
            void do_ds_recurse(void);
            void do_ds_iteration(void);
            void do_ds_calc(void);
            void do_recals_recurse(const S32 &mid_x,const S32 &mid_y,const U32 &n,const U32 &half,const U32 &level,const F32 &ratio);
            void copyToDataValue(void);
            array2d<ds_point> m_data;
            array2d<F32> m_data_value;
            std::vector<ds_point*> m_point_order;
            F32 m_prescaler;

    };
}

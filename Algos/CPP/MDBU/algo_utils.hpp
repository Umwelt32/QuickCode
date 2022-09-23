      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2022                                         */
   /* MDBU algorithm.                                         */
  /* References:                                             */
 /* https://doi.org/10.1145/1294685.1294717                 */
/*---------------------------------------------------------*/
#pragma once
#include "utils.hpp"
#include "array2d.hpp"

namespace algo
{
    class utils
    {
        public:
            static void setRandomSeed(const U32 &seed);
            static F32 randomFloat(const F32 &range,const bool &sign);
            static F32 normalizeF32(const F32 &_min,const F32 &_max,const F32 &v);
            static F32 euclideanDistance(const F32 &x1,const F32 &y1,const F32 &x2,const F32 &y2);
            static F32 dBU(const F32 &e,const F32 &d,const F32 &dmax);
            static F32 dFi(const S32 &i);
            static void loadBmpFile(array2d<F32> &out,const std::string &path);
            static void saveBmpFile(array2d<F32> &in,const std::string &path);
            static void saveTxtFile(array2d<F32> &in,const std::string &path);
            static void normalizeArray(array2d<F32> &in);
            static void getMinMaxArrayElement(array2d<F32> &in,F32 &_min,F32 &_max);
            static U32 getNearPowerOf2(U32 v);
    };
}


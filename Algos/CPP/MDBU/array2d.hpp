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
    template <class T>
    class array2d
    {
        public:
            array2d() {m_n=0;m_m=0;clear();}
            ~array2d(){clear();}
            void init(const U32 &n,const U32 &m)
            {
                clear();
                m_n=n;
                m_m=m;
                m_data.resize(m_n);
                for(U32 i=0;i<m_n;++i){m_data[i].resize(m_m);}
                for(U32 i=0;i<m_n;++i){for(U32 j=0;j<m_m;++j){m_data[i][j]=new T();}}
            }
            void clear(void)
            {
                for(U32 i=0;i<m_n;++i){for(U32 j=0;j<m_m;++j){delete m_data[i][j];}}
                m_n=0;
                m_m=0;
                m_data.clear();
            }
            T &get(const S32 &x,const S32 &y){return (*m_data[x][y]);}
            T *getPtr(const S32 &x,const S32 &y){if(x>=0&&y>=0&&x<m_n&&y<m_m){return m_data[x][y];}else{return nullptr;}}
            T *getElementPtr(const U32 &idx){return getPtr(idx%m_n,std::floor(idx/m_n));}
            void getSize(U32 &n,U32 &m)const{n=m_n;m=m_m;}
            void copyFrom(array2d<T> &data){U32 n;U32 m;data.getSize(n,m);init(n,m);for(U32 i=0;i<m_n;++i){for(U32 j=0;j<m_m;++j){get(i,j)=data.get(i,j);}}}
            const U32 getElementsCount(void)const{return (U32)(m_n*m_m);}

        private:
            U32 m_n;
            U32 m_m;
            std::vector<std::vector<T*>> m_data;
    };
}


#pragma once
#include "defs.hpp"

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
    void getSize(U32 &n,U32 &m){n=m_n;m=m_m;}

    private:
        U32 m_n;
        U32 m_m;
        std::vector<std::vector<T*>> m_data;
};

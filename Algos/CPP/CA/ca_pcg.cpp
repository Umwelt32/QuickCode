#include "ca_pcg.hpp"
#include "bmpwriter.hpp"
#include <fstream>
#include <cmath>

typedef enum
{
    CA_CELL_TYPE_FLOOR=0,
    CA_CELL_TYPE_WALL,
    CA_CELL_TYPE_ROCK,
    CA_CELL_TYPE_COUNT
}CA_CELL_TYPE_t;

typedef struct _point2d_t_
{
    _point2d_t_(const S16 &x1,const S16 &y1){x=x1;y=y1;};
    S16 x;
    S16 y;
}point2d_t;


ca_pcg::ca_pcg(){};
ca_pcg::~ca_pcg(){};

void ca_pcg::init(const U16 &w,const U16 &h)
{
    m_cells.init(w,h);
    for (S32 i=0;i<w;++i)
    for (S32 j=0;j<h;++j)
    {
        m_cells.getPtr(i,j)->type=CA_CELL_TYPE_FLOOR;
        m_cells.getPtr(i,j)->neightborhood_value=0;
        m_cells.getPtr(i,j)->floor_value=0;
    }
}

void ca_pcg::recalculate_all_nv_value(const S16 &M)
{
    U32 w,h;
    m_cells.getSize(w,h);
    for (S32 i=0;i<w;++i){for (S32 j=0;j<h;++j){recalculate_nv_for_node(i,j,M);}}
}

void ca_pcg::random_set(const F32 &r)
{
    U32 w,h;
    m_cells.getSize(w,h);
    for (S32 i=0;i<w;++i)
    for (S32 j=0;j<h;++j)
    {
        F32 v = getUniformRandomValue();
        m_cells.getPtr(i,j)->type=(v<=r)?CA_CELL_TYPE_ROCK:CA_CELL_TYPE_FLOOR;
    }
}

void ca_pcg::iterate_once(const S16 &T,const S16 &M)
{
    recalculate_all_nv_value(M);
    recalculate_rocks(T);
}

void ca_pcg::iterate_n_epoch(const U16 &N,const S16 &T,const S16 &M)
{
    for (S32 i=0;i<N;++i)iterate_once(T,M);
}

F32 ca_pcg::getUniformRandomValue(void)
{
    U16 rnd=(rand()%1000)+1;
    F32 frnd = ((F32)rnd/(F32)1000.0f);
    return frnd;
}

void ca_pcg::reset(const F32 &r)
{
    this->random_set(r);
}

void ca_pcg::saveToIo(std::ostream &s)
{
    U32 w,h;
    const char o_pattern[CA_CELL_TYPE_COUNT]={' ','#','*'}; /*floor,wall,rock*/
    if(!s.good())return;
    m_cells.getSize(w,h);
    for (S32 i=0;i<w;++i)
    {
        for (S32 j=0;j<h;++j)
        {
            s<<o_pattern[m_cells.getPtr(i,j)->type];
        }
        s<<std::endl;
    }
}

void ca_pcg::saveToFile(const std::string &path)
{
    std::ofstream file(path,std::ios_base::out|std::ios_base::trunc|std::ios_base::binary);
    saveToIo(file);
    file.close();
}

void ca_pcg::recalculate_nv_for_node(const S32 &x,const S32 &y,const S16 &M)
{
    U16 nv=0;
    U16 floors=0;
    ca_node_t *node;
    node=m_cells.getPtr(x,y);
    if (node==nullptr)return;
    if (M<=0)return;
    point2d_t S(x,y);
    point2d_t A(S.x-M,S.y-M);/*AABB*/
    point2d_t B(S.x+M+1,S.y+M+1);
    for (S16 i=A.x;i<B.x;++i)
    for (S16 j=A.y;j<B.y;++j)
    {
        ca_node_t *current=m_cells.getPtr(i,j);
        if(current == nullptr)continue;
        if((S.x==i)&&(S.y==j))continue;
        if(current->type==CA_CELL_TYPE_ROCK){++nv;}else if(current->type==CA_CELL_TYPE_FLOOR){++floors;}else {;}
    }
    node->neightborhood_value=nv;
    node->floor_value=floors;
}

void ca_pcg::saveToBmp(const std::string &path)
{
    U32 w,h;
    m_cells.getSize(w,h);
    bmpwriter_init(w,h);
    bmpwriter_clear(0);
    for (S32 i=0;i<w;++i)for (S32 j=0;j<h;++j)
    {
        unsigned char r=0,g=0,b=0;
        r=m_cells.getPtr(i,j)->type==CA_CELL_TYPE_ROCK?255:0;
        g=m_cells.getPtr(i,j)->type==CA_CELL_TYPE_WALL?255:0;
        bmpwriter_setPixel(i,j,r,g,b);
    }
    bmpwriter_save(path.c_str());
    bmpwriter_deinit();
}

void ca_pcg::recalculate_walls()
{
    U32 w,h;
    m_cells.getSize(w,h);
    for (S32 i=0;i<w;++i)for (S32 j=0;j<h;++j)
    {
        ca_node_t *current=m_cells.getPtr(i,j);
        if ((current->type==CA_CELL_TYPE_ROCK)||(current->type==CA_CELL_TYPE_WALL))
        {
            if(current->floor_value>0)
            {
                current->type=CA_CELL_TYPE_WALL;
            }
            else
            {
                current->type=CA_CELL_TYPE_ROCK;
            }
        }
    }
}

void ca_pcg::recalculate_rocks(const U16 &T)
{
    U32 w,h;
    m_cells.getSize(w,h);
    for (S32 i=0;i<w;++i)for (S32 j=0;j<h;++j)
    {
        ca_node_t *current=m_cells.getPtr(i,j);
        current->type=(current->neightborhood_value>=T)?CA_CELL_TYPE_ROCK:CA_CELL_TYPE_FLOOR;
    }
}

void ca_pcg::generate(const U16 &seed,const F32 &r,const U16 &N,const S16 &T,const S16 &M)
{
    this->setSeed(seed);
    this->reset(r);
    this->iterate_n_epoch(N,T,M);
    this->recalculate_walls();
}

void ca_pcg::generate_f(const U16 &seed,const F32 &r,const U16 &N,const F32 &M_F,const S16 &M)
{
    this->generate(seed,r,N,FT_T(M,M_F),M);
}

void ca_pcg::generate_sub(const U16 &seed,const F32 &r,const U16 &N,const S16 &T,const S16 &M,const U8 &sub,const U8 &n1)
{
    this->setSeed(seed);
    this->reset(r);
    this->iterate_n_epoch(N,T,M);
    for (U8 i=0;i<sub;++i)
    {
        subdivide();
        this->iterate_n_epoch(n1,T,M);
    }
    this->recalculate_walls();
}

void ca_pcg::generate_sub_f(const U16 &seed,const F32 &r,const U16 &N,const F32 &M_F,const S16 &M,const U8 &sub,const U8 &n1)
{
    this->generate_sub(seed,r,N,FT_T(M,M_F),M,sub,n1);
}

void ca_pcg::setSeed(const U16 &seed)
{
    srand(seed);
}

void ca_pcg::subdivide(void)
{
    U32 w,h;
    array2d<ca_node_t> cell_copy;
    m_cells.getSize(w,h);
    cell_copy.copy(m_cells);
    m_cells.init(2*w,2*h);
    for (U32 i=0;i<w;++i)
    {
        for (U32 j=0;j<h;++j)
        {
            m_cells.set((2*i)+0,(2*j)+0,cell_copy.getPtr(i+0,j+0));
            m_cells.set((2*i)+1,(2*j)+0,cell_copy.getPtr(i+0,j+0));
            m_cells.set((2*i)+0,(2*j)+1,cell_copy.getPtr(i+0,j+0));
            m_cells.set((2*i)+1,(2*j)+1,cell_copy.getPtr(i+0,j+0));
        }
    }
    cell_copy.clear();
}

S16 ca_pcg::FT_T(const S16 &M, const F32 &M_F)
{
    S16 T_P1=((2*M)+1);
    S16 T_P2=((T_P1*T_P1)-1);
    F32 T_F=T_P2*M_F;
    S16 T  = std::ceil(T_F);
    if (T_P2<T)T=T_P2;
    return T;
}


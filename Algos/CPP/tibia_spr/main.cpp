#include <iostream>
#include "spr.hpp"

using namespace std;

int main()
{
    U16 size;
    U8 *data;
    tibia_spr ss;
    ss.load_spr("Tibia.spr");
    U16 cc=(*ss.getHeader().spr_count_2B);
    cout <<((U32)(*ss.getHeader().spr_count_2B))<<" "<<std::hex<<((U32)(*ss.getHeader().spr_count_2B))<<"\n";
    data = ss.getSpriteRawData(128,size);
    //for (int idx=0;idx<size;++idx){cout <<std::hex<<(U32)data[idx]<<" ";}
    U32 offset = ss.getHeader().spr_offsets_4B[128];
    cout <<std::hex<<offset<<std::endl;
    for (int i=0;i<128;++i)
    {
        ;
    }
    cout <<"\n";
    return 0;
}

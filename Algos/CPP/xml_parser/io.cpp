#include "io.hpp"
#include <fstream>

void io_load_data_from_file(std::vector<U8> &data,const std::string &path)
{
    U8 bufor[ 1024 ];
    std::ifstream file;
    data.clear();
    file.open(path.c_str(),std::ios::in|std::ios::binary);
    while((file.good())&(!file.eof()))
    {
        file.read((char*)&bufor[0],1024);
        for (int i=0;i<file.gcount();++i)
        {
            data.push_back(bufor[i]);
        }
    }
    data.push_back(0);
    file.close();
}

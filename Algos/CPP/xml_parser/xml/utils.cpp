#include "utils.hpp"
#include "defs.hpp"
#include <algorithm>

namespace _xml
{
    S32 utils::get_int_from_buffer(const U8 &bytes,const U32 &offset,const U8* buffer,const bool &swap_order)
    {
        U8 internal_buffer[8]={0,0,0,0,0,0,0,0};
        S32 *pointer = (S32*)&internal_buffer[0];
        for (U8 i = 0u;((i < bytes)&&(i < 4u)); i++)
        {
            internal_buffer[swap_order?(3u-i):i] = buffer[i + offset];
        }
        return (*pointer);
    }

    F64 utils::string_to_float(const std::string &str,bool *res)
    {
        F64 value=0;
        bool result;
        try
        {
            value=std::atof(str.c_str());
            result=true;
        }
        catch (...){result=false;}
        if (res!=nullptr)(*res)=result;
        return value;
    }

    S32 utils::string_to_int(const std::string &str,bool *res)
    {
        S32 value=0;
        bool result;
        try
        {
            value=std::atoi(str.c_str());
            result=true;
        }
        catch (...){result=false;}
        if (res!=nullptr)(*res)=result;
        return value;
    }

    std::vector<std::string> utils::string_split(const std::string &str,const char &delimiter)
    {
        U32 start=0;
        U32 bytes=0;
        std::vector<std::string> data;
        data.clear();
        for (U32 i = 0; i < (str.length()+1);++i)
        {
            if ((str.length()==i)||(str[i]==delimiter))
            {
                data.push_back(str.substr(start,bytes));
                start=i+1;
                bytes=0;
            }
            else
            {
                ++bytes;
            }
        }
        return data;
    }

    std::string utils::string_to_lower(const std::string str)
    {
        std::string new_str="";
        for (U32 i = 0; i < str.length();++i)
        {
            new_str.push_back(tolower(str[i]));
        }
        return new_str;
    }
}

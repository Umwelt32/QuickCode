#pragma once
#include "defs.hpp"

namespace _xml
{
    class utils
    {
        public:
            static S32 get_int_from_buffer(const U8 &bytes,const U32 &offset,const U8* buffer,const bool &swap_order=false);
            static std::vector<std::string> string_split(const std::string &str,const char &delimiter);
            static F64 string_to_float(const std::string &str,bool *res=nullptr);
            static S32 string_to_int(const std::string &str,bool *res=nullptr);
            static std::string string_to_lower(const std::string str);
    };
}

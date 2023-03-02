#pragma once
#include "ixmlnode.hpp"

namespace _xml
{
    class ixmlfile
    {
        public:
            virtual bool open(const std::string &path)=0;
            virtual bool isOpen(void)=0;
            virtual void close(void)=0;
            virtual ixmlnode *getRootElement(void)=0;
            virtual void printDebug(void)=0;
    };
}

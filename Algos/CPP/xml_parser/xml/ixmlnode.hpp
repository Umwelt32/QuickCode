#pragma once
#include "defs.hpp"
#include <vector>
#include <map>

namespace _xml
{
    class ixmlnode
    {
        public:
            virtual const std::string &getName(void)=0;
            virtual const std::string &getText(void)=0;
            virtual const std::string &getComment(void)=0;
            virtual const std::string &getCdata(void)=0;
            virtual const std::string &getAttribute(const std::string &name)=0;
            virtual void getAttributeByIdx(const U32 &idx,std::string &name,std::string &value)=0;
            virtual const S32 getTextAsInt(const S32 &def=0)=0;
            virtual const F32 getTextAsFloat(const F32 &def=0)=0;
            virtual const S32 &getAttributeAsInt(const std::string &name,const S32 &def=0)=0;
            virtual const F32 &getAttributeAsFloat(const std::string &name,const F32 &def=0)=0;
            virtual const U32 &getAttributesCount(void)=0;
            virtual const bool &isEmptyElement(void)=0;
            virtual bool hasAttrib(const std::string &name)=0;
            virtual ixmlnode *getParent(void)=0;
            virtual ixmlnode *getNode(const U32 &id)=0;
            virtual ixmlnode *getNode(const std::string &name,const S32 &nth=-1)=0;
            virtual U32 getNodesCount(void)=0;
            virtual void clear(void)=0;
    };
}

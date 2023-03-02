#pragma once
#include <vector>
#include <map>
#include "ixmlnode.hpp"


namespace _xml
{
    class xmlnode : public ixmlnode
    {
        friend class xmlfile;
        public:
            xmlnode();
            ~xmlnode();
            const std::string &getName(void);
            const std::string &getText(void);
            const std::string &getComment(void);
            const std::string &getCdata(void);
            const std::string &getAttribute(const std::string &name);
            void getAttributeByIdx(const U32 &idx,std::string &name,std::string &value);
            const S32 getTextAsInt(const S32 &def=0);
            const F32 getTextAsFloat(const F32 &def=0);
            const S32 &getAttributeAsInt(const std::string &name,const S32 &def=0);
            const F32 &getAttributeAsFloat(const std::string &name,const F32 &def=0);
            const U32 &getAttributesCount(void);
            const bool &isEmptyElement(void);

            bool hasAttrib(const std::string &name);
            xmlnode *getParent(void);
            xmlnode *getNode(const U32 &id);
            xmlnode *getNode(const std::string &name,const S32 &nth=-1);
            U32 getNodesCount(void);
            void clear(void);
        private:
            xmlnode *addNode(void);
            void setParent(xmlnode *ptr);
            void setName(const std::string &name);
            void setText(const std::string &text);
            void setComment(const std::string &comment);
            void setCdata(const std::string &cdata);
            void setAttribute(const std::string &name,const std::string &value);
            void setEmpty(const bool &element);
            void setAttributesCount(const U32 &cnt);
            std::string m_name;
            std::string m_text;
            std::string m_comment;
            std::string m_cdata;
            std::map<std::string, std::string> m_attributes;
            std::vector<xmlnode> m_nodes;
            xmlnode *m_parent;
            bool m_empty;
            U32 m_attribCount_u32;
        };
}

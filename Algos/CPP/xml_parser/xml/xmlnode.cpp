#include "xmlnode.hpp"
#include "utils.hpp"

#define XML_NODE_THROW_EXCEPTION(msg) {throw std::invalid_argument( msg );}

namespace _xml
{
    xmlnode::xmlnode()
    {
        m_parent=nullptr;
        m_empty=false;
        clear();
    }

    xmlnode::~xmlnode()
    {
        clear();
    }

    void xmlnode::clear(void)
    {
        m_attribCount_u32=0u;
        m_empty=false;
        m_attributes.clear();
        m_nodes.clear();
    }

    void xmlnode::setEmpty(const bool &element)
    {
        m_empty=element;
    }

    void xmlnode::setName(const std::string &name)
    {
        m_name=name;
    }

    void xmlnode::setText(const std::string &text)
    {
        m_text=text;
    }

    void xmlnode::setComment(const std::string &comment)
    {
        m_comment=comment;
    }

    void xmlnode::setCdata(const std::string &cdata)
    {
        m_cdata=cdata;
    }

    void xmlnode::setAttribute(const std::string &name,const std::string &value)
    {
        m_attributes[utils::string_to_lower(name)]=value;
    }

    bool xmlnode::hasAttrib(const std::string &name)
    {
        /*return (m_attributes.contains(utils::string_to_lower(name)));*/
        return (m_attributes.find(utils::string_to_lower(name))!=m_attributes.end())?true:false;
    }

    xmlnode *xmlnode::addNode(void)
    {
        U32 idx = getNodesCount();
        m_nodes.resize(idx+1u);
        m_nodes[idx].clear();
        m_nodes[idx].m_parent=this;
        return &m_nodes[idx];
    }

    xmlnode *xmlnode::getParent(void)
    {
        return m_parent;
    }

    U32 xmlnode::getNodesCount(void)
    {
        return m_nodes.size();
    }

    xmlnode *xmlnode::getNode(const U32 &id)
    {
        if (id < getNodesCount())
        {
            return &m_nodes[id];
        }
        else
        {
            return nullptr;
        }
    }

    xmlnode *xmlnode::getNode(const std::string &name,const S32 &nth)
    {
        std::string n1;
        std::string n2;
        U32 n=0;
        xmlnode *node=nullptr;
        n1=utils::string_to_lower(name);
        for (int i = 0u; i < getNodesCount();++i)
        {
            n2=utils::string_to_lower(getNode(i)->getName());
            if (n1==n2)
            {
                if ((nth<0) || (nth==n))
                {
                    node=getNode(i);
                    break;
                }
                else
                {
                    ++n;
                }
            }
        }
        if (node==nullptr){XML_NODE_THROW_EXCEPTION("OUT_OF_NODES");}
        return node;
    }

    void xmlnode::setParent(xmlnode *ptr)
    {
        m_parent=ptr;
    }

    const std::string &xmlnode::getName(void)
    {
        return m_name;
    }

    const std::string &xmlnode::getText(void)
    {
        return m_text;
    }

    const std::string &xmlnode::getComment(void)
    {
        return m_comment;
    }

    const std::string &xmlnode::getCdata(void)
    {
        return m_cdata;
    }

    const std::string &xmlnode::getAttribute(const std::string &name)
    {
        return m_attributes[utils::string_to_lower(name)];
    }

    const S32 xmlnode::getTextAsInt(const S32 &def)
    {
        S32 value;
        bool res=false;
        value = utils::string_to_int(getText(),&res);
        if (res==false)value=def;
        return value;
    }

    const F32 xmlnode::getTextAsFloat(const F32 &def)
    {
        F32 value;
        bool res=false;
        value =  utils::string_to_float(getText(),&res);
        if (res==false)value=def;
        return value;
    }

    const S32 &xmlnode::getAttributeAsInt(const std::string &name,const S32 &def)
    {
        S32 value;
        bool res=false;
        value = utils::string_to_int(getAttribute(name),&res);
        if (res==false)value=def;
        return value;
    }

    const F32 &xmlnode::getAttributeAsFloat(const std::string &name,const F32 &def)
    {
        F32 value;
        bool res=false;
        value = utils::string_to_float(getAttribute(name),&res);
        if (res==false)value=def;
        return value;
    }

    const bool &xmlnode::isEmptyElement(void)
    {
        return m_empty;
    }

    void xmlnode::setAttributesCount(const U32 &cnt)
    {
        m_attribCount_u32=cnt;
    }

    const U32 &xmlnode::getAttributesCount(void)
    {
        return m_attribCount_u32;
    }

    void xmlnode::getAttributeByIdx(const U32 &idx,std::string &name,std::string &value)
    {
        U32 id=0;
        std::map<std::string, std::string>::iterator it;
        name="";
        value="";
        for(it=m_attributes.begin();it!=m_attributes.end();it++)
        {
            if (id==idx)
            {
                name=it->first;
                value=it->second;
                break;
            }
            else
            {
                ++id;
            }
        }
    }
}

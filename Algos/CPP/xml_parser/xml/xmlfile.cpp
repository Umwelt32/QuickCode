#include "xmlfile.hpp"
#include "xmlnode.hpp"
#include <irrXML.h>
#include <utility>

namespace _xml
{
    xmlfile::xmlfile()
    {
        m_open = false;
        m_rootElement = new xmlnode();
    }

    xmlfile::~xmlfile()
    {
        delete m_rootElement;
    }

    bool xmlfile::open(const std::string &path)
    {
        m_open = false;
        irr::io::IrrXMLReader* irrXmlFilePtr_p = irr::io::createIrrXMLReader(path.c_str());
        if (irrXmlFilePtr_p==nullptr)return m_open;
        ((xmlnode*)m_rootElement)->setParent((xmlnode*)m_rootElement);
        m_currentNodePtr = m_rootElement;
        m_open = (irrXmlFilePtr_p!=nullptr);
        this->parse((void*)irrXmlFilePtr_p);
        return (m_open==true);
    }

    bool xmlfile::isOpen(void)
    {
        return m_open;
    }

    void xmlfile::close(void)
    {
        m_open=false;
    }

    ixmlnode *xmlfile::getRootElement(void)
    {
        return m_rootElement;
    }

    ixmlnode *xmlfile::enterElement(void)
    {
        ixmlnode *node=nullptr;
        if (m_currentNodePtr==nullptr)
        {
            m_currentNodePtr=m_rootElement;
            node=static_cast<xmlnode*>(m_currentNodePtr)->addNode();
        }
        else
        {
            node=static_cast<xmlnode*>(m_currentNodePtr)->addNode();
        }

        m_currentNodePtr=node;
        return node;
    }

    ixmlnode *xmlfile::leaveElement(void)
    {
        ixmlnode *node=nullptr;
        if (m_currentNodePtr==nullptr)
        {
            ;
        }
        else
        {
            if (m_currentNodePtr->getParent()==nullptr)
            {
                node=m_rootElement;
            }
            else
            {
                node=m_currentNodePtr->getParent();
            }
        }

        m_currentNodePtr=node;
        return node;
    }

    ixmlnode *xmlfile::getCurrentNode(void)
    {
        return m_currentNodePtr;
    }

    void xmlfile::printXmlNodes(ixmlnode *root)
    {
        char buffer[128];
        int idx=0;
        if (root->isEmptyElement()==false)sprintf(buffer,"<%s>\n",root->getName().c_str());else sprintf(buffer,"<%s/>\n",root->getName().c_str());
        _DBG_MSG_(buffer);
        while(true)
        {
            ixmlnode *childs=root->getNode(idx++);
            if (childs==nullptr)break;
            printXmlNodes(childs);
        }
        if (root->isEmptyElement()==false)
        {
            sprintf(buffer,"\n</%s>",root->getName().c_str());
            _DBG_MSG_(buffer);
        }
    }

    void xmlfile::printDebug(void)
    {
        printXmlNodes(getRootElement());
    }

    void xmlfile::parse(void *xmlFilePtr_p)
    {
        irr::io::IrrXMLReader* xml = (irr::io::IrrXMLReader*)xmlFilePtr_p;
        _ASSERT_(xml!=nullptr);
        while((xml) && (xml->read()))
        {
            switch(xml->getNodeType())
            {
                case irr::io::EXN_NONE:;break;
                case irr::io::EXN_ELEMENT:
                    (void)enterElement();
                    if (getCurrentNode()!=nullptr)
                    {
                        static_cast<xmlnode*>(getCurrentNode())->setName(xml->getNodeName());
                        static_cast<xmlnode*>(getCurrentNode())->setEmpty(xml->isEmptyElement());
                        static_cast<xmlnode*>(getCurrentNode())->setAttributesCount(xml->getAttributeCount());
                        for (int i=0u ; i < getCurrentNode()->getAttributesCount(); ++i)
                        {
                            static_cast<xmlnode*>(getCurrentNode())->setAttribute(xml->getAttributeName(i),xml->getAttributeValue(i));
                        }
                        if (static_cast<xmlnode*>(getCurrentNode())->isEmptyElement())
                        {
                            (void)leaveElement();
                        }
                    }
                break;
                case irr::io::EXN_ELEMENT_END:
                (void)leaveElement();
                break;
                case irr::io::EXN_TEXT:
                    if (getCurrentNode()!=nullptr)
                    {
                        static_cast<xmlnode*>(getCurrentNode())->setText(xml->getNodeData());
                    }
                break;
                case irr::io::EXN_COMMENT:
                    if (getCurrentNode()!=nullptr)
                    {
                        static_cast<xmlnode*>(getCurrentNode())->setComment(xml->getNodeData());
                    }
                break;
                case irr::io::EXN_CDATA:
                    if (getCurrentNode()!=nullptr)
                    {
                        static_cast<xmlnode*>(getCurrentNode())->setCdata(xml->getNodeData());
                    }
                break;
                case irr::io::EXN_UNKNOWN:;break;
                default:;break;
            }
        }
    }
}

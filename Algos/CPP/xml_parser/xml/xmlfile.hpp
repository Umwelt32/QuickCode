#pragma once
#include "ixmlnode.hpp"
#include "ixmlfile.hpp"

namespace _xml
{
    class xmlfile : public ixmlfile
    {
        public:
            xmlfile();
            ~xmlfile();
            bool open(const std::string &path);
            bool isOpen(void);
            void close(void);
            ixmlnode *getRootElement(void);
            void printDebug(void);
        private:
            void printXmlNodes(ixmlnode *root);
            ixmlnode *getCurrentNode(void);
            void parse(void *xmlFilePtr_p);
            ixmlnode *enterElement(void);
            ixmlnode *leaveElement(void);
            bool m_open;
            ixmlnode *m_currentNodePtr;
            ixmlnode *m_rootElement;
    };
}

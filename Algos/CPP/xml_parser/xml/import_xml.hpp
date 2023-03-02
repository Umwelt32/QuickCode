#pragma once
#include "defs.hpp"
#include "ixmlfile.hpp"

namespace _xml
{
    class import_xml
    {
        public:
            import_xml();
            ~import_xml();
            void init(void);
            void deinit(void);
            void clear(void);
            void load(const std::string &path);
            ixmlnode *getRootElementPtr(void);
        private:
            ixmlfile *m_file;
    };
}

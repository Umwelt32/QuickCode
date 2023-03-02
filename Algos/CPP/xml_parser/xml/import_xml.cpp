#include "import_xml.hpp"
#include "xmlfile.hpp"

namespace _xml
{
    import_xml::import_xml()
    {
        m_file=nullptr;
    }

    import_xml::~import_xml()
    {
        deinit();
    }

    void import_xml::init(void)
    {
        m_file=new xmlfile();
    }

    void import_xml::deinit(void)
    {
        if (m_file!=nullptr)
        {
            m_file->close();
            delete m_file;
            m_file=nullptr;
        }
    }

    void import_xml::clear(void)
    {
        m_file->close();
    }

    void import_xml::load(const std::string &path)
    {
        m_file->open(path);
        /*m_file->printDebug();*/
    }

    ixmlnode *import_xml::getRootElementPtr(void)
    {
        return m_file->getRootElement();
    }
}

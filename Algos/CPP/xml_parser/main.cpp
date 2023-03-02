#include <iostream>
#include "import_xml.hpp"

using namespace std;

    void load_xml_shaders(void)
    {
        S32 idx=0;
        bool run=true;
        _xml::import_xml xml;
        xml.init();
        xml.load("config.xml");
        while (run)
        {
            try
            {
                bool enabled=xml.getRootElementPtr()->getNode("shaders")->getNode("shader",idx)->getNode("enabled")->getTextAsInt(0);
                std::string name=xml.getRootElementPtr()->getNode("shaders")->getNode("shader",idx)->getNode("name")->getText();
                std::string type=xml.getRootElementPtr()->getNode("shaders")->getNode("shader",idx)->getNode("name")->getAttribute("type");
                std::string vs=xml.getRootElementPtr()->getNode("shaders")->getNode("shader",idx)->getNode("vs_file")->getText();
                std::string fs=xml.getRootElementPtr()->getNode("shaders")->getNode("shader",idx)->getNode("fs_file")->getText();
                std::cout<<"Enabled: "<<enabled<<"\nname: "<<name<<"\ntype: "<<type<<"\nvs: "<<vs<<"\nfs "<<fs<<std::endl;
            }
            catch(...)
            {
                run=false;
            }
                ++idx;
        }
        xml.deinit();
    }

int main()
{
    load_xml_shaders();
    return 0;
}

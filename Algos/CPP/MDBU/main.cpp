#include "ca_pcg.hpp"

using namespace std;

static U32 paramnr=0;
static U32 param_count=0;
static char **params=nullptr;
static U32 m_seed = 128u;
static U32 m_w = 256u;
static U32 m_h = 256u;
static U32 m_n = 16u;
static U32 m_m = 2u;
static U32 m_t = 16u;
static F32 m_r = 0.75f;
std::string m_output_file="bitmap_out.bmp";

bool isParamToRead(void){return (paramnr<param_count);}
std::string read_param_str(void){if(isParamToRead()){return std::string(params[paramnr++]);}else{return "";}}
int read_param_int(void){return (std::stoi(read_param_str()));}
float read_param_f(void){return (std::stof(read_param_str()));}

void help_view(void)
{
    std::cout<<"-----------"<<std::endl;
    std::cout<<"-seed <int>"<<std::endl;
    std::cout<<"-out <file>"<<std::endl;
    std::cout<<"-n <int>"<<std::endl;
    std::cout<<"-r <float>"<<std::endl;
    std::cout<<"-t <int>"<<std::endl;
    std::cout<<"-m <int>"<<std::endl;
    std::cout<<"-w <int>"<<std::endl;
    std::cout<<"-h <int>"<<std::endl;
    std::cout<<"-help"<<std::endl;
    std::cout<<"-----------"<<std::endl;
}

void parse_params(int argc, char **argv)
{
    paramnr=0;
    param_count=argc;
    params=(char**)&argv[0];
    while(isParamToRead())
    {
        std::string param = read_param_str();
        std::cout<<param<<" ";
        if (param=="-seed"){m_seed=read_param_int();}
        else if(param=="-out"){m_output_file=read_param_str();}
        else if(param=="-w"){m_w=read_param_int();}
        else if(param=="-h"){m_h=read_param_int();}
        else if(param=="-r"){m_r=read_param_f();}
        else if(param=="-n"){m_n=read_param_int();}
        else if(param=="-m"){m_m=read_param_int();}
        else if(param=="-t"){m_t=read_param_int();}
        else if(param=="-help"){help_view();exit(0);}
    }
}

int main(int argc, char **argv)
{
    ca_pcg node;
    node.init(m_w,m_h);
    node.generate(m_seed,m_r,m_n,m_t,m_m);
    node.saveToBmp(m_output_file.c_str());
    return 0;
}

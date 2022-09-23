      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2022                                         */
   /* MDBU algorithm.                                         */
  /* References:                                             */
 /* https://doi.org/10.1145/1294685.1294717                 */
/*---------------------------------------------------------*/

#include <iostream>
#include "ds_mdbu.hpp"
#include "utils.hpp"
#include "algo_utils.hpp"

using namespace std;
static U32 paramnr=0;
static U32 param_count=0;
static char **params=nullptr;
std::string m_input_file="bitmap.bmp";
std::string m_output_file="bitmap";
U32 m_seed = 128u;
F32 m_h = 0.3f;
int m_n=0;
bool m_mdbu = true;
bool m_recurse = false;
bool m_gen_all_out = false;

bool isParamToRead(void){return (paramnr<param_count);}
std::string read_param_str(void){if(isParamToRead()){return std::string(params[paramnr++]);}else{return "";}}
int read_param_int(void){return (std::stoi(read_param_str()));}
float read_param_f(void){return (std::stof(read_param_str()));}

void help_view(void)
{
    std::cout<<"-----------"<<std::endl;
    std::cout<<"-seed <int>"<<std::endl;
    std::cout<<"-h <float>"<<std::endl;
    std::cout<<"-file <file>"<<std::endl;
    std::cout<<"-out <file>"<<std::endl;
    std::cout<<"-n <int>"<<std::endl;
    std::cout<<"-i <int>"<<std::endl;
    std::cout<<"-all_out"<<std::endl;
    std::cout<<"-r"<<std::endl;
    std::cout<<"-dis_mdbu"<<std::endl;
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
        else if(param=="-h"){m_h=read_param_f();}
        else if(param=="-file"){m_input_file=read_param_str();}
        else if(param=="-out"){m_output_file=read_param_str();}
        else if(param=="-n"){m_n=read_param_int();}
        else if(param=="-i"){algo::utils::setIConst(read_param_int());}
        else if(param=="-all_out"){m_gen_all_out=true;}
        else if(param=="-r"){m_recurse=true;}
        else if(param=="-dis_mdbu"){m_mdbu=false;}
        else if(param=="-help"){help_view();exit(0);}
    }
}

int main(int argc, char **argv)
{
    parse_params(argc,argv);
    algo::array2d<F32> bmp;
    algo::array2d<F32> o[3];
    algo::ds_mdbu w;
    if (m_n<=0)
    {
        algo::utils::loadBmpFile(bmp,m_input_file);
        w.setInitialPoints(bmp,0.1);
    }
    else
    {
        w.init(m_n);
    }
    w.do_ds(m_seed,m_mdbu,m_recurse,m_h,&o[0],&o[1],&o[2]);
    algo::utils::normalizeArray(o[2]);
    algo::utils::normalizeArray(o[1]);
    if(m_gen_all_out)
    {
        algo::utils::saveBmpFile(o[0],m_output_file+"_1.bmp");
        algo::utils::saveBmpFile(o[1],m_output_file+"_2.bmp");
        algo::utils::saveTxtFile(o[2],m_output_file+"_3.txt");
    }
    algo::utils::saveBmpFile(o[2],m_output_file+"_3.bmp");
    return 0;
}

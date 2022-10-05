#include <iostream>
#include "fbm.hpp"
#include "algo_utils.hpp"
#include "particle.hpp"

using namespace std;

int main()
{
    std::vector<F32> oo;
    algo::fbm::generate(oo,10,0.5f,128);
    algo::utils::normalize_vector(oo,true);
    //algo::utils::print_vector(oo);
    algo::array2d<F32> bmp;
    algo::river_particle particle;
    bmp.init(1025,1025);
    particle.init(&bmp,0.5,11,128);
    for (U32 i =0; i <8;++i)
    {
        particle.randomize(0.3+((F32)(i%5)/(F32)10),11,i+1);
        particle.setParticle(700,700,0.5f,(i%7)+1,0.1);
        particle.do_steps_until_flows();
    }
    algo::utils::normalizeArray(bmp);
    //algo::fbm::fill_array_with_fbm(bmp,0.5,128,true);
    //algo::fbm::fill_array_with_fbm(bmp,0.7,256,true);
    //algo::fbm::fill_array_with_fbm(bmp,0.3,512,true);
    algo::utils::saveBmpFile(bmp,"out.bmp");
    cout << "Hello world!" << endl;
    return 0;
}

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

int main()
{
    algo::array2d<F32> bmp;
    algo::array2d<F32> o[3];
    algo::ds_mdbu w(10);
    algo::utils::loadBmpFile(bmp,"bitmap.bmp");
    w.setInitialPoints(bmp,0.1);
    w.do_ds(128,true,false,0.3,&o[0],&o[1],&o[2]);
    algo::utils::normalizeArray(o[2]);
    algo::utils::normalizeArray(o[1]);
    algo::utils::saveBmpFile(o[0],"1.bmp");
    algo::utils::saveBmpFile(o[1],"2.bmp");
    algo::utils::saveBmpFile(o[2],"3.bmp");
    algo::utils::saveTxtFile(o[2],"3.txt");
    cout << "Hello world!" << endl;
    return 0;
}

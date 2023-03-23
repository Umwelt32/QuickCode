#include "ca_pcg.hpp"

using namespace std;

int main()
{
    srand(time(NULL));
    ca_pcg node;
    node.init(256,256);
    node.generate(128,0.5f,1,5,1);
    node.saveToBmp("out.bmp");

    cout << "Hello world!" << endl;
    return 0;
}

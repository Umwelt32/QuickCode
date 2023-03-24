#include "ca_pcg.hpp"

using namespace std;

int main()
{
    srand(time(NULL));
    ca_pcg node;
    node.init(1024,1024);
    node.generate(128,0.75f,20,16,2);
    node.saveToBmp("out.bmp");

    cout << "Hello world!" << endl;
    return 0;
}

#include <iostream>
#include <fstream>
#include "TestController.h"
#include "vhdController.h"
#include "Params.h"
using namespace std;
int main() {
    //std::cout << "Hello, World!" << std::endl;
    /*  Test Utils  */
    //TestController TestAll;
    //TestAll.testVhd();
    //TestAll.testFs();
    /*  Features    */
    // TO-DO
//    char wrbuff[BLOCK_SIZE];
//    memset(1, wrbuff, BLOCK_SIZE);
//    char rdbuff[BLOCK_SIZE];
//    memset(2, rdbuff, BLOCK_SIZE);
    vhdController _vhd_;
    if(TestController::testvhdController(_vhd_))
        cout<<"vhdcontroller is right"<<endl;

    return 0;
}





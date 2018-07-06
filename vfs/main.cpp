#include <iostream>
#include <fstream>
#include "TestController.h"
#include "vhdController.h"
#include "Params.h"

using namespace std;

bool test(vhdController &_vhd_);

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
    test(_vhd_);


//    strcpy(wbuff, "test");
//
//
    return 0;
}

bool test(vhdController &_vhd_) {
    _vhd_.create_vhd();
    _vhd_.load_vhd();
    //    _vhd_.create_vhd();
//
//    _vhd_.write_vhd(wrbuff, 0, 1);
//    _vhd_.read_vhd(rdbuff, 0, 1);
    char wbuff[BLOCK_SIZE];
    memset(wbuff, 0, BLOCK_SIZE);
    strcpy(wbuff, "test0");
    _vhd_.write_vhd(wbuff, 0);
    strcpy(wbuff, "test1");
    _vhd_.write_vhd(wbuff, 1);
    char rdbuff[BLOCK_SIZE];
    _vhd_.read_vhd(rdbuff, 0);
    cout << rdbuff << endl;
    return true;
}





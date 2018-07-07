#include <iostream>
#include <fstream>
#include "TestController.h"
#include "vhdController.h"
#include "iNodeController.h"
#include "DataStructure.h"
#include "Params.h"
using namespace std;
int main() {
    vhdController _vhd_;
    fsController _fs_;
    iNodeController my_iNode;
    _fs_.init_iNode_free_list();
    if(TestController::testvhdController(_vhd_))
        cout<<"vhdcontroller is right"<<endl;
    if (TestController::testiNodeController(my_iNode,_fs_))
        cout<<"iNode is right"<<endl;
    return 0;
}





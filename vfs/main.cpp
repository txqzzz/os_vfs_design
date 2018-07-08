#include <iostream>
#include <fstream>
#include "TestController.h"
#include "vhdController.h"
#include "iNodeController.h"
#include "DataStructure.h"
#include "Params.h"

using namespace std;

int main() {
    /* Test Utils */
    vhdController _vhd_;
    fsController _fs_;
    iNodeController my_iNode;
    userController _user_;
    _fs_.init_iNode_free_list();
    if (TestController::testvhdController(_vhd_))
        cout << "[ok] vhdController Test Checked." << endl;
    if (TestController::testuserController(_user_))
        cout << "[ok] userController Test Checked." << endl;

    //bool k;
    //k=(TestController::testiNodeController(my_iNode, _fs_));
    return 0;
}





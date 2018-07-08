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
    _fs_.init_iNode_free_list();
    if (TestController::testvhdController(_vhd_))
        cout << "[ok] vhdController Test Checked." << endl;
    if (TestController::testiNodeController(my_iNode, _fs_))
        cout << "[ok] iNode Test Checked." << endl;

    return 0;
}





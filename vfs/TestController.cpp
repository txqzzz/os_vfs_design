//
// Created by Xingqi Tang on 2018/7/5.
//

#include "TestController.h"
#include "vhdController.h"

using namespace std;

TestController::TestController() {
    //
}

TestController::~TestController() {
    //
}

bool TestController::testVhd() {
    vhdController vhdController_test;
    perror(vhdController_test.create_vhd() ? "[ok]vhd_create_success." : "[err]vhd_create_failed.");
    //perror(vhdController_test.del_vhd() ? "[ok]vhd_del_success." : "[err]vhd_del_failed.");
    return true;
}



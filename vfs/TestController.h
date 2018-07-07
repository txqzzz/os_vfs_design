//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_TEST_H
#define VFS_TEST_H
#include"vhdController.h"

class TestController {
public:
    TestController();
    ~TestController();

    bool testVhd();
    static bool testvhdController(vhdController &_vhd_);
};

#endif //VFS_TEST_H

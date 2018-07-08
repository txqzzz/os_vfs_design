//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_TEST_H
#define VFS_TEST_H

#include"vhdController.h"
#include"iNodeController.h"
#include "userController.h"

class TestController {
public:
    TestController();

    bool testVhd();

    static bool testvhdController(vhdController &_vhd_);

    static int testiNodeController(iNodeController &_iNode_, fsController _fs_);

    static bool testuserController(userController &_user_);
};

#endif //VFS_TEST_H

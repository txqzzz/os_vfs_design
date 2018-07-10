//
// Created by Xingqi Tang on 2018/7/5.
//

#include "TestController.h"
#include "vhdController.h"
#include "DataStructure.h"

using namespace std;

TestController::TestController() {
    //
}

bool TestController::testVhd() {
    vhdController vhdController_test;
    perror(vhdController_test.create_vhd() ? "[ok]vhd_create_success." : "[err]vhd_create_failed.");
    //perror(vhdController_test.del_vhd() ? "[ok]vhd_del_success." : "[err]vhd_del_failed.");
    return true;
}

bool TestController::testvhdController(vhdController &_vhd_) {
    if (!_vhd_.Exists_vhd()) {
        cout << "create vhd" << endl;
        if (_vhd_.create_vhd())
            cout << "the vhd has been create" << endl;
        else
            cout << "create error" << endl;
    }
    if (_vhd_.load_vhd())
        cout << "vhd load done" << endl;
    else
        cout << "vhd load error" << endl;
    char wbuff[BLOCK_SIZE];
    memset(wbuff, 0, BLOCK_SIZE);
    strcpy(wbuff, "test0");
    _vhd_.write_vhd(wbuff, 0);
    strcpy(wbuff, "test1");
    _vhd_.write_vhd(wbuff, 1);
    char rdbuff[BLOCK_SIZE];
    _vhd_.read_vhd(rdbuff, 0);
    string rdbuff_s(rdbuff);
    if (rdbuff_s == "test0") {
        _vhd_.read_vhd(rdbuff, 1);
        string rdbuff_s(rdbuff);
        if (rdbuff_s == "test1")
            return true;
        else {
            cout << "test fails" << endl;
            return false;
        }
    } else {
        cout << "test fails 2" << endl;
        return false;
    }
}

bool TestController::testuserController(userController &_user_) {
    user my_user;
    user my_user2;
    vhdController vhd_Controller;
    my_user.id = 100;
    string name = "wz";
    string password = "9264944wz";
    my_user.name = (char *) name.c_str();
    my_user.password = (char *) password.c_str();
    my_user.authority = 1;
    _user_.addUser(my_user);
    my_user2 = my_user;
    my_user2.id = 2;
    _user_.addUser(my_user2);
    _user_.wirteUser();
    _user_.loadUser();
    _user_.showUser();
    _user_.delUser(2);
    _user_.showUser();
    _user_.addUser(my_user);
    _user_.showUser();
    _user_.UserloginIN();
    return true;
}

int TestController::testiNodeController(iNodeController &_iNode_, fsController _fs_) {
    iNode myiNode, myiNode2;
    vhdController _vhd_;
    myiNode.i_ino = 1;
    myiNode.i_uid = 0;
    myiNode.i_gid = 1;
    //myiNode.i_addr[1, 2, 3];
    myiNode.i_block = 1;
    myiNode.i_bytes = 234;
    myiNode.i_mode = 1;
    myiNode.i_sb = 1;
    myiNode.i_nlink = 2;
    //cout << myiNode.i_mode << endl;
    _iNode_.write_iNode(myiNode);
    char *rdbuf = new char[BLOCK_SIZE];
    //cout << _fs_.get_next_free_list_index()<<endl;
    _vhd_.load_vhd();
    _vhd_.read_vhd(rdbuf, 0);
    memcpy((char *) &myiNode2, rdbuf, BLOCK_SIZE);
    cout << myiNode2.i_bytes << endl;
    _iNode_.read_iNode(0, myiNode2);
    cout << myiNode2.i_nlink << endl;
    cout << _iNode_.find_iNode_by_uid(0) << endl;
    cout << _iNode_.find_iNode_by_uid(1) << endl;
    delete[] rdbuf;
    cout << "return" << endl;
    return 0;
}

bool TestController::testdataController(dataBController &_dataB_) {
    _dataB_.init_datab();
    sB superBlock_;
    superBlock_ = _dataB_.get_sB();
    //cout<<superBlock_.DstackDepth<<endl;
    int temp, temp2, temp3;
    temp = _dataB_.alloc_datab();
    temp2 = _dataB_.alloc_datab();
    //_dataB_.show();

    _dataB_.release_datab(temp2);
    _dataB_.release_datab(temp);
    temp3 = _dataB_.alloc_datab();
    cout << temp << endl;
    cout << temp2 << endl;
    cout << temp3 << endl;
    _dataB_.show();
    return true;
}

#include <iostream>
#include <fstream>
#include "TestController.h"
#include "vhdController.h"
#include "iNodeController.h"
#include "DataStructure.h"
#include "Params.h"
#include "dataBController.h"

using namespace std;

//
//int nextUid;
//int nextGid;
//bool isLogin;
//int root_dir;
//int cur_dir;
//
//char *cur_username = new char[110];
//char *cur_groupname = new char[110];
//char *cur_dir_name = new char[310];
//char * cur_usr_dir_name = new char[110];
//int d_bitmap_beg = 0 ;
//
//const int i_beg = d_bitmap_beg + 20 * BLOCK_SIZE;
//
//char *buffer = new char[10 * 1024 * 1024];

int main() {
    /* Test Utils */
    vhdController _vhd_;
    fsController _fs_;
    iNodeController my_iNode;
    dataBController _dataB;
    userController _user_;
    //_fs_.init_iNode_free_list();
    if (TestController::testvhdController(_vhd_))
        cout << "[ok] vhdController Test Checked." << endl;
    if (TestController::testdataController(_dataB));
        cout<<"[ok] dataController test checked"<<endl;
    //if (TestController::testuserController(_user_))
    //   cout << "[ok] userController Test Checked." << endl;
    //bool k;
    //k=(TestController::testiNodeController(my_iNode, _fs_));
    return 0;
}

/*int test() {
    //
    vhdController _vhdc;
    fsController _fsc;
    // create if not exist
    if (!_vhdc.Exists_vhd()) {
        _vhdc.create_vhd();

        _vhdc.load_vhd();

        // initialize params
        nextUid = 0;
        nextGid = 0;
        isLogin = false;

        // initialize root access
        strcpy(Cur_User_Name, "root");
        strcpy(Cur_Group_Name, "root");

        // get hostname
        // TO-DO


        // get iNode, current dir and name
        root_dir = i_beg;
        cur_dir = root_dir;
        strcpy(Cur_Dir_Name, "./");

        // format
        if (!_fsc.formatfs()) {
            perror("[err] format failed.");
            return 0;
        }

        cout << "[ok] format is done!" << endl;
        cout << "welcome to new fs." << endl;

        if (!_fsc.mountfs()) {
            perror("[err] mount failed.");
            return 0;
        }
    } else {
        // if vhd files existed
        _vhdc.load_vhd();
        _vhdc.read_vhd(buffer, 0);

    }

    // initialize params
    nextUid = 0;
    nextGid = 0;
    isLogin = false;

    // initialize root access
    strcpy(cur_username, "root");
    strcpy(cur_groupname, "root");

    // get hostname
    // TO-DO :include <limits.h>



    // get iNode, current dir and name
    root_dir = i_beg;
    cur_dir = root_dir;
    strcpy(cur_dir_name, "./");

    if (!_fsc.mountfs()) {
        perror("[err] mount failed.");
        return 0;
    }

    while( _fsc.mountfs()) {
        if(isLogin) {
            // TO-DO
            // print user/client info and print cmd notification

            auto * cur_info = new char[100];
            if(strstr(cur_dir_name, cur_usr_dir_name) == nullptr) {

            }
            cout << "[ok] Login successfully." << endl;
            // _fsc.loadcmd();
        }
        else {
            // TO-DO
            // print login attention
            cout << " [err] Login first!" << endl;
        }
    }
    return 0;
}*/





//
// Created by Xingqi Tang on 2018/7/5.
//
#include "fsController.h"

fsController::fsController() {
//    this->user_num = 0;
    //  this->root_path = ROOT_PATH;
}


bool fsController::mountfs() {
    this->init_iNode_free_list();
}


bool fsController::loadfs() {

}

bool fsController::init_iNode_free_list() {
    next_free_list_index = 0;
    this->fs.iNodeSb.stackDepth = 0;
    while (!this->fs.iNodeSb.iSbStack.empty()) {
        this->fs.iNodeSb.iSbStack.pop();
    } // if iNode superblock is not empty, pop them out.

    for (int i = 0; i < INDEX_LIST_NUMBER; i++) {
        for (int j = 0; j < INDEX_LIST_LENGTH; j++) {
            fs.i_free_list[i][j] = i * INDEX_LIST_LENGTH + INDEX_LIST_LENGTH - j - 1;
        }
    }
    return true;
}

bool fsController::init_user_info() {
    this->fs.user_info[user_num].name = "root";
    this->fs.user_info[user_num++].password = "0831";
    return true;
}

bool fsController::init_dentry_info() {
    this->fs.dentry.fliename = "root";
    this->fs.dentry.i_ino = 0; //first iNode allocated from now on.
    //this->fs.dentry
}





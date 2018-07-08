//
// Created by Xingqi Tang on 2018/7/5.
//
#include "iNodeController.h"

bool iNodeController::write_iNode(iNode i_inode, fsController &fs_Controller) {
    this->inode = i_inode;
    char buf[BLOCK_SIZE];
    memcpy(buf, (char *) &i_inode, BLOCK_SIZE);
    vhdController vhd_Controller;
    vhd_Controller.load_vhd();
    vhd_Controller.write_vhd(buf, fs_Controller.get_next_free_list_index());
    return true;
}

bool iNodeController::del_iNode(iNode i_Node) {
    int id;
    id = i_Node.i_ino;
    vhdController vhd_Controller;
    vhd_Controller.load_vhd();
    vhd_Controller.del_vhd(id);
    return true;
}

bool iNodeController::read_iNode(int id, iNode &i_Node) {
    char buf[BLOCK_SIZE];
    vhdController vhd_Controller;
    vhd_Controller.load_vhd();
    vhd_Controller.read_vhd(buf, id);
    memcpy((char *) &i_Node, buf, sizeof(buf));
    return true;
}

int iNodeController::find_iNode_by_uid(int uid) {
    for (size_t i = INODE_START; i <= INODE_END; i++) {
        char buf[sizeof(this->inode)];
        vhdController vhd_Controller;
        vhd_Controller.load_vhd();
        vhd_Controller.read_vhd(buf, i, BLOCK_SIZE);
        memcpy((char *) &(this->inode), buf, BLOCK_SIZE);
        if (this->inode.i_uid == uid)
            return inode.i_ino;
    }
    return -1;
}
/*bool iNodeController::modify_iNode(iNode &i_Node, vhdController &vhd_Controller1) {
    //char *i_Node_tmp;
    int id;
    id=i_Node.i_ino;
    vhd_Controller1.read_vhd(i_Node_tmp,id,BLOCK_SIZE);
}*/

//
// Created by Xingqi Tang on 2018/7/5.
//

#include "iNodeController.h"
#include "DataStructure.h"
#include "vhdController.h"
#include "fsController.h"
bool iNodeController::write_iNode(iNode i_inode, vhdController &vhd_Controller, fsController &fs_Controller) {
    this->inode=i_inode;

    vhd_Controller.write_vhd(this->inode,fs_Controller.get_next_free_list_index());
    return true;
}
bool iNodeController::del_iNode() {

}
//
// Created by Xingqi Tang on 2018/7/5.
//
#include "iNodeController.h"

bool iNodeController::write_iNode(iNode i_inode) {
    this->inode = i_inode;
    char buf[BLOCK_SIZE];
    memcpy(buf, (char *) &i_inode, BLOCK_SIZE);
    vhdController vhd_Controller;
    vhd_Controller.load_vhd();
    vhd_Controller.write_vhd(buf, this->next_i_free_list_index);
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
    for (size_t i = INODE_BEG; i <= INODE_END; i++) {
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
iNode iNodeController::get_iNode_by_ino(int id) {
    for (size_t i = INODE_BEG;i<=INODE_END;i++){
        char buf[sizeof(this->inode)];
        vhdController vhds;
        vhds.load_vhd();
        vhds.read_vhd(buf ,i);
        memcpy((char *)&(this->inode),buf,BLOCK_SIZE);
        if (this ->inode.i_ino==id){
            return this->inode;
        }
    }
}
iNode iNodeController::get_iNode_by_id(int id) {
    vhdController vhds;
    vhds.load_vhd();
    char buf[sizeof(this->inode)];
    vhds.read_vhd(buf ,id);
    memcpy((char *)&(this->inode),buf,BLOCK_SIZE);
    return this->inode;
}
int iNodeController::alloc_iNode() {
/* find free iNode i_ino in i_bitmap, return i_ino */
    fsController fsc;
    vhdController _vhdc;
    _vhdc.load_vhd();
    if (_vhdc.load_vhd()) {
        if (fsc.get_fs().superblock.s_free_inode_num == 0) {
            cout << "No free i_ino for allocation." << endl;
            return -1;
        } else {
            size_t i;
            for (i = 0; i < fsc.get_fs().superblock.s_inode_num; i++) {
                if (fsc.get_fs().superblock.i_bitmap[i] == 0)
                    break;
            }
            // update superblocks
            fileSystem _fs;
            _fs = fsc.get_fs();
            _fs.superblock.s_free_inode_num--;
            fsc.is_get_fs(_fs);

            auto *buf = new char[sizeof(fsc.get_fs().superblock)];
            sB tmp_sb = fsc.get_fs().superblock;
            memcpy(buf, (char *) &(tmp_sb), sizeof(fsc.get_fs().superblock));
            _vhdc.write_vhd(buf, ISB, sizeof(tmp_sb));

            // update i_bitmap
            bool tmp_i_bitmap = fsc.get_fs().superblock.i_bitmap[i];
            _vhdc.write_vhd((char *) &(tmp_i_bitmap), INODE_BEG, sizeof(bool));


            return INODE_BEG + (int) i * (fsc.get_fs().superblock.s_inode_size);
        }
    } else {
        return -1;
    };
}

bool iNodeController::free_iNode(int addr){
    fsController fsc;
    vhdController _vhdc;
    _vhdc.load_vhd();
    fileSystem _fs;
    _fs = fsc.get_fs();
    //check whether it's the right address
    if ((addr - INODE_BEG) % (_fs.superblock.s_inode_size) != 0){
        perror("[err] wrong inode start address.");
        return false;
    }
    auto ino =(addr - INODE_BEG) /(_fs.superblock.s_inode_size);
    if(_fs.superblock.i_bitmap[ino] == 0){
        perror("[err] the inode is not used.");
        return false;
    }
    // update superblock
    fileSystem _fs;
    _fs = fsc.get_fs();
    _fs.superblock.s_free_inode_num++;
    fsc.is_get_fs(_fs);

    //update inode
    iNode tmp_i_node = {0};

    //update inode bitmap
    bool tmp_i_bitmap = fsc.get_fs().superblock.i_bitmap[ino];
    _vhdc.write_vhd((char *) &(tmp_i_bitmap), INODE_BEG, sizeof(bool));
}


//
// Created by Xingqi Tang on 2018/7/5.
//
#include "fsController.h"
#include "vhdController.h"
#include "Params.h"
#include "DataStructure.h"

fsController::fsController() {
//    this->user_num = 0;
    //  this->root_path = ROOT_PATH;
}

bool fsController::init_superblock() {
    this->fs.superblock.s_inode_num = INODE_NUM;
    this->fs.superblock.s_block_num = BLOCK_NUM;
    this->fs.superblock.s_superblock_size = sizeof(superblock);
    this->fs.superblock.s_inode_size= INODE_SIZE;
    this->fs.superblock.s_datab_size = BLOCK_SIZE;
    this->fs.superblock.s_free_inode_num = INODE_NUM;
    this->fs.superblock.s_free_datab_num = BLOCK_NUM;
    this->fs.superblock.s_blocks_per_group = INDEX_LIST_NUMBER;
//    this->fs.superblock.s_free_addr = Block_StartAddr;	//空闲块堆栈指针为第一块block
//    this->fs.superblock.s_Superblock_StartAddr = Superblock_StartAddr;
//    this->fs.superblock.s_BlockBitmap_StartAddr = BlockBitmap_StartAddr;
//    this->fs.superblock.s_InodeBitmap_StartAddr = InodeBitmap_StartAddr;
//    this->fs.superblock.s_Block_StartAddr = Block_StartAddr;
//    this->fs.superblock.s_Inode_StartAddr = Inode_StartAddr;
    return true;
}

bool fsController::writeSbIntoVHD() {
    vhdController _vhdc;
    char *buf = new char[sizeof(this->fs.superblock)];
    memcpy(buf, (char *)&this->fs.superblock, sizeof(this->fs.superblock));
    _vhdc.load_vhd();
    _vhdc.write_vhd(buf,INODE_BEG,sizeof(this->fs.superblock));
    return true;
}

bool fsController::init_i_bitmap(){
    vhdController _vhdc;
    char *buf = new char[sizeof(this->fs.superblock.i_bitmap)];
    memset(this->fs.superblock.i_bitmap, 0 , sizeof(this->fs.superblock.i_bitmap));
    memcpy(buf, this->fs.superblock.i_bitmap, sizeof(this->fs.superblock.i_bitmap));
    _vhdc.load_vhd();
    _vhdc.write_vhd(buf, INODE_BEG + 1);
    delete[] buf;
    return true;
}

bool fsController::mountfs() {
    this->init_iNode_free_list();
    return true;
}


bool fsController::loadfs() {
    return true;
}

bool fsController::formatfs() {
    //vhdController vhd_Controller;
    // initial bb
    // TO-DO

    // initialize super block



    // initialize inode bitmap

    //this->init_i_bitmap();


    // initialize datab bitmap

    // initialize link group manager
//    for(i=BLOCK_NUM/BLOCKS_PER_GROUP-1;i>=0;i--){	//一共INODE_NUM/BLOCKS_PER_GROUP组，一组FREESTACKNUM（128）个磁盘块 ，第一个磁盘块作为索引
//        if(i==BLOCK_NUM/BLOCKS_PER_GROUP-1)
//            superblock->s_free[0] = -1;	//没有下一个空闲块了
//        else
//            superblock->s_free[0] = Block_StartAddr + (i+1)*BLOCKS_PER_GROUP*BLOCK_SIZE;	//指向下一个空闲块
//        for(j=1;j<BLOCKS_PER_GROUP;j++){
//            superblock->s_free[j] = Block_StartAddr + (i*BLOCKS_PER_GROUP + j)*BLOCK_SIZE;
//        }
//        fseek(fw,Block_StartAddr+i*BLOCKS_PER_GROUP*BLOCK_SIZE,SEEK_SET);
//        fwrite(superblock->s_free,sizeof(superblock->s_free),1,fw);	//填满这个磁盘块，512字节
//    }

    // create root dir
    iNode cur;
    //cur.i_ino = i

    return true;

}

bool fsController::init_iNode_free_list() {
    next_i_free_list_index = 0;
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
    //this->fs.user_info[user_num].name = "root";
    //this->fs.user_info[user_num++].password = "0831";
    return true;
}

bool fsController::init_dentry_info() {
    //this->fs.dentry.fliename = "root";
    //this->fs.dentry.i_ino = 0; //first iNode allocated from now on.
    //this->fs.dentry
    return true;
}

fileSystem fsController::get_fs() {
    return this->fs;
}

int fsController::get_next_i_free_list_index() {
    return next_i_free_list_index;
}

bool fsController::is_get_fs(fileSystem _fs) {
    this->fs=_fs;
    return true;
}





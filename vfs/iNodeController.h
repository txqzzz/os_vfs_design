//
// Created by Xingqi Tang on 2018/7/5.
//
#ifndef VFS_INODECONTROLLER_H
#define VFS_INODECONTROLLER_H

#include "iNodeController.h"
#include "vhdController.h"
#include "fsController.h"
#include "DataStructure.h"

class iNodeController {
public:
    bool write_iNode(iNode i_inode);
    bool del_iNode(iNode i_Node);
    bool read_iNode(int id, iNode &i_Node);
    int find_iNode_by_uid(int id);
    int alloc_iNode();
    int release_iNode();
    iNode get_iNode_by_ino(int id);
    iNode get_iNode_by_id(int id);
private:
    iNode inode;
    int next_i_free_list_index;
};
//    int i_ino;
//    vector i_hash;
//    list i_sb_list;
//    int i_mode;
//    int i_uid;
//    int i_nlink;
//    int g_gid;
//    int i_size;
//    string i_atime;
//    string i_mtime;
//    int i_block;
//    int i_bytes;
//    int i_sb;
//    int i_addr[1024];//index array,lengths need discuss
#endif //VFS_INODECONTROLLER_H

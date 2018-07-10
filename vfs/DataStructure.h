//
// Created by Xingqi Tang on 2018/7/6.
//


#ifndef VFS_DATA_STRUCTURE_H
#define VFS_DATA_STRUCTURE_H

#include "Params.h"
#include <stack>
#include <list>
#include <string>
#include <vector>

using namespace std;

struct bootstrapBlock {
    bootstrapBlock() {
        string welcomeMsg = "Welcome to vfs!";
        cout << welcomeMsg << endl;
    }
};

struct sB {
    int get_s_free_inode_num() {
        return s_free_inode_num;
    }
    int s_inode_num;
    int s_block_num;
    int s_superblock_size;
    int s_inode_size;
    int s_datab_size;
    int s_free_inode_num;
    int s_free_datab_num;
    int s_blocks_per_group;
    int s_free_addr;
    int s_Superblock_StartAddr;
    int s_BlockBitmap_StartAddr;
    int s_InodeBitmap_StartAddr;
    int s_Block_StartAddr;
    int s_Inode_StartAddr;
    int DstackDepth;
    int stack[DATAB_END-DATAB_BEG];
    bool i_bitmap[512]; //inode_num = inode_end - inode_beg + 1
    bool d_bitmap[15869];
};

struct iSb {
    int stackDepth;
    stack<int> iSbStack;
};
/*struct dSb {
    int stackDepth;
    stack<int> dSbStack;
};*/

struct iNode {
    int i_ino;
    vector<int> i_hash;
    list<int> i_sb_list;
    int i_mode;
    int i_parent;
    char i_name[15];
    int i_uid;
    int i_nlink; //hard link num;
    int i_gid;
    int i_size;
    time_t atime;
    time_t mtime;
    char i_mtime[15];
    int i_block;
    int i_bytes;
    int i_sb;
    int i_addr[1024];//index array,lengths need discuss
    //i_ino/i_hash/i_sb_list/i_mode/i_uid/i_nlink/i_gid/i_size/i_atime/i_mtime/i_ctime(*)/i_blocks/i_bytes/i_sb
};
//dentry_obj: filename/i_ino/`
//            file_obj: dir_entry/file_mode/this.filename/this.ino
struct Dentry {
    char name[FILENAME_MAX_LEN];
    int i_ino;
};

struct user {
    int id;
    char *name = new char[10];
    char *password = new char[10];
    int authority;

//    ~user()
//    {
//        delete[] name;
//        delete[] password;
//    }

};

struct fileSystem {
    bootstrapBlock bB;
    iSb iNodeSb;
    sB superblock;
    Dentry dentry;
    user user_info[MAX_USER_NUMBER];
    int i_free_list[INDEX_LIST_NUMBER][INDEX_LIST_LENGTH];
    int d_free_list[DATAB_LIST_NUMBER][DATAB_LIST_LENGTH];
};


#endif //VFS_DATA_STRUCTURE_H

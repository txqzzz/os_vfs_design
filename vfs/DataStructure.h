//
// Created by Xingqi Tang on 2018/7/6.
//
#include "Params.h"
#include <stack>
#include <list>
#include <string>
#ifndef VFS_DATA_STRUCTURE_H
#define VFS_DATA_STRUCTURE_H
typedef struct BootstrapBlock{
    BootstrapBlock(){
        cout<<"welcome to VFS!"<<endl;
    }
};
typedef struct iSb{
    int stackDepth;
    stack< int > iSbStack;
};

typedef struct dSb{
    int stackDepth;
    stack < int > dSbStack;
}

typedef struct iNode{
    int i_ino;
    vector i_hash;
    list i_sb_list;
    int i_mode;
    int i_uid;
    int i_nlink;
    int g_gid;
    int i_size;
    string i_atime;
    string i_mtime;
    int i_block;
    int i_bytes;
    int i_sb;
    int i_addr[1024];//index array,lengths need discuss
    //i_ino/i_hash/i_sb_list/i_mode/i_uid/i_nlink/i_gid/i_size/i_atime/i_mtime/i_ctime(*)/i_blocks/i_bytes/i_sb
};
//dentry_obj: filename/i_ino/
//            file_obj: dir_entry/file_mode/this.filename/this.ino
typedef struct Dentry{
    string fliename;
    int i_ino;
};

typedef struct user{
    int id;
    string name;
    string password;
    int authority;
};

struct fileSystem {
    iSb iNodeSb;
    dSb dentrySb;
    Dentry dentry;
    user user_info[MAX_USER_NUMBER];
    int i_free_list[INDEX_LIST_NUMBER][INDEX_LIST_LENGTH];
    int d_free_list[DENTRY_LIST_NUMBER][DENTRY_LIST_LENGTH];
};

#endif //VFS_DATA_STRUCTURE_H

//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_FSCONTROLLER_H
#define VFS_FSCONTROLLER_H

#include "DataStructure.h"
#include "vhdController.h"

class fsController {
public:
    fsController();

    bool mountfs(); // install + initialize filesystem info
    bool loadfs(); // load super block/dentry block/root dir
    bool formatfs();

    bool init_iNode_free_list();

    bool init_user_info();
    bool init_dentry_info();
    bool init_i_bitmap();
    bool init_superblock();
    bool writeSbIntoVHD();
    int get_next_i_free_list_index();
    bool is_get_fs(fileSystem _fs);

    fileSystem get_fs();


private:
    fileSystem fs;
    iSb isb;
    dSb dsb;
    sB superblock;
    int user_num;
    int cur_dir_inode;
    int next_i_free_list_index;
    string root_path;
    string welcomeMsg;
};

#endif //VFS_FSCONTROLLER_H

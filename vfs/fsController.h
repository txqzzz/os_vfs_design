//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_FSCONTROLLER_H
#define VFS_FSCONTROLLER_H
#include "DataStructure.h"

class fsController {
public:
    fsController();
    bool mountfs(); // install + initialize filesystem info
    bool loadfs(); // load super block/dentry block/root dir
    bool formatfs();
    bool init_iNode_free_list();
    bool init_user_info();
    bool init_dentry_info();
    int get_next_free_list_index() {
        return next_free_list_index;
    }

private:
    fileSystem fs;
    int user_num;
    int cur_dir_inode;
    int next_free_list_index;
    string root_path;
};

#endif //VFS_FSCONTROLLER_H

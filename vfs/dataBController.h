//
// Created by Xingqi Tang on 2018/7/9.
//

#ifndef OS_VFS_DESIGN_DATABCONTROLLER_H
#define OS_VFS_DESIGN_DATABCONTROLLER_H


#include "fsController.h"

class dataBController {
public:
    dataBController();
    int  alloc_datab(fsController &_fs);
    bool init_datab_free_list(fsController &_fs);
    bool d_free_listintoDsbStack(fsController &_fs);
    bool release_datab(fsController &_fs);
private:
    unsigned int datab_free_num;
    unsigned int next_d_free_list_index;
};


#endif //OS_VFS_DESIGN_DATABCONTROLLER_H

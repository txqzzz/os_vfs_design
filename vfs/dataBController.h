//
// Created by Xingqi Tang on 2018/7/9.
//

#ifndef OS_VFS_DESIGN_DATABCONTROLLER_H
#define OS_VFS_DESIGN_DATABCONTROLLER_H


#include "fsController.h"
#include "vhdController.h"
class dataBController {
public:
    dataBController();
    ~dataBController();
    int  alloc_datab();
    bool init_datab();
    bool load_Sb();
    bool load_Sb(int id);
    bool save_Sb();
    bool save_Sb(int id);
    bool release_datab(int id);
    bool setfullflag();
    bool show();
    sB get_sB();
private:
    sB superblock;
    //vhdController _vhd_;
    int superblockid;
    unsigned int datab_free_num;
    unsigned int next_d_free_list_index;

};


#endif //OS_VFS_DESIGN_DATABCONTROLLER_H

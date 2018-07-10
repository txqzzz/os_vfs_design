//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_FSCONTROLLER_H
#define VFS_FSCONTROLLER_H

#include "DataStructure.h"
#include "vhdController.h"
#include "dataBController.h"
#include "iNodeController.h"

class fsController {
public:
    fsController();

    bool mountfs(); // install + initialize filesystem info
    bool loadfs(); // load super block/dentry block/root dir
    bool formatfs();

    //bool init_iNode_free_list();


    bool init_user_info();

    bool init_dentry_info();

    bool init_i_bitmap();

    bool init_superblock();

    bool writeSbIntoVHD();

    int get_next_i_free_list_index();

    bool is_get_fs(fileSystem _fs);

    fileSystem get_fs();

    bool GetBIDByFOff(const iNode &cur, int foff, int *dst);

    bool ReadFileToBuf(const iNode &cur, int start, int len, char *buf);

    bool AppendBlocksToFile(iNode &cur, int blockCnt);

    bool WriteFileFromBuf(iNode &cur, int start, int len, char *buf);

    bool DeleteFile(const iNode &cur);

    bool create_empty_Flie(iNode& curDir, char* name, char mode, int ownerUid, iNode* rst);
    bool GetContentInDir(const iNode& curDir, Dentry* rst);
    bool FindContentInDir(const Dentry* DirSet, const int len, const char* name, int* rst);

private:
    fileSystem fs;
    iSb isb;
    sB superblock;
    int user_num;
    int cur_dir_inode;
    string root_path;
    string welcomeMsg;
};

#endif //VFS_FSCONTROLLER_H

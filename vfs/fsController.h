//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_FSCONTROLLER_H
#define VFS_FSCONTROLLER_H

#include "DataStructure.h"
#include "vhdController.h"
#include "dataBController.h"
#include "iNodeController.h"
#include "dentryController.h"

class fsController {
public:
    fsController();
    //format
    bool mountfs(); // install + initialize filesystem info
    bool loadfs(); // load super block/dentry block/root dir
    bool formatfs();

    // common operations
    bool copy(const iNode& src, iNode& des, char* name, int uid);
    bool move(iNode& src, iNode& des, char* name);
    bool changeMode(iNode& cur, char mode);
    bool create_empty_Flie(iNode& curDir, char* name, char mode, int ownerUid, iNode* rst); //touch

    // Utils
    void get_AbspluteDir(const iNode& cur, char* dir);
    bool parsePath(const iNode& curDir, char* path, bool last, iNode* rst);
    fileSystem get_fs();
    bool is_get_fs(fileSystem _fs);
    // Rubbish
    bool init_user_info();
    bool init_dentry_info();
    bool init_i_bitmap();
    bool init_superblock();
    bool writeSbIntoVHD();
    // inode ops
    bool GetiNodeByID(int id, iNode* rst);
    bool SaveiNodeByID(int id, const iNode& inode);
    // file ops
    bool ReadFileToBuf(const iNode &cur, int start, int len, char *buf);
    bool GetBIDByFOff(const iNode &cur, int foff, int *dst);
    bool AppendBlocksToFile(iNode &cur, int blockCnt);
    bool WriteFileFromBuf(iNode &cur, int start, int len, char *buf);
    bool DeleteFile(const iNode &cur);
    bool CopyFile(const iNode& src, iNode& des, char* name, int uid);

    //dir ops
    bool GetContentInDir(const iNode& curDir, Dentry* rst);
    bool FindContentInDir(const Dentry* DirSet, const int len, const char* name, int* rst);
    bool DeleteDir(const iNode& cur);
    bool CopyDir(const iNode& src, iNode&des, char *name, int uid);


    //block operations
    bool DeleteDirectBlocks(const iNode& cur);
    bool DeleteIndir1Blocks(const iNode& cur);
    bool DeleteIndir2Blocks(const iNode& cur);

    // TO-DO


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

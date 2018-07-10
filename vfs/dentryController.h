//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_DENTRYCONTROLLER_H
#define VFS_DENTRYCONTROLLER_H

#include "DataStructure.h"
#include "vhdController.h"
#include "fsController.h"
#include "iNodeController.h"


class dentryController {
public:
    dentryController();

    //dir ops
    bool init_DirSFDList(iNode& cur,int parent_ino);
    bool create_RootDir();
    bool create_SubDir(iNode& curDir, char* name, char mode, int ownerUid, iNode* rst);


    // SFD operation
    bool delete_SFDEntry(const iNode& cur);
    bool AppendSFDEntry(iNode& parent, const Dentry& newSFD);

    bool GetContentInDir(const iNode& curDir, Dentry* rst);
    bool FindContentInDir(const Dentry* DirSet, const int len, const char* name, int* rst);
    bool InitDirSFDList(iNode& cur,int parent_ino);
    bool CreateRootDir();
    bool CreateSubDir(iNode& curDir, char* name, char mode, int ownerUid, iNode* rst);
    bool DeleteDir(const iNode& cur);
    bool WriteFlieFromBuf(iNode &cur,int start,int len , char *buf);
};


#endif //VFS_DENTRYCONTROLLER_H

//
// Created by Xingqi Tang on 2018/7/5.
//
#include "dentryController.h"

<<<<<<< HEAD
bool dentryController::dentryController() {
    return;
}

bool dentryController::init_DirSFDList(iNode &cur, int parent_ino) {

=======
bool dentryController::InitDirSFDList(iNode &cur, int parent_ino) {
>>>>>>> parent of 7736a5a... 规范化了一些函数
    Dentry sfdList[2];
    // Dot denode itself
    strcpy(sfdList[0].name, DOT);
    sfdList[0].i_ino = cur.i_ino;
    // Dot Dot denode its parent
    strcpy(sfdList[1].name, DOTDOT);
    sfdList[1].i_ino = parent_ino;
    return true;
}
bool dentryController::CreateRootDir()
{
    // Create RootDir iNode
    iNode rootiNode;
    rootiNode.i_ino = ROOTDIRiNODE;
    rootiNode.i_parent = ROOTDIRiNODE;
    strcpy(rootiNode.i_name, "/");
    rootiNode.i_mode = DIRFLAG | OWNER_RFLAG | OWNER_WFLAG | OWNER_XFLAG | PUBLIC_RFLAG |
                     PUBLIC_XFLAG;
    rootiNode.i_nlink = 1;
    rootiNode.i_uid = ROOT_UID;
    rootiNode.i_size = 0;
    rootiNode.atime = rootiNode.mtime = time(nullptr);
    rootiNode.i_block = 0;
    rootiNode.i_bytes = 0;
    iNodeController _iNode;
    fsController _fs;
    if (!this->InitDirSFDList(rootiNode, ROOTDIRiNODE))
        return false;
    if (!_iNode.write_iNode(rootiNode))
        return false;
    return true;
}
<<<<<<< HEAD

bool dentryController::create_SubDir(iNode& curDir, char* name, char mode, int ownerUid, iNode* rst) {
=======
bool dentryController::CreateSubDir(iNode& curDir, char* name, char mode, int ownerUid, iNode* rst)
{
>>>>>>> parent of 7736a5a... 规范化了一些函数
    fsController fsC;
    iNodeController iNC;
    fsC.create_empty_Flie(curDir, name, mode | DIRFLAG, ownerUid, rst);
    this->InitDirSFDList(*rst, curDir.i_ino);
    iNC.write_iNode(curDir);
    return true;
}
<<<<<<< HEAD


bool dentryController::delete_SFDEntry(const iNode& cur) {
    fsController fsc;
    // Get parent iNode
    fsController fsc;
    iNode piNode;
    if (!fsc.GetiNodeByID(cur.i_parent, &piNode)) return false;
    if (!(piNode.i_mode & DIRFLAG)) return false;
    // Get SFD List
    Dentry* SFDList = new Dentry[piNode.i_size / sizeof(Dentry)];
    if (!fsc.ReadFileToBuf(piNode, 0, piNode.i_size, (char*)SFDList))
=======
/*bool dentryController::DeleteDir(const iNode& cur)
{
    // Only for dir
    if (!(cur.i_mode & DIRFLAG)) return false;
    // Readin SFD List
    iNode nowiNode;
    Dentry* SFDList = new Dentry[cur.i_size / sizeof(Dentry)];
    if (!GetContentInDir(cur, SFDList))
>>>>>>> parent of 7736a5a... 规范化了一些函数
    {
        delete[] SFDList;
        return false;
    }
    // Iterate over SFD List
    // Note: it's forbidden to create a hard link of a dir,
    // so we don't need to consider that.
    for (int i = 0; i < (int)(cur.i_size / sizeof(Dentry)); i++)
    {
        // Ignore . or ..
        if (strcmp(SFDList[i].name, DOT) == 0 ||
            strcmp(SFDList[i].name, DOTDOT) == 0)
            continue;
        // Readin iNode
        if (!this->GetiNodeByID(SFDList[i].inode, &nowiNode))
        {
            delete[] SFDList;
            return false;
        }
        if (nowiNode.i_mode & DIRFLAG)    // Delete subdir recursively
        {
            if (!this->DeleteDir(nowiNode))
            {
                delete[] SFDList;
                return false;
            }
        }
        else    // Delete regular file
        {
            if (!this->DeleteFile(nowiNode))
            {
                delete[] SFDList;
                return false;
            }
        }
    }
    // Delete SFD in parent
    if (!this->DeleteSFDEntry(cur))
    {
        delete[] SFDList;
        return false;
    }
    // Recycle iNode block
    if (!this->ifbc.Recycle(cur.bid))
    {
        delete[] SFDList;
        return false;
    }
    delete[] SFDList;
    return true;
}
 */
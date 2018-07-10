//
// Created by Xingqi Tang on 2018/7/5.
//
#include "dentryController.h"

bool dentryController::dentryController() {
    return;
}

bool dentryController::init_DirSFDList(iNode &cur, int parent_ino) {
    Dentry sfdList[2];
    // Dot denode itself
    strcpy(sfdList[0].name, DOT);
    sfdList[0].i_ino = cur.i_ino;
    // Dot Dot denode its parent
    strcpy(sfdList[1].name, DOTDOT);
    sfdList[1].i_ino = parent_ino;
    return true;
}
bool dentryController::create_RootDir() {
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
    if (!this->init_DirSFDList(rootiNode, ROOTDIRiNODE))
        return false;
    if (!_iNode.write_iNode(rootiNode))
        return false;
    return true;
}
bool dentryController::create_SubDir(iNode& curDir, char* name, char mode, int ownerUid, iNode* rst) {
    fsController fsC;
    iNodeController iNC;
    fsC.create_empty_Flie(curDir, name, mode | DIRFLAG, ownerUid, rst);
    this->init_DirSFDList(*rst, curDir.i_ino);
    iNC.write_iNode(curDir);
    return true;
}

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
    {
        delete[] SFDList;
        return false;
    }
    // Delete target SFD
    int newp = 0;
    Dentry* newSFDList = new Dentry[piNode.i_size / sizeof(Dentry) - 1];
    bool finded = false;
    for (int i = 0; i < (int)(piNode.i_size / sizeof(Dentry)); i++)
    {
        if (strcmp(SFDList[i].name, cur.i_name) == 0)
        {
            finded = true;
            continue;
        }
        else
        {
            memcpy((char*)&newSFDList[newp], (char*)&SFDList[i], sizeof(Dentry));
            newp++;
        }
    }
    if (!finded)
    {
        delete[] SFDList;
        delete[] newSFDList;
        return false;
    }
    // Save new SFD List
    if (!fsc.WriteFileFromBuf(piNode, 0, piNode.i_size - sizeof(Dentry), (char*)newSFDList))
    {
        delete[] SFDList;
        delete[] newSFDList;
        return false;
    }
    // Update parent iNode
    piNode.i_size -= sizeof(Dentry);
    if (!fsc.SaveiNodeByID(piNode.i_ino, piNode))
    {
        delete[] SFDList;
        delete[] newSFDList;
        return false;
    }
    delete[] newSFDList;
    delete[] SFDList;
    return true;
}
bool dentryController::AppendSFDEntry(iNode& parent, const Dentry& newSFD) {
    // Only for dir
    fsController fsc;

    if (!(parent.i_mode & DIRFLAG)) return false;
    // Get SFD List
    Dentry* SFDList = new Dentry[parent.i_size / sizeof(Dentry)];
    if (!fsc.ReadFileToBuf(parent, 0, parent.i_size, (char*)SFDList))
    {
        delete[] SFDList;
        return false;
    }
    // Check if already exists
    int rst = 0;
    if (fsc.FindContentInDir(SFDList, parent.i_size / sizeof(Dentry), newSFD.name, &rst))
    {
        delete[] SFDList;
        return false;
    }
    // Append new SFD entry
    if (!fsc.WriteFileFromBuf(parent, parent.i_size, sizeof(Dentry), (char*)&newSFD))
    {
        delete[] SFDList;
        return false;
    }
    delete[] SFDList;
    return true;
}


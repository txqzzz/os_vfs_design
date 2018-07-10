//
// Created by Xingqi Tang on 2018/7/5.
//
#include "fsController.h"
#include "dataBController.h"
#include <cmath>

fsController::fsController() = default;
/*format*/
bool fsController::mountfs() {
    //this->init_iNode_free_list();
    return true;
}
bool fsController::loadfs() {
    return true;
}
bool fsController::formatfs() {

    // initial bb
    // TO-DO

    // initialize super block
    vhdController _vhdc;
    dataBController _dbc;
    _dbc.setfullflag();
    for (int i = DATAB_BEG; i <= DATAB_END; i++) {
        if (!_dbc.release_datab(i)) {
            perror("[err] release dataBlock failed.");
            return false;
        } else {
            if (!_dbc.save_Sb(i)) {
                perror("[err] save superBlock failed.");
                return false;
            }
        }
    }
    // initialize iNode
    dataBController _idbc;
    _idbc.setfullflag();
    for (int i = INODE_BEG + 1; i <= INODE_END; i++) {
        if (!_dbc.release_datab(i)) {
            perror("[err] release dataBlock failed.");
            return false;
        } else {
            if (!_dbc.save_Sb(i)) {
                perror("[err] save superBlock failed.");
                return false;
            }
        }
    }


    // create root dir
//  if (!create_RootDir) {
//    perror("[err] create root dir failed.");
//    return false;
//  }

    //cur.i_ino = i

    return true;

}

/*common operations*/
bool fsController::copy(const iNode& src, iNode& des, char* name, int uid) {
    if (src.i_mode & DIRFLAG)
        return this->CopyDir(src, des, name, uid);
    else
        return this->CopyFile(src, des, name, uid);
}
bool fsController::move(iNode& src, iNode& des, char* name) {
    // Delete SFD entry from old parent dir
    dentryController DC;
    DC.delete_SFDEntry(src);
    // Append new SFD entry
    Dentry newSFD;
    strcpy(newSFD.name, name);
    newSFD.i_ino = src.i_ino;
    DC.AppendSFDEntry(des, newSFD);
    // Update src iNode
    strcpy(src.i_name, name);
    src.i_parent = des.i_ino;
    if (!this->SaveiNodeByID(src.i_ino, src)) return false;
    // Update src SFD List
    Dentry* SFDList = new Dentry[src.i_size / sizeof(Dentry)];
    if (!this->GetContentInDir(src, SFDList))
    {
        delete[] SFDList;
        return false;
    }
    int rst;
    if (!this->FindContentInDir(SFDList, src.i_size / sizeof(Dentry), DOTDOT, &rst))
    {
        delete[] SFDList;
        return false;
    }
    SFDList[rst].i_ino = des.i_ino;
    if (!this->WriteFileFromBuf(src, 0, src.i_size, (char*)SFDList))
    {
        delete[] SFDList;
        return false;
    }
    delete[] SFDList;
    return true;
}
bool fsController::changeMode(iNode& cur, char mode) {
    cur.i_mode = mode;
    return this->SaveiNodeByID(cur.i_ino, cur);
}
bool fsController::create_empty_Flie(iNode &curDir, char *name, char mode, int ownerUid, iNode *rst) {
    // Read current dir
    auto *dir = new Dentry[curDir.i_size / sizeof(Dentry)];
    this->GetContentInDir(curDir, dir);
    int find;
    // Check if name already exists
    this->FindContentInDir(dir, curDir.i_size / sizeof(Dentry), name, &find);
    // Create new iNode
    iNode my_iNode;    //parent.size += sizeof(SFD);
    dataBController _dataB_;
    int alloc_tmp;
    alloc_tmp = _dataB_.alloc_datab();
    my_iNode.i_parent = curDir.i_ino;
    my_iNode.i_mode = mode;
    my_iNode.i_nlink = 1;
    my_iNode.i_uid = ownerUid;
    my_iNode.i_size = 0;
    my_iNode.atime = my_iNode.mtime = time(nullptr);
    my_iNode.i_block = 0;
    my_iNode.i_bytes = 0;
    strcpy(my_iNode.i_name, name);
    iNodeController iNodeC;
    iNodeC.write_iNode(my_iNode);
    memcpy((char *) rst, (char *) &my_iNode, sizeof(iNode));
    // Append new SFD
    Dentry new_dentry;
    strcpy(new_dentry.name, name);
    new_dentry.i_ino = my_iNode.i_ino;
    if (!this->WriteFileFromBuf(curDir, curDir.i_size, sizeof(Dentry), (char *) &new_dentry)) {
        delete[] dir;
        return false;
    }
    // Clean up
    delete[] dir;
    return true;
}
/*utils*/
void fsController::get_AbspluteDir(const iNode& cur, char* dir) {
    char rootName[] = "/";
    if (strcmp(cur.i_name, rootName) == 0)
    {
        strcpy(dir, rootName);
        return;
    }
    string ts;
    iNode now;
    memcpy((char*)&now, (char*)&cur, sizeof(iNode));
    while (strcmp(now.i_name, "/") != 0)
    {
        string tt(now.i_name);
        ts = "/" + tt + ts;
        this->GetiNodeByID(now.i_parent, &now);
    }
    strcpy(dir, ts.c_str());
    return;
}
bool fsController::parsePath(const iNode& curDir, char* path, bool last, iNode* rst) {
    iNode nowiNode, tmpiNode;
    vhdController vhdc;
    int pp = 0, totLinkCnt = 0;
    if(path[0] == '/')
    {
        if (!this->GetiNodeByID(ROOTDIRiNODE, &nowiNode))
            return false;
        pp++;
    }
    else memcpy((char*)&nowiNode, (char*)&curDir, sizeof(iNode));

    char buf[128 + 10];
    int bufp, target;
    while(true)
    {
        bufp = 0;
        while(path[pp] == '/') pp++;
        while(path[pp] != '/')
        {
            if(path[pp] == '\0') break;
            buf[bufp]=path[pp];
            bufp++;
            pp++;
        }

        // Skip last level if last == false
        if((!last) && (path[pp] == '\0')) break;

        buf[bufp] = '\0';
        if(bufp == 0) break;

        int subDirnum = nowiNode.i_size / sizeof(Dentry);
        Dentry* DirSet = new Dentry[subDirnum];
        if(!GetContentInDir(nowiNode, DirSet))
        {
            delete[] DirSet;
            return false;
        }
        if(FindContentInDir(DirSet, subDirnum, buf, &target))
        {
            vhdc.read_vhd((char*)&tmpiNode,DirSet[target].i_ino,  sizeof(iNode));
            // If the last level of the path is a file -> true
            // else -> false
            if(!(tmpiNode.i_mode & DIRFLAG) && (path[pp] != '\0'))
                return false;
            // Follow symbolic link
            if(tmpiNode.i_mode & SYNLINKFLAG)
            {
                totLinkCnt++;
                if(totLinkCnt > MAXFOLLOWLINK) return false;

                char* synlink = new char[tmpiNode.size];
                if(!ReadFileToBuf(tmpiNode, 0, tmpiNode.size, synlink))
                {
                    delete[] synlink;
                    return false;
                }
                if(synlink[0] == '/')
                {
                    if(!vhdc.ReadBlock(ROOTDIRiNODE, (char*)&nowiNode, sizeof(iNode)))
                    {
                        delete[] synlink;
                        return false;
                    }
                }
                int len = strlen(path);
                char* newpath = new char[len + tmpiNode.size + 10];
                int newlen = 0;
                for(int i = 0; i < len; i++)
                {
                    newpath[newlen++] = path[i];
                    if(i == pp)
                    {
                        for(int j = 0;  j < (int)tmpiNode.size; j++)
                            newpath[newlen++] = synlink[j];
                        newpath[newlen++] = '/';
                    }
                }
                strcpy(path, newpath);
                path[newlen++] = '\0';
                delete[] synlink;
                delete[] newpath;
                continue;
            }
            memcpy((char*)&nowiNode, (char*)&tmpiNode, sizeof(iNode));
        }
        else return false;
    }

    memcpy((char*)rst, (char*)&nowiNode, sizeof(iNode));
    return true;
}
fileSystem fsController::get_fs() {
    return this->fs;
}
bool fsController::is_get_fs(fileSystem _fs) {
    this->fs = _fs;
    return true;
}
/*junks*/
bool fsController::init_superblock() {
    this->fs.superblock.s_inode_num = INODE_NUM;
    this->fs.superblock.s_block_num = BLOCK_NUM;
    this->fs.superblock.s_superblock_size = sizeof(superblock);
    this->fs.superblock.s_inode_size = INODE_SIZE;
    this->fs.superblock.s_datab_size = BLOCK_SIZE;
    this->fs.superblock.s_free_inode_num = INODE_NUM;
    this->fs.superblock.s_free_datab_num = BLOCK_NUM;
    this->fs.superblock.s_blocks_per_group = INDEX_LIST_NUMBER;

//    this->fs.superblock.s_free_addr = Block_StartAddr;	//空闲块堆栈指针为第一块block
//    this->fs.superblock.s_Superblock_StartAddr = Superblock_StartAddr;
//    this->fs.superblock.s_BlockBitmap_StartAddr = BlockBitmap_StartAddr;
//    this->fs.superblock.s_InodeBitmap_StartAddr = InodeBitmap_StartAddr;
//    this->fs.superblock.s_Block_StartAddr = Block_StartAddr;
//    this->fs.superblock.s_Inode_StartAddr = Inode_StartAddr;
    return true;
}
bool fsController::writeSbIntoVHD() {
    vhdController _vhdc;
    char *buf = new char[sizeof(this->fs.superblock)];
    memcpy(buf, (char *) &this->fs.superblock, sizeof(this->fs.superblock));
    _vhdc.load_vhd();
    _vhdc.write_vhd(buf, INODE_BEG, sizeof(this->fs.superblock));
    return true;
}
bool fsController::init_i_bitmap() {
    vhdController _vhdc;
    char *buf = new char[sizeof(this->fs.superblock.i_bitmap)];
    memset(this->fs.superblock.i_bitmap, 0, sizeof(this->fs.superblock.i_bitmap));
    memcpy(buf, this->fs.superblock.i_bitmap, sizeof(this->fs.superblock.i_bitmap));
    _vhdc.load_vhd();
    _vhdc.write_vhd(buf, INODE_BEG + 1);
    delete[] buf;
    return true;
}
bool fsController::init_user_info() {
    //this->fs.user_info[user_num].name = "root";
    //this->fs.user_info[user_num++].password = "0831";
    return true;
}
bool fsController::init_dentry_info() {
    //this->fs.dentry.fliename = "root";
    //this->fs.dentry.i_ino = 0; //first iNode allocated from now on.
    //this->fs.dentry
    return true;
}

/*inode operations*/
bool fsController::GetiNodeByID(int id, iNode* rst){
    vhdController _vhdc;
    _vhdc.load_vhd();
    return _vhdc.read_vhd((char*)rst, id, sizeof(iNode));
}
bool fsController::SaveiNodeByID(int id, const iNode& inode) {
    return this->vhdc.WriteBlock(id, (char*)&inode, sizeof(iNode));
}

/*file operations*/
bool fsController::ReadFileToBuf(const iNode &cur, int start, int len, char *buf) {
    if (start > (int) cur.i_size - 1 || start + len - 1 > (int) cur.i_size - 1)
        return false;
    char *tbuf = new char[len + 2 * BLOCK_SIZE];
    int filep = start;  // File reader pointer
    char *bufp = tbuf;   // Buffer writer pointer
    int tBID;
    vhdController _vhd_;
    _vhd_.load_vhd();
    while (filep < start + len + BLOCK_SIZE - (start + len) % BLOCK_SIZE) {
        if (!GetBIDByFOff(cur, filep, &tBID))
            return false; //读完赋值到tBID
        //if (!this->vhdc.ReadBlock(tBID, bufp, BLOCK_SIZE)) return false;
        if (!_vhd_.read_vhd(bufp, tBID))
            return false;
        filep += BLOCK_SIZE;
        bufp += BLOCK_SIZE;
    }
    char *s = tbuf + (start % BLOCK_SIZE);
    //char* e = tbuf + start % BLOCK_SIZE + len;
    memcpy(buf, s, len);
    delete[] tbuf;
    return true;
}
bool fsController::GetBIDByFOff(const iNode &cur, int offset, int *dst) {
    int startBlock = offset / BLOCK_SIZE;
    vhdController _vhd_;
    _vhd_.load_vhd();
    if (startBlock <= INODE_DIRECT_MAX)
        *dst = cur.i_addr[startBlock];
    else if (startBlock <= (int) (INODE_DIRECT_MAX + INODE_INDIR1_SIZE)) {
        char buf1[BLOCK_SIZE];
        if (!_vhd_.read_vhd(buf1, cur.i_addr[INODE_INDIR1_MAX]))
            return false;
        char *src = buf1 + (startBlock - INODE_DIRECT_CNT) * sizeof(int);
        memcpy(dst, src, sizeof(int));
    } else {
        char buf1[BLOCK_SIZE];
        if (!_vhd_.read_vhd(buf1, cur.i_addr[INODE_INDIR2_MAX])) {
            return false;
        }
        int offset_of_indir2 = (startBlock - INODE_DIRECT_CNT - INODE_INDIR1_CNT) / INODE_INDIR1_SIZE;
        char *src = buf1 + offset_of_indir2 * sizeof(int);
        int BID_in_indir2;
        memcpy(&BID_in_indir2, src, sizeof(int));

        char buf2[BLOCK_SIZE];
        if (!_vhd_.read_vhd(buf2, offset_of_indir2)) {
            return false;
        }
        src = buf2 + ((startBlock - INODE_DIRECT_CNT - INODE_INDIR1_CNT) % (INODE_INDIR2_SIZE * sizeof(int)));
        memcpy(dst, src, sizeof(int));
    }
    return true;
}
bool fsController::AppendBlocksToFile(iNode &cur, int blockCnt) {
    // Check if blockCnt is bigger than the max block cnt
    if (cur.i_block + blockCnt > DATAB_END)
        return false;

    int dataBid = 0, directBid = 0, indir1Bid = 0, indir2Bid = 0;
    char indir1Buf[BLOCK_SIZE], indir2Buf[BLOCK_SIZE];
    int indir1BufLoadBid = 0, indir2BufLoadBid = 0;
    memset(indir1Buf, 0, BLOCK_SIZE);
    memset(indir2Buf, 0, BLOCK_SIZE);
    // Append new blocks one by one
    for (int i = (int) cur.i_block; i < (int) cur.i_block + blockCnt; i++) {
        if (i < INODE_DIRECT_CNT)   // direct index
        {
            dataBController fbc;
            directBid = fbc.alloc_datab();
            cur.i_addr[i] = directBid;
        } else if (i == INODE_DIRECT_CNT) // level 1 indirect index(new)
        {
            // Distribute level 1 indirect index block
            dataBController fbc;
            indir1Bid = fbc.alloc_datab();
            cur.i_addr[INODE_INDIR1_MAX] = indir1Bid;
            indir1BufLoadBid = indir1Bid;
            // and distribute 1 i_addr block
            dataBid = fbc.alloc_datab();
            memcpy(indir1Buf, (char *) &dataBid, sizeof(int));
            vhdController my_vhd;
            my_vhd.write_vhd(indir1Buf, indir1Bid);
        } else if (i < (int) (DIRECT_BLOCK_CNT + INDIR1_BLOCK_CNT))   // level 1 indirect index
        {
            // Load level 1 indirect index block if needed
            if (indir1BufLoadBid != cur.i_addr[INODE_INDIR1_MAX]) {
                indir1Bid = cur.i_addr[INODE_INDIR1_MAX];
                vhdController my_vhd;
                my_vhd.load_vhd();
                my_vhd.read_vhd(indir1Buf, indir1Bid);
                indir1BufLoadBid = cur.i_addr[INODE_INDIR1_MAX];
            }
            // and distribute 1 i_addr block
            dataBController fbc;
            dataBid = fbc.alloc_datab();
            memcpy(indir1Buf + (i - DIRECT_BLOCK_CNT) * sizeof(int),
                   (char *) &dataBid, sizeof(int));
            vhdController vhds;
            vhds.load_vhd();
            vhds.write_vhd(indir1Buf, indir1Bid);
        } else if (i == DIRECT_BLOCK_CNT + INDIR1_BLOCK_CNT)  // level 2 indirect index(new)
        {
            // Distribute level 2 indirect index block
            dataBController fbc;
            vhdController vhdc;
            indir2Bid = fbc.alloc_datab();
            cur.i_addr[INODE_INDIR2_MAX] = indir2Bid;
            indir2BufLoadBid = indir2Bid;
            // Distribute level 1 indirect index block
            memset(indir1Buf, 0, BLOCK_SIZE);
            indir1Bid = fbc.alloc_datab();
            memcpy(indir2Buf, (char *) &indir1Bid, sizeof(int));
            vhdc.write_vhd(indir2Buf, indir2Bid);
            // and distribute 1 i_addr block
            dataBid = fbc.alloc_datab();
            memcpy(indir1Buf, (char *) &dataBid, sizeof(int));
            vhdc.write_vhd(indir1Buf, indir1Bid);
            indir1BufLoadBid = indir1Bid;
        } else    // level 2 indirect index
        {
            dataBController fbc;
            vhdController vhdc;
            vhdc.load_vhd();
            // Load level 2 indirect index block if needed
            if (indir2BufLoadBid != cur.i_addr[INODE_INDIR2_MAX]) {
                indir2Bid = cur.i_addr[INODE_INDIR2_MAX];
                vhdc.read_vhd(indir2Buf, indir2Bid);
                indir2BufLoadBid = cur.i_addr[INODE_INDIR2_MAX];
            }
            // Load level 1 indirect index block or
            // append new level 1 indirect index block if needed
            unsigned int indir21Off = i - DIRECT_BLOCK_CNT - INDIR1_BLOCK_CNT;
            if (indir21Off % (BLOCK_SIZE / sizeof(int)) == 0) {
                // Distribute level 1 indirect index block
                memset(indir1Buf, 0, BLOCK_SIZE);
                indir1Bid = fbc.alloc_datab();
                indir1BufLoadBid = indir1Bid;
                memcpy(indir2Buf + (indir21Off / (BLOCK_SIZE / sizeof(int))) * sizeof(int),
                       (char *) &indir1Bid, sizeof(int));
                vhdc.write_vhd(indir2Buf, indir2Bid);
            } else {
                // Load level 1 indirect index block if needed
                int indir21Bid = 0;
                // toff is the address of the level 1 indirect index block ID of
                // the level 2 indirect index
                char *toff = indir2Buf + (indir21Off /
                                          (BLOCK_SIZE / sizeof(int))) * sizeof(int);
                memcpy((char *) &indir21Bid, toff, sizeof(int));
                if (indir1BufLoadBid != indir21Bid) {
                    vhdc.read_vhd(indir1Buf, indir21Bid);
                    indir1BufLoadBid = indir21Bid;
                }
            }
            // and distribute 1 i_addr block

            dataBid = fbc.alloc_datab();
            memcpy(indir1Buf + (indir21Off % (BLOCK_SIZE / sizeof(int))) * sizeof(int),
                   (char *) &dataBid, sizeof(int));
            vhdc.write_vhd(indir1Buf, indir1BufLoadBid);
        }
    }
    // Update iNode
    cur.i_block += blockCnt;
    iNodeController iNC;
    iNC.write_iNode(cur);
    return true;
}
bool fsController::DeleteFile(const iNode &cur) {
    // Not for directory
    if (cur.i_mode & DIRFLAG) return false;
    // Recycle data blocks and iNode if nlink == 1
    // (if nlink > 1, then there're other hard links,
    //  which means we can't recycle its data blocks or iNode yet)
    if (cur.i_nlink == 1) {
        if (cur.i_block <= DIRECT_BLOCK_CNT) {
            if (!this->DeleteDirectBlocks(cur))
                return false;
        } else if (cur.i_block <= DIRECT_BLOCK_CNT + INDIR1_BLOCK_CNT) {
            if (!this->DeleteDirectBlocks(cur) & this->DeleteIndir1Blocks(cur))
                return false;
        } else {
            if (!this->DeleteDirectBlocks(cur) & this->DeleteIndir1Blocks(cur) &
                this->DeleteIndir2Blocks(cur))
                return false;
        }
        // Recycle iNode block
        if (!idc.Recycle(cur.bid)) return false;
    }
    // Delete SFD in parent
    return this->DeleteSFDEntry(cur);
}
bool fsController::WriteFileFromBuf(iNode &cur, int start, int len, char *buf) {
    // Append new blocks if needed
    vhdController vhdc;
    vhdc.load_vhd();
    int deltaBlocks = ceil(((float) start + len) / (float) (BLOCK_SIZE)) - cur.i_block;
    if (deltaBlocks > 0) {
        if (!this->AppendBlocksToFile(cur, deltaBlocks))
            return false;
    }
    // Write buf to file block by block
    int startBlock = start / BLOCK_SIZE;
    int endBlock = (start + len) / BLOCK_SIZE;
    int nowBlock = startBlock;
    char *bufp = buf;
    int curBID = 0;
    int filep = start;
    char rbuf[BLOCK_SIZE];

    if (startBlock == endBlock) // Write in one block
    {
        if (!GetBIDByFOff(cur, start, &curBID))
            return false;
        vhdc.read_vhd(rbuf, curBID);
        memcpy(rbuf + start % BLOCK_SIZE, buf, len);
        vhdc.write_vhd(rbuf, curBID);
    } else    // Write in multiple blocks
    {
        while (filep < start + len + BLOCK_SIZE - (start + len) % BLOCK_SIZE) {
            if (!GetBIDByFOff(cur, filep, &curBID))
                return false;

            vhdc.read_vhd(rbuf, curBID, BLOCK_SIZE);
            if (nowBlock == startBlock) // First block
            {
                int toff = start % BLOCK_SIZE;
                int tlen = BLOCK_SIZE - toff;
                memcpy(rbuf + toff, bufp, tlen);
                bufp += tlen;
            } else if (nowBlock == endBlock)  // Last block
            {
                int tlen = (start + len) % BLOCK_SIZE;
                memcpy(rbuf, bufp, tlen);
                bufp += tlen;
            } else    // Middle block
            {
                memcpy(rbuf, bufp, BLOCK_SIZE);
                bufp += BLOCK_SIZE;
            }
            vhdc.write_vhd(rbuf, curBID, BLOCK_SIZE);
            filep += BLOCK_SIZE;
            nowBlock++;
        }
    }
    // Update iNode
    cur.mtime = time(nullptr);
    cur.atime = time(nullptr);
    if (start + len > (int) cur.i_size) {
        cur.i_size = start + len;
        cur.i_bytes = (start + len) % BLOCK_SIZE;
    }
    iNodeController iNC;
    iNC.write_iNode(cur);
    return true;
}
bool fsController::CopyFile(const iNode& src, iNode& des, char* name, int uid) {
    // Only for file
    if (src.mode & DIRFLAG) return false;
    // Touch new file with given name
    iNode newiNode;
    if (!this->Touch(des, name, src.mode, uid, &newiNode))
        return false;
    // If the size of src file is 0, just touch it
    if (src.size == 0) return true;
    // Readin src file
    char* buf = new char[src.size];
    if (!this->ReadFileToBuf(src, 0, src.size, buf))
    {
        delete[] buf;
        return false;
    }
    // Write to des
    if (!this->WriteFileFromBuf(newiNode, 0, src.size, buf))
    {
        delete[] buf;
        return false;
    }
    delete[] buf;
    return true;
}


/*directory operations*/
bool fsController::GetContentInDir(const iNode &curDir, Dentry *rst) {
    if (!(curDir.i_mode & DIRFLAG))
        return false;
    this->ReadFileToBuf(curDir, 0, curDir.i_size, (char *) rst);
    return true;
}
bool fsController::FindContentInDir(const Dentry *Dir, const int len, const char *name, int *rst) {
    for (int i = 0; i < len; i++) {
        if (strcmp(Dir[i].name, name) == 0) {
            *rst = i;
            return true;
        }
    }
    return false;
}
bool fsController::DeleteDir(const iNode& cur) {
    // Only for dir
    if (!(cur.i_mode & DIRFLAG)) return false;
    // Readin SFD List
    iNode nowiNode;
    Dentry* SFDList = new Dentry[cur.i_size / sizeof(Dentry)];
    if (!GetContentInDir(cur, SFDList))
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
        if (!this->GetiNodeByID(SFDList[i].i_ino, &nowiNode))
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
bool fsController::CopyDir(const iNode& src, iNode& des, char* name, int uid) {
    // Only for dir
    if (!(src.mode & DIRFLAG)) return false;
    // Create subdir
    iNode newdiriNode;
    if (!this->CreateSubDir(des, name, src.mode, uid, &newdiriNode))
        return false;
    // Readin SFD List
    iNode nowiNode;
    SFD* SFDList = new SFD[src.size / sizeof(SFD)];
    if (!GetContentInDir(src, SFDList))
    {
        delete[] SFDList;
        return false;
    }
    // Iterate over srcdir
    for (int i = 0; i < (int)(src.size / sizeof(SFD)); i++)
    {
        // Ignore . or ..
        if (strcmp(SFDList[i].name, DOT) == 0 || strcmp(SFDList[i].name, DOTDOT) == 0)
            continue;
        // Readin iNode
        if (!this->GetiNodeByID(SFDList[i].inode, &nowiNode))
        {
            delete[] SFDList;
            return false;
        }
        if (nowiNode.mode & DIRFLAG)    // Copy subdir recursively
        {
            if (!this->CopyDir(nowiNode, newdiriNode, nowiNode.name, uid))
            {
                delete[] SFDList;
                return false;
            }
        }
        else    // Copy regular file
        {
            if (!this->CopyFile(nowiNode, newdiriNode, nowiNode.name, uid))
            {
                delete[] SFDList;
                return false;
            }
        }
    }
    delete[] SFDList;
    return true;
}

/*block operations*/
bool fsController::DeleteDirectBlocks(const iNode &cur) {
    int maxi = cur.i_block > DIRECT_BLOCK_CNT ? DIRECT_BLOCK_CNT : cur.i_block;
    for (int i = 0; i < (unsigned int) maxi; i++) {
        fbc.Recycle(cur.i_addr[i]);
    }
    return true;
}
bool fsController::DeleteIndir1Blocks(const iNode &cur) {
    // Readin indir1 block
    int indir1Buf[BLOCK_SIZE / sizeof(int)];
    vhdController vhdc;
    dataBController fbc;
    vhdc.load_vhd();
    vhdc.read_vhd((cur.i_addr[INODE_INDIR1_MAX], (char *) indir1Buf);
    // Recycle data blocks
    int maxi = cur.i_block - DIRECT_BLOCK_CNT > INDIR1_BLOCK_CNT ?
               (int) INDIR1_BLOCK_CNT : (int) cur.i_block - DIRECT_BLOCK_CNT;
    for (int i = 0; i < maxi; i++) {

        fbc.release_datab(indir1Buf[i]);
    }
    // Recycle indir1 block
    fbc.release_datab(cur.i_addr[INODE_INDIR1_MAX]);
}
bool fsController::DeleteIndir2Blocks(const iNode &cur) {
    // Readin indir2 block
    vhdController vhdc;
    dataBController dbc;
    int indir2Buf[BLOCK_SIZE / sizeof(int)];
    if (!vhdc.read_vhd((char *) indir2Buf, cur.i_addr[INODE_INDIR2_MAX] ))
        return false;
    // Calc total indir1 count and last indir1 count
    int indir1Cnt = (cur.i_block - DIRECT_BLOCK_CNT - INDIR1_BLOCK_CNT)
                    / (BLOCK_SIZE / sizeof(int));
    int lastIndir1Cnt = (cur.i_block - DIRECT_BLOCK_CNT - INDIR1_BLOCK_CNT)
                        % (BLOCK_SIZE / sizeof(int));
    // Do recycle
    int indir1Buf[BLOCK_SIZE / sizeof(int)];
    for (int i = 0; i < indir1Cnt; i++) {
        // Readin indir1 block
        if (!vhdc.read_vhd((char *) &indir1Buf, indir2Buf[i]))
            return false;
        // Recycle data block (end with 0)
        int maxj = (i == indir1Cnt - 1 ? lastIndir1Cnt : BLOCK_SIZE / sizeof(int));
        for (int j = 0; j < maxj; j++) {
            if (!dbc.release_datab(indir1Buf[j])) return false;
        }
        // Recycle indir1 block
        if (!dbc.release_datab(indir2Buf[i])) return false;
    }
    // Recycle indir2 block
    return dbc.release_datab(cur.i_addr[INODE_INDIR2_MAX]);
}
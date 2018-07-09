//
// Created by Xingqi Tang on 2018/7/5.
//
#include "fsController.h"
#include "dataBController.h"

fsController::fsController() {
  //return;
  // this->root_path = ROOT_PATH;
}

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
  if (!create_RootDir) {
    perror("[err] create root dir failed.");
    return false;
  }

  //cur.i_ino = i

  return true;

}

/*bool fsController::init_iNode_free_list() {
    next_i_free_list_index = 0;
    this->fs.iNodeSb.stackDepth = 0;
    while (!this->fs.iNodeSb.iSbStack.empty()) {
        this->fs.iNodeSb.iSbStack.pop();
    } // if iNode superblock is not empty, pop them out.

    for (int i = 0; i < INDEX_LIST_NUMBER; i++) {
        for (int j = 0; j < INDEX_LIST_LENGTH; j++) {
            fs.i_free_list[i][j] = i * INDEX_LIST_LENGTH + INDEX_LIST_LENGTH - j - 1;
        }
    }
    return true;
}*/

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

fileSystem fsController::get_fs() {
  return this->fs;
}

bool fsController::is_get_fs(fileSystem _fs) {
  this->fs = _fs;
  return true;
}

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

  bool fsController::GetBIDByFOff(const iNode &cur, int offset, int *dst) // offset = BLOCK_SIZE * BLOCK_NUM
  {
    int startBlock = offset / BLOCK_SIZE;
    vhdController _vhd_;
    _vhd_.load_vhd();
    if (startBlock <= INODE_DIRECT_MAX)
      *dst = cur.i_addr[startBlock];
    else if (startBlock <= (int) (INODE_DIRECT_MAX + INODE_INDIR1_SIZE)) {
      char buf1[BLOCK_SIZE];
      if (!_vhd_.read_vhd(buf1, cur.i_addr[INODE_INDIR1_MAX]), BLOCK_SIZE)
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

  bool fsController::create_empty_Flie(iNode &curDir, char *name, char mode, int ownerUid, iNode *rst) {
    // Read current dir
    Dentry *dir = new Dentry[curDir.i_size / sizeof(Dentry)];
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
  bool fsController::AppendBlocksToFile(iNode &cur, int blockCnt) {
    // Check if blockCnt is bigger than the max block cnt
    if (cur.i_block + blockCnt > DATAB_END)
      return false;

    int dataBid = 0, directBid = 0, indir1Bid = 0, indir2Bid = 0;
    char indir1Buf[BLOCK_SIZE], indir2Buf[BLOCK_SIZE];
    int indir1BufLoadBid = 0, indir2BufLoadBid = 0;
    memset(indir1Buf, 0, sizeof(BLOCK_SIZE));
    memset(indir2Buf, 0, sizeof(BLOCK_SIZE));
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

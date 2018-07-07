//
// Created by Xingqi Tang on 2018/7/4.
//

#ifndef VFS_PARAMS_H
#define VFS_PARAMS_H

#include<iostream>
#include<algorithm>
#include<cstdio>
#include<vector>
#include<cstring>
#include<map>
#include<queue>
#include<stack>
#include<string>
#include<cstdlib>
#include<ctime>
#include<set>

using namespace std;


/* define basic data structures and common params
 * we are focus on a linux file system now,
 * and our virtual hard disk is divided into 3 parts from now on:
 * superblock(sb)/inode table/data table
 */

/*  VHD */
#define VHD_SIZE 1024*1024*64 // 64MB
#define VHD_FILENAME "VFS.VHD"

/*  BLOCK */
#define BLOCK_SIZE 1024*4 // 4KB
#define BLOCK_NUM (VHD_SIZE)/(BLOCK_SIZE)
#define BB_0
#define ISB 1
#define DSB 2
#define INODE_START 3
#define INODE_END 514
#define DATAB_START 515
#define DATAB_END 16383
/* INDEX */
#define INDEX_LIST_LENGTH 512
#define INDEX_LIST_NUMBER (INODE_END-INODE_START+1)/(INDEX_LIST_LENGTH)

/* DENTRY */
#define DENTRY_LIST_LENGTH 512
#define DENTRY_LIST_NUMBER (DATAB_END-DATAB_START+1)/(DENTRY_LIST_LENGTH)

/* ROOT PATH */
#define ROOT_PATH "~/dev/"

/* USER */
#define MAX_USER_NUMBER 8

#endif //VFS_PARAMS_H



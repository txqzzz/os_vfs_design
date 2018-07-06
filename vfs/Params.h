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

#endif //VFS_PARAMS_H

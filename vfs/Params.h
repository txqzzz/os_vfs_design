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
#define INODE_BEG 3
#define INODE_END 514
#define USER 515
#define DATAB_BEG 516
#define DATAB_END 16383
#define INODE_NUM (INODE_END-INODE_BEG+1)
#define DATAB_NUM (DATAB_END-DATAB_BEG+1)
#define INODE_SIZE (BLOCK_SIZE)
/* INDEX */
#define INDEX_LIST_LENGTH 512
#define INDEX_LIST_NUMBER (INODE_NUM)/(INDEX_LIST_LENGTH)

/* DATAB */
#define DATAB_LIST_LENGTH 512
#define DATAB_LIST_NUMBER (DATAB_NUM)/(DATAB_LIST_LENGTH)

/* ROOT PATH */
#define ROOT_PATH "~/dev/"

/* USER */
#define MAX_USER_NUMBER 8

/* GLOBAL */
extern string welcomeMsg;
extern FILE * fw;
extern FILE * fr;
extern int nextUid;
extern int nextGid;
extern int Root_Dir_Addr;
extern int Cur_Dir_Addr;
extern char Cur_Dir_Name[310];
extern char Cur_Host_Name[110];
extern char Cur_User_Name[110];
extern char Cur_Group_Name[110];
extern char Cur_User_Dir_Name[310];


extern bool isLogin;
//extern superblock *superblock;
extern bool i_bitmap[INODE_NUM];
extern bool d_bitmap[BLOCK_NUM];

#define WRX 0777   // root read+write+execute


#endif //VFS_PARAMS_H
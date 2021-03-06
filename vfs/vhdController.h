//
// Created by Xingqi Tang on 2018/7/4.
//

#ifndef VFS_VHD_H
#define VFS_VHD_H

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
#include<fstream>
#include "Params.h"

using namespace std;
/*MACRO*/
#define FORi(l, r) for(int i=(l);i<(r);i++)
#define FORj(l, r) for(int j=(l);j<(r);j++)
#define FORk(l, r) for(int k=(l);k<(r);k++)
#define MEMSET0(i) memset((i),0,sizeof((i)))
#define MEMSET1(i) memset((i),-1,sizeof((i)))


class vhdController {
public:
    vhdController();

    bool create_vhd();

    bool del_vhd();

    bool load_vhd();

    bool Exists_vhd();

    bool read_vhd(char *buffer, int blockId, int len = BLOCK_SIZE);

    bool write_vhd(char *buffer, int blockId, int len = BLOCK_SIZE);

    bool del_vhd(int Block_id);

private:
    const char *filename;
    int size;
    fstream vfile;
//protected:

};

#endif //VFS_VHD_H

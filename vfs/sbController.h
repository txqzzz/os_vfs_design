//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_SBCONTROLLER_H
#define VFS_SBCONTROLLER_H
#include "vhdController.h"
#include "DataStructure.h"

class sbController {
public:
public:
    sbController(vhdController& _vhdc, int _sbID, int _minbID, int _maxbID);
    ~sbController();
    bool LoadSuperBLock();
    bool LoadSuperBLock(int blockID);
    bool SaveSuperBlock();
    bool SaveSuperBlock(int blockID);
    bool Recycle(const iNode &blockID);
    bool Distribute(int* blockID);
    void SetFullFlag();

protected:

private:
    vhdController& vhdc;
    int superblockID;
    int minblockID;
    int maxblockID;
    sB superblock;
};


#endif //VFS_SBCONTROLLER_H

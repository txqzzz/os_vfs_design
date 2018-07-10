//
// Created by Xingqi Tang on 2018/7/5.
//

#include "sbController.h"


sbController::sbController(vhdController& _vhdc, int _sbID, int _minbID, int _maxbID) :
        vhdc(_vhdc), superblockID(_sbID), minblockID(_minbID), maxblockID(_maxbID)
{
    this->LoadSuperBLock();
    return;
}

sbController::~sbController()
{
    this->SaveSuperBlock();
    return;
}

bool sbController::LoadSuperBLock()
{
    return this->vhdc.read_vhd((char*)&this->superblock, this->superblockID, sizeof(superblock));
}

bool sbController::LoadSuperBLock(int blockID)
{
    return this->vhdc.read_vhd((char*)&this->superblock,blockID, sizeof(superblock));
}

bool sbController::SaveSuperBlock()
{
    return this->vhdc.write_vhd((char*)&this->superblock, this->superblockID, sizeof(superblock));
}

bool sbController::SaveSuperBlock(int blockID)
{
    return this->vhdc.write_vhd((char*)&this->superblock,blockID, sizeof(superblock));
}

bool sbController::Recycle(const iNode &blockID)
{
    if (this->superblock.DstackDepth == LEN_PER_GROUP)
    {
        if(!this->SaveSuperBlock(blockID))
            return false;
        memset((char*)&this->superblock, 0, sizeof(superblock));
        this->superblock.DstackDepth = 1;
    }
    else this->superblock.DstackDepth++;
    this->superblock.stack[this->superblock.DstackDepth-1] = blockID;
    return true;
}

bool sbController::Distribute(int* blockID)
{
    if(this->superblock.stack[this->superblock.DstackDepth-1] == 0) return false;
    *blockID = this->superblock.stack[this->superblock.DstackDepth-1];
    if(this->superblock.DstackDepth == 1)
    {
        if(!this->LoadSuperBLock(*blockID))
            return false;
    }
    else this->superblock.DstackDepth--;
    return true;
}

void sbController::SetFullFlag()
{
    this->superblock.stack[0] = 0;
    this->superblock.DstackDepth = 1;
}


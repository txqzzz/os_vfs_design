//
// Created by Xingqi Tang on 2018/7/9.
//

#include "dataBController.h"
#include "Params.h"
#include "fsController.h"

dataBController::dataBController() {
    this->datab_free_num = DATAB_NUM;
    this->load_Sb();
    return ;
}
dataBController::~dataBController(){
    this->save_Sb();
    return ;
}
bool dataBController::load_Sb() {
    char * buf = new char[BLOCK_SIZE];
    vhdController _vhd_;
    _vhd_.load_vhd();
    _vhd_.read_vhd(buf,DSB);
    memcpy((char *) &this->superblock, buf, BLOCK_SIZE);
    delete[] buf;
    return true;
}
bool dataBController::load_Sb(int id) {
    char * buf = new char[BLOCK_SIZE];
    vhdController _vhd_;
    _vhd_.load_vhd();
    _vhd_.read_vhd(buf,id);
    memcpy((char *) &this->superblock, buf, BLOCK_SIZE);
    delete[](buf);
    return true;
}
bool dataBController::save_Sb() {
    auto * buf = new char [BLOCK_SIZE];
    memcpy(buf ,(char*)&this->superblock,BLOCK_SIZE);
    vhdController _vhd_;
    _vhd_.load_vhd();
    _vhd_.write_vhd(buf,DSB);
    delete [] buf;
    return true;
}
bool dataBController::save_Sb(int id) {
    auto * buf = new char [BLOCK_SIZE];
    memcpy(buf ,(char*)&this->superblock,BLOCK_SIZE);
    vhdController _vhd_;
    _vhd_.load_vhd();
    _vhd_.write_vhd(buf,id);
    delete [] buf;
    return true;
}
bool dataBController::init_datab() {
    for (int i = DATAB_BEG;i<DATAB_END;i++)
        if(!this->release_datab(i)) return false;
    if (!this->save_Sb()) return false;
    this->superblockid=DSB;
    return true;
} // 初始化 回收所有数据块
bool dataBController::release_datab(int BlockID) {
    if (this->superblock.DstackDepth  == DATAB_LIST_LENGTH) {
        if (!this->save_Sb(BlockID))
            return false;
        memset((char*)&this->superblock,0, sizeof(this->superblock));
        this->superblock.DstackDepth = 1;
    }
    else this->superblock.DstackDepth++;
    this->superblock.stack[this->superblock.DstackDepth-1]=BlockID;
    return true;
}
int  dataBController::alloc_datab() {
    if (this->superblock.stack[this->superblock.DstackDepth -1 ]== 0)
        return false;
    int temp;
    temp=this->superblock.stack[this->superblock.DstackDepth-1];
    cout<<"alloc_datab:"<<temp<<endl;
    if (this->superblock.DstackDepth==1){
        this->superblockid=temp;
        this->load_Sb(this->superblockid);
    }
    else this->superblock.DstackDepth--;
    return temp;
}
bool dataBController::setfullflag() {
    this->superblock.DstackDepth=1;
    this->superblock.stack[0]=0;
    return true;
}
bool dataBController::show(){
    cout<<this->superblock.DstackDepth<<endl;
    for (int i=0;i<this->superblock.DstackDepth;i++){
        cout<<this->superblock.stack[i]<<endl;
    }
    return true;
}
sB dataBController::get_sB() {
    return this->superblock;
}
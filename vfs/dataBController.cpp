//
// Created by Xingqi Tang on 2018/7/9.
//

#include "dataBController.h"
#include "Params.h"
#include "fsController.h"

dataBController::dataBController() {
    this->datab_free_num = DATAB_NUM;
}

int dataBController::alloc_datab(fsController &fsc ) {
    //check if data blocks is empty
    fileSystem _fs;
    _fs = fsc.get_fs();
    int top = _fs.dataSb.dSbStack.top();
    if (_fs.superblock.s_free_datab_num == 0) {
        cout << "[err] no free data blocks for allocation. " << endl;
        return -1;
    } else {
        d_free_listintoDsbStack(fsc);
        int result = _fs.dataSb.dSbStack.top();
        _fs.dataSb.dSbStack.pop();
        _fs.dataSb.stackDepth--;
        fsc.is_get_fs(_fs);
        return result;
    }
}
bool dataBController::release_datab(fsController &_fs){
    return true;
}
    
bool dataBController::d_free_listintoDsbStack(fsController &fsc) {
    fileSystem _fs;
    _fs = fsc.get_fs();
    for (size_t i=0;i<DATAB_LIST_LENGTH;i++){
        int temp = _fs.d_free_list[next_d_free_list_index][i];
        _fs.dataSb.dSbStack.push(temp);
        _fs.d_free_list[next_d_free_list_index][i]=-1;
    }
    _fs.dataSb.stackDepth=INDEX_LIST_LENGTH;
    if(next_d_free_list_index<DATAB_LIST_NUMBER){
        next_d_free_list_index++;
}
    fsc.is_get_fs(_fs);
    return true;
  /*  else{ // if we have left blocks
        top = _fs.superblock.s_free_datab_num % DATAB_LIST_LENGTH;
    }

    if(top == 0) { // if top -> stack.bottom
        _fs.superblock.s_free_addr;
    }
    //将栈顶取出
    //如果已是栈底，将当前块号地址返回，即为栈底块号，并将栈底指向的新空闲块堆栈覆盖原来的栈
    int retAddr;

    if(top==0){
        retAddr = _fs.superblock.s_free_addr;
        _fs.superblock.s_free_addr = _fs.superblock.s_free_next_ist[0];	//取出下一个存有空闲块堆栈的空闲块的位置，更新空闲块堆栈指针

        //取出对应空闲块内容，覆盖原来的空闲块堆栈

        //取出下一个空闲块堆栈，覆盖原来的
        fseek(fr,_fs.superblock->s_free_addr,SEEK_SET);
        fread(_fs.s_free,sizeof(_fs.superblock.s_free),1,fr);
        fflush(fr);

        superblock->s_free_BLOCK_NUM--;

    }
    else{	//如果不为栈底，则将栈顶指向的地址返回，栈顶指针-1.
        retAddr = superblock->s_free[top];	//保存返回地址
        superblock->s_free[top] = -1;	//清栈顶
        top--;		//栈顶指针-1
        superblock->s_free_BLOCK_NUM--;	//空闲块数-1

    }*/
}

bool dataBController::init_datab_free_list(fsController &fsc) {
    next_d_free_list_index = 0;
    fileSystem _fs;
    _fs = fsc.get_fs();
    _fs.dataSb.stackDepth = 0;
    while (!_fs.dataSb.dSbStack.empty()) {
        _fs.dataSb.dSbStack.pop();
    } // if dNode superblock is not empty, pop them out.

    for (int i = 0; i < DATAB_LIST_NUMBER; i++) {
        for (int j = 0; j < DATAB_LIST_LENGTH; j++) {
            _fs.d_free_list[i][j] = i * DATAB_LIST_LENGTH + DATAB_LIST_LENGTH - j - 1;
        }
    }
    return true;
}

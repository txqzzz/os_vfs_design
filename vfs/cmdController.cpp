//
// Created by Xingqi Tang on 2018/7/10.
//

#include "cmdController.h"

cmdController::cmdController(fsController &_fsc_, userController &_uc_, vhdController &_vhdc_, int _uid_, const iNode& cur_iNode):
fsc(_fsc_), uc(_uc_), vhdc(_vhdc_), uid(_uid_) {
  memcpy((char *) &this->cur_iNode, (char *) cur_iNode, sizeof(iNode));
  return;
}

bool printScreen(){
  char cmd_buf[MAX_CMD_LENGTH];
  this->fsc.Get 
}
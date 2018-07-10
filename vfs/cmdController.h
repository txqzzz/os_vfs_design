//
// Created by Xingqi Tang on 2018/7/10.
//

#ifndef OS_VFS_DESIGN_CMDCONTROLLER_H
#define OS_VFS_DESIGN_CMDCONTROLLER_H

#include "fsController.h"
#include "userController.h"
#include "vhdController.h"
#include "iNodeController.h"
#include "dentryController.h"
#include "dataBController.h"

class cmdController {
public:
  cmdController(fsController &_fsc_, userController &_uc_, vhdController &_vhdc_, int _uid_, const iNode& cur_iNode):
      fsc(_fsc_), uc(_uc_), vhdc(_vhdc_), uid(_uid_) {
      memcpy((char *)&this->cur_iNode, (char * )cur_iNode, sizeof(iNode));

  }
  bool printScreen();
  bool getCommand();
  bool getMode();
  bool getSegment();
private:
  fsController &fsc;
  userController &uc;
  vhdController &vhdc;
  int uid;
  iNode cur_iNode;

};

#endif //OS_VFS_DESIGN_CMDCONTROLLER_H

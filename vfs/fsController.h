//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_FSCONTROLLER_H
#define VFS_FSCONTROLLER_H


class fsController {
    fsController();
    ~fsController();
    bool mountfs();
    bool loadfs();
    bool formatfs();

};


#endif //VFS_FSCONTROLLER_H

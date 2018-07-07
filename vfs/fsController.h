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

//mount 安装： 执行操作为：
//load  执行操作为： 加载超级快 加载用户信息 加载目录信息 初始化根目录
// format 执行操作为：
#endif //VFS_FSCONTROLLER_H

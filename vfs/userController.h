//
// Created by faithstill on 2018/7/8.
//

#ifndef OS_VFS_DESIGN_USERCONTROLLER_H
#define OS_VFS_DESIGN_USERCONTROLLER_H

#include "DataStructure.h"
#include "vhdController.h"

class userController {
public:
    userController();

    bool loadUser();

    bool wirteUser();

    bool addUser(user _user_);

    bool delUser(int id);

    short UserloginIN();

    short checkUser(string name, string password);

    bool showUser();

private:
    user _user_[8];
    int num;
};
/*
 * user数据存在数据块的第一块，数据块的第一个（int）空间存当前用户数，后面的空间存了user结构体
 * user id 为-1表示该user结构体未被使用
 *
 */
#endif //OS_VFS_DESIGN_USERCONTROLLER_H

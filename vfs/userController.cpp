//
// Created by faithstill on 2018/7/8.
//

#include "userController.h"

userController::userController() {
    this->num = 0;
    for (size_t i = 0; i < 8; i++) {
        this->_user_[i].id = -1;
    }
}

bool userController::loadUser() {
    vhdController vhd_Controller;
    char *buf = new char[BLOCK_SIZE];
    vhd_Controller.load_vhd();
    vhd_Controller.read_vhd(buf, USER);
    memcpy((char *) &this->num, buf, sizeof(int));
    memcpy(this->_user_, buf + sizeof(int), sizeof(user) * 8);
    return true;
}

bool userController::delUser(int id) {
    for (int i = 0; i < 8; i++) {
        if (this->_user_[i].id == id) {
            this->_user_[i].id = -1;
            this->num = this->num - 1;
            return true;
        }
    }
    return false;
}

bool userController::wirteUser() {
    vhdController vhd_Controller;
    char *buf = new char[BLOCK_SIZE];
    memcpy(buf, (char *) &this->num, sizeof(int));
    memcpy(buf + sizeof(int), this->_user_, (BLOCK_SIZE) - sizeof(int));
    vhd_Controller.load_vhd();
    vhd_Controller.write_vhd(buf, USER);
    delete[] buf;
    return true;
}

bool userController::addUser(user _user_) {
    this->_user_[this->num] = _user_;
    this->num = this->num + 1;
    return true;
}

short userController::checkUser(string name, string password) {
    for (int i = 0; i < this->num; i++) {
        if (this->_user_[i].name == name && this->_user_[i].password == password) {
            return this->_user_[i].id;
        }
    }
    return -1;
}

short userController::UserloginIN() {
    int chance = 3;
    string name;
    string password;
    while (chance > 0) {
        cout << "please input username:" << endl;
        cin >> name;
        cout << "please input password:" << endl;
        cin >> password;
        short temp;
        temp = userController::checkUser(name, password);
        if (temp != -1) {
            cout << "welcome login vhd" << endl;
            return temp;
        } else {
            cout << "User name or password error,you have " << --chance << " more chance" << endl;
        }
    }
    return -1;
}

bool userController::showUser() {
    cout << "the num of user is :" << this->num << endl;
    for (int i = 0; i < 8; i++) {
        if (this->_user_[i].id != -1) {
            cout << this->_user_[i].id << endl;
            cout << this->_user_[i].name << endl;
            cout << this->_user_[i].password << endl;
            cout << this->_user_[i].authority << endl;
            cout << endl;
        }
    }
    return true;
}
//
// Created by Xingqi Tang on 2018/7/4.
//
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<vector>
#include<cstring>
#include<map>
#include<queue>
#include<stack>
#include<string>
#include<cstdlib>
#include<ctime>
#include<set>
#include<fstream>
#include"vhdController.h"
#include"Params.h"

using namespace std;

vhdController::vhdController() {
    this->filename = VHD_FILENAME;
    this->size = VHD_SIZE;
    //this->vfile = vhd_file;
}
bool vhdController::Exists_vhd(){
    ifstream f(this->filename);
    return f.good();
}

bool vhdController::create_vhd() {
    ofstream vhd_file;
    vhd_file.open(this->filename, ios::out | ios::binary);
    if (!vhd_file) {
        cout << "[err]vhd_file open failed." << endl;
        return false;
    }
    char *buffer = new char[this->size];
    memset(buffer, 0, this->size);
    //strcat(buffer,"test");
    //vhd_file.open(this->filename, ios::out | ios::binary);
    vhd_file.write(buffer, this->size);
    if (!vhd_file) {
        cout << "[err]vhd_file write failed." << endl;
        return false;
    }
    //this->vfile = vhd_file;
    //this->vfile= vhd_file.open(this->filename, ios::in | ios::out| ios::binary);
    vhd_file.close();
    delete[] buffer;
    return true;
}
bool vhdController::load_vhd(){
    this->vfile.open(this->filename, ios::in | ios::out| ios::binary);
    return true;
}
bool vhdController::del_vhd() {
    ofstream vhd_file;
    vhd_file.open(this->filename, ios::out | ios::binary);
    if (vhd_file.is_open()) {
        cout << "[ok]vhd_file is open successfully." << endl;
        cout << ifstream(this->filename).rdbuf() << endl;
    }
    vhd_file.close();
    remove(this->filename);

    return true;
}

bool vhdController::read_vhd(char *buffer, int blockId, int len) {
    //ifstream vhd_file;
    //vhd_file.open(this->filename, ios::in | ios::binary);
    this->vfile.seekg(blockId * BLOCK_SIZE); //set postion in input sequence
    vfile.read(buffer, len);
    // delete []  buffer;
    return true;

}

bool vhdController::write_vhd(char *buffer, int blockId, int len) {

    if (blockId < BLOCK_NUM && len <= BLOCK_SIZE) {
        this->vfile.seekp(blockId * BLOCK_SIZE);
        this->vfile.write(buffer, len);
        //delete[] buffer;
        //vfile.close();
        return !this->vfile ? false : true;
    } else {
        return false;
    }
}

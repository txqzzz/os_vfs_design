//
// Created by Xingqi Tang on 2018/7/5.
//

#ifndef VFS_SBCONTROLLER_H
#define VFS_SBCONTROLLER_H


class sbController {
public:
    sbController();

    ~sbController();

    bool alloc_sb();

    bool free_sb();

    bool load_sb();

    bool read_sb();

private:
    //
};


#endif //VFS_SBCONTROLLER_H

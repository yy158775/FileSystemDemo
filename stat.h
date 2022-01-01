#include<stdio.h>

typedef unsigned int ino_t;
typedef unsigned int block_t;
typedef unsigned int mode_t;

enum {
    DT_BLK = 0, //block device
    DT_CHR,
    DT_DIR,     //directory 
    DT_FIFO,
    DT_SOCK,
    DT_REG      //file
};

enum {
    R = 0,  //read
    W,      //write
    X       //execute
};

struct inode
{
    mode_t mode; //权限
    size_t block_count; //块数
    block_t block[10];  //对应磁盘块号
};

struct dirent 
{
    ino_t d_ino; 
    unsigned char d_type;
    char d_name[256];
};
const int size = sizeof(dirent)*10;


struct block 
{
    union
    {
        dirent b_dir[10];
        char filedata[size];
    }data;
};

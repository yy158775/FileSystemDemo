#pragma once

#include <stdio.h>

typedef unsigned int ino_t;
typedef unsigned int block_t;

//D_TYPE
enum D_TYPE {
    DT_BLK = 0, //block device
    DT_CHR,
    DT_DIR,     //directory 
    DT_FIFO,
    DT_SOCK,
    DT_REG      //file
};

//I_MODE
enum I_MODE {
    R = 0,  //read
    W,      //write
    X       //execute
};

struct block_seq
{
    block_t i_block_num;
};

struct inode
{
    I_MODE i_mode; //权限
    block_t i_block[10];
    ino_t i_ino; //节点号
    unsigned int i_count; //引用计数
    unsigned int i_offsize; //字节 i_offsize from 0 to block_size - 1; block[blocks-1]
    size_t i_blocks; //块数
};

struct dirent 
{
    ino_t d_ino; 
    D_TYPE d_type; //能不能读取元数据，是存放在目录项里面比较好，还是存放在metadata中比较好
    char d_name[256];
};

const unsigned int dircount = 10;
const unsigned int dir_size = sizeof(dirent);
const unsigned int block_size = dir_size*dircount;
const unsigned int index_size = sizeof(block_seq); //每个索引的长度

struct block 
{
    union
    {
        dirent b_dir[dircount];
        char filedata[block_size];
    }b_data;
};

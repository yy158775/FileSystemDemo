#include "stat.hpp"

//bitmap
//其他区先不做

inode inodes[100];
block blocks[1000];

block_t next_block() {
    static block_t next_block_num = 0;
    return next_block_num++;
}

ino_t next_inode() {
    static ino_t next_inode_num = 0;
    inodes[next_inode_num].i_ino = next_inode_num;
    return next_inode_num++;
}

void adjust_blocks_table(inode *dst_node,block_t block_num) {
    if(dst_node->i_blocks < 10) {
        dst_node->i_block[dst_node->i_blocks-1] = block_num;    
    }
    else if(dst_node->i_blocks == 10) {
        dst_node->i_block[9] = next_block(); //这个块是给索引用的        
        unsigned int *block_inode_index = (unsigned int *)(blocks[dst_node->i_block[9]].b_data.filedata);
        block_inode_index[0] = block_num; //这个块是存储数据的
        //用索引块指向存储数据的块        
        //这一点其实优点不太多吧
    } else {
        block_t block_index_num = dst_node->i_block[9];
        unsigned int *block_inode_index = (unsigned int *)(blocks[block_index_num].b_data.filedata);
        block_inode_index[dst_node->i_blocks - 10] = block_num; 
    }   
}
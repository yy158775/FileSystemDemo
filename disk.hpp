#pragma once

#include "stat.hpp"

extern inode inodes[100];
extern block blocks[1000];

block_t next_block();
ino_t next_inode();
void adjust_blocks_table(inode *dst_node,block_t block_num);
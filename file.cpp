#include "stat.hpp"
#include "disk.hpp"
#include <cstring>
#include <vector>

void printdir(inode *dst_inode);
inode *root_inode = &inodes[0];
inode *cur_inode;


//只支持在当前目录下创建
int mkdirorfile(const char *pathname,I_MODE mode) {
    //block用到哪里如何表示
    unsigned int block_count = cur_inode->i_blocks - 1;
    unsigned int offsize = cur_inode->i_offsize;
    block_t block_num = cur_inode->i_block[block_count];

    //新创建文件的inode
    ino_t ino_next = next_inode();
    inodes[ino_next].i_mode = mode;
    inodes[ino_next].i_count++;
    inodes[ino_next].i_blocks = 0;
    inodes[ino_next].i_offsize = 0;
    
    //调整目录项
    blocks[block_num].b_data.b_dir[offsize/dir_size].d_ino  = ino_next;
    blocks[block_num].b_data.b_dir[offsize/dir_size].d_type = DT_DIR;
    strcpy(blocks[block_num].b_data.b_dir[offsize/dir_size].d_name,pathname);

    
    //修改父inode的信息
    cur_inode->i_offsize += dir_size;
    if(cur_inode->i_offsize >= block_size) {
        cur_inode->i_blocks++;
        cur_inode->i_offsize = 0;
        //就是当前块不够用了
        //要开始分配新的块了,这个块是给记录数据用的
        //但是一旦增加数据块，会不会导致索引项不够用呢
        adjust_blocks_table(cur_inode,next_block());         
    }
}

// int rmfile(const char *filename) {

// }

// int rmdir(const char *pathname) {

// }

int ls(const char* pathname) {
    if(pathname[0] == '\0') {
        printdir(cur_inode);
    } else {
        inode* i = solvedir(pathname);
    }
}

inode *solvedir(const char* pathname) {
    const size_t str_size = strlen(pathname);
    char path_t[str_size];
    strcpy(path_t,pathname);
    
    std::vector<char*>names;
    char *name = strtok(path_t,"/");
    while (name != NULL)
    {
      names.push_back(name);
      name = strtok (NULL, "/");
    }

    inode *search_inode;
    if(pathname[0] == '/') {
        search_inode = root_inode;
    } else {
        search_inode = cur_inode;
    }

    for(int i = 0;i < names.size();i++) {
        // search_inode-> 不行一个一个太麻烦，得用链表，去做，用指针
        // 改一改目录项的表示形式
        // 需要深厚的数据结构知识
        // 感觉还是和索引有直接练习，到底该怎么去改数据结构的知识
    }
}


int cd(const char* pathname) {
    
}

int pwd() {

}

void printdir(inode *dst_inode) {
    int size = dst_inode->i_offsize;
    
    for(size_t i = 0;i < dst_inode->i_blocks-1;i++) {
        for(size_t j = 0;j < dircount;j++) {
            printf("inode:%d\n",blocks[dst_inode->i_block[i]].b_data.b_dir[j].d_ino);
            printf("name: %s\n",blocks[dst_inode->i_block[i]].b_data.b_dir[j].d_name);
            printf("type: %d\n\n",blocks[dst_inode->i_block[i]].b_data.b_dir[j].d_type);
        }
    }

    block_t final_block = dst_inode->i_blocks - 1;
    int num = size/dir_size;
    int i = 0;
    while(i < num) { //无符号整数会出现什么问题
        size -= dir_size;
        printf("inode:%d\n",blocks[final_block].b_data.b_dir[i].d_ino);
        printf("name: %s\n",blocks[final_block].b_data.b_dir[i].d_name);
        printf("type: %d\n\n",blocks[final_block].b_data.b_dir[i].d_type);
        i++;
    }
}
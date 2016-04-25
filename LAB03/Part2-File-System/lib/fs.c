#include <stdlib.h>
#include <stdio.h>

typedef struct inode inode;
typedef struct data_block data_block;
typedef struct super_block super_block;


struct inode{
  char name[8];           // file name
  int32_t  size;             // file size (in number of blocks)
  int32_t  blockPointers[8]; // direct block pointers
  int32_t  used;             // 0 => inode is free; 1 => in use
};

struct super_block{
  char free_block_list[128];
  inode i[16];
};


// memory allocation routines.
inode *new_inode(){
  inode *new_i = (inode *)malloc(sizeof(inode));
  return new_i;
}

super_block *new_super_block(){
  super_block *s = (super_block *)malloc(sizeof(super_block));

  // init the free block to a '0' character type
  for (size_t i = 0; i < 16; i++) {
    /* code */
    s->free_block_list[i] = '0';
    s->i[i] = *new_inode();
    // printf("size of new_inode: %lu\n", sizeof(s->i[i]));
  }
  return s;
}

// overview: the fs API.
/*
TODO: create a new file with this name and with these many blocks.
(We shall assume that the file size is specified at file creation time
and the file does not grow or shrink from this point on)
*/
void create(char name[8], int32_t size){

}

// overview: delete the file with this name.
void delete(char name[8]){

}

// overview: read the specified block from this file into the specified buffer;
// blockNum can range from 0 to 7.
void read(char name[8], int32_t blockNum, char buf[1024]){

}

// overview: write the data in the buffer to the specified block in this file.
void write(char name[8], int32_t blockNum, char buf[1024]){

}

// overview: list the names of all files in the file system and their sizes.
void ls(){

}

// the main driving method.
int main(int argc, char const *argv[]) {
  /* code */
  super_block *sb = new_super_block();
  sb->free_block_list[0] = '1'; // the super block is not free.

  printf("size of super block: %lu\n", sizeof(*sb));
  return 0;
}

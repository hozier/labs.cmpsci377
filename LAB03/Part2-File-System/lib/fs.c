#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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

  // new addition: init the free block to a '0' character type
  for (size_t i = 0; i < 16; i++) {
    /* code */
    s->free_block_list[i] = '0';
    s->i[i] = *new_inode();
    // printf("size of new_inode: %lu\n", sizeof(s->i[i]));
  }
  return s;
}

super_block *sb;

// overview: the fs API.
/*
create a new file with this name and with these many blocks.
(We shall assume that the file size is specified at file creation time
and the file does not grow or shrink from this point on)
*/

void create(char name[8], int32_t size){
  for(int j = 0; j<16; j++){
    if(sb.i[j].used ==0){
      FILE disk = fopen("../resources/disk0", "r+");
      FILE *newFile = fopen(name, "w");
      int blockNum = super_block.i[j]
      //somehow limit size of fi
      fseek(disk, 0 ,(SEEK_SET+(48*j))); //not a method yet
      fwrite(file, 1024, 8, disk);
      sb.i[j].used =1;
      sb.i[j].name = name;
      break;
    }
  }
}

// overview: delete the file with this name.

void delete(char name[8]){
  for(int j=0; j<16;j++){
    if(sb.i[j].name == name){
      FILE *disk = fopen("../resources/disk0", "r+");
      fseek(disk, blockNum*1024, (SEEK_SET+(48*j)));
      //delete FILE
      sb.i[j].name = "";
      sb.i[j].used =0;
    }
  }
}

// overview: read the specified block from this file into the specified buffer;
// blockNum can range from 0 to 7.

void read(char name[8], int32_t blockNum, char buf[1024]){
  for(int j = 0; j<16; j++){
    if(sb.i[j].name == name){
      FILE *disk = fopen("../resources/disk0", "r+");
      fseek(disk, blockNum*1024, (SEEK_SET+(48*j));
      fread(buf, 1024, 1, disk);
    }
  }
}

// overview: write the data in the buffer to the specified block in this file.

void write(char name[8], int32_t blockNum, char buf[1024]){
  for(int j = 0; j<16; j++){
    if(sb.i[j].name == name){
      FILE *disk = fopen("../resources/disk0", "r+");
      fseek(disk, blockNum*1024, (SEEK_SET+(48*j)));
      fwrite(buf, 1024, 1 disk);
    }
  }
}

// overview: list the names of all files in the file system and their sizes.
void ls(){
  for(int j = 0; j<16; j++){
    if(sb.i[j].used==1){
      printf('%s\n',sb.i[j].name);
    }
  }
}

// overview: returns a string array of the currently parsed line in input text file
// excludes new line characters as well as the firstmost line known as 'disk0'
char ** dictionary(char *str){
  const char s[2] = " ";
  char **dictionary = (char **)malloc(sizeof(char)*3);
  int i = 0;

  /* get the first token */
  dictionary[i] = strtok(str, s);

  /* walk through other tokens */
  while( dictionary[i] != NULL ){
    // printf( " %s\n", dictionary[i] ); // debug.
    dictionary[++i] = strtok(NULL, s);
  }
  return dictionary;
}

// overview: parse each line of the input file, execute parsed commands
void parse(){
  FILE *fp = fopen("../resources/lab3Input.txt", "r");
  char * line = NULL;
  size_t len = 0;

  size_t read;
  while ((read = getline(&line, &len, fp)) != -1) {
    // printf("Retrieved line of length %zu :\n", read); //debug.
    if(line[0] != '\n' && strcmp(line, "disk0") != 0){
      // printf("%s", line); // debug
      char command = line[0];
      char **options = dictionary(line); // works! // options for fs API calls.
        switch (command){
          case 'C':
            create(options[1], atoi(options[2]));
            break;
          case 'L':
            ls();
            break;
          case 'R':
            read(options[1],atoi(options[2]), NULL);
            break;
          case 'W':
            write(options[1],atoi(options[2]), NULL);
            break;
          case 'D':
            delete(options[1]);
            break;
          default:
            break;
      } // end switch.
    }
  } // end while.
}

// the main driving method.
int main(int argc, char const *argv[]) {
  /* code */
  super_block *sb = new_super_block();
  sb->free_block_list[0] = '1'; // the super block is not free.
  parse();


  printf("size of super block: %lu\n", sizeof(*sb));
  return 0;
}

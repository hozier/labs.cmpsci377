#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// overview: the data structure of the fs
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

// overview: memory allocation routine.
inode *new_inode(){
  inode *new_i = (inode *)malloc(sizeof(inode));
  return new_i;
}

// overview: memory allocation routine.
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

// overview: the global super_block variable.
super_block *sb;

// overview: the file system interface
// (otherwise known as the fs API.)
/*
create a new file with this name and with these many blocks.
(We shall assume that the file size is specified at file creation time
and the file does not grow or shrink from this point on)
*/
void create(char name[8], int32_t size){
  for(int j = 0; j<16; j++){
    if(sb->i[j].used ==0){
      printf("inode [%d] of the super block is free\n", j);
      FILE *disk = fopen("../resources/disk0", "r+");
      FILE *newFile = fopen(name, "w");

      fseek(disk, 0 ,(SEEK_SET+(48*j)));
      fwrite(newFile, 1024, 8, disk);
      sb->i[j].used =1;
      strcpy(sb->i[j].name, name);       // replaced: <====> sb->i[j].name = name;
      break;
    }
  }
}

// overview: delete the file with this name.
void delete(char name[8]){
  for(int j=0; j<16;j++){
    if(strcmp(sb->i[j].name, name) == 0 ){
      printf("inode [%d] of the super block contains %s\n", j, name);
      FILE *disk = fopen("../resources/disk0", "r+");
      fseek(disk, 0, (SEEK_SET+(48*j)));
      //delete FILE
      for(int k = 0; k<8; k++){
          sb->i[j].name[k] = '\0';
      }
      sb->i[j].used =0;
    }
  }
}

// overview: read the specified block from this file into the specified buffer;
// blockNum can range from 0 to 7.
void read(char name[8], int32_t blockNum, char buf[1024]){
  for(int j = 0; j<16; j++){
    if(strcmp(sb->i[j].name, name) == 0 ){
      printf("inode [%d] of the super block contains %s\n", j, name);
      FILE *disk = fopen("../resources/disk0", "r+");
      fseek(disk, blockNum*1024, (SEEK_SET+(48*j)));
      fread(buf, 1024, 1, disk);
    }
  }
}

// overview: write the data in the buffer to the specified block in this file.
void write(char name[8], int32_t blockNum, char buf[1024]){
  for(int j = 0; j<16; j++){
    if(strcmp(sb->i[j].name, name) == 0 ){
      printf("inode [%d] of the super block contains %s\n", j, name);
      FILE *disk = fopen("../resources/disk0", "r+");
      fseek(disk, blockNum*1024, (SEEK_SET+(48*j)));
      fwrite(buf, 1024, 1, disk);
    }
  }
}

// overview: list the names of all files in the file system and their sizes.
void ls(){
  for(int j = 0; j<16; j++){
    if(sb->i[j].used==1){
      printf("%s\n",sb->i[j].name);
    }
  }
  printf("\n\n");
}

// overview: returns a string array of the currently parsed line in input text file
// excludes new line characters as well as the firstmost line known as 'disk0'
char ** dictionary(char *str, const char *delim){
  char **dictionary = (char **)malloc(sizeof(char*)*3);
  char *token = NULL;
  int i = 0;

  /* get the first token */
  token = strtok(str, delim);
  dictionary[i] = token;

  /* walk through other tokens */
  while(1){
    // printf( " %s\n", dictionary[i] ); // debug.
    token = strtok(NULL, delim);
    if(token != NULL){ dictionary[++i] = token; }
    else {
      char **L = &dictionary[0];
      free(dictionary);
      return L;
    }
  }
  return dictionary;
}

// overview: logs all interactions to disk0
void debug(char *command, char *name, int32_t blockNum){
  printf("%s %s using [%d] blocks of memory.\n", command, name, blockNum);
}

// overview: the entry point for calls to the file system interface
void exec(char **options){
  // the name of file on disk
  char buf[1024];
  char *command = options[0];
  char *name = options[1];
  switch (*command){
    case 'C':
      create(name, atoi(options[2]));
      debug("Created", name, atoi(options[2]));
      break;
    case 'L':
      // printf("calling ls:\n");
      debug("\n\nListing", "cwd", 0);
      ls();
      break;
    case 'R':
      read(name,atoi(options[2]), buf);
      debug("Read", name, atoi(options[2]));
      break;
    case 'W':
      write(name,atoi(options[2]), buf);
      debug("Wrote", name, atoi(options[2]));
      break;
    case 'D':
      name[strlen(name)-1] = '\0';
      delete(name);
      debug("Deleted", name, 0);
      break;
    default:
      break;
  } // end switch.
}

// overview: parse each line of the input file, execute parsed commands
void parse(char *path){
  FILE *fp = fopen(path, "r");
  char line[20];

  while( fgets (line, 20, fp)!=NULL ){ exec(dictionary(line, " "));   } // the engine implemented against the fs API.
  fclose(fp); // end of fs API usage.
}

// the main driving method.
int main(int argc, char const *argv[]) {
  /* code */

  // overview: initialize the new global super block
  sb = new_super_block();
  sb->free_block_list[0] = '1'; // the super block is not free.
  parse("../resources/lab3Input.txt"); // run the file system interface

  // overview: verifying the size of the space on the fs data structure.
  printf("size of super block: %d\n", (int)sizeof(*sb));
  return 0;
}

#include "paging.h"
#include <math.h>

// TODO: needs to be deubugged.
 uint16_t compute_page_number( uint16_t m,  uint16_t n,  uint16_t v){
  return (v >> ((1 << (n))-2));  // v DIV 2**n
}
 uint16_t compute_page_offset( uint16_t m,  uint16_t n,  uint16_t v){
  uint16_t d = (1 << (n)) - 1; // v MOD 2**n
  return v & (d);
}

void init_data_structure(logical_address **logical_address_array, uint16_t *v, int S){
  // initialize logical add. array
  for (size_t i = 0; i < S; i++) {
    /* code */
    // printf("ghjkl %d\n", v[i+2]);

    logical_address_array[i] = new_logical_address();
    logical_address_array[i]->page_number = compute_page_number(v[1], v[0], v[i+2]);
    logical_address_array[i]->offset =   compute_page_offset(v[1], v[0], v[i+2]);
    printf("virtual address v[%lu]=%d is in page number %u and offset %u\n", i, v[i+2], logical_address_array[i]->page_number, logical_address_array[i]->offset );
  }
}

int SIZE(FILE *fp){
  int lines = 0;
  char ch = '\0';
  while(!feof(fp)){
    ch = fgetc(fp);
    if(ch == '\n'){ lines++; }
  }
  return lines;
}

void parse_logical_addresses(FILE *fp, int S, uint16_t l[]){
  int lines = 0;
  int ch;
  while(!feof(fp)){
    fscanf (fp, "%hd", (&l[lines]));
    lines++;
  }
}


int main(int argc, char const *argv[]) {
  /* code */

  FILE *fp;

  fp = fopen("trace", "r");
  int S = SIZE(fp)-2; // size of logical_address array

  logical_address **logical_address_array = (logical_address **)malloc(sizeof(logical_address) * S); // creates an array of logical_address structs
  fp = fopen("trace", "r");


  uint16_t v[S]; // virtual addresses from file
  parse_logical_addresses(fp, S, v);

  init_data_structure(logical_address_array, v, S);
  fclose(fp);

  return 0;
}

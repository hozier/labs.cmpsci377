#include "paging.h"

// overview: helps map the page number of a given logical address
uint16_t compute_page_number( uint16_t m,  uint16_t n,  uint16_t v){
  return (v >> ((1 << (n))-2));  // v DIV 2**n
}

// overview: helps map the offset of a given logical address
uint16_t compute_page_offset( uint16_t m,  uint16_t n,  uint16_t v){
  uint16_t d = (1 << (n)) - 1; // v MOD 2**n
  return v & (d);
}

// overview: performs the mappings of v. addresses which need to be mapped
// from to a page number and an offset
void perform_paging(logical_address **logical_address_array, uint16_t *v, int S){
  // initialize logical add. array
  for (size_t i = 0; i < S; i++) {
    /* code */
    logical_address_array[i] = new_logical_address();
    logical_address_array[i]->page_number = compute_page_number(v[1], v[0], v[i+2]);
    logical_address_array[i]->offset =   compute_page_offset(v[1], v[0], v[i+2]);
    printf("virtual address v[%lu]=%d is in page number %u and offset %u\n", i, v[i+2], logical_address_array[i]->page_number, logical_address_array[i]->offset );
  }
}

// overview: returns the length of the virtual address array to be created
int SIZE(FILE *fp){
  int lines = 0;
  char ch = '\0';
  while(!feof(fp)){
    ch = fgetc(fp);
    if(ch == '\n'){ lines++; }
  }
  return lines;
}

// overview: parses the virtual addresses from tracefile
void parse_logical_addresses(FILE *fp, int S, uint16_t l[]){
  int lines = 0;
  int ch;
  while(!feof(fp)){
    fscanf (fp, "%hd", (&l[lines]));
    lines++;
  }
}

// the main driving method.
int main(int argc, char const *argv[]) {
  /* code */

  FILE *fp; // file descriptor
  fp = fopen("trace", "r"); // paging tracefile
  int S = SIZE(fp)-2; // size of logical_address array
  uint16_t v[S]; // virtual addresses from file

  // overview: creates an array of logical_address structs which will store
  // the computed page numbers and offsets
  logical_address **logical_address_array = (logical_address **)malloc(sizeof(logical_address) * S);

  // overview: parse trace
  parse_logical_addresses(fopen("trace", "r"), S, v);

  // overview: complete mappings from v[i] to (p) and (d)
  // store mappings within the pointer array `logical_address_array`
  perform_paging(logical_address_array, v, S);
  fclose(fp);

  return 0;
}

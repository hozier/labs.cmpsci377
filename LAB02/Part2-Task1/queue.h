#include<stdio.h>
#include <stdlib.h>

// overview: implemented data structures below:
typedef struct node node;
typedef struct data data;
typedef struct linked linked;

struct node{
  node *next;
  data *data;
  int request_id;
};

struct data{
  int id;
  int length;
};

struct linked{
  node *head;
  int r_id;
  int counter; // counts how many requests have ever been sent to queue
  int N; // size of queue
  int offset_count;
};

// overview: memory allocation function
// returns a new node pointer
node *new_node();

// overview: memory allocation function
// returns a new linked pointer
linked *new_linked();

// overview: memory allocation function
// @params: length of request (int) and optional int
// returns a new data pointer
data *new_data(int additional_data, int request_length);

// overview: add to tail of queue
// @params: a linked pointer
// returns void
void add(linked *l, data *d);

// overview: pop and return the head of queue
// @params: a linked pointer
// returns a node pointer
node *pop(linked *l);

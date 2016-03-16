#include<stdio.h>
#include <stdlib.h>

// overview: implemented data structures
typedef struct node node;
typedef struct data data;
typedef struct linked linked;

struct node{
  node *next;
  data *data;
  int request_id;
};

struct data{
  int additional_data;
};

struct linked{
  node *head;
  int counter;
};

// overview: allocation functions
node *new_node();

linked *new_linked();

data *new_data(int additional_data);


// overview: add to tail of queue
void add(linked *l, data *d);

// overview: pop and return the head of queue
node *pop(linked *l);

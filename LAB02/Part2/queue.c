#include<stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "queue.c"

// overview: circular queue implementation.

// overview: allocation functions
data *new_data(int additional_data){
  data *d = (data *)malloc(sizeof(data));
  return d;
}

node *new_node(){
  node *n = (node *)malloc(sizeof(node));
  n->next = NULL;
  return n;
}

linked *new_linked(int N){
  linked *l = (linked *)malloc(sizeof(linked));
  l->r_id = 0;
  l->counter = 0;
  l->offset_count = 0;
  l->head = NULL;
  l->N = N;
  return l;
}

// overview: initialize new node
void init_new_node(node *ptr, linked *l, data* d){
  ptr->next = new_node();
  ptr->next->data = d;
  ptr->next->request_id = l->r_id;
  ++(l->r_id); // increments the r_id of the queue so that each node can have a unique request_id;
  ++(l->counter); // increments the current number of requests in the queue
}


// overview: initialize new linked list/queue
void init_new_queue(linked *l, data* d){
  l->head = new_node();
  l->head->data = d;
  l->head->request_id = l->r_id;
  ++(l->r_id); // increments the r_id of the queue so that each node can have a unique request_id;
  ++(l->counter); // increments the current number of requests in the queue
}


// overview: add to tail of (circular) queue
void add(linked *l, data *d){
  node *ptr = l->head;
  if (l->head == NULL/* condition */) {
    init_new_queue(l, d);
    return;
  }

  // if we have reached the limit of our bounded queue, then wrap around...
  // and compute offset.
  if(l->counter >= l->N){
    l->offset_count = (l->offset_count == l->N)?0:l->offset_count; // reset if offset_count is >= N..
    // printf("if (offset_count[%d]) == (N[%d])\n", l->offset_count, l->N);
    int offset = ((l->counter) % l->N) +  l->offset_count; // cycle through this many elements in the queue (beginning at the head) -- and insert at this position.
    printf("offset_count: %d\n", l->offset_count);

    int i = 0;
    node *prev = l->head;
    // printf("offset count: %d\n", l->offset_count);

    if(offset == 0){
      l->head = new_node();
      l->head->data = d;
      l->head->request_id = l->r_id;
      l->head->next = ptr->next;
      ++(l->r_id); // increments the r_id of the queue so that each node can have a unique request_id;
      ++(l->offset_count);
      return;
    }

    while (i <= offset) {
      /* code */
      prev = ptr;
      ptr = ptr->next;
      ++i;
    }


    node *temp = ptr;
    prev->next = new_node();
    prev->data = d;
    prev->request_id = l->r_id;
    ++(l->r_id); // increments the r_id of the queue so that each node can have a unique request_id;
    if(temp == NULL){ prev->next->next = temp;}
    else{ prev->next->next = temp->next; }

    ++(l->offset_count);

    return;
  }

  while (ptr->next != NULL/* condition */) {
    /* code */
    ptr = ptr->next;
  }
  init_new_node(ptr, l, d);
}

// overview: pop and return the head of queue
node *pop(linked *l){
  if (l->head == NULL){ return NULL; }
  if (l->head->next == NULL){
    node *n = l->head;
    l->head = NULL;
    (l->offset_count>0)?--(l->offset_count):0;
    --(l->counter); // decrements the current number of requests in the queue
    return n;
  }
  else{
    node *n = l->head;
    l->head = NULL;
    l->head = n->next;
    --(l->counter); // decrements the current number of requests in the queue
    (l->offset_count>0)?--(l->offset_count):0;
    return n;
  }
}

// overview: testing expected behavior of queue.
int main(int argc, char const *argv[]) {
  /* code */
  linked *queue = new_linked(10);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  pop(queue);
  pop(queue);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  pop(queue);
  pop(queue);
  add(queue, NULL);
  add(queue, NULL);
  pop(queue);
  add(queue, NULL);
  add(queue, NULL);
  // add(queue, NULL);
  // add(queue, NULL);
  // add(queue, NULL);
  add(queue, NULL);
  // pop(queue);
  // add(queue, NULL);
  // add(queue, NULL);
  // add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);
  add(queue, NULL);

  node *ptr =queue->head;
  while (ptr !=NULL/* condition */) {
    /* code */
    printf("request_id: %d\n", ptr->request_id);
    ptr = ptr->next;
  }
  printf("queue size: %d\n", queue->counter);
  return 0;
}

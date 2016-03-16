#include<stdio.h>
#include <stdlib.h>
#include "queue.h"
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

linked *new_linked(){
  linked *l = (linked *)malloc(sizeof(linked));
  l->counter = 0;
  l->head = NULL;
  return l;
}

// overview: add to tail of queue
void add(linked *l, data *d){
  node *ptr = l->head;
  if (ptr == NULL/* condition */) {
    /* code */
    l->head = new_node();
    l->head->data = d;
    l->head->request_id = l->counter;
    ++(l->counter);
    return;
  }

  while (ptr->next != NULL/* condition */) {
    /* code */
    ptr = ptr->next;
  }
  ptr->next = new_node();
  ptr->next->data = d;
  ptr->next->request_id = l->counter;
  ++(l->counter); // increments the counter of the queue so that each node can have a unique request_id;
}

// overview: pop and return the head of queue
node *pop(linked *l){
  if (l->head == NULL){ return NULL; }
  if (l->head->next == NULL){
    node *n = l->head;
    l->head = NULL;
    return n;
  }
  else{
    node *n = l->head;
    l->head = n->next;
    return n;
  }
}

// overview: testing expected behavior of queue.
// int main(int argc, char const *argv[]) {
//   /* code */
//   linked *queue = new_linked();
//   add(queue, NULL);
//   add(queue, NULL);
//   add(queue, NULL);
//   add(queue, NULL);
//   add(queue, NULL);
//   add(queue, NULL);
//   add(queue, NULL);
//   add(queue, NULL);
//   add(queue, NULL);
//   pop(queue);
//   pop(queue);
//
//   node *ptr =queue->head;
//   while (ptr !=NULL/* condition */) {
//     /* code */
//     printf("request_id: %d\n", ptr->request_id);
//     ptr = ptr->next;
//   }
//   return 0;
// }

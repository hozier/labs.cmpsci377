#include<stdio.h>
#include<pthread.h>


semaphore mutex = 1
semaphore fillCount = 0;
semaphore emptyCount = BUFFER_SIZE;

void listen_request(void *arg){
  while(true){
    item = produceItem();
    down(emptyCount);
      down(mutex);
        add_to_request_queue(item);
      up(mutex);
    up(fillCount);
  }
}

void consumer(){
  while(true){
    down(fillCount);
      down(mutex);
        item = remove_from_request_queue();
      up(mutex);
    up(emptyCount);
    consumeItem(item);
  }
}

int main(){
  int pthread_create(pthread_t *master, //actual thread object, master is it's name
    pthread_attr_t *attr,   //attributes applied to this thread
    void *(*listen_request)(void *), //this thread executes listen_request
    void *arg);   //arguments to pass to thread function above
}

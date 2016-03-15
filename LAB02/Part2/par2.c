#include<stdio.h>
#include<pthread.h>

// void listen_request(void *arg){
//   while(true){
//     item = produceItem();
//     down(emptyCount);
//       down(mutex);
//         add_to_request_queue(item);
//       up(mutex);
//     up(fillCount);
//   }
// }

void down(sem_t *s){ sem_wait(s); }

void up(sem_t *s){ sem_post(s); }

// overview: (producer) function which is executed by master thread
void listen_request(void *arg){
  while (1) {
    /* code */
    //item = produceItem();
    down(&empty_count);
    down(&mutex);
    //  add_to_request_queue(item);
    up(&mutex);
    up(&fill_count);
    // consumeItem(item);
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
  sem_t mutex;
  sem_t fill_count;
  sem_t empty_count;

  // api: int sem_init(sem_t *sem,
  // int pshared [0 for shared between the threads of a process],
  // unsigned int value);
  sem_init(&mutex, 0, 1);
  sem_init(&fill_count, 0, 0); // items produced
  sem_init(&empty_count, 0, N); // remaining space, N == BUFFER_SIZE

  // overview: master executes listen.
  int pthread_create(pthread_t *master, //actual thread object, master is it's name
    pthread_attr_t *attr,   //attributes applied to this thread
    void *(*listen_request)(void *), //this thread executes listen_request
    void *arg);   //arguments to pass to thread function above
}

/*
 The producer's job is to generate a piece of data, put it into the buffer and start again.
 At the same time, the consumer is consuming the data (i.e., removing it from the buffer) one piece at a time.

 The problem is to make sure that the producer won't try to add data into the buffer if it's full
 and that the consumer won't try to remove data from an empty buffer.

 The solution for the producer is to either go to sleep or discard data if the buffer is full.

*/
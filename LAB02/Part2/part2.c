#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <stdlib.h>
#include "queue.h"

typedef struct thread_data{
  int id;
} thread_data;

sem_t mutex;
sem_t fill_count; //number of job requests in queue
sem_t empty_count; //number of spaces available
linked *queue;
pthread_t master;
//pthread_t slaves[];
int res0;


//void down(sem_t *s){ sem_wait(s); }
//void up(sem_t *s){ sem_post(s); }

// overview: (producer) function which is executed by master thread
void producer(){
  while (1) {
    /* code */
    data item = produceItem();
    sem_wait(&empty_count);
      sem_wait(&mutex);
        add_to_request_queue(queue, item);
      sem_post(&mutex);
    sem_post(&fill_count);
  }
}

void consumer(){
  while(1){
    sem_wait(&fill_count); //wait until theres an item
      sem_wait(&mutex); //get access to buffer
        node* request = pop(queue);
      sem_post(&mutex); //release buffer for access for someone else
    sem_post(&empty_count); //one more free slot
    //consume(request);
  }
}


int main(int argc, char const *argv[]) {
  /* code */

  int buffer;
  printf("Enter a Buffer Size\t ");
  scanf("%d",&buffer); //read in desired buffer Size
  int N = buffer;

  int num;
  printf("Enter Number of Slaves:\t "); //ask for number of slaves
  scanf("%d",&num); //read in number of slaves
  int slave_number = num; //assign number of slaves to variable

  // api: int sem_init(sem_t *sem,
  // int pshared [0 for shared between the threads of a process],
  // unsigned int value);
  sem_init(&mutex, 0, 1);
  sem_init(&fill_count, 0, 0); // items produced
  sem_init(&empty_count, 0, N); // remaining space, N == BUFFER_SIZE

  queue = new_linked(N);

  // overview: master executes listen.
  // int pthread_create(pthread_t *master, //actual thread object, master is it's name
  //   pthread_attr_t *attr,   //attributes applied to this thread
  //   void *(*listen_request)(void *), //this thread executes listen_request
  //   void *arg);   //arguments to pass to thread function above

  //int res0 = pthread_create(&master, NULL, void *(*producer)(void *), void *arg);
  res0 = pthread_create(&master, NULL, (void *)producer, (void *) "");
  //create an array of slave threads
  pthread_t slaves[slave_number]; //initialize array of threads for slaves
  thread_data slaves_with_id[slave_number]; //initialize array of threads for slaves that will also have an id
  int i, rc;
  for(i = 0; i <slave_number; ++i){
    slaves_with_id[i].id = i; //assign in id to each index
    // if((rc = pthread_create(&slaves[i],NULL, consumer, &slaves_with_id[i]))){
    //   fprintf(stderr, "error: pthread_create, rc: %d\n",rc);
    //   return EXIT_FAILURE;
    // }
    rc = pthread_create(&slaves[i],NULL, (void*)consumer, &slaves_with_id[i]);
  }
  // for (i=0;i <slave_number; ++i){
  //   pthread_join(slaves[i], NULL);
  // }
  return 0;

}

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include"queue.h"
#include "queue.c"

typedef struct thread_data{
  int id;
  char* status[];
} thread_data;

sem_t mutex;
sem_t fill_count; //number of job requests in queue
sem_t empty_count; //number of spaces available
linked *queue;
pthread_t master;
//pthread_t slaves[];
int res0;
data *new_request;
node *request;

// overview: (producer) function which is executed by master thread
void producer(){
  while (1) {
    /* code */
    new_request = new_data(5);
    sem_wait(&empty_count);
      sem_wait(&mutex);
        add(queue, new_request);
      sem_post(&mutex);
    sem_post(&fill_count);
  }
}

void consumer(){
  while(1){
    sem_wait(&fill_count); //wait until theres a new request
      sem_wait(&mutex); //get access to buffer
          request = pop(queue);
      sem_post(&mutex); //release buffer for access for someone else
    sem_post(&empty_count); //one more free slot
    int duration = request->data->additional_data;
    sleep(duration);
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

  res0 = pthread_create(&master, NULL, (void *)producer, (void *) "");
  //create an array of slave threads
  pthread_t slaves[slave_number]; //initialize array of threads for slaves
  thread_data slaves_with_id[slave_number]; //initialize array of threads for slaves that will also have an id
  int i, rc;
  for(i = 0; i <slave_number; ++i){
    slaves_with_id[i].id = i; //assign in id to each index
    rc = pthread_create(&slaves[i],NULL, (void*)consumer, &slaves_with_id[i]);
  }
  pthread_join(master, NULL);
  for (i=0;i <slave_number; ++i){
    pthread_join(slaves[i], NULL);
  }
  return 0;
}

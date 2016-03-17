#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<unistd.h>
#include"queue.h"
//#include"queue.c"
//#include"queue.c"

typedef struct thread_data{
  int id;
  //char* status[];
} thread_data;

sem_t mutex;
sem_t fill_count; //number of job requests in queue
sem_t empty_count; //number of spaces available
linked *queue;
pthread_t master;

int request_id=0;
int res0;
int sleep_time; //given by user input
int request_max_length; //given by user input
int random_length;
data *new_request;
node *request;

// overview: (producer) function which is executed by master thread
void producer(){
  printf("im a producer\n");
  while (1) {
    sem_wait(&empty_count); //wait for available space
      sem_wait(&mutex); //grab mutex, blocking other threads
        random_length = rand() % request_max_length; //assign random job length
        new_request = new_data(request_id, random_length); //create new data using job id and length
        printf("Producer: produced request ID: %d , length: %d seconds\n", request_id, random_length);
        request_id = request_id + 1; //increment request id so next one is different
        add(queue, new_request); //add new request to queue
      sem_post(&mutex); //release mutex
    sem_post(&fill_count); //signal that theres a job available
    sleep(sleep_time); //sleep between requests based on time inputted by user
    printf("Producer: sleeping for: %d seconds\n", sleep_time);
  }
}

void consumer(void *arg){
  while(1){
    sem_wait(&fill_count); //wait until theres a new request
      sem_wait(&mutex); //get access to buffer
          request = pop(queue);
      sem_post(&mutex); //release buffer for access for someone else
    sem_post(&empty_count); //one more free slot
    int duration = request->data->length; //get duration of job
    int re_id = request->data->id; //get id of job
    thread_data *data = (thread_data *)arg; //get this threads data
    int thread_id = data->id; //get this threads id
    printf("Consumer %d: assigned request ID: %d , processing request for next %d seconds\n",thread_id, re_id, duration);
    sleep(duration); //simulate the time it takes to consume the request
    printf("Consumer %d:completed request ID: %d \n", thread_id , re_id);
  }
}


int main(int argc, char const *argv[]) {

  int buffer;
  printf("Enter a Buffer Size\t ");
  scanf("%d",&buffer); //read in desired buffer Size
  int N = buffer;

  int num;
  printf("Enter Number of Slaves:\t "); //ask for number of slave
  scanf("%d",&num); //read in number of slaves
  int slave_number = num; //assign number of slaves to variable

  int sleep;
  printf("how long should the producer sleep between requests:\t "); //ask how long the master should sleep between producing
  scanf("%d",&sleep); //read in how long the master should sleep between producing
  sleep_time = sleep;

  int max;
  printf("what should the maximum job request length be:\t "); //ask for maximum request length
  scanf("%d",&max); //read in maximum request length
  request_max_length = max;

  // api: int sem_init(sem_t *sem,
  // int pshared [0 for shared between the threads of a process],
  // unsigned int value);
  sem_init(&mutex, 0, 1);
  sem_init(&fill_count, 0, 0); // number of items produced
  sem_init(&empty_count, 0, N); // remaining space, N == BUFFER_SIZE
  queue = new_linked(N);
  pthread_create(&master, NULL, (void *)producer, (void *) "");
  //create an array of slave threads
  pthread_t slaves[slave_number]; //initialize array of threads for slaves
  thread_data slaves_with_id[slave_number]; //initialize array of threads for slaves that will also have an id
  int i;
  for(i = 0; i <slave_number; ++i){
     slaves_with_id[i].id = i; //assign in id to each index
     pthread_create(&slaves[i],NULL, (void*)consumer, &slaves_with_id[i]);
  }
  pthread_join(master, NULL); //activate master thread
  for (i=0;i <slave_number; ++i){
    pthread_join(slaves[i], NULL);
  } //activate slave threads
  return 0;
}

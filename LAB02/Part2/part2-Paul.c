#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

typedef struct thread_data{
  int id;
} thread_data;


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

int main(in argc, char **argv){
  sem_t mutex;
  sem_t num_in_buffer; //number of items already in buffer
  sem_t num_of_avail_spaces; //number of available spaces in buffer

  int pthread_create(pthread_t *master, //actual thread object, master is it's name
    pthread_attr_t *attr,   //attributes applied to this thread
    void *(*listen_request)(void *), //this thread executes listen_request
    void *arg);   //arguments to pass to thread function above

  printf("Enter Number of Slaves:\t "); //ask for number of slaves
  scanf("%d",&num); //read in number of slaves
  int slave_number = num; //assign number of slaves to variable

  printf("Enter a Buffer Size\t ")
  scanf("%d",&buffer); //read in desired buffer Size
  int buffer_size = buffer;

  pthread_t slaves[slave_number]; //initialize array of threads for slaves
  thread_data slaves_with_id[slave_number]; //initialize array of threads for slaves that will also have an id
  int i, rc;
  for(i = 0; i <slave_number; ++i){
    slaves_with_id[i].id = i; //assign in id to each index
    if((rc = pthread_create(&slaves[i],NULL, consumer, &slaves_with_id[i]))){
      fprintf(stderr, "error: pthread_create, rc: %d\n",rc);
      return EXIT_FAILURE;
    }
  }
  for (i=0;i <slave_number; ++i){
    pthread_join(slaves[i], NULL);
  }
  return EXIT_SUCCESS;
  }

  // api: int sem_init(sem_t *sem,
  // int pshared [0 for shared between the threads of a process],
  // unsigned int value);
  sem_init(&mutex, 0, 1);
  sem_init(&num_in_buffer, 0, 0); // items produced
  sem_init(&num_of_avail_spaces, 0, buffer_size); // remaining space, N == BUFFER_SIZE


}

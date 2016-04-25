import java.util.*;

public class Part2 {

	public static class request{
		public int id;
		public int length;
		public request(int request_id, int request_length){
			id = request_id;
			length = request_length;
		}
	}

	static long end, slept; //for sleeping
	static boolean interrupted= false;
	static int new_id= 0;
	static int buffer_size; // constant for the buffer size
  static int slave_number; //constant for number of slaves
  static int sleep_time;	//constant for time between producing jobs
  static int request_max_length; //constant for max_length
	static Producer producer; // instantiate a new Producer thread
  static Consumer consumer; // instantiate a new Consumer thread
  static Monitor monitor; // instantiate a new Monitor


    public static void main(String args[]) {
    	Scanner scan1 = new Scanner(System.in);
    	System.out.println("please enter a buffer size");
    	buffer_size  = scan1.nextInt();
    	System.out.println("Enter Number of Slaves");
    	slave_number  = scan1.nextInt();
    	System.out.println("Enter sleep time for producer between producing items");
    	sleep_time = scan1.nextInt();
    	System.out.println("What should the maximum job request length be?");
    	request_max_length  = scan1.nextInt();
    	scan1.close();

    	Consumer[] slaves = new Consumer [slave_number];
    	monitor = new Monitor();

    	producer = new Producer();
    	producer.start(); // start the Producer thread

    	for(int i=0; i < (slave_number); i++){
    		consumer = new Consumer(i);
    		slaves[i] = consumer;
    		slaves[i].start(); // start the Consumer thread
    	}
   }

   static class Producer extends Thread {
      public void run() {
         request r;
          while (true) {
             r = produceItem(); //get new request
             monitor.insert(r); //add new request to buffer
             try {
            	System.out.println("Producer: sleeping for " + sleep_time + " seconds");
							Thread.sleep(sleep_time*1000);
							//break;
						} catch (InterruptedException e) {
							System.out.println("Interrupt occurred!");
						}
         	}
      }
      private request produceItem() {
         Random rand = new Random();
         int num = rand.nextInt(request_max_length)+1; //get random job length
          request new_request = new request(new_id, num); //get id for job
          new_id = new_id+1; //increment id so next job will have a different one
         System.out.println("Producer: produced request ID "+ new_request.id + " , length " + num);
          return new_request;
      }
   } // class Producer

   public static class Consumer extends Thread {
	  public int id;
	  public Consumer(int cid){
		  id = cid;
	  }

    public void run() { // run method with the thread code
    	request r;
      while (true) { // consumer loop : infinite loop
          r = monitor.remove();
          consumeItem(r);
      }
    }
    private void consumeItem(request request) {
    	try {
        System.out.println("Consumer "+ this.id + ": assigned request ID " + request.id + " , processing request for next "+ request.length + " seconds");
				Thread.sleep(request.length*1000);
				System.out.println("Consumer "+ this.id + ": completed request ID " + request.id);
			} catch (InterruptedException e) {
				System.out.println("Interrupt occurred!");
			} //it takes the length of the job to consume the item
      }
   	} // class Consumer

   static class Monitor { // Monitor definition
       private request queue[] = new request [buffer_size];
       private int count = 0;
       private int lo = 0, hi = 0;

      public synchronized void insert(request job) {
         if (count==buffer_size) gotoSleep(); // if the buffer is full, go to sleep
          queue[hi] = job; // insert an item into the buffer
          hi=(hi+1)%buffer_size; // slot to place next item in
          count = count + 1; //keep track of how much is in buffer
          if (count==1) notify(); // if consumer was sleeping, wake it up
       } // insert()

      public synchronized request remove() {
         request job;
         if (count==0) gotoSleep(); // if the buffer is empty, go to sleep
          job = queue[lo]; // fetch an item from the buffer
          lo=(lo+1)%buffer_size; // slot to fetch next item from
          count = count - 1; //keep track of how much is in buffer
          if (count==buffer_size-1) notify(); // if producer was sleeping, wake it up
         return job;
       } // remove()

      private void gotoSleep() {
          try {
             wait(); // wait() can be interrupted
          } catch(InterruptedException exc) {
             System.out.println("Interrupt occurred!");
          };
      } // gotoSleep()
   } // class OurMonitor
} // class ProducerConsumer

import java.util.LinkedList;
import static java.lang.System.out;

/**
 * Created by phozier on 3/1/16.
 */

// command + shift + t; creates test.
public class RRScheduler { // main driver class
    int time_slice;
    int context_switch;
    int current_time;
    public LinkedList<Job> waiting_queue, // tracking threads which are waiting to run.
            ready_queue; // a way to add processes to the ready queue.


    public RRScheduler(int time_slice, int context_switch){
        this.time_slice = time_slice;
        this.context_switch = context_switch;

        // the new_queue has been hard coded into constructor for a simpler design
        this.waiting_queue = new LinkedList<>();
        this.ready_queue = new LinkedList<>();

        this.waiting_queue.add(new Job('A', 0, 10));
        this.waiting_queue.add(new Job('B', 4, 8));
        this.waiting_queue.add(new Job('C', 7, 3));
        this.current_time = 0;
    }

    // adds to ready queue, completes time slice computing and returns new, updated job
    // send time-sliced job to wait queue
    public Job add_to_ready_queue(Job j){
        if(j == null){ return null; }

        ready_queue.add(j);
        Job current_job = ready_queue.poll();
        //current_time - current_job.getWait_time() - current_job.getLength();

        if (current_job.getLength() == 0){ --current_time; }
        if (current_job.getLength() == -1){ // the job has finished! remove off CPU forever! :)
            out.printf("[Job %s has finished]\n\n",current_job.getProcessName());
            return null;
        }

        current_time+=time_slice;
        current_job.setLength(current_job.getLength() - time_slice);
        log(current_job);
        return current_job;
    }

    // adds jobs back onto the waiting queue which still have remaining execution times.
    // if a job is finished executing, it will NOT be placed back onto the waiting_queue.
    public void add_to_waiting_queue(Job j){
        if(j != null){
            out.printf("Added to waiting queue: Job %s\n", j.getProcessName());
            waiting_queue.add(j);
        }
    }

    // prints the current state of the running process (job)
    private void log(Job current_job){
        int length = current_job.getLength() < 0?0:current_job.getLength();
        out.println(String.format("Added to ready queue: Job %s\n" +
                        "Remaining length of Job %s: %d\n" +
                        "Time elapsed: %d\n",
                current_job.getProcessName(), current_job.getProcessName(), length, current_time));
    }

    // prints current state of waiting queue when RR first begins
    public void print_starting_data(){
        out.printf("Starting RRScheduler... \nJobs on waiting queue: %d \n", this.waiting_queue.size());
        for (Job j:waiting_queue) {
            out.printf("Job %s [arrival time, %d] [length, %d]\n", j.getProcessName(), j.getArrival(), j.getLength());
        }

        out.printf("\n\n");
    }


    // Your simulation should keep track of which threads are waiting to run
    public static void main(String ...args){
        // ie api call:
        // remove from new queue (the head) and add onto the ready queue
        // ready_queue.add(new_queue.poll());

        RRScheduler rr = new RRScheduler(/*time_slice = */1, 0);

        rr.print_starting_data();

        // retrieve head of wait queue, set head to ready queue
        // when in ready queue, job executes.
        // once job is finished executing its time slice,
        // update the length, and add to waiting queue
        while (true){
            // if theres only one process left, do not take the process of CPU and do not place into waiting queue
            if(rr.ready_queue.isEmpty() && rr.waiting_queue.size() == 1){ while(true){Job j = rr.add_to_ready_queue(rr.add_to_ready_queue(rr.waiting_queue.poll()));} }

            Job head = rr.waiting_queue.poll(); // return the head of waiting queue
            rr.add_to_waiting_queue(rr.add_to_ready_queue(head));

            // if there are no more jobs to execute within the waiting queue,
            // we are done.
            if(rr.waiting_queue.isEmpty()){ break; }
        }
    }
}

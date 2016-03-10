/**
 * Created by phozier on 3/1/16.
 */
class Job {
    int arrival;
    int length;
    int wait_time;
    char process;
    Job (char process, int arrival, int length){
        this.process = process;
        this.arrival = arrival;
        this.length = length;
        this.wait_time = 0;
    }

    // updates the remaining seconds left for the job to run
    protected void setLength(int length) {
        this.length = length;
    }

    protected int getLength() { return length; }

    protected String getProcessName() { return String.format("%c", process); }

    protected int getArrival() { return arrival; }

    protected int getWait_time() { return wait_time; }

    protected void setWait_time(int wait_time) { this.wait_time = wait_time; }
}

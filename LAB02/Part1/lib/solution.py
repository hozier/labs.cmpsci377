import json
import os
from collections import deque

class Solution:
    def __init__(self):
        self.time_slice = 1

    def initialize_variables(self, f):
        f = open(f, 'r')
        data = json.loads(f.read()) # data now contains a dict of the jobs
        self.waiting_queue = deque([self.map_key(x) for x in data['jobs']])
        self.simulation_time = data['simulation_time']
        self.number_of_jobs = data['number_of_jobs']
        self.data = data

    def _exec(self):
        files = [f for f in os.listdir('.') if os.path.isfile(f)]
        for f in files:
            if 'json' in f:
                self.initialize_variables(f)
                self.rr(f)
                # self.fcfs(f)

    def out(self, algorithm, AWT, JSON):
        solution = "{0} {1}: [{2}]".format(algorithm, JSON.split('.')[0], AWT)
        f = open("solution.txt", "ab")
        f.write(solution+"\n")
        print solution
        f.close()

    def map_key(self, x):
        x['wait_time'] = 0
        return x

    # def fcfs(self, JSON):
    #
    #     simulation_time = data['simulation_time']
    #     number_of_jobs = data['number_of_jobs']
    #     timer = 0
    #     count  = 0
    #     awt  = 0
    #     for job in waiting_queue:
    #         pass
            # if count == 0:
            #     job['wait_time'] = timer - 0 # job start time - time taken so far
            # else:
            #     job['wait_time'] = timer - job['start_time'] # job start time - time taken so far
            #
            # timer += job['job_length']
            # print 'timer, ', timer
            # print 'time taken to process job {0} is {1}:'.format(count, job['wait_time'])
            #  wt[i]=btt-at[i];
            # count += 1

    # for(Job job:jobList){
    #             if(count==0){
    #                 job.processArrivalTime = job.getArrivalTime();
    #                 job.ProcessCompletionTime = job.getArrivalTime()+job.getCpuTime();
    #                 }else{
    #                 job.processArrivalTime = temp-job.getArrivalTime();
    #                 job.ProcessCompletionTime = temp+job.getCpuTime();
    #             }
    #
    #             temp = job.ProcessCompletionTime;
    #             job.turnAroundTime = temp-job.getArrivalTime();
    #             job.waitingTime = job.turnAroundTime-job.getCpuTime();
    #             count++;
    #
    #             avgWaitingTime =  avgWaitingTime+job.waitingTime;
    #             avgTurnAroundTime = avgTurnAroundTime+job.turnAroundTime;
    #             System.out.println("   "+job.getProcessId()+"  | "+"   "+job.turnAroundTime+"  | "+"   "+job.waitingTime+" ");
    #             System.out.println("----------------------------------------");
    #         }
    def rr(self, JSON):
        waiting_queue = self.waiting_queue
        current_jobs = deque()
        finished_jobs = deque()
        timer = 0

        while timer <= self.simulation_time:
            if waiting_queue:
                jobx_start_time = waiting_queue[0]['start_time']

                if jobx_start_time == timer:
                    current_jobs.append(waiting_queue.popleft())

                    if current_jobs[0]['job_length'] == 0:
                        finished_jobs.append(current_jobs.popleft())
                        continue

                    current_jobs[0]['job_length'] -= 1


                    current_jobs.append(current_jobs.popleft())
                    for j in range(1,len(current_jobs)):
                        current_jobs[j]['wait_time']+=1
                    continue

            if len(current_jobs) > 0:
                job_length = current_jobs[0]['job_length']
                # print "job_length: ", job_length
                if job_length == 0:
                    finished_jobs.append(current_jobs.popleft())

                else:
                    '''time left == the job_length remaining'''
                    job_length -= self.time_slice
                    current_jobs[0]['job_length'] = job_length

                    for j in range(1,len(current_jobs)):
                        current_jobs[j]['wait_time']+=1

                    current_jobs.append(current_jobs.popleft())
            timer += 1

        ''' end while loop '''

        total_time = 0
        for job in finished_jobs:
            total_time += job['wait_time']
        # print 'total_time, ', total_time
        # print 'number_of_jobs', number_of_jobs

        AWT = total_time/float(self.number_of_jobs)
        self.out('RR', AWT, JSON)

Solution()._exec()

'''the test json'''
# rr('t.json')

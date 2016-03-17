import os, math, json
from collections import deque

class Solution:

    ''' overview: initialize global class variable '''
    def __init__(self):
        self.time_slice = 1 # time quantum variable


    '''
    overview: continue the initialization of global class variables
    @params: f: an open file object
    returns void
    '''
    def initialize_variables(self, f):
        f = open(f, 'r')
        data = json.loads(f.read()) # data now contains a dictionary (json) of the jobs

        # maps the key 'wait_time' into the waiting_queue structure
        # used as the starting point queue where all jobs initially reside
        self.waiting_queue = deque([self.map_key(x) for x in data['jobs']])
        self.map_init("simulation_time", "number_of_jobs", data)


    '''
    overview: continue the initialization of global class variables
    @params: s: the simulation_time field of the data json
    @params: n: the number_of_jobs field of the data json
    @params: d: the json read in from the .json file. the jobs
    listed in the tracefiles are stored here.
    returns void
    '''
    def map_init(self, s, n, d):
        self.s = d[s]
        self.n = d[n]
        self.d = d


    '''
    overview: apply an algorithm -- either rr or fcfs to the json: f
    @params: files: list of files in the cwd
    @params: algorithm: a callback function
    returns a json file to callback if condition is true
    '''
    def select_algorithm(self, files, algorithm):
        for f in files:
            if 'json' in f:
                self.initialize_variables(f)
                algorithm(f)


    '''
    overview: executes the solution script
    returns void
    '''
    def _exec(self):
        # returns a list of all files in the cwd
        files = [f for f in os.listdir('.') if os.path.isfile(f)]

        self.select_algorithm(files, self.rr)
        self.select_algorithm(files, self.fcfs)


    '''
    overview: writes the AWT solution for the current json to file
    @params: algorithm: denotes either RR of FCFS
    @params: AWT: denotes the average wait time number
    @params: JSON: denotes the name of the trace
    returns void
    '''
    def out(self, algorithm, AWT, JSON):
        solution = "{0} {1}: [{2}]".format(algorithm, JSON.split('.')[0], AWT)
        f = open("solution.txt", "ab")
        f.write(solution+"\n")
        print solution
        f.close()


    '''
    overview: inserts the key 'wait_time' and value 0 into the read in json
    (this helper function maps this key/value pair to all jobs in the data json)
    @params: x: an individual job json
    returns void
    '''
    def map_key(self, x):
        x['wait_time'] = 0
        return x


    '''
    overview: the first come first serve algorithm implementation
    @param: trace_name: the name of the current trace file
    returns AWT solution to the file writing method
    '''
    def fcfs(self, trace_name):
        waiting_queue = self.waiting_queue
        finished_jobs = deque() # a queue which contains all the finished jobs
        processing_time = 0 # the running sum of the duration of time it takes for all jobs to complete

        while waiting_queue:
            current_job = waiting_queue.popleft()
            # print 'wait_time = processing_time[{0}] - start_time[{1}]'.format(processing_time, current_job['start_time'])
            # print "Processing time: {0}. Current job start time: {1}. Job length: {2}".format(processing_time, current_job['start_time'], current_job['job_length'])
            if processing_time > current_job['start_time']:
                current_job['wait_time'] = processing_time - current_job['start_time']
                processing_time += current_job['job_length']
            else:
                current_job['wait_time'] = 0
                processing_time += (current_job['job_length'] + (current_job['start_time'] - processing_time)) # last thing to do.

            finished_jobs.append(current_job)

        total_time = 0
        for job in finished_jobs:
            total_time += job['wait_time']

        AWT = int(math.ceil(total_time/float(self.n)))
        self.out('FCFS', AWT, trace_name)


    '''
    overview: the round robin algorithm implementation
    @param: trace_name: the name of the current trace file
    returns AWT solution to the file writing method
    '''
    def rr(self, trace_name):
        waiting_queue = self.waiting_queue
        current_jobs = deque()
        finished_jobs = deque()
        timer = 0

        while True:

            # if the wait queue is not empty then check to whether the next job's  start_time
            # in the wait queue is equal to the current time step

            # if so, remove it from the wait queue and add it to the current job queue
            # if there is time remaining, subtract time_slice and add current job to the
            # back of current job queue.
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

                # if the job is finished, remove it from current_jobs
                # and place it into the finished_jobs queue
                if job_length == 0:
                    finished_jobs.append(current_jobs.popleft())

                # there is still time remaining on for the current_job
                # subtract time quantum and set the new number into the
                # job_length field of the current_job
                else:
                    '''time left == the job_length remaining'''
                    job_length -= self.time_slice
                    current_jobs[0]['job_length'] = job_length

                    for j in range(1,len(current_jobs)):
                        current_jobs[j]['wait_time']+=1

                    current_jobs.append(current_jobs.popleft())

            # if both current_jobs and waiting_queue queues are empty,
            # all jobs have been completed.
            if len(current_jobs) == 0 and len(waiting_queue) == 0:
                # print 'timer: ', timer
                break;
            timer += 1

        ''' end while loop '''

        total_time = 0
        for job in finished_jobs:
            total_time += job['wait_time']
        # print 'total_time, ', total_time
        # print 'number_of_jobs', number_of_jobs

        AWT = int(math.ceil(total_time/float(self.n))) # round up and convert to an integer
        self.out('RR', AWT, trace_name)


''' function call executes the solution script '''
Solution()._exec()

import os, math, json
from collections import deque

class Solution:
    def __init__(self):
        self.time_slice = 1

    def initialize_variables(self, f):
        f = open(f, 'r')
        data = json.loads(f.read()) # data now contains a dict of the jobs
        self.waiting_queue = deque([self.map_key(x) for x in data['jobs']])
        self.map_init("simulation_time", "number_of_jobs", data)

    def map_init(self, s, n, d):
        self.s = d[s]
        self.n = d[n]
        self.d = d

    def select_algorithm(self, files, algorithm):
        for f in files:
            if 'json' in f:
                self.initialize_variables(f)
                algorithm(f)

    def _exec(self):
        files = [f for f in os.listdir('.') if os.path.isfile(f)]
        self.select_algorithm(files, self.rr)
        self.select_algorithm(files, self.fcfs)

    def out(self, algorithm, AWT, JSON):
        solution = "{0} {1}: [{2}]".format(algorithm, JSON.split('.')[0], AWT)
        f = open("solution.txt", "ab")
        f.write(solution+"\n")
        print solution
        f.close()

    def map_key(self, x):
        x['wait_time'] = 0
        return x

    def fcfs(self, JSON):
        waiting_queue = self.waiting_queue
        finished_jobs = deque()
        processing_time = 0
        timer = 0

        while waiting_queue:
            current_job = waiting_queue.popleft()
            # print 'wait_time = processing_time[{0}] - start_time[{1}]'.format(processing_time, current_job['start_time'])
            current_job['wait_time'] = processing_time - current_job['start_time']
            processing_time += current_job['job_length'] # last thing to do.
            finished_jobs.append(current_job)

        total_time = 0
        for job in finished_jobs:
            total_time += job['wait_time']

        AWT = int(math.ceil(total_time/float(self.n)))
        self.out('FCFS', AWT, JSON)


    def rr(self, JSON):
        waiting_queue = self.waiting_queue
        current_jobs = deque()
        finished_jobs = deque()
        timer = 0

        # while timer <= self.simulation_time:
        while True:
            # print 'current_jobs len: ', len(current_jobs)
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

        AWT = int(math.ceil(total_time/float(self.n)))
        self.out('RR', AWT, JSON)

Solution()._exec()

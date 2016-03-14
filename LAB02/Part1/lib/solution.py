import json
import os
from collections import deque

def driver():
    files = [f for f in os.listdir('.') if os.path.isfile(f)]
    for f in files:
        if 'json' in f:
            rr(f)

def out(AWT, JSON):
    solution = "RR {0}: [{1}]".format(JSON.split('.')[0], AWT)
    f = open("solution.txt", "ab")
    f.write(solution+"\n")
    print solution
    f.close()

def map_key(x):
    x['wait_time'] = 0
    return x

def rr(JSON):
    f = open(JSON, 'r')
    data = json.loads(f.read()) # job now contains a dict of the jobs

    waiting_queue = deque([map_key(x) for x in data['jobs']])
    simulation_time = data['simulation_time']
    number_of_jobs = data['number_of_jobs']
    current_jobs = deque()
    finished_jobs = deque()

    time_slice = 1
    timer = 0


    while timer <= simulation_time:
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
                job_length -= time_slice
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

    AWT = total_time/float(number_of_jobs)
    out(AWT, JSON)

driver()

'''the test json'''
# rr('t.json')

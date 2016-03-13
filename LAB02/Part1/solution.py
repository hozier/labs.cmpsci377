import json
from collections import deque

f = open('t.json', 'r') # <------ replace this json file with one of the randomly generated ones......
data = json.loads(f.read()) # job now contains a dict of the jobs
waiting_queue = deque(data['jobs'])
system_time = int(data['simulation_time'])+1
number_of_jobs = int(data['number_of_jobs'])
time_slice = 1

i = 0
head = waiting_queue.popleft()
next_job_starts = int(waiting_queue[0]['start_time'])
while i < system_time:
    try:

        if i == next_job_starts:
            print "i: ", i, "job start_time: ", next_job_starts
            waiting_queue.append(head)
            head = waiting_queue.popleft()
            old_start_time = next_job_starts
            next_job_starts = int(waiting_queue[0]['start_time'])

            # while there are multiple jobs to run at the same start_time, do the following
            while old_start_time == next_job_starts:
                if int(head['job_length']) == 0:
                    head = waiting_queue.popleft()
                else:
                    head['job_length']= int(head['job_length']) - time_slice

                    # take head off CPU
                    waiting_queue.append(head)

                    #get new head
                    head = waiting_queue.popleft()
                    next_job_starts = int(waiting_queue[0]['start_time'])
                    print "i: ", i, "job start_time: ", next_job_starts

        else:
            head['job_length']= int(head['job_length']) - time_slice
            if int(head['job_length']) == 0:
                head = waiting_queue.popleft()

        # waiting_queue.append(head)
            print head['job_length']
    except:
        pass
    i += 1

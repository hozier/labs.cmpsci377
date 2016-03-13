import json
from collections import deque

f = open('100-100-8.json', 'r') # <------ replace this json file with one of the randomly generated ones......
data = json.loads(f.read()) # job now contains a dict of the jobs
waiting_queue = data['jobs']
system_time = int(data['simulation_time'])+1
number_of_jobs = int(data['number_of_jobs'])
time_slice = 1

i = 0
# waiting_queue[count] = waiting_queue.popleft()

flag = 0
time = 0
count = 0
wait_time = 0
turnaround_time = 0
remaining_processes = len(waiting_queue)
for count in range(int(data['simulation_time'])):

    if remaining_processes <= 0:
        break


    job_length = int(waiting_queue[count]['job_length'])
    waiting_queue[count]['remaining_time'] = job_length
    # waiting_queue[count]['job_length']
    remaining_time = waiting_queue[count]['remaining_time']
    if remaining_time <= time_slice and remaining_time > 0: #if process is left and the process time is less than the time slice
        print "Current time: {0}\n Time Left in process: {1}\n\n".format(time , remaining_time) # rt[count] == waiting_queue[count]['job_length']
        time += remaining_time;
        remaining_time = 0;
        waiting_queue[count]['remaining_time'] = remaining_time
        flag = 1
        start_time = int(waiting_queue[count]['start_time'])
        print "time: {0}, start_time: {1}, remaining_time: {2},".format(time, start_time, remaining_time)


    elif remaining_time > 0:
        print "Current time: {0}\n Time Left in process: {1}\n\n".format(time , remaining_time) # rt[count] == waiting_queue[count]['job_length']
        remaining_time -= time_slice
        waiting_queue[count]['remaining_time'] = remaining_time
        time += time_slice
        start_time = int(waiting_queue[count]['start_time'])
        # print "time: {0}, start_time: {1}, remaining_time: {2},".format(time, start_time, remaining_time)


    if remaining_time == 0 and flag == 1:
        start_time = int(waiting_queue[count]['start_time'])
        remaining_processes-=1
        waiting_queue[count]['remaining_time'] = remaining_time
        # print "time: {0}, start_time: {1}, remaining_time: {2},".format(time, start_time, remaining_time)
        wait_time += (time - start_time - job_length);
        turnaround_time += time - job_length;
        print "Process is over, total waiting time is {0}\n\n".format(wait_time);
        flag = 0

    if count == number_of_jobs - 1:
        count = 0;

    elif int(waiting_queue[count + 1]['start_time']) <= time: # if the next job waiting is...
        count+=1;

        # put new process onto CPU

    else:
        # put new process onto CPU
        count = 0;

print  "\nAverage Waiting Time= {0}".format(wait_time*1.0/number_of_jobs);
print "\nAvg Turnaround Time = {0}".format(turnaround_time*1.0/number_of_jobs);

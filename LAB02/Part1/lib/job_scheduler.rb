require_relative 'job.rb'

class JobScheduler

  def initialize
  end

  # perform operations on currently queued jobs
  def timeStepFCFS(jobQueue)  #simulation of time step for FCFS
    if(jobQueue.empty?)   #if no jobs, do nothing
      return
    else
      # increment the job in the front of the line. Add to wait time of others
      jobQueue[0].increment     #grab job in first index
      (1..((jobQueue.length)-1)).each do |i|    #for each job in the queue
        jobQueue[i].wait    #use wait to increase wait time by 1
      end
    end
  end

  def timeStepRR(jobQueue) #simulation of time step for round robin
    if(jobQueue.empty?) #if the queue is empty, do nothing
      return
    else
      jobQueue[0].increment  #increment first job
      (1..((jobQueue.length)-1)).each do |i| #increase wait time of all other jobs
        jobQueue[i].wait
      end
      # Here's the money, right here
      jobQueue.push(jobQueue.shift)
    end
  end

  # FCFS algorithm
  def fcfs(traceFile, outputFile)
    puts("Analyzing: #{traceFile} in FCFS")

    # read lines of trace file as an array
    traceArr = IO.readlines(traceFile)

    # assign important variables: no. of jobs, simulation time, maximum length
    totalJobs = traceArr[0].to_i    #first line is total jobs
    simTime = traceArr[1].to_i      #second line is simulation time
    maxJobLength = traceArr[2].to_i   #third line is the maximum length of jobs

    # set variables for simulation
    timer = 0
    allJobs = Array.new
    currJobs = Array.new
    finishedJobs = Array.new
    totalWaitTime = 0

    # for each line in the trace file that describes a job, create job object
    (3..((traceArr.length)-1)).each do |i|
      str = traceArr[i].split(" ")
      allJobs.push(Job.new(str[0], str[1])) #passing the start time and job length of each job
    end

    while(timer <= simTime) do   #for as long as the simulation continues
      # take the next job from the allJobs queue if necessary
      unless allJobs.empty?      #if theres jobs left
        nextTime = allJobs[0].getStartTime   #get first job's start time
        if(nextTime == timer)                # if the start time is equivilent to timer
          currJobs.push(allJobs.shift)       #remove and retrieve first item then push it to current jobs
        end
      end

      unless currJobs.empty?    #if theres jobs left in current job
        if(currJobs[0].getTimeLeft == 0)    #if the first job is finished
          finishedJobs.push(currJobs.shift)   #remove finished job from job queue and push it to finished queue
        end
      end

      # do operations on the current job queue
      timeStepFCFS(currJobs)    #increment timeStep for the job queue
      timer+=1                  #increase the timer
    end

    finishedJobs.each do |job|  #get the wait times of each job once the simulation is over
      totalWaitTime += job.getWaitTime
    end

    puts("Finished jobs from #{traceFile}.")
    puts("Total wait time: #{totalWaitTime}")
    puts("Total jobs: #{totalJobs}")
    averageWait = totalWaitTime.to_f/totalJobs.to_f
    puts("Average wait time: #{averageWait}")

    formattedFile = traceFile.split("/").pop.split(".").shift
    outputFile.puts("FCFS #{formattedFile}: #{averageWait}")
  end # end fcfs

  def rr(traceFile, outputFile)
    puts("Analyzing #{traceFile} in RR")

      # read lines of trace file as an array
      traceArr = IO.readlines(traceFile)

      # assign important variables: no. of jobs, simulation time, maximum length
      totalJobs = traceArr[0].to_i
      simTime = traceArr[1].to_i
      maxJobLength = traceArr[2].to_i

      # set variables for simulation
      timer = 0
      allJobs = Array.new
      currJobs = Array.new
      finishedJobs = Array.new
      totalWaitTime = 0

      # for each line in the trace file that describes a job, create job object
      (3..((traceArr.length)-1)).each do |i|
        str = traceArr[i].split(" ")
        allJobs.push(Job.new(str[0], str[1]))
      end

      #while theres time left in simulation
      while(timer <= simTime) do
        unless allJobs.empty?   #if theres jobs left
          nextTime = allJobs[0].getStartTime  #get start time of first job
          if(nextTime == timer)     #if the job has just started
            currJobs.push(allJobs.shift)  #add to current jobs
          end
        end # end unless

        unless currJobs.empty?    #if theres jobs left in current job queue
          if(currJobs[0].getTimeLeft == 1)  #if a jobs about to finish
            finishedJobs.push(currJobs.shift) #push to finished job queue and remove from current job queue
            currJobs.each do |job|   #then wait for every other job
              job.wait
            end
          else
            timeStepRR(currJobs)  #perform the time step on the job queue
          end
        end # end unless

        # timeStepRR(currJobs)
        timer+=1
      end # end while

      finishedJobs.each do |job|
        totalWaitTime += job.getWaitTime
      end #get wait time of finished jobs

      puts("Finished jobs from #{traceFile}.")
      puts("Total wait time: #{totalWaitTime}")
      puts("Total jobs: #{totalJobs}")
      averageWait = totalWaitTime.to_f/totalJobs.to_f
      puts("Average wait time: #{averageWait}")

      formattedFile = traceFile.split("/").pop.split(".").shift
      outputFile.puts("RR #{formattedFile}: #{averageWait}")
  end # end rr

  def main
    File.open("solution.txt", "w") do |outputFile|
      files = Dir.glob("./resources/*.txt")
      files.each do |traceFile| #read in each trace file
        fcfs(traceFile, outputFile) #perform on them using fcfs
      end
      files.each do |traceFile|   #read in each trace file
        rr(traceFile, outputFile) #perform on them using rr
      end
    end
  end # end main

end # end class

p1 = JobScheduler.new
p1.main

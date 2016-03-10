#include<stdio.h>

int main()
{
  /*variables*/
  int count;
  int j;
  int num; //number of processes entering the Round Robin
  int time;
  int remaining;
  int flag=0;
  int time_slice; //time slice for each process
  int wait_time=0; //waiting time  variable
  int turnaround_time=0; //finish time variable
  int arrival_time[10]; //list of process arrival times for each process
  int burst_time[10]; //list of burst times for each process
  char job_name[10]; //list of process names
  int rt[10];


  printf("Enter Number of Processese:\t "); //ask for number of processes
  scanf("%d",&num); //get number of processes
  remaining=num; //remaining amount of processes

  for(count=0;count<num;count++) //for each process
  {
    printf("Enter Name of Process:"); //ask user to input burst time and wait time
    scanf("%s",&job_name[count]); //enter the process burst time in proper array
  }

  for(count=0;count<num;count++) //for each process
  {
    printf("Enter Arrival Time and Burst Time for Process Process Number %d :",count+1); //ask user to input burst time and wait time
    scanf("%d",&arrival_time[count]); //enter the process arival time in proper array
    scanf("%d",&burst_time[count]); //enter the process burst time in proper array
    rt[count]=burst_time[count]; //copy burst time into rf[]
  }
  printf("Enter Time Slice\t"); //ask user for a time slice
  scanf("%d",&time_slice); //assign the time slice variable as the users input
  printf("\n\nProcess\t|Turnaround Time|Waiting Time\n\n");
  for(time=0,count=0;remaining !=0;)
  {
    if(rt[count]<=time_slice && rt[count]>0) //if process is left and the process time is less than the time slice
    {
      printf("Current time: %d\n Name of process: %c\n Time Left in process: %d\n\n",time,job_name[count],rt[count]);
      time+=rt[count]; //time equals time plus the process time that remains
      rt[count]=0; //set the process time to 0 since its done
      flag=1; //process is done
    }
    else if(rt[count]>0) //if process if left but process time isnt less than the time left
    {
      printf("Current time: %d\n Name of process: %c\n Time Left in process: %d\n\n",time,job_name[count],rt[count]);
      rt[count]-=time_slice; //subtract time slice from the process time
      time+=time_slice; //add time_slice to total time
    }
    if(rt[count]==0 && flag==1) //if the process is over and not handled yet
    {
      remaining--; //subtract from the remaining processes variable
      wait_time+=time-arrival_time[count]-burst_time[count];
      turnaround_time+=time-arrival_time[count];
      printf("Process %c is over, total waiting time is %d\n\n",job_name[count],wait_time);
      flag=0;
    }
    if(count==num-1)
      count=0;
    else if(arrival_time[count+1]<=time)
      count++;
    else
      count=0;
  }
  printf("\nAverage Waiting Time= %f\n",wait_time*1.0/num);
  printf("\nAvg Turnaround Time = %f",turnaround_time*1.0/num);

  return 0;
}

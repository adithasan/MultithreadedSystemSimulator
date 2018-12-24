# MultithreadedSystemSimulator
This is a Multithreaded System Simulator that I created for my Operating Systems course

It takes instructions from a file that specifies all the tasks to be done, the resources they need and how long to execute for and also specifies in one line the resources available for the program.

The program then launches a thread for each task and "executes" the task. There is also a monitor thread that monitors all the tasks and their progress.

Example Input File:  
```
# An instance of the Dining Philosophers Problem with 5 people 
# resources name1:value1 name2:value2 ...
# task taskName busyTime idleTime name1:value1 name2:value2 ...

resources  A:1 B:1 C:1 D:1 E:1  
task       t1 50 100  A:1 B:1  
task       t2 50 100  B:1 C:1  
task       t3 50 100  C:1 D:1  
task       t4 50 100  D:1 E:1  
task       t5 50 100  E:1 A:1  
```
## Building the program  
  1. Download source code into desired directory  
  2. cd to the directory in terminal (shell)
  3. run make
  
## Running the program
Run the command:
```
a4tasks inputFile monitorTime NITER
```
where   
• inputFile: is the input file describing the system, as detailed above,  
• monitorTime: an integer (in millisec) that specifies how often a monitor thread (described below) runs, and  
• NITER: the simulator finishes when each task in the system executes NITER times (a one time execution of a task is considered as one iteration of the task).

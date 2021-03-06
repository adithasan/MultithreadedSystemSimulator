#include "UtilityClasses.cpp"

using namespace std;

void *RunTask(void* ptr);
void *RunMonitor(void* ptr);

int main(int argc, char** argv) 
{
    ProgramStateInfo stateInfo;
    ResourceDepot resourceDepot;
    TaskDepot taskDepot;
    clock_t startTime = clock();
    if (argc != 4)
    {
        cout<<"Invalid number of arguments. Exiting program\n";
        exit(1);
    }
    else { stateInfo = ParseUtility::ParseProgramStateInfo(argv); }

    PrintUtility::PrintProgramStateInfo(&stateInfo);

    ParseUtility::ParseInputFile(stateInfo.mFileName, taskDepot, resourceDepot, stateInfo.mNumRequiredIterations);
    cout<< "Number of tasks: " << taskDepot.GetNumTasks() << endl;
    cout<< "Number of resource types: " << resourceDepot.GetNumOfResourceTypes() << endl;

    DataDepo* dataDepo = new DataDepo(&stateInfo, &taskDepot, &resourceDepot, &startTime);
    pthread_t TID[taskDepot.GetNumTasks()];
    pthread_t monitorThread;
    // create threads for each task
    for (int i = 0; i < taskDepot.GetNumTasks(); i++)
    {
        pthread_create(&TID[i], NULL, RunTask, (void*) dataDepo);
    }

    pthread_create(&monitorThread, NULL, RunMonitor, (void*) dataDepo);

    // wait for each thread to finish
    for (int i = 0; i < taskDepot.GetNumTasks(); i++)
    {
        pthread_join(TID[i], NULL);
    }
    dataDepo->mIsProgramFinished = true;
    pthread_join(monitorThread, NULL);
    PrintUtility::PrintSystemResources(*dataDepo->mResourceDepot);
    PrintUtility::PrintTaskStatus(*dataDepo->mTaskDepot);

    double sysRunTime = ((double)(clock() - startTime)/CLOCKS_PER_SEC)*1000;
    cout<<"System runtime: " << sysRunTime << " msec" << endl;
}

void *RunTask(void* ptr)
{
    // do this for num iterations
    int counter = 0;
    pthread_t thId = pthread_self();
    // select a task
    DataDepo* depo = (struct DataDepo*)ptr;
    AsynchronousTask& task = depo->mTaskDepot->AcquireTask();
    while (counter < task.GetNumIterations())
    {
        // acquire resources
        while(true)
        {
            while(depo->mMonitorPrinting) {} // spin while monitor is printing
            task.SetState(Waiting);
            if (depo->mResourceDepot->AcquireResources(task.GetRequiredResources()))
            {
                while(depo->mMonitorPrinting) {} // spin while monitor is printing
                task.SetState(Running);
                break;
            } 
            task.Wait(10);
            
        }
        // wait for busy time
        task.Wait(task.GetBusyTime());
        // relinquish resources
        depo->mResourceDepot->ReleaseResources(task.GetRequiredResources());
        // wait for idle time
        while(depo->mMonitorPrinting) {} // spin while monitor is printing
        task.SetState(Idle);
        task.Wait(task.GetIdleTime());
        
        // task: t1 (tid= 140214559033088, iter= 3, time= 600 msec)
        double timeTaken = ((double)(clock() - *depo->mStartTime)/CLOCKS_PER_SEC)*1000;
        cout<< "Task: " << task.GetName();
        cout<< " (threadId: " << thId<< ",";
        cout<<" iter: " << counter<< ",";
        cout<<" time: " << timeTaken << " msec)" << endl;
        counter++;
    }
    pthread_exit(NULL);
}

void MonitorWait(int intervalInMillisec)
{
    struct timespec interval;
    interval.tv_sec =  (long) intervalInMillisec / 1000;              // seconds 
    interval.tv_nsec = (long) ((intervalInMillisec % 1000) * 1E6);    // nanoseconds
    if (nanosleep(&interval, NULL) < 0)
    {
        cout<<"Delay error"<<endl;
        exit(1);
    }
}

void *RunMonitor(void* ptr)
{
    DataDepo* depo = (struct DataDepo*)ptr;
    while (!depo->mIsProgramFinished)
    {
        depo->mMonitorPrinting = true;
        PrintUtility::PrintMonitorStateInfo(*depo->mTaskDepot);
        depo->mMonitorPrinting = false;
        MonitorWait(depo->mProgramStateInfo->mMonitorTime);
    }
    pthread_exit(NULL);
}
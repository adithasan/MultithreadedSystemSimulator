#include "UtilityClasses.cpp"

using namespace std;

void *RunTask(void* ptr);

int main(int argc, char** argv) 
{
    ProgramStateInfo stateInfo;
    ResourceDepot resourceDepot;
    TaskDepot taskDepot;
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

    DataDepo* dataDepo = new DataDepo(&taskDepot, &resourceDepot);
    pthread_t TID[taskDepot.GetNumTasks()];
    // create threads for each task
    for (int i = 0; i < taskDepot.GetNumTasks(); i++)
    {
        pthread_create(&TID[i], NULL, RunTask, (void*) dataDepo);
    }

    // wait for each thread to finish
    for (int i = 0; i < taskDepot.GetNumTasks(); i++)
    {
        pthread_join(TID[i], NULL);
    }

    PrintUtility::PrintSystemResources(resourceDepot);
    PrintUtility::PrintTaskStatus(taskDepot);
}

void *RunTask(void* ptr)
{
    // do this for num iterations
    int counter = 0;
    pthread_t thId = pthread_self();
    // select a task
    DataDepo* depo = (struct DataDepo*)ptr;
    AsynchronousTask task = depo->mTaskDepot->AcquireTask();
    while (counter < task.GetNumIterations())
    {
        // acquire resources
        while(true)
        {
            task.SetState(Waiting);
            if (depo->mResourceDepot->AcquireResources(task.GetRequiredResources()))
            {
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
        task.SetState(Idle);
        task.Wait(task.GetIdleTime());
        counter++;
    }
    cout<<"Thread id: " << thId << " has completed " <<task.GetNumIterations() << " iterations"<<endl;
    pthread_exit(NULL);
}
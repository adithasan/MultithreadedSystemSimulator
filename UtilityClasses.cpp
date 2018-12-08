#include "UtilityClasses.h"

using namespace std;

void ParseUtility::Tokenize(const string& inputLine, vector<string>& tokens, char delimiter)
{
    size_t start, end;
    string intermediate;
    stringstream check1(inputLine); 
    while(getline(check1, intermediate, delimiter)) 
    { 
        tokens.push_back(intermediate); 
    }
}

void ParseUtility::ParseResource(string inputLine, ResourceDepot& resourceDepot)
{
    // resources A:1 B:1 C:1 D:1 E:1
    vector<string> tokens;
    char delim = ' ';
    char colonDelim = ':';
    ParseUtility::Tokenize(inputLine, tokens, delim);
    for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) 
    {
        if (it->find(colonDelim) != string::npos)
        {
            vector<string> resourceParams;
            const string resourceToken = *it;
            ParseUtility::Tokenize(resourceToken, resourceParams, colonDelim);
            if (resourceParams.size() == 2)
            {
                Resource resouce(resourceParams.at(0), stoi(resourceParams.at(1)));
                resourceDepot.mResources.push_back(resouce);
            }
        }
    }
}

void ParseUtility::ParseAsynchronousTask(string inputLine, TaskDepot& taskDepot, int numIterations)
{
    // task t1 50 100 A:1 B:1
    vector<string> tokens;
    char delim = ' ';
    char colonDelim = ':';
    ParseUtility::Tokenize(inputLine, tokens, delim);
    if (tokens.size() >= 4)
    {
        string name = tokens.at(1);
        int busyTime = stoi(tokens.at(2));
        int idleTime = stoi(tokens.at(3));
        AsynchronousTask task(name, busyTime, idleTime, numIterations);
        for (int i = 0; i < tokens.size(); i++)
        {
            if (tokens.at(i).find(colonDelim) != string::npos)
            {
                vector<string> resourceParams;
                ParseUtility::Tokenize(tokens.at(i), resourceParams, colonDelim);
                if (resourceParams.size() == 2)
                {
                    Resource resource(resourceParams.at(0), stoi(resourceParams.at(1)));
                    task.AddRequiredResource(resource);
                }
            }
        }
        taskDepot.mTasks.push_back(task);
    }
    
}

void ParseUtility::ParseInputFile(string fileName, TaskDepot& taskDepot, ResourceDepot& resourceDepot, int numIterations)
{
    string currentLine;
    string delim = " ";
    string token;
    ifstream inputFile(fileName);
    if (inputFile.is_open())
    {
        while(getline(inputFile, currentLine))
        {
            token = currentLine.substr(0, currentLine.find(delim));
            if (token.compare("resources") == 0) { ParseUtility::ParseResource(currentLine, resourceDepot); }
            else if (token.compare("task") == 0) { ParseUtility::ParseAsynchronousTask(currentLine, taskDepot, numIterations); }
        }
    }
    else 
    { 
        cout<< "Error opening input file" << endl;
        exit(1);
    }
    inputFile.close();
}

ProgramStateInfo ParseUtility::ParseProgramStateInfo(char** arguments)
{
    //a4tasks inputFile monitorTime NITER
    ProgramStateInfo stateInfo;
    stateInfo.mFileName = arguments[1];
    stateInfo.mMonitorTime = atoi(arguments[2]);
    if (stateInfo.mMonitorTime <= 0) 
    {
        cout<<"Invalid monitor time entered\n";
        exit(1);
    }
    stateInfo.mNumRequiredIterations = atoi (arguments[3]);
    if (stateInfo.mNumRequiredIterations <= 0) 
    {
        cout<<"Invalid number of iterations entered\n";
        exit(1);
    }

    return stateInfo;
}

void PrintUtility::PrintProgramStateInfo(ProgramStateInfo* stateInfo) 
{
    cout<< "Started program with following parameters: "<<endl;
    cout<< "Input File Name: " << stateInfo->mFileName <<endl;
    cout<< "Monitor time (millisec): " << stateInfo->mMonitorTime <<endl;
    cout<< "Required Iterations: " << stateInfo->mNumRequiredIterations <<endl;
}

void PrintUtility::PrintResourceVector(vector<Resource>& resources)
{
    for (Resource resource : resources)
    {
        cout<< resource.GetName() << ":" <<resource.GetValue()<<" ";
    }
}

void PrintUtility::PrintSystemResources(ResourceDepot& resourceDepot)
{
    cout<<"System Resources: " <<endl;
    for (Resource resource : resourceDepot.mResources)
    {
        cout << "\t" << resource.GetName() << ": (maxAvail= " << resource.GetValue() << "), held = 0"<<endl;
    }
}

void PrintUtility::PrintTaskStatus(TaskDepot& taskDepot)
{
    /*
    System Tasks:
    [0] t1 (IDLE, runTime= 50 msec, idleTime= 100 msec):
    (tid= 140233984644864)
    A: (needed= 1, held= 0)
    B: (needed= 1, held= 0)
    (RUN: 20 times, WAIT: 10 msec)
    */
    int counter = 0;
    cout<<endl;
    cout<<endl;
    cout<< "System Tasks: " <<endl;
    for (AsynchronousTask task: taskDepot.mTasks)
    {
        cout<< "[" << counter << "] " << task.GetName();
        string state;
        switch(task.GetState())
        {
            case Idle:
                state = "IDLE";
                break;
            case Running:
                state = "RUNNING";
                break;
            case Waiting:
                state = "WAITING";
                break;
            default:
                state = "UNKNOWN";
                break;
        }
        cout<< "(" << state << ", runtime=" << task.GetBusyTime() << " msec, " <<"idleTime= " <<task.GetIdleTime() <<" msec):"<<endl;
        cout<< "\t(Thread ID: " << task.GetThreadId() << ")" << endl;
        cout<< "\t(RUN: " << task.GetNumIterations() << " times, WAIT: " << task.GetTimeWaited() << " msec)"<<endl;
        counter++;
        cout<<endl;
    }
    cout<<endl;
}

Resource::Resource(string name, int value)
{
    mName = name;
    mValue = value;
}

string Resource::GetName() const { return mName; }
int Resource::GetValue() const { return mValue; }
void Resource::AllocateResourceValue(int value) { mValue -= value; }
void Resource::ReAcquireResourceValue(int value) { mValue += value; }

AsynchronousTask::AsynchronousTask(string name, int busyTime, int idleTime, int numIterations)
{
    mName = name;
    mBusyTime = busyTime;
    mIdleTime = idleTime;
    mNumIterations = numIterations;
    mIsClaimed = false;
    mTaskState = Idle;
    mTimeWaited = 0;
}

string AsynchronousTask::GetName() const { return mName; }
int AsynchronousTask::GetTimeWaited() const { return mTimeWaited; }
int AsynchronousTask::GetNumIterations() const { return mNumIterations; }
int AsynchronousTask::GetBusyTime() const { return mBusyTime; }
int AsynchronousTask::GetIdleTime() const { return mIdleTime; }
pthread_t AsynchronousTask::GetThreadId() const { return mThreadId; }
void AsynchronousTask::AddRequiredResource(Resource resource) { mRequiredResources.push_back(resource); }
int AsynchronousTask::GetNumOfRequiredResourceTypes() const { return mRequiredResources.size(); }
bool AsynchronousTask::IsClaimed() const { return mIsClaimed; }
void AsynchronousTask::SetClaimed(pthread_t threadId) 
{
    if (mIsClaimed == false)
    {
        mIsClaimed = true;
        mThreadId = threadId;
    } 
}
void AsynchronousTask::SetState(TaskState state){ mTaskState = state; }
TaskState AsynchronousTask::GetState() const { return mTaskState; }

vector<Resource>& AsynchronousTask::GetRequiredResources() 
{
    return mRequiredResources;
}



TaskDepot::TaskDepot() { pthread_mutex_init(&mMutex, NULL); }
ResourceDepot::ResourceDepot() { pthread_mutex_init(&mMutex, NULL); }

AsynchronousTask& TaskDepot::AcquireTask()
{
    int foundIndex = -1;
   
    if (pthread_mutex_lock(&mMutex))
    {
        cout<< "Error locking mutex in TaskDepot::AcquireTask()";
        exit(1);
    }
    for (int i = 0; i < mTasks.size(); i++)
    {
        if (!mTasks.at(i).IsClaimed()) 
        {
            mTasks.at(i).SetClaimed(pthread_self());
            foundIndex = i;
            break;
        }
    }
    if (pthread_mutex_unlock(&mMutex))
    {
        cout<< "Error unlocking mutex in TaskDepot::AcquireTask()";
        exit(1);
    }
    if (foundIndex == -1) 
    {
        cout<< "Error finding available task";
        exit(1);
    }
    return mTasks.at(foundIndex);
}

int TaskDepot::GetNumTasks() const
{
    return mTasks.size();
}

bool ResourceDepot::AcquireResources(vector<Resource>& requestedResources)
{
    int numResourceFound = 0;
    bool allocatedResource = false;
    if (pthread_mutex_lock(&mMutex))
    {
        cout<< "Error locking mutex in ResourceDepot::AcquireResources()";
        exit(1);
    }
    
    // loop through all required resources and check if they exist
    for (int i = 0; i < requestedResources.size(); i++)
    {
        for (int j = 0; j < mResources.size(); j++)
        {
            if (mResources.at(j).GetName() == requestedResources.at(i).GetName() && mResources.at(j).GetValue() >= requestedResources.at(i).GetValue())
            {
                numResourceFound++;
                break;
            }
        }
    }

    // loop through all required resources and allocate them
    if (numResourceFound == requestedResources.size())
    {
        for (int i = 0; i < requestedResources.size(); i++)
        {
            for (int j = 0; j < mResources.size(); j++)
            {
                if (mResources.at(j).GetName() == requestedResources.at(i).GetName() && mResources.at(j).GetValue() >= requestedResources.at(i).GetValue())
                {
                    mResources.at(j).AllocateResourceValue(requestedResources.at(i).GetValue());
                    break;
                }
            }
        }
        allocatedResource = true;    
    }
    if (pthread_mutex_unlock(&mMutex))
    {
        cout<< "Error unlocking mutex in ResourceDepot::AcquireResources()";
        exit(1);
    }

    return allocatedResource;
}

bool ResourceDepot::ReleaseResources(vector<Resource>& requestedResources)
{
    if (pthread_mutex_lock(&mMutex))
    {
        cout<< "Error locking mutex in ResourceDepot::ReleaseResources()";
        exit(1);
    }

    for (int i = 0; i < requestedResources.size(); i++)
    {
        for (int j = 0; j < mResources.size(); j++)
        {
            if (mResources.at(j).GetName() == requestedResources.at(i).GetName())
            {
                mResources.at(j).ReAcquireResourceValue(requestedResources.at(i).GetValue());
                break;
            }
        }
    }
    if (pthread_mutex_unlock(&mMutex))
    {
        cout<< "Error unlocking mutex in ResourceDepot::ReleaseResources()";
        exit(1);
    }
    return true;
}

int ResourceDepot::GetNumOfResourceTypes() const { return mResources.size(); }

void AsynchronousTask::Wait(int intervalInMillisec)
{
    mTimeWaited += intervalInMillisec;
    struct timespec interval;
    interval.tv_sec =  (long) intervalInMillisec / 1000;              // seconds 
    interval.tv_nsec = (long) ((intervalInMillisec % 1000) * 1E6);    // nanoseconds
    if (nanosleep(&interval, NULL) < 0)
      printf("warning: delay: %s\n", strerror(errno));
}

#ifndef UTILITYCLASSES_H
#define UTILITYCLASSES_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <sys/types.h>

#define NRES_TYPES 10
#define NTASKS 25

using namespace std;

enum TaskState { Waiting, Running, Idle };

struct ProgramStateInfo
{
    string mFileName;
    int mMonitorTime;
    u_int mNumRequiredIterations;
};

class Resource
{
    string mName;
    int mValue;

    public:
    Resource(string name, int value);
    string GetName() const;
    int GetValue() const;

    void AllocateResourceValue(int value);
    void ReAcquireResourceValue(int value);
};

class AsynchronousTask
{
    string mName;
    int mBusyTime;
    int mIdleTime;
    int mNumIterations;
    bool mIsClaimed;
    int mTimeWaited;
    TaskState mTaskState;
    pthread_t mThreadId;
    vector<Resource> mRequiredResources;

    public:
    AsynchronousTask(string name, int busytime, int idletime, int numIterations);
    string GetName() const;
    int GetNumIterations() const;
    int GetBusyTime() const;
    int GetIdleTime() const;
    void AddRequiredResource(Resource resource);
    int GetNumOfRequiredResourceTypes() const;
    bool IsClaimed() const;
    vector<Resource>& GetRequiredResources();
    void Wait(int interval); // in millisec
    void SetClaimed(pthread_t threadId);
    void SetState(TaskState state);
    TaskState GetState() const;
    pthread_t GetThreadId() const;
    int GetTimeWaited() const;
};

class ResourceDepot
{
    vector<Resource> mResources;
    pthread_mutex_t mMutex;
    friend class ParseUtility;
    friend class PrintUtility;

    public:
    ResourceDepot();
    bool AcquireResources(vector<Resource>& requestedResources);
    bool ReleaseResources(vector<Resource>& heldResources);
    int GetNumOfResourceTypes() const;
};

class TaskDepot
{
    friend class ParseUtility;
    friend class PrintUtility;
    pthread_mutex_t mMutex;
    vector<AsynchronousTask> mTasks;
    public:
    TaskDepot();
    AsynchronousTask& AcquireTask();
    int GetNumTasks() const;
};

struct DataDepo
{
    ProgramStateInfo* mProgramStateInfo;
    TaskDepot* mTaskDepot;
    ResourceDepot* mResourceDepot;
    clock_t* mStartTime;
    bool mIsProgramFinished;
    bool mMonitorPrinting;
    DataDepo(ProgramStateInfo* programStateInfo, TaskDepot* taskDepot, ResourceDepot* resourceDepot, clock_t* startTime) 
        : mProgramStateInfo(programStateInfo), mTaskDepot(taskDepot), mResourceDepot(resourceDepot), mIsProgramFinished(false), mMonitorPrinting(false), mStartTime(startTime) {}
};

class ParseUtility 
{
    private:
    static void Tokenize(const string& inputLine, vector<string>& tokens, char delim);
    static void ParseResource(string inputLine, ResourceDepot& resourceDepot);
    static void ParseAsynchronousTask(string inputLine, TaskDepot& taskDepot, int numIterations);

    ParseUtility(){} // this is to disallow public instantiation of static utility class

    public:
    static void ParseInputFile(string filePath, TaskDepot& taskDepot, ResourceDepot& resourceDepot, int numIterations);
    static ProgramStateInfo ParseProgramStateInfo(char** arguments);
};

class PrintUtility
{
    public:
    static void PrintProgramStateInfo(ProgramStateInfo* stateInfo);
    static void PrintResourceVector(vector<Resource>& resources);
    static void PrintSystemResources(ResourceDepot& resourceDepot);
    static void PrintTaskStatus(TaskDepot& taskDepot);
    static void PrintMonitorStateInfo(TaskDepot& taskDepot);
};

#endif
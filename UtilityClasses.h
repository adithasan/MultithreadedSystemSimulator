#ifndef UTILITYCLASSES_H
#define UTILITYCLASSES_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#define NRES_TYPES 10
#define NTASKS 25

using namespace std;

enum RunState { Waiting, Running, Idle };

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
};

class AsynchronousTask
{
    string mName;
    int mBusyTime;
    int mIdleTime;
    int mNumIterations;
    bool isFinished;
    vector<Resource> mRequiredResources;

    public:
    AsynchronousTask(string name, int busytime, int idletime, int numIterations);
    string GetName() const;
    int GetNumIterations() const;
    void AddRequiredResource(Resource resource);
    int GetNumOfRequiredResourceTypes() const;
};

class ResourceDepot
{
    vector<Resource> mResources;
    friend class ParseUtility;

    public:
    bool AcquireResources(vector<Resource> requestedResources);
    bool ReleaseResources(vector<Resource> heldResources);
};

class TaskDepot
{
    friend class ParseUtility;
    vector<AsynchronousTask> mTasks;
    public:
    AsynchronousTask& AcquireTask();
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
};

#endif
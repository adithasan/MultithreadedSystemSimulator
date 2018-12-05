#ifndef UTILITYCLASSES_H
#define UTILITYCLASSES_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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
};

class AsynchronousTask
{
    string mName;
    int mBusyTime;
    int mIdleTime;
    int mNumIterations;
    bool isFinished;
    vector<Resource> mRequiredResources;
};

class ResourceDepot
{

};

class TaskDepot
{

};

class ParseUtility 
{
    private:
    static void ParseResource(string inputLine, vector<Resource> resources);
    static void ParseAsynchronousTask(string inputLine, vector<AsynchronousTask> tasks);

    ParseUtility(){} // this is to disallow public instantiation of static utility class

    public:
    static void ParseInputFile(string filePath, vector<AsynchronousTask> tasks, vector<Resource> resources);
    static ProgramStateInfo ParseProgramStateInfo(char** arguments);
};

class PrintUtility
{
    public:
    static void PrintProgramStateInfo(ProgramStateInfo* stateInfo);
};

#endif
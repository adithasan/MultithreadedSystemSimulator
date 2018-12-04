#ifndef UTILITYCLASSES_H
#define UTILITYCLASSES_H

#include <iostream>
#include <string>

#define NRES_TYPES 10
#define NTASKS 25

using namespace std;

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
    Resource mRequiredResources[NRES_TYPES];
};

class ParseUtility 
{
    private:
    static void ParseResource(string inputLine, Resource resources[]);
    static void ParseAsynchronousTask(string inputLine, AsynchronousTask tasks[]);

    ParseUtility(){} // this is to disallow public instantiation of static utility class

    public:
    static void ParseInputFile(string filePath);
    static ProgramStateInfo ParseProgramStateInfo(char** arguments);
};

class PrintUtility
{
    public:
    static void PrintProgramStateInfo(ProgramStateInfo* stateInfo);
};

#endif
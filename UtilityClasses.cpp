#include "UtilityClasses.h"

using namespace std;

void ParseUtility::ParseResource(string inputLine, Resource resources[])
{
    // resources A:1 B:1 C:1 D:1 E:1
}

void ParseUtility::ParseAsynchronousTask(string inputLine, AsynchronousTask tasks[])
{

}

void ParseUtility::ParseInputFile(string fileName)
{
    
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
        cout<<"Invalid monitor time entered\n";
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
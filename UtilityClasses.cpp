#include "UtilityClasses.h"

using namespace std;

void ParseUtility::ParseResource(string inputLine, vector<Resource>& resources)
{
    // resources A:1 B:1 C:1 D:1 E:1
    cout<< "Entered ParseResource function" <<endl;
}

void ParseUtility::ParseAsynchronousTask(string inputLine, vector<AsynchronousTask>& tasks)
{
    // task t1 50 100 A:1 B:1
    cout<< "Entered ParseAsynchronousTask function" << endl;
}

void ParseUtility::ParseInputFile(string fileName, vector<AsynchronousTask>& tasks, vector<Resource>& resources)
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
            if (token.compare("resources") == 0) { ParseUtility::ParseResource(currentLine, resources); }
            else if (token.compare("task") == 0) { ParseUtility::ParseAsynchronousTask(currentLine, tasks); }
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
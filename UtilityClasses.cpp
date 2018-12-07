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
        cout<< "Created Task following params: " <<endl;
        cout<< "Name: " << task.GetName()<<endl;;
        cout<< " Numiterations: " <<task.GetNumIterations()<<endl;;
        cout<< "Num of Required Resource Types: " << task.GetNumOfRequiredResourceTypes()<<endl;
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

Resource::Resource(string name, int value)
{
    mName = name;
    mValue = value;
}

string Resource::GetName() const { return mName; }
int Resource::GetValue() const { return mValue; }

AsynchronousTask::AsynchronousTask(string name, int busyTime, int idleTime, int numIterations)
{
    mName = name;
    mBusyTime = busyTime;
    mIdleTime = idleTime;
    mNumIterations = numIterations;
}

string AsynchronousTask::GetName() const { return mName; }
int AsynchronousTask::GetNumIterations() const { return mNumIterations; }
void AsynchronousTask::AddRequiredResource(Resource resource) { mRequiredResources.push_back(resource); }
int AsynchronousTask::GetNumOfRequiredResourceTypes() const { return mRequiredResources.size(); }
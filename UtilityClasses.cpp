#include <iostream>
#include <string>

#define NRES_TYPES 10
#define NTASKS 25

using namespace std;
class ParseUtility
{
    private:
    static void ParseUtility::ParseResource(string inputLine, Resource resources[])
    {
        // resources A:1 B:1 C:1 D:1 E:1
    }

    static void ParseUtility::ParseAsynchronousTask(string inputLine, AsynchronousTask tasks[])
    {

    }
    public:
    static void ParseUtility::ParseInputFile(string filePath)
    {
        
    }
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
    Resource mRequiredResources [NRES_TYPES];
};
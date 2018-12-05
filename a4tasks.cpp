#include "UtilityClasses.cpp"
using namespace std;

int main(int argc, char** argv) 
{
    ProgramStateInfo stateInfo;
    AsynchronousTask tasks[25];
    Resource resources [10];
    if (argc != 4)
    {
        cout<<"Invalid number of arguments. Exiting program\n";
        exit(1);
    }
    else { stateInfo = ParseUtility::ParseProgramStateInfo(argv); }

    PrintUtility::PrintProgramStateInfo(&stateInfo);

    ParseUtility::ParseInputFile(stateInfo.mFileName, tasks, resources);
}
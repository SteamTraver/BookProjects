#include "FutureUtilTest.h"

int main()
{
    using namespace FutureTest;

    TasksPtr tasks = std::make_shared<Tasks>();
    ResultsPtr results = std::make_shared<Results>();

    TaskGenerator generator(tasks, results);
    TaskProcessor taskProc(tasks);
    ResultProcesser resultProc(results);

    std::thread generatorThread(generator);
    std::thread taskProcThread(taskProc);
    std::thread resultProcThread(resultProc);

    generatorThread.join();
    taskProcThread.join();
    resultProcThread.join();

    return EXIT_SUCCESS;
}
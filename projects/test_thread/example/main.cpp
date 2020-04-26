
#include <iostream>
#include <stdio.h>

#include <OpenThreads/Atomic>
#include <OpenThreads/Barrier>
#include <OpenThreads/Block>
#include <OpenThreads/Condition>
#include <OpenThreads/Mutex>
#include <OpenThreads/ReadWriteMutex>
#include <OpenThreads/ReentrantMutex>
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Thread>

// #include "../../common/launcher.h"
// #include "../../common/use_osg_static_library.h"


class MyThread
    :public OpenThreads::Thread
{
public:
    MyThread() {}
    ~MyThread() {}

    virtual void run()
    {
        m.lock();


        m.unlock();
    }

    OpenThreads::Mutex m;

private:
    
};


int main()
{
    OpenThreads::Atomic atomic(99);

    MyThread* t = new MyThread();

    t->start();

    while (!t->isRunning())
    {
        printf("t");
    }


    if (t)
    {
        t->join();
        delete t;
        t = 0;
    }

    return 0;
}


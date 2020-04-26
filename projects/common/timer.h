
#ifndef _TIMER_H_
#define _TIMER_H_

#if _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <time.h>

/*
    //run
    Timer t;
    while( (!viewer.done()) && (!app->isDone()))
    {
    //计算更新时间 ms
    double dt = t.GetCounter();
    t.StartCounter();

    app->onPreFrame( &viewer, dt/1000.0 );
    viewer.frame();
    app->onPostFrame(&viewer, dt/1000.0 );
    }

    */

class Timer
{
public:

    Timer()
        :PCFreq(0.0)
        , CounterStart(0)
    {
        StartCounter();
    }

    void StartCounter()
    {

#if _WIN32
        LARGE_INTEGER li;

        if (!QueryPerformanceFrequency(&li))
        {
            std::cout << "QueryPerformanceFrequency failed!\n";
        }
        PCFreq = double(li.QuadPart) / 1000.0;

        QueryPerformanceCounter(&li);
        CounterStart = li.QuadPart;
#endif//_WIN32

#if __GNUG__ && (!_WIN32)

        //目前还没有能够计算出精确值

        timespec ts;
        clock_getres(CLOCK_MONOTONIC,&ts);
        //printf("0-%d %d\n",ts.tv_sec,ts.tv_nsec);

        clock_gettime(CLOCK_MONOTONIC,&ts);
        //printf("1-%d %d\n",ts.tv_sec,ts.tv_nsec);

#endif //__CYGWIN__

    }

    // ms
    double GetCounter()
    {

#if _WIN32
        LARGE_INTEGER li;

        QueryPerformanceCounter(&li);
        return double(li.QuadPart - CounterStart) / PCFreq;
#endif//_WIN32

#if __GNUG__ && (!_WIN32)
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC,&ts);
#endif //__CYGWIN__

        return 1000.0 / 60.0; //如果系统不支持计时器
    }

private:

    double PCFreq;
    long long CounterStart;
};



#endif //_TIMER_H_


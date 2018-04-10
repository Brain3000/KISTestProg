// KISTestProg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <chrono>
#include <iostream>
#include <list>
#include <thread>

#include "main_functions.h"
#include "request.h"
#include "request_queue.h"

using Threads = std::list<std::thread>;
void StartFillQueue(Threads& threads, RequestQueue& queue, Stopper stopSignal);
void StartProcessRequests(Threads& threads, RequestQueue& queue, Stopper stopSignal);

int main()
{
    const std::chrono::seconds WorkTime(30);
    const size_t MinThreadCount(2);
    const size_t ThreadCount = MinThreadCount + rand() % 10;
    
    try
    {
        Stopper stopSignal;
        Threads threads;
        RequestQueue requestQueue;
        StartFillQueue(threads, requestQueue, stopSignal);
        StartProcessRequests(threads, requestQueue, stopSignal);

        std::this_thread::sleep_for(WorkTime);

        stopSignal.GetInstance().Stop();
        for (auto& thread : threads)
            thread.join();
        requestQueue.Clear();
    }
    catch (const std::exception& exc)
    {
        std::cout << "Error: " << exc.what() << std::endl;
        return 1;
    }
    return 0;
}

void StartFillQueue(Threads& threads, RequestQueue& queue, Stopper stopSignal)
{
    Request* request(nullptr);
    do
    {
        request = GetRequest(stopSignal);
        std::cout << "Request with id " << request->Id() << " was created\n";
        if (request)
            queue.Push(request);
    } while (request);
}

void StartProcessRequests(Threads& threads, RequestQueue& queue, Stopper stopSignal)
{
    while (!stopSignal.GetInstance().IsStopped())
    {
        Request* request = queue.Pop();
        if (request)
        {
            ProcessRequest(request, stopSignal);
            std::cout << "Request with id " << request->Id() << " processed, state: "/* << request->State()*/ << std::endl;
            DeleteRequest(request);
        }
    }
}

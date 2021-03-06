// KISTestProg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <assert.h>
#include <chrono>
#include <conio.h>
#include <iostream>
#include <list>
#include <thread>

#include "main_functions.h"
#include "request.h"
#include "request_queue.h"

using Threads = std::list<std::thread>;
void FillCreatorsQueue(Threads& threads, RequestQueue& queue, Stopper stopSignal, size_t threadCount);
void FillProcessorQueue(Threads& threads, RequestQueue& queue, Stopper stopSignal, size_t threadCount);

int main()
{
    const std::chrono::seconds WorkTime(1);
    const size_t MinThreadCount(2);
	std::srand(time(nullptr));
	const size_t FillQueueThreadCount = MinThreadCount + std::rand() % 10;
	const size_t ProcessQueueThreadCount = MinThreadCount + std::rand() % 10;

	std::cout << "There will be created " << FillQueueThreadCount << " threads for creating requests\n";
	std::cout << "There will be created " << ProcessQueueThreadCount << " threads for processing requests\n";

    try
    {
        Stopper stopSignal;
        Threads threads;
        RequestQueue requestQueue;
        FillCreatorsQueue(threads, requestQueue, stopSignal, FillQueueThreadCount);
		FillProcessorQueue(threads, requestQueue, stopSignal, ProcessQueueThreadCount);

        std::this_thread::sleep_for(WorkTime);

		std::cout << "Stop signal will be sent\n";
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
	std::cout << "The end\n";
#ifdef _DEBUG
	_getch();
#endif
    return 0;
}

void CreateAndAddRequest(RequestQueue& queue, Stopper stopSignal)
{
	Request* request(nullptr);
	do
	{
		if (request = GetRequest(stopSignal))
		{
			queue.Push(request);
		}
		else
		{
			if (!stopSignal.GetInstance().IsStopped())
			{
				std::cout << "Request wasn't created\n";
				assert(false);
			}
		}
	} while (request && !stopSignal.GetInstance().IsStopped());
}

void FillCreatorsQueue(Threads& threads, RequestQueue& queue, Stopper stopSignal, size_t threadCount)
{
	for (unsigned i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(&CreateAndAddRequest, std::ref(queue), stopSignal);
	}
}

void ProcessRequests(RequestQueue& queue, Stopper stopSignal)
{
    while (!stopSignal.GetInstance().IsStopped())
    {
        if (Request* request = queue.Pop())
        {
            ProcessRequest(request, stopSignal);
            DeleteRequest(request);
        }
    }
}

void FillProcessorQueue(Threads& threads, RequestQueue& queue, Stopper stopSignal, size_t threadCount)
{
	for (unsigned i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(&ProcessRequests, std::ref(queue), stopSignal);
	}
}

#include "stdafx.h"
#include <atomic>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "request.h"
#include "main_functions.h"


void EmulateLongTimeOperation(Stopper stopSignal, bool& result)
{
    result = false;
    DWORD waitResult = ::WaitForSingleObject(stopSignal.GetInstance().GetEventHandle(), 10 + rand() % 90);
    result = (waitResult == WAIT_TIMEOUT);
}

Request* GetRequest(Stopper stopSignal)
{
	bool operationResult(!stopSignal.GetInstance().IsStopped());
	if (operationResult)
	{
		std::thread longOperationThread(EmulateLongTimeOperation, stopSignal, std::ref(operationResult));
		longOperationThread.join();
	}
	//if (!operationResult)
	//	std::cout << "GetRequest canceled\n";
    return operationResult ? new (std::nothrow) Request : nullptr;
}

void ProcessRequest(Request* request, Stopper stopSignal)
{
    assert(request);
    if (!request)
        return;
	if (bool operationResult = !stopSignal.GetInstance().IsStopped())
	{
		request->SetState(RequestState::ProcessingBegin);
		std::thread longOperationThread(EmulateLongTimeOperation, stopSignal, std::ref(operationResult));
		longOperationThread.join();
		//if (!operationResult)
		//	std::cout << "Process Request with id " << request->Id() << "canceled\n";
		request->SetState(operationResult ? RequestState::ProcessingEndSuccess : RequestState::ProcessingEndFail);
	}
}

void DeleteRequest(Request* request)
{
    delete request;
}

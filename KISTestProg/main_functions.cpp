#include "stdafx.h"
#include <chrono>
#include <atomic>
#include <assert.h>
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
    bool operationResult(false);
    std::thread longOperationThread(EmulateLongTimeOperation, stopSignal, std::ref(operationResult));
    longOperationThread.join();

    return operationResult ? nullptr : new (std::nothrow) Request;
}

void ProcessRequest(Request* request, Stopper stopSignal)
{
    assert(request);
    if (!request)
        return;
    request->SetState(RequestState::ProcessingBegin);
    bool operationResult(false);
    std::thread longOperationThread(EmulateLongTimeOperation, stopSignal, std::ref(operationResult));
    longOperationThread.join();
    request->SetState(operationResult ? RequestState::ProcessingEndSuccess : RequestState::ProcessingEndFail);
}

void DeleteRequest(Request* request)
{
    delete request;
}
#include "stdafx.h"

#include <iostream>

#include "main_functions.h"
#include "request.h"
#include "request_queue.h"


RequestQueue::RequestQueue() :
    m_mutex(::CreateMutex(nullptr, FALSE, nullptr), ::CloseHandle)
{    
    if (!m_mutex)
        throw std::runtime_error("RequestQueue: can't create mutex");
}

bool RequestQueue::Push(Request* request)
{
    if (!request)
        return false;
    DWORD res = ::WaitForSingleObject(m_mutex.get(), INFINITE);
    ReleaseMutexGuard guard(m_mutex.get());
    if (res != WAIT_OBJECT_0)
    {
        std::cout << "Error in RequestQueue::Push: WaitForSingleObject returned " << res;
        return true;
    }
    m_queue.push(request);
    return true;
}

Request* RequestQueue::Pop()
{
    Request* requestToReturn(nullptr);
    DWORD res = ::WaitForSingleObject(m_mutex.get(), INFINITE);
    ReleaseMutexGuard guard(m_mutex.get());

    if (res != WAIT_OBJECT_0)
    {
        std::cout << "Error in RequestQueue::Pop: WaitForSingleObject returned " << res << std::endl;
        return requestToReturn;
    }

    if (!m_queue.empty())
    {
        requestToReturn = m_queue.front();
        m_queue.pop();
    }

    return requestToReturn;
}

bool RequestQueue::Clear()
{
    DWORD res = ::WaitForSingleObject(m_mutex.get(), INFINITE);
    ReleaseMutexGuard guard(m_mutex.get());

    if (res != WAIT_OBJECT_0)
    {
        std::cout << "Error in RequestQueue::Clear: WaitForSingleObject returned " << res << std::endl;
        return false;
    }
    std::cout << "Queue will be cleare\n";
    std::cout << (m_queue.empty() ? "Queue is empty" : "Queue is not empty") << std::endl;
    while (!m_queue.empty())
    {
        Request* request(m_queue.front());
        std::cout << "Request with id " << request->Id() << " has status " << request->StateAsString() << std::endl;
        m_queue.pop();
        DeleteRequest(request);
    }
    return true;
}

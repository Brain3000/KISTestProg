#pragma once
#include <queue>

#include "common.h"

class Request;

class RequestQueue
{
public:
    RequestQueue();
    bool Push(Request* request);
    Request* Pop();
    bool Clear();

private:
    std::queue<Request*> m_queue;
    RaiiHandle m_mutex;
};


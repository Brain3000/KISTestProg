#pragma once
#include <atomic>
#include <string>

enum class RequestState
{
    New,
    ProcessingBegin,
    ProcessingEndSuccess,
    ProcessingEndFail
};

class Request
{
public:
    Request();
    unsigned Id() const;
    RequestState State() const;
    const std::string& StateAsString() const;
    void SetState(RequestState newState);

private:
    static std::atomic_uint s_lastId;
    RequestState m_state;
    const unsigned m_id;
};
#include "stdafx.h"

#include <map>

#include "request.h"

std::atomic_uint Request::s_lastId(0);


Request::Request() : m_id(++s_lastId), m_state(RequestState::New)
{
}

unsigned Request::Id() const
{
    return m_id;
}

RequestState Request::State() const
{
    return m_state;
}

const std::string& Request::StateAsString() const
{
    static const std::map<RequestState, std::string> stateStrings
    {
        { RequestState::New, "New" },
        { RequestState::ProcessingBegin, "ProcessBegin" },
        { RequestState::ProcessingEndSuccess, "ProcessingEndSuccess" },
        { RequestState::ProcessingEndFail, "ProcessingEndFail" },
    };
    return stateStrings.at(m_state);
}

void Request::SetState(RequestState newState)
{
    m_state = newState;
}
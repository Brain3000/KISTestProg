#include "stdafx.h"

#include <stdexcept>

#include "stopper.h"

StopperInstance::StopperInstance() :
    m_event(::CreateEvent(nullptr, TRUE, FALSE, nullptr), ::CloseHandle)
{
    if (!m_event.get())
        throw std::runtime_error("StopperInstance: can't create event");
}

const bool StopperInstance::IsStopped() const
{
    return (::WaitForSingleObject(m_event.get(), 0) == WAIT_OBJECT_0);
}

void StopperInstance::Stop()
{
    SetEvent(m_event.get());
}

HANDLE StopperInstance::GetEventHandle() const
{
    return m_event.get();
}

StopperInstance& Stopper::GetInstance()
{
    static StopperInstance s_instance;
    return s_instance;
}

#pragma once

#include "common.h"

class StopperInstance
{
    friend struct Stopper;
private:
    StopperInstance(const StopperInstance&) = delete;
    StopperInstance(StopperInstance&&) = delete;
    StopperInstance();

public:
    const bool IsStopped() const;
    void Stop();
    HANDLE GetEventHandle() const;

private:
    RaiiHandle m_event;
};

struct Stopper
{
    StopperInstance& GetInstance();
};


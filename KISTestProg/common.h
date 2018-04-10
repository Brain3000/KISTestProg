#pragma once
#include <memory>
#include <Windows.h>

using RaiiHandle = std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(&::CloseHandle)>;

class ReleaseMutexGuard
{
public:
    ReleaseMutexGuard(HANDLE mutex) : m_mutex(mutex) {}
    ~ReleaseMutexGuard() { ::ReleaseMutex(m_mutex); }

private:
    HANDLE m_mutex;

private:
    ReleaseMutexGuard() = delete;
    ReleaseMutexGuard(const ReleaseMutexGuard&) = delete;
    ReleaseMutexGuard(ReleaseMutexGuard&&) = delete;
};
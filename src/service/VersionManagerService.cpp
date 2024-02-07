#include "VersionManagerService.hpp"
#include "ThreadPool.hpp"
#include <Globals.hpp>

boomstation::VersionManagerService::VersionManagerService(QObject *parent) :
    AbstractWindowsService(Globals::SERVICE_NAME),
    QObject(parent),
    m_stopping(false),
    m_stoppedEvent(CreateEvent(NULL, TRUE, FALSE, NULL))
{
    if (!m_stoppedEvent.get())
    {
        throw std::runtime_error("CreateEvent failed");
    }
}

void boomstation::VersionManagerService::onStart(DWORD argc, wchar_t **argv)
{
    writeEventLogEntry(m_name + L" in onStart", EVENTLOG_INFORMATION_TYPE);
    ThreadPool::QueueUserWorkItem(&VersionManagerService::serviceWorkerThread, this);
}

void boomstation::VersionManagerService::onStop()
{
    writeEventLogEntry(m_name + L" in onStop", EVENTLOG_INFORMATION_TYPE);
    m_stopping = true;
    if (WaitForSingleObject(m_stoppedEvent.get(), INFINITE) != WAIT_OBJECT_0)
    {
        throw std::runtime_error("WaitForSingleObject failed");
    }
}

void boomstation::VersionManagerService::onPause() {}
void boomstation::VersionManagerService::onContinue() {}
void boomstation::VersionManagerService::onShutdown() {}

void boomstation::VersionManagerService::serviceWorkerThread()
{
    while (!m_stopping)
    {
        emit checkForUpdates();
        Sleep(Globals::SERVICE_TIMEOUT_MSEC);
    }
    SetEvent(m_stoppedEvent.get());
}

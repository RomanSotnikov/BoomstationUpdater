#include "AbstractWindowsService.hpp"
#include <stdexcept>
#include <strsafe.h>

AbstractWindowsService::AbstractWindowsService(std::wstring serviceName,
                                               bool canStop,
                                               bool canShutdown,
                                               bool canPauseContinue) :
    m_name(std::move(serviceName))
{
    DWORD controlsAccepted = 0;
    if (canStop) controlsAccepted |= SERVICE_ACCEPT_STOP;
    if (canShutdown) controlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
    if (canPauseContinue) controlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;

    m_status.dwControlsAccepted = controlsAccepted;
    m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_status.dwCurrentState = SERVICE_START_PENDING;
    m_status.dwWin32ExitCode = NO_ERROR;
    m_status.dwServiceSpecificExitCode = 0;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;
}

BOOL AbstractWindowsService::run(AbstractWindowsService &service)
{
    s_service = &service;

    SERVICE_TABLE_ENTRYW serviceTable[] =
    {
        { &service.m_name[0], serviceMain },
        { nullptr, nullptr }
    };

    return StartServiceCtrlDispatcherW(serviceTable);
}

void AbstractWindowsService::stop()
{
    DWORD originalState = m_status.dwCurrentState;
    try
    {
        setStatus(SERVICE_STOP_PENDING);
        onStop();
        setStatus(SERVICE_STOPPED);
    }
    catch (DWORD error)
    {
        writeErrorLogEntry(L"Service Stop", error);
        setStatus(originalState);
    }
    catch (...)
    {
        writeEventLogEntry(L"Service failed to stop.", EVENTLOG_ERROR_TYPE);
        setStatus(originalState);
    }
}

void AbstractWindowsService::setStatus(DWORD state, DWORD exitCode, DWORD waitHint)
{
    static DWORD checkPoint = 1;

    m_status.dwCurrentState = state;
    m_status.dwWin32ExitCode = exitCode;
    m_status.dwWaitHint = waitHint;
    m_status.dwCheckPoint = ((state == SERVICE_RUNNING) || (state == SERVICE_STOPPED)) ? 0 : checkPoint++;

    ::SetServiceStatus(m_statusHandle, &m_status);
}

void AbstractWindowsService::writeEventLogEntry(const std::wstring &message, WORD type)
{
    HANDLE eventSource = NULL;
    LPCWSTR strings[2] = { NULL, NULL };

    eventSource = RegisterEventSourceW(NULL, m_name.c_str());
    if (!eventSource) return;

    strings[0] = m_name.c_str();
    strings[1] = message.c_str();

    ReportEventW(eventSource, type, 0, 0, NULL, 2, 0, strings, NULL);
    DeregisterEventSource(eventSource);
}

void AbstractWindowsService::writeErrorLogEntry(const std::wstring &function, DWORD error)
{
    wchar_t message[260];
    StringCchPrintfW(message, ARRAYSIZE(message), L"%s failed w/err 0x%08lx", function.c_str(), error);
    writeEventLogEntry(message, EVENTLOG_ERROR_TYPE);
}

void AbstractWindowsService::serviceMain(DWORD argc, wchar_t **argv)
{
    s_service->m_statusHandle = RegisterServiceCtrlHandlerW(s_service->m_name.c_str(), serviceCtrlHandler);
    if (!s_service->m_statusHandle)
    {
        throw std::runtime_error("RegisterServiceCtrlHandlerW failed");
    }

    s_service->startService(argc, argv);
}

void AbstractWindowsService::serviceCtrlHandler(DWORD ctrl)
{
    switch (ctrl)
    {
    case SERVICE_CONTROL_STOP: s_service->stop(); break;
    case SERVICE_CONTROL_PAUSE: s_service->pauseService(); break;
    case SERVICE_CONTROL_CONTINUE: s_service->continueService(); break;
    case SERVICE_CONTROL_SHUTDOWN: s_service->shutdownService(); break;
    case SERVICE_CONTROL_INTERROGATE: default: break;
    }
}

void AbstractWindowsService::startService(DWORD argc, wchar_t **argv)
{
    try
    {
        setStatus(SERVICE_START_PENDING);
        onStart(argc, argv);
        setStatus(SERVICE_RUNNING);
    }
    catch (DWORD error)
    {
        writeErrorLogEntry(L"Service Start", error);
        setStatus(SERVICE_STOPPED, error);
    }
    catch (...)
    {
        writeEventLogEntry(L"Service failed to start.", EVENTLOG_ERROR_TYPE);
        setStatus(SERVICE_STOPPED);
    }
}

void AbstractWindowsService::pauseService()
{
    try
    {
        setStatus(SERVICE_PAUSE_PENDING);
        onPause();
        setStatus(SERVICE_PAUSED);
    }
    catch (DWORD error)
    {
        writeErrorLogEntry(L"Service Pause", error);
        setStatus(SERVICE_RUNNING);
    }
    catch (...)
    {
        writeEventLogEntry(L"Service failed to pause.", EVENTLOG_ERROR_TYPE);
        setStatus(SERVICE_RUNNING);
    }
}

void AbstractWindowsService::continueService()
{
    try
    {
        setStatus(SERVICE_CONTINUE_PENDING);
        onContinue();
        setStatus(SERVICE_RUNNING);
    }
    catch (DWORD error)
    {
        writeErrorLogEntry(L"Service Continue", error);
        setStatus(SERVICE_PAUSED);
    }
    catch (...)
    {
        writeEventLogEntry(L"Service failed to resume.", EVENTLOG_ERROR_TYPE);
        setStatus(SERVICE_PAUSED);
    }
}

void AbstractWindowsService::shutdownService()
{
    try
    {
        onShutdown();
        setStatus(SERVICE_STOPPED);
    }
    catch (DWORD error)
    {
        writeErrorLogEntry(L"Service Shutdown", error);
    }
    catch (...)
    {
        writeEventLogEntry(L"Service failed to shut down.", EVENTLOG_ERROR_TYPE);
    }
}

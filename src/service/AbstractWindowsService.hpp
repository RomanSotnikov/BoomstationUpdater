#pragma once

#include <Windows.h>
#include <string>

class AbstractWindowsService
{    
public:
    AbstractWindowsService(std::wstring serviceName,
                           bool canStop = true,
                           bool canShutdown = true,
                           bool canPauseContinue = false);
    virtual ~AbstractWindowsService() = default;

    static BOOL run(AbstractWindowsService &service);
    void stop();

protected:
    virtual void onStart(DWORD argc, wchar_t **argv)= 0;
    virtual void onStop() = 0;
    virtual void onPause() = 0;
    virtual void onContinue() = 0;
    virtual void onShutdown() = 0;

    void setStatus(DWORD state, DWORD exitCode = NO_ERROR, DWORD waitHint = 0);
    void writeEventLogEntry(const std::wstring &message, WORD type);
    void writeErrorLogEntry(const std::wstring &function, DWORD error = GetLastError());

protected:
    static AbstractWindowsService *s_service;
    std::wstring m_name;
    SERVICE_STATUS m_status;
    SERVICE_STATUS_HANDLE m_statusHandle;

private:
    static void __stdcall serviceMain(DWORD argc, wchar_t **argv);
    static void __stdcall serviceCtrlHandler(DWORD ctrl);

    void startService(DWORD argc, wchar_t **argv);
    void pauseService();
    void continueService();
    void shutdownService();

}; // class AbstractWindowsService

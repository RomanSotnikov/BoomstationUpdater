#include "serviceInstaller.hpp"
#include <wil/resource.h>
#include <QDebug>

void installService(const std::wstring &exec,
                    const std::wstring &name,
                    const std::wstring &displayName,
                    const DWORD startType)
{
    wil::unique_schandle manager(OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE));
    if (!manager.get())
    {
        qCritical() << "OpenSCManager failed";
        return;
    }

    wil::unique_schandle service(CreateServiceW(manager.get(),
                                                name.c_str(),
                                                displayName.c_str(),
                                                SERVICE_QUERY_STATUS,
                                                SERVICE_WIN32_OWN_PROCESS,
                                                startType,
                                                SERVICE_ERROR_NORMAL,
                                                exec.c_str(),
                                                nullptr,
                                                nullptr,
                                                nullptr,
                                                nullptr,
                                                nullptr));
    if (!service.get())
    {
        qCritical() << "CreateServiceW failed";
        return;
    }

    qInfo() << "Service" << name << "was successfully installed";
}

void uninstallService(const std::wstring &name)
{
    wil::unique_schandle manager(OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT));
    if (!manager.get())
    {
        qCritical() << "OpenSCManager failed";
        return;
    }

    wil::unique_schandle service(OpenServiceW(manager.get(), name.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE));
    if (!service.get())
    {
        qCritical() << "OpenServiceW failed";
        return;
    }

    SERVICE_STATUS status = {};
    if (ControlService(service.get(), SERVICE_CONTROL_STOP, &status))
    {
        qInfo() << "Stopping" << name;
        Sleep(1000);

        while(QueryServiceStatus(service.get(), &status))
        {
            if (status.dwCurrentState == SERVICE_STOP_PENDING)
            {
                qInfo() << ".";
                Sleep(1000);
            }
            else break;
        }

        if (status.dwCurrentState == SERVICE_STOPPED) qInfo() << name << "was stopped";
        else qInfo() << name << "was failed to stop";
    }

    if (!DeleteService(service.get()))
    {
        qCritical() << "DeleteService failed";
        return;
    }
    qInfo() << name << " was successfully uninstalled";
}

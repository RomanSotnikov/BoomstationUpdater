#pragma once

#include "AbstractWindowsService.hpp"
#include <QObject>
#include <wil/resource.h>

namespace boomstation
{

class VersionManagerService : public AbstractWindowsService, public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(VersionManagerService)

public:
    explicit VersionManagerService(QObject *parent = nullptr);
    ~VersionManagerService() override = default;

protected:
    void onStart(DWORD argc, wchar_t **argv) override;
    void onStop() override;
    void onPause() override;
    void onContinue() override;
    void onShutdown() override;

    void serviceWorkerThread();

private:
    bool m_stopping;
    wil::unique_handle m_stoppedEvent;

signals:
    void checkForUpdates() const;

}; // class VersionManagerService

} // namespace boomstation

#pragma once

#include <string>
#include <Windows.h>
#include <QUrl>
#include <QString>

namespace boomstation
{

class Globals
{
    Globals() = delete;
    ~Globals() = delete;

public:
    static const std::wstring SERVICE_NAME;
    static const std::wstring SERVICE_DISPLAY_NAME;
    static const DWORD SERVICE_START_TYPE;
    static const unsigned int SERVICE_TIMEOUT_MSEC;

    static const QUrl BOOMSTATION_URL;
    static const QString QUERY_SHOPUID_KEY;
    static const QString QUERY_CURRENT_VERSION_KEY;

}; // class Globals

} // boomstation

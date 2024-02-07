#include "Globals.hpp"

const std::wstring boomstation::Globals::SERVICE_NAME{L"BoomstationVersionManagerService"};
const std::wstring boomstation::Globals::SERVICE_DISPLAY_NAME{L"Boomstation Version Manager Service"};
const DWORD boomstation::Globals::SERVICE_START_TYPE{SERVICE_AUTO_START};
const unsigned int boomstation::Globals::SERVICE_TIMEOUT_MSEC{300000};

const QUrl boomstation::Globals::BOOMSTATION_URL{};
const QString boomstation::Globals::QUERY_SHOPUID_KEY{"shopUID"};
const QString boomstation::Globals::QUERY_CURRENT_VERSION_KEY{"currentVersion"};

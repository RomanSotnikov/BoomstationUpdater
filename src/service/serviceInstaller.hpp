#pragma once

#include <string>
#include <Windows.h>

void installService(const std::wstring &exec,
                    const std::wstring &name,
                    const std::wstring &displayName,
                    const DWORD startType);
void uninstallService(const std::wstring &name);

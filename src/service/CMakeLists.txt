project(service LANGUAGES CXX)

set(CXX_SOURCE_FILES
    ThreadPool.hpp
    serviceInstaller.hpp serviceInstaller.cpp
    AbstractWindowsService.hpp AbstractWindowsService.cpp
    VersionManagerService.hpp VersionManagerService.cpp
)

add_library(${PROJECT_NAME} ${CXX_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    WIL
    globals
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

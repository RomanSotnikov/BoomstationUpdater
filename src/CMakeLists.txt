project(BoomstationUpdater LANGUAGES CXX)

set(CXX_SOURCE_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${CXX_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
)

add_subdirectory(wil)
add_subdirectory(globals)
add_subdirectory(service)
add_subdirectory(updater)

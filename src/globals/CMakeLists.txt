project(globals LANGUAGES CXX)

set(CXX_SOURCE_FILES
    Globals.hpp Globals.cpp
)

add_library(${PROJECT_NAME} STATIC ${CXX_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

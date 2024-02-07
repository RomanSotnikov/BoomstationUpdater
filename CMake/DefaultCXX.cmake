set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PRECOMPILED_HEADER pch.h)

set(CPP_FILES_EXT *.h *.hpp *.cpp *.cxx *.cc *.hxx *.inc *.asm *.ixx)
set(QML_FILES_EXT *.qml)
set(RESOURCE_FILES_EXT *.svg *.png *.ttf)

if(MSVC)
    add_link_options(
        "$<$<CONFIG:Debug>:/DEBUG;/VERBOSE>;"
        "$<$<CONFIG:Release>:/OPT:REF;/INCREMENTAL:NO>;"
        "/OPT:NOICF"
    )
endif()
SET (this_target extrapolator)

project(${this_target})
file(GLOB src *.cpp *.h)
add_library(${this_target} ${src})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (${OPENMIND_BUILD_TESTS})
    add_subdirectory(test)
endif ()

if (${OPENMIND_BUILD_3RD_PARTY_TESTS})
    add_subdirectory(3rdPartyTests)
endif ()

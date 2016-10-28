cmake_minimum_required(VERSION 3.2)
project(globpos)

option(GLOBPOS_BUILD_TESTS "Build tests" OFF)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
endif()

add_library(globpos STATIC
    globpos/NmeaParser.cpp
    globpos/NmeaParser.h
    globpos/NmeaSentence.cpp
    globpos/NmeaSentence.h
    globpos/settings.h
    globpos/GlobalPosition.h
    globpos/gpgga.cpp
    globpos/gpgga.h
)

install(DIRECTORY globpos DESTINATION include FILES_MATCHING PATTERN "*.h")
install(TARGETS globpos DESTINATION lib)

if (GLOBPOS_BUILD_TESTS)
    add_executable(nmeaparser tests/nmeaparser.cpp)
    target_link_libraries(nmeaparser globpos)
endif()

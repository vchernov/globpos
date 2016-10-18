cmake_minimum_required(VERSION 3.2)
project(globpos)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
endif()

add_library(globpos STATIC
    globpos/NmeaParser.h
    globpos/NmeaParser.cpp
    globpos/NmeaSentence.h
    globpos/settings.h
)

install(DIRECTORY globpos DESTINATION include FILES_MATCHING PATTERN "*.h")
install(TARGETS globpos DESTINATION lib)

enable_testing()

add_executable(parser tests/parser.cpp)
target_link_libraries(parser globpos)
add_test(Parser parser)

cmake_minimum_required(VERSION 2.6)
project(blog_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O0 -Wall -Wextra -Wpedantic -Wno-reorder") 

set(LIBRARY_FILES
    libraries/format.cc
    libraries/sha256.cpp
)

set(SERVER_FILES
    http_parser.cpp
    request_handler.cpp
    listener.cpp
    utils.cpp
    main.cpp
)

add_executable(blog_server ${SERVER_FILES} ${LIBRARY_FILES})

set(LIBRARIES 
    pthread
    dl
)

target_link_libraries(blog_server ${LIBRARIES})
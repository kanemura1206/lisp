cmake_minimum_required(VERSION 2.6)

project(lisp)

set(CMAKE_CXX_FLAGS "-Wall")

add_executable(lisp lisp_main.c lisp_token.c)

target_link_libraries(lisp readline) 

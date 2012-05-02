cmake_minimum_required(VERSION 2.6)

project(lisp)

set(CMAKE_C_FLAGS "-Wall -w")
set(CMAKE_C_FLAGS_DEBAG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-O2")

add_executable(lisp lisp_main.c lisp_token.c lisp_tree.c lisp_cal.c)

target_link_libraries(lisp readline) 

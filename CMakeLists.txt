cmake_minimum_required(VERSION 2.6)

project(lisp)

set(CMAKE_C_FLAGS_DEBAG "-o0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-o2")

add_executable(lisp lisp_main.c lisp_token.c lisp_cons_cell.c lisp_cal.c)

target_link_libraries(lisp readline) 

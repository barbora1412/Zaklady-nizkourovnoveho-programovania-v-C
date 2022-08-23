cmake_minimum_required(VERSION 3.0)
project(hw01)

set(MAIN_SOURCES hw01.c hw01_main.c)
set(TEST_SOURCES hw01.c hw01_tests.c)

add_executable(spellcheck ${MAIN_SOURCES})
add_executable(tests      ${TEST_SOURCES})

# Configure compiler warnings
if (CMAKE_C_COMPILER_ID MATCHES Clang OR ${CMAKE_C_COMPILER_ID} STREQUAL GNU)
  # using regular Clang, AppleClang or GCC
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -ggdb3 -Wall -Wextra -pedantic")
  set_target_properties(tests PROPERTIES COMPILE_FLAGS "-Wno-unused-variable")
elseif (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
  # using Visual Studio C++
  target_compile_definitions(spellcheck PRIVATE _CRT_SECURE_NO_DEPRECATE)
  target_compile_definitions(tests      PRIVATE _CRT_SECURE_NO_DEPRECATE)
  set(CMAKE_CXX_FLAGS "/permissive- /W4 /EHsc")
endif()

if(MINGW)
    target_compile_definitions(spellcheck PRIVATE __USE_MINGW_ANSI_STDIO=1)
    target_compile_definitions(tests      PRIVATE __USE_MINGW_ANSI_STDIO=1)
endif()

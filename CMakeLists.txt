cmake_minimum_required (VERSION 2.6)
include_directories (/usr/local/lib ./src/include)

set (CMAKE_C_FLAGS "-std=c99")
set (CMAKE_C_FLAGS_DEBUG "-O0 -g")
set (CMAKE_C_FLAGS_RELEASE "-O3")

set (LIB_SOURCE src/bigint.c)

project (cbigint)
add_library (cbigint SHARED ${LIB_SOURCE})
target_link_libraries (cbigint)

project (cbigint_static)
add_library (cbigint_static STATIC EXCLUDE_FROM_ALL ${LIB_SOURCE})
target_link_libraries (cbigint)

project (smalltest)
add_executable (smalltest EXCLUDE_FROM_ALL src/test.c)
target_link_libraries (smalltest cbigint)

cmake_minimum_required (VERSION 2.6)
include_directories (/usr/local/lib ./src/include)

set (CMAKE_C_FLAGS "-std=c99")

if (ASM_DUMP)
message (AUTHOR_WARNING "Will not compile! Open CMakeFiles/*.dir/src/*.c.o with text editor to see ASM code.")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -S")
endif()

set (CMAKE_C_FLAGS_DEBUG "-O0 -g")
set (CMAKE_C_FLAGS_RELEASE "-O3")

set (LIB_SOURCE src/bigint.c)

project (cbigint)
add_library (cbigint SHARED ${LIB_SOURCE})
target_link_libraries (cbigint)

project (cbigint_static)
add_library (cbigint_static STATIC EXCLUDE_FROM_ALL ${LIB_SOURCE})
target_link_libraries (cbigint)

if (TESTING_ENABLED)

enable_testing ()

project (smalltest)
add_executable (smalltest src/tests/test.c)
target_link_libraries (smalltest cbigint)

project (add_benchmark)
add_executable (add_benchmark src/tests/add_benchmark.c)
target_link_libraries (add_benchmark cbigint)

add_test (verification ./smalltest)
add_test (add_benchmark ./add_benchmark CONFIGURATIONS Release)

endif()
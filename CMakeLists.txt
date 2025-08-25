cmake_minimum_required(VERSION 3.10)
project(ThreadPool C)

# Build the thread pool library
add_library(thread_pool STATIC thread_pool.c)

add_executable(
    main
    main.c
)

target_link_libraries(main PRIVATE thread_pool)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(benchmark benchmark.c)
target_link_libraries(benchmark PRIVATE thread_pool pthread)
target_include_directories(benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_subdirectory(tests)

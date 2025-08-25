# Tests for ThreadPool
add_executable(test_thread_pool test_thread_pool.c)
# Ensure test can find headers in project root
target_include_directories(test_thread_pool PRIVATE ${PROJECT_SOURCE_DIR})
# Link with the thread_pool library
target_link_libraries(test_thread_pool PRIVATE thread_pool)
add_test(NAME thread_pool_test COMMAND test_thread_pool)

add_executable(test_empty_queue test_empty_queue.c)
target_include_directories(test_empty_queue PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(test_empty_queue PRIVATE thread_pool)
add_test(NAME empty_queue_test COMMAND test_empty_queue)

add_executable(test_enqueue_dequeue test_enqueue_dequeue.c)
target_include_directories(test_enqueue_dequeue PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(test_enqueue_dequeue PRIVATE thread_pool)
add_test(NAME enqueue_dequeue_test COMMAND test_enqueue_dequeue)

add_executable(test_multithreaded test_multithreaded.c)
target_include_directories(test_multithreaded PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(test_multithreaded PRIVATE thread_pool pthread)
add_test(NAME multithreaded_test COMMAND test_multithreaded)

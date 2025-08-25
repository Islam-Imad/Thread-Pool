# Thread-Pool

This repository contains a simple task queue and a minimal thread-pool-style API implemented in C. It is intended for learning and experimentation. The current implementation provides a task queue and helpers to enqueue, dequeue, and execute tasks; it does not yet start worker threads.

## Build and test

Generate a build directory, build the library and tests, then run tests with CTest:

```bash
cmake -S . -B build
cmake --build build -- -j
ctest --test-dir build --verbose
```

## Included tests

- `thread_pool_test` — basic functional test that enqueues and executes three increment tasks.
- `empty_queue_test` — verifies calling `execute_tasks` on an empty queue does nothing and doesn't crash.
- `enqueue_dequeue_test` — verifies enqueue followed by dequeue returns a valid task and it can be executed.

## Notes

If you want help extending this into a full thread pool with worker threads, synchronization for concurrent enqueues/dequeues, or CI integration, tell me which direction you'd like and I'll implement it.

# Sequential DAG Job Scheduler

An in-memory sequential task execution engine written in **C++17**. This system models tasks and their relationships as a **Directed Acyclic Graph (DAG)** to ensure jobs are executed in a valid, dependency-resolved topological order.

This repository is currently under active development, starting with the core logging utility.

---

## Active Components

### Logger (`Logger.h` / `logger.cpp`)
The system utilizes a custom, lightweight, stream-separated logging utility designed to serve as the baseline for all upcoming scheduler components:
- **Stream Segregation:** Directs standard execution tracking (`LogLevel::INFO` and `LogLevel::WARNING`) to `std::cout`, while directing error logs (`LogLevel::ERROR`) to `std::cerr` for clean production log redirection.
- **Zero-Dependency Design:** Built strictly using the standard library to maintain low overhead and high portability.

---

## Prerequisites

To compile and verify the current build, you need a C++17 compatible compiler (such as GCC or Clang) installed on your system.

---

## Build Instructions

To compile the active components using a standard C++17 compiler from the root directory:

```bash
g++ -std=c++17 -Iinclude src/logger.cpp -o job_scheduler
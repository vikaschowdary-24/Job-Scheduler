# Job Scheduler with Dependency Resolution (DAG)

A C++17 implementation of a task scheduling system that executes tasks while respecting dependency relationships. The scheduler models jobs as a Directed Acyclic Graph (DAG) and determines a valid execution order using topological sorting.

## Features

- **Dependency-based Job Execution:** Resolves parent-child relationships before scheduling.
- **Priority-based Scheduling:** Uses an in-memory Min-Heap to execute ready tasks based on priority.
- **Parallel Worker Dispatch:** Simulates parallel execution across multiple simulated worker resources.
- **Cycle Detection:** Implements Kahn's Algorithm to detect circular dependencies and prevent deadlocks.
- **Job Execution Logs:** Custom formatted console logging utility to track simulation progress.
- **Object-Oriented Design:** Modular, clean, and maintainable C++17 architecture.

## Technologies Used

- C++17
- Standard Template Library (STL)
- Object-Oriented Programming (OOP)
- Graph Algorithms

## Project Structure

```text
.
├── include/
│   ├── Types.h
│   ├── Job.h
│   ├── Worker.h
│   ├── DependencyGraph.h
│   ├── Scheduler.h
│   ├── Logger.h
│   └── ExecutionEngine.h
├── src/
│   ├── Job.cpp
│   ├── Worker.cpp
│   ├── DependencyGraph.cpp
│   ├── Scheduler.cpp
│   ├── Logger.cpp
│   └── ExecutionEngine.cpp
├── main.cpp
├── .gitignore
├── LICENSE
└── README.md
```

## How It Works

1. Create jobs with specified durations, priorities, and dependencies.
2. Build the dependency graph (adjacency list).
3. Run cycle detection (Kahn's Algorithm).
4. If acyclic, push independent jobs (in-degree = 0) to the Priority Queue (Heap).
5. Dispatched jobs run on available Idle Workers, decrementing dependent in-degrees upon completion.
6. Execution continues until all jobs reach the `COMPLETED` state.

## Example (Diamond Dependency Simulation)

Suppose the dependencies are:[JobA (dur:2, pri:5)]
         /            \
 [JobB (dur:3, pri:10)]  [JobC (dur:1, pri:8)]
         \            /
      [JobD (dur:2, pri:15)]The scheduler executes:
- **JobA** runs first (0 dependencies).
- Once JobA completes, **JobB** and **JobC** are unlocked.
- **JobB** is scheduled before **JobC** due to its higher priority ($10 > 8$).
- **JobD** executes only after both JobB and JobC are completed.

## Algorithms Used

- Topological Sort (Kahn's BFS-based Algorithm)
- In-degree Node Tracking
- Priority Queue Scheduling (Heap Sort)

## Time Complexity

| Operation | Complexity |
|----------|------------|
| Add Job | $O(1)$ |
| Cycle Detection | $O(V + E)$ |
| Topological Sort | $O(V + E)$ |
| Priority Queue Push/Pop | $O(\log N)$ |

where:
- **V** = Number of jobs
- **E** = Number of dependencies
- **N** = Number of ready tasks in the queue

## Future Improvements

- Real-time preemption (pre-empting lower priority tasks when higher priority tasks unlock).
- Execution deadline management (canceling/failing tasks that miss their defined deadlines).
- GUI visualization of the dependency graph and worker timelines.
- File input/output support to load task maps from JSON or CSV files.

## Learning Outcomes

This project strengthened my understanding of:

- Directed Acyclic Graphs (DAGs)
- Topological Sorting (Kahn's Algorithm)
- Priority Queues and Heap Operations
- Object-Oriented Programming & Smart Pointer Memory Management in C++17
- Software Design Principles (separating interface from implementation)
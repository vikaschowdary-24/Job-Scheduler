# Single-Threaded Job Scheduler with Dependencies and Deadlines

This project is a single-threaded, tick-based job scheduler simulation written in C++. It manages and executes jobs with priority constraints, directed dependencies (using a Directed Acyclic Graph), and absolute deadlines. 

The scheduler runs entirely on a single thread using a simulated clock ("ticks") to model execution, avoiding the complexity and overhead of multithreading.

---

## Features

- **Single-Threaded Tick Simulation:** Simulates discrete time slices (ticks) to advance job execution.
- **Dependency Management:** Uses a Directed Acyclic Graph (DAG) and Kahn's Algorithm to detect circular dependencies (deadlocks) before the simulation starts.
- **Priority-Based Scheduling:** Selects ready jobs using a priority queue (max-priority based on job priority values).
- **Deadline Enforcement:** Evaluates active jobs against their absolute deadline tick.
- **Cascading Failures:** If a job misses its deadline, the engine automatically fails it, preempts the worker running it, and transitively marks all its dependent jobs as failed to prevent system hang-ups.

---

## File Structure

The project consists of the following components:

| Component | Files | Description |
| :--- | :--- | :--- |
| **Types** | `Types.h` | Defines states for jobs (`BLOCKED`, `READY`, `RUNNING`, `COMPLETED`, `FAILED`) and workers (`IDLE`, `BUSY`). |
| **Job** | `Job.h`, `Job.cpp` | Represents an individual task, tracking its ID, remaining execution duration, priority, deadline, and dependencies. |
| **Worker** | `Worker.h`, `Worker.cpp` | Models a processing unit that executes a job tick-by-tick. |
| **Dependency Graph** | `DependencyGraph.h`, `DependencyGraph.cpp` | Tracks in-degrees and adjacency lists of jobs. Performs cycle detection. |
| **Scheduler** | `Scheduler.h`, `Scheduler.cpp` | Manages the `READY` priority queue and updates the state of dependent jobs when their blockers complete. |
| **Execution Engine** | `ExecutionEngine.h`, `ExecutionEngine.cpp` | Drives the main simulation loop, processes worker execution, maps ready jobs to idle workers, and handles deadline checks. |
| **Logger** | `Logger.h`, `Logger.cpp` | Provides formatted outputs (`INFO`, `WARNING`, `ERROR`) to trace execution state. |

---

## How the Deadline Logic Works

A job's `deadline` represents the absolute tick number by which the job must have transitioned to the `COMPLETED` state. 

### Simulation Loop Sequence Per Tick:
1. **Step A (Process busy workers):** Active workers decrement the remaining duration of their assigned jobs. Jobs finishing on this step transition to `COMPLETED`, releasing the worker and notifying the scheduler to unblock dependent tasks.
2. **Step B (Assign jobs):** The scheduler assigns available `READY` jobs to `IDLE` workers. Newly assigned jobs transition to `RUNNING` but do not receive execution time until Step A of the *next* tick.
3. **Step C (Check deadlines):** For any job that is not `COMPLETED` or `FAILED`, if the current $\text{tick} \ge \text{deadline}$, the job is marked `FAILED`.

### Cascading Failure Resolution
If a job fails at Step C:
- If a worker is currently executing it, the worker is immediately interrupted and set to `IDLE`.
- The engine recursively traverses the dependency graph and marks all jobs depending on the failed job as `FAILED` (since their dependencies can now never be completed).
- Any failed jobs waiting in the scheduler's priority queue are discarded when popped.

---

## Compilation and Execution

To compile the project, place all `.h` and `.cpp` files in the same directory and compile using a C++11 compatible compiler (such as `g++`):

```bash
g++ -std=c++11 -Wall -O2 main.cpp Job.cpp Worker.cpp DependencyGraph.cpp Scheduler.cpp ExecutionEngine.cpp Logger.cpp -o scheduler_sim
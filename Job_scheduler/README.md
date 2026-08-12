# Deterministic Job Scheduler Simulation

This project is a tick-based simulation of a job scheduler and execution engine. It models how an operating system or a workflow engine manages task dependencies, priorities, resource limits (workers), and execution deadlines.

The scheduler utilizes a simulated virtual clock ("ticks") to model and control execution states deterministically.

---

## 1. Project Overview

The system simulates a set of compute **Workers** executing a Directed Acyclic Graph (DAG) of **Jobs**. The scheduler uses a virtual clock measured in **ticks** to step through execution, ensuring reproducible and predictable scheduling behavior.

### Key Capabilities
- **Dependency Resolution:** Jobs are held in a `BLOCKED` state until all their parent dependencies are `COMPLETED`.
- **Priority-Based Dispatching:** When multiple jobs are `READY`, the scheduler uses a max-priority queue to assign the highest-priority jobs first.
- **Resource Constraints:** The simulation restricts execution to a fixed number of workers. If all workers are `BUSY`, ready jobs remain in the queue.
- **Deadlock Prevention:** Pre-execution checks utilize Kahn's Algorithm to detect circular dependencies and abort the engine if a deadlock is present.
- **SLA/Deadline Tracking:** Jobs that do not finish before their specified deadline are failed, releasing their assigned worker and transitively failing any downstream dependent jobs (cascading failures).

---

## 2. Core Concepts & States

### Job States
A job transitions through the following lifecycle states during the simulation:
- **`BLOCKED`**: The job has unfinished dependencies and cannot run yet.
- **`READY`**: All dependencies are met. The job is in the scheduler's priority queue waiting for an idle worker.
- **`RUNNING`**: The job is assigned to a worker and is decrementing its remaining execution time tick-by-tick.
- **`COMPLETED`**: The job has successfully finished execution.
- **`FAILED`**: The job exceeded its absolute deadline and was terminated.

### Worker States
- **`IDLE`**: The worker has no assigned task and is ready to accept a job.
- **`BUSY`**: The worker is executing a job.

---

## 3. System Architecture & Components

The project is structured into modular components:

### A. Dependency Graph (`DependencyGraph`)
Represents the relationship between jobs as a directed graph.
- Calculates the "in-degree" (count of unmet dependencies) for each job.
- Builds an adjacency list to track downstream jobs.
- Implements Kahn's Algorithm to verify that the graph contains no cycles before processing begins.

### B. Scheduler (`Scheduler`)
Responsible for managing ready tasks.
- Maintains a max-priority queue using a custom comparator (`JobComparator`) based on the job's priority integer.
- Monitors when jobs complete and decrements the dependency counts of downstream jobs, transitioning them to `READY` when their in-degree reaches zero.

### C. Worker (`Worker`)
Acts as a virtual CPU core.
- Holds a reference to a single running job.
- Processes execution on each simulation tick, reducing the job's remaining run time.

### D. Execution Engine (`ExecutionEngine`)
Drives the core simulation loop. Each loop iteration represents one virtual time unit (Tick):
1. **Step A (Execution):** Ticks all busy workers. If a job's remaining time reaches 0, it transitions to `COMPLETED`, freeing the worker and updating the scheduler.
2. **Step B (Scheduling):** Identifies idle workers and assigns them the highest-priority jobs popped from the scheduler's ready queue.
3. **Step C (Monitoring):** Scans all non-completed jobs. If `tick >= job->getDeadline()`, the job fails. Its worker is released, and its downstream dependent jobs are recursively marked `FAILED`.

---

## 4. File Structure

```text
Job_scheduler/
├── include/
│   ├── DependencyGraph.h   # Graph definition and cycle detection
│   ├── ExecutionEngine.h   # Core simulation loop driver
│   ├── Job.h               # Job structure, states, and properties
│   ├── Logger.h            # Static logger helper
│   ├── Scheduler.h         # Priority queue and dependency tracker
│   ├── Types.h             # Shared Enums (JobState, WorkerState, LogLevel)
│   └── Worker.h            # Worker processing class
├── src/
│   ├── DependencyGraph.cpp
│   ├── ExecutionEngine.cpp
│   ├── Job.cpp
│   ├── Logger.cpp
│   ├── Scheduler.cpp
│   └── Worker.cpp
└── main.cpp    
```            

[INFO] Booting Execution Engine...
[INFO] --- Tick 1 ---
[INFO] Assigned Job [IngestData] to Worker 0
[INFO] --- Tick 2 ---
[INFO] Job [IngestData] completed.
[INFO] --- Tick 3 ---
[INFO] Assigned Job [ValidateData] to Worker 0
[INFO] --- Tick 4 ---
[INFO] Job [ValidateData] completed.
[INFO] --- Tick 5 ---
[INFO] Assigned Job [ProcessAnalytics] to Worker 0
[INFO] Assigned Job [GenerateReport] to Worker 1
[WARNING] Job [GenerateReport] failed (missed deadline).
[WARNING] Worker 1 was interrupted from failed Job [GenerateReport].
[WARNING] Cascaded failure: Job [SendNotification] marked FAILED because dependency [GenerateReport] failed.
[INFO] --- Tick 6 ---
[INFO] --- Tick 7 ---
[INFO] Job [ProcessAnalytics] completed.
[INFO] Simulation finished in 7 ticks.

========================================================
Final Simulation Job States:
========================================================
Job [IngestData] -> Status: COMPLETED
Job [ValidateData] -> Status: COMPLETED
Job [ProcessAnalytics] -> Status: COMPLETED
Job [GenerateReport] -> Status: FAILED (Deadline Missed)
Job [SendNotification] -> Status: FAILED (Deadline Missed)
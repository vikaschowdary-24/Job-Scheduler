#ifndef TYPES_H
#define TYPES_H

enum class JobState {
    BLOCKED,    // Waiting on dependencies
    READY,      // Dependencies met, waiting in priority queue
    RUNNING,    // Actively running on a worker
    COMPLETED,  // Finished execution successfully
    FAILED      // Missed its deadline
};

enum class WorkerState {
    IDLE,       // Worker is free to accept a job
    BUSY        // Worker is currently executing a job
};

#endif 
#ifndef WORKER_H
#define WORKER_H

#include "Types.h"
#include "Job.h"

#include <memory>

class Worker {
private:
    int workerId;
    WorkerState state;
    std::shared_ptr<Job> currentJob;

public:
    Worker(int id);

    // Getters
    int getWorkerId() const;
    WorkerState getState() const;
    std::shared_ptr<Job> getCurrentJob() const;

    // Core Simulation Methods
    void assignJob(std::shared_ptr<Job> job);
    bool tick();
    void release();
};

#endif
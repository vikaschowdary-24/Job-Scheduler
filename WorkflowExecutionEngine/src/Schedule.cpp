#include "Schedule.h"
#include <iostream>

Scheduler::Scheduler(int numWorkers, DependencyGraph& g, const std::vector<std::shared_ptr<Job>>& jobs)
    : graph(g) {
    
    // Spawn the requested number of workers
    for (int i = 0; i < numWorkers; ++i) {
        workers.push_back(Worker(i));
    }

    // Copy initial in-degrees from the graph to track during run
    inDegrees = graph.getInDegrees();

    // Queue up any jobs that start with 0 dependencies
    for (const auto& job : jobs) {
        if (inDegrees[job->getId()] == 0) {
            job->setState(JobState::READY);
            readyQueue.push(job);
        }
    }
}

void Scheduler::run(std::vector<std::shared_ptr<Job>>& jobs) {
    // Abort early if the graph contains a circular dependency
    if (graph.detectCycle()) {
        std::cerr << "Error: Deadlock detected in dependency graph. Aborting execution.\n";
        return;
    }

    while (true) {
        // Stop when all jobs in our master list reach COMPLETED
        bool allFinished = true;
        for (const auto& job : jobs) {
            if (job->getState() != JobState::COMPLETED) {
                allFinished = false;
                break;
            }
        }
        if (allFinished) {
            break;
        }

        // 1. Process busy workers
        for (auto& worker : workers) {
            if (worker.getState() == WorkerState::BUSY) {
                bool finished = worker.tick();
                if (finished) {
                    std::shared_ptr<Job> finishedJob = worker.getCurrentJob();
                    std::cout << "Job [" << finishedJob->getId() << "] completed.\n";

                    // Safely decrement in-degrees of dependent jobs
                    const auto& adjList = graph.getAdjList();
                    auto it = adjList.find(finishedJob->getId());
                    if (it != adjList.end()) {
                        for (const std::string& neighborId : it->second) {
                            inDegrees[neighborId]--;
                            if (inDegrees[neighborId] == 0) {
                                std::shared_ptr<Job> neighborJob = graph.getJob(neighborId);
                                if (neighborJob) {
                                    neighborJob->setState(JobState::READY);
                                    readyQueue.push(neighborJob);
                                }
                            }
                        }
                    }
                    worker.release();
                }
            }
        }

        // 2. Assign ready jobs to available idle workers
        for (auto& worker : workers) {
            if (worker.getState() == WorkerState::IDLE && !readyQueue.empty()) {
                std::shared_ptr<Job> nextJob = readyQueue.top();
                readyQueue.pop();

                worker.assignJob(nextJob);
                std::cout << "Assigned Job [" << nextJob->getId() << "] to Worker " 
                          << worker.getWorkerId() << "\n";
            }
        }
    }

    std::cout << "Simulation finished successfully.\n";
}
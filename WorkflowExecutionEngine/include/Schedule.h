#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Job.h"
#include "Worker.h"
#include "DependencyGraph.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <memory>

// Basic comparator sorting jobs solely by priority (higher priority first)
struct JobComparator {
    bool operator()(const std::shared_ptr<Job>& a, const std::shared_ptr<Job>& b) const {
        return a->getPriority() < b->getPriority(); 
    }
};

class Scheduler {
private:
    std::vector<Worker> workers;
    DependencyGraph& graph;
    
    // Priority queue to manage jobs currently ready to execute
    std::priority_queue<std::shared_ptr<Job>, std::vector<std::shared_ptr<Job>>, JobComparator> readyQueue;
    std::unordered_map<std::string, int> inDegrees;

public:
    Scheduler(int numWorkers, DependencyGraph& g, const std::vector<std::shared_ptr<Job>>& jobs);
    
    // Core simulation loop
    void run(std::vector<std::shared_ptr<Job>>& jobs);
};

#endif // SCHEDULER_H
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Job.h"
#include "DependencyGraph.h"
#include <queue>
#include <unordered_map>
#include <memory>
#include <vector>

struct JobComparator {
    bool operator()(const std::shared_ptr<Job>& a, const std::shared_ptr<Job>& b) const {
        return a->getPriority() < b->getPriority();
    }
};

class Scheduler {
private:
    DependencyGraph& graph;
    std::priority_queue<std::shared_ptr<Job>, std::vector<std::shared_ptr<Job>>, JobComparator> readyQueue;
    std::unordered_map<std::string, int> inDegrees;

public:
    Scheduler(DependencyGraph& g);

    // Check if there are any jobs ready to run
    bool hasReadyJobs() const;

    // Retrieve and remove the highest-priority ready job
    std::shared_ptr<Job> popNextReadyJob();

    // Decrements dependencies and moves newly freed jobs to the ready queue.
    void onJobCompleted(const std::string& jobId);
};

#endif // SCHEDULER_H
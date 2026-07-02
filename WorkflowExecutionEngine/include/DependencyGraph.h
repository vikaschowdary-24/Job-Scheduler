#ifndef DEPENDENCY_GRAPH_H
#define DEPENDENCY_GRAPH_H

#include "Job.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

class DependencyGraph {
private:
    // Adjacency list: tracks which jobs depend on a completed job
    std::unordered_map<std::string, std::vector<std::string>> adjList;

    // Tracks how many dependencies a job is waiting on
    std::unordered_map<std::string, int> inDegrees;

    // Registry of all job pointers
    std::unordered_map<std::string, std::shared_ptr<Job>> allJobs;

public:
    void addJob(std::shared_ptr<Job> job);
    bool detectCycle();

    // Getters
    const std::unordered_map<std::string, int>& getInDegrees() const;
    const std::unordered_map<std::string, std::vector<std::string>>& getAdjList() const;
    std::shared_ptr<Job> getJob(const std::string& id) const;
};

#endif // DEPENDENCY_GRAPH_H
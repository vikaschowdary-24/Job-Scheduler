#include "DependencyGraph.h"
#include <queue>

void DependencyGraph::addJob(std::shared_ptr<Job> job) {
    std::string jobId = job->getId();
    allJobs[jobId] = job;

    // Ensure the job exists in the inDegrees map
    if (inDegrees.find(jobId) == inDegrees.end()) {
        inDegrees[jobId] = 0;
    }

    inDegrees[jobId] = job->getDependencies().size();

    // Map each dependency to notify this job when completed
    for (const std::string& dep : job->getDependencies()) {
        adjList[dep].push_back(jobId);
    }
}

// Kahn's Algorithm for Topological Sort / Cycle Detection
bool DependencyGraph::detectCycle() {
    std::unordered_map<std::string, int> tempInDegrees = inDegrees;
    std::queue<std::string> readyQueue;

    // Load initial independent jobs (in-degree == 0)
    for (const auto& pair : tempInDegrees) {
        if (pair.second == 0) {
            readyQueue.push(pair.first);
        }
    }

    size_t processedCount = 0;

    while (!readyQueue.empty()) {
        std::string current = readyQueue.front();
        readyQueue.pop();
        processedCount++;

        // Decrement dependencies of neighbor nodes
        for (const std::string& neighbor : adjList[current]) {
            tempInDegrees[neighbor]--;
            if (tempInDegrees[neighbor] == 0) {
                readyQueue.push(neighbor);
            }
        }
    }

    // If processed count doesn't match total jobs, a circular dependency exists
    return processedCount != allJobs.size();
}

const std::unordered_map<std::string, int>& DependencyGraph::getInDegrees() const {
    return inDegrees;
}

const std::unordered_map<std::string, std::vector<std::string>>& DependencyGraph::getAdjList() const {
    return adjList;
}

std::shared_ptr<Job> DependencyGraph::getJob(const std::string& id) const {
    auto it = allJobs.find(id);
    if (it != allJobs.end()) {
        return it->second;
    }
    return nullptr; // Job not found
}
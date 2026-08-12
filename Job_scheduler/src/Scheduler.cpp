#include "Scheduler.h"

// Constructor: Initializes in-degrees and loads initial READY jobs
Scheduler::Scheduler(DependencyGraph& g) : graph(g) {
    // 1. Copy the initial in-degrees from the graph
    inDegrees = graph.getInDegrees();

    // 2. Loop through the in-degrees map to find jobs with 0 dependencies
    for (const auto& pair : inDegrees) {
        const std::string& jobId = pair.first;
        int degree = pair.second;

        if (degree == 0) {
            std::shared_ptr<Job> job = graph.getJob(jobId);
            if (job) {
                job->setState(JobState::READY);
                readyQueue.push(job);
            }
        }
    }
}

// Check if there are any jobs ready to run
bool Scheduler::hasReadyJobs() const {
    return !readyQueue.empty();
}

// Retrieve and remove the highest-priority ready job
std::shared_ptr<Job> Scheduler::popNextReadyJob() {
    while (!readyQueue.empty()) {
        std::shared_ptr<Job> nextJob = readyQueue.top();
        readyQueue.pop();
        
        // Only return the job if it is still in the READY state 
        // (discards jobs that failed due to deadlines while in the queue)
        if (nextJob && nextJob->getState() == JobState::READY) {
            return nextJob;
        }
    }
    return nullptr;
}

// Decrements dependencies and moves newly freed jobs to the ready queue
void Scheduler::onJobCompleted(const std::string& jobId) {
    // Retrieve dependent jobs from the graph's adjacency list
    const auto& adjList = graph.getAdjList();
    auto it = adjList.find(jobId);
    
    if (it != adjList.end()) {
        for (const std::string& dependentJobId : it->second) {
            // Decrement the in-degree for the dependent job in our local tracker
            inDegrees[dependentJobId]--;

            // If all dependencies are met, transition and queue the job
            if (inDegrees[dependentJobId] == 0) {
                std::shared_ptr<Job> readyJob = graph.getJob(dependentJobId);
                if (readyJob) {
                    readyJob->setState(JobState::READY);
                    readyQueue.push(readyJob);
                }
            }
        }
    }
}
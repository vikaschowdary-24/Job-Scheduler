#include "ExecutionEngine.h"
#include "DependencyGraph.h"
#include "Schedule.h"
#include "Logger.h"

ExecutionEngine::ExecutionEngine(int workers, const std::vector<std::shared_ptr<Job>>& jobList)
    : numWorkers(workers), jobs(jobList) {}

void ExecutionEngine::execute() {
    Logger::log(LogLevel::INFO, "Booting Execution Engine...");

    DependencyGraph graph;
    
    // Register all jobs in the dependency graph
    for (const auto& job : jobs) {
        graph.addJob(job);
    }

    // Initialize scheduler with our graph and run the simulation
    Scheduler scheduler(numWorkers, graph, jobs);
    scheduler.run(jobs);
}
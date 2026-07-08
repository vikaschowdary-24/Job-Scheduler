#include "ExecutionEngine.h"
#include "DependencyGraph.h"
#include "Scheduler.h"
#include "Logger.h"
#include "Worker.h"
#include <iostream>
#include <string>

ExecutionEngine::ExecutionEngine(int workers, const std::vector<std::shared_ptr<Job>>& jobList) 
    : numWorkers(workers), jobs(jobList) {}

void ExecutionEngine::execute() {
    Logger::log(LogLevel::INFO, "Booting Execution Engine...");

    // 1. Register all jobs in a local dependency graph
    DependencyGraph graph;
    for (const auto& job : jobs) {
        graph.addJob(job);
    }

    // 2. Perform cycle detection before starting the simulation
    if (graph.detectCycle()) {
        Logger::log(LogLevel::ERROR, "Deadlock detected in dependency graph. Aborting execution.");
        return;
    }

    // 3. Initialize the Scheduler
    Scheduler scheduler(graph);

    // 4. Initialize the local workers for this execution
    std::vector<Worker> workers;
    for (int i = 0; i < numWorkers; ++i) {
        workers.emplace_back(i);
    }

    int tick = 0;

    // 5. Main Simulation Loop
    while (true) {
        // Stop when all jobs in our master list reach COMPLETED or FAILED
        bool allFinished = true;
        for (const auto& job : jobs) {
            JobState state = job->getState();
            if (state != JobState::COMPLETED && state != JobState::FAILED) {
                allFinished = false;
                break;
            }
        }

        if (allFinished) {
            break;
        }

        tick++;
        Logger::log(LogLevel::INFO, "--- Tick " + std::to_string(tick) + " ---");

        // Step A: Process busy workers
        for (auto& worker : workers) {
            if (worker.getState() == WorkerState::BUSY) {
                bool finished = worker.tick();
                if (finished) {
                    std::shared_ptr<Job> finishedJob = worker.getCurrentJob();
                    Logger::log(LogLevel::INFO, "Job [" + finishedJob->getId() + "] completed.");

                    // Notify scheduler that the job is done to free up dependent jobs
                    scheduler.onJobCompleted(finishedJob->getId());

                    // Release the worker
                    worker.release();
                }
            }
        }

        // Step B: Assign ready jobs to available idle workers
        for (auto& worker : workers) {
            if (worker.getState() == WorkerState::IDLE && scheduler.hasReadyJobs()) {
                std::shared_ptr<Job> nextJob = scheduler.popNextReadyJob();
                if (nextJob) {
                    worker.assignJob(nextJob);
                    Logger::log(LogLevel::INFO, "Assigned Job [" + nextJob->getId() + "] to Worker " + std::to_string(worker.getWorkerId()));
                }
            }
        }
    }

    Logger::log(LogLevel::INFO, "Simulation finished successfully in " + std::to_string(tick) + " ticks.");
}
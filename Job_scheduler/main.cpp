#include "include/Job.h"
#include "include/ExecutionEngine.h"
#include <memory>
#include <vector>
#include <iostream>

int main() {
    std::cout << "====================================================\n";
    std::cout << "Starting Job Scheduler Simulation\n";
    std::cout << "====================================================\n\n";

    // Scenario: A data processing pipeline
    // We have 2 workers available.
    std::vector<std::shared_ptr<Job>> jobList = {
        // 1. Ingest Data: Needs 2 ticks. Deadline is tick 3.
        // Expected: Completes successfully at Tick 2.
        std::make_shared<Job>(
            "IngestData",   // ID
            2,              // Duration
            3,              // Deadline
            10,             // Priority
            std::vector<std::string>{} // No dependencies
        ),

        // 2. Parse Data: Needs 2 ticks. Depends on IngestData. Deadline is tick 6.
        // Expected: Starts at Tick 3, completes successfully at Tick 4.
        std::make_shared<Job>(
            "ParseData", 
            2, 
            6, 
            8, 
            std::vector<std::string>{"IngestData"}
        ),

        // 3. Analytics (High Priority): Needs 3 ticks. Depends on ParseData. Deadline is tick 8.
        // Expected: Starts at Tick 5, completes successfully at Tick 7.
        std::make_shared<Job>(
            "Analytics", 
            3, 
            8, 
            9, 
            std::vector<std::string>{"ParseData"}
        ),

        // 4. Generate Report (Tight Deadline): Needs 2 ticks. Depends on ParseData. Deadline is tick 5.
        // Expected: Starts at Tick 5. By the end of Tick 5, it still has 1 tick remaining.
        // Since current tick (5) >= deadline (5) and it is not completed, it will FAIL.
        std::make_shared<Job>(
            "GenerateReport", 
            2, 
            5, 
            1, 
            std::vector<std::string>{"ParseData"}
        ),

        // 5. Send Email Alert: Needs 1 tick. Depends on GenerateReport. Deadline is tick 10.
        // Expected: Since its prerequisite (GenerateReport) fails, this job will immediately 
        // fail via cascading failure at Tick 5.
        std::make_shared<Job>(
            "SendEmailAlert", 
            1, 
            10, 
            5, 
            std::vector<std::string>{"GenerateReport"}
        )
    };

    // Initialize the engine with 2 workers
    ExecutionEngine engine(2, jobList);
    
    // Run the simulation
    engine.execute();

    std::cout << "\n====================================================\n";
    std::cout << "Simulation Completed. Final Job States:\n";
    std::cout << "====================================================\n";
    for (const auto& job : jobList) {
        std::string stateStr;
        switch (job->getState()) {
            case JobState::BLOCKED:   stateStr = "BLOCKED"; break;
            case JobState::READY:     stateStr = "READY"; break;
            case JobState::RUNNING:   stateStr = "RUNNING"; break;
            case JobState::COMPLETED: stateStr = "COMPLETED"; break;
            case JobState::FAILED:    stateStr = "FAILED (Deadline Missed)"; break;
        }
        std::cout << "Job [" << job->getId() << "] -> " << stateStr << "\n";
    }

    return 0;
}
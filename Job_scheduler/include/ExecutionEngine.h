#ifndef EXECUTION_ENGINE_H
#define EXECUTION_ENGINE_H

#include "Job.h" // Ensure this case matches your actual job.h file
#include <vector>
#include <memory>

class ExecutionEngine {
private:
    int numWorkers;
    std::vector<std::shared_ptr<Job>> jobs;

public:
    ExecutionEngine(int workers, const std::vector<std::shared_ptr<Job>>& jobList);

    void execute();
};

#endif // EXECUTION_ENGINE_H
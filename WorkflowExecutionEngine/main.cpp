#include "Job.h"
#include "ExecutionEngine.h"
#include <vector>
#include <memory>

int main() {
    // Setup a standard "diamond" dependency structure:
    //
    //           [JobA (dur:2, pri:5)]
    //              /            \
    //      [JobB (dur:3, pri:10)]  [JobC (dur:1, pri:8)]
    //              \            /
    //           [JobD (dur:2, pri:15)]
    //
    // Expected execution order:
    // - JobA runs first (0 dependencies).
    // - Once JobA completes, both JobB and JobC unlock.
    // - Because JobB has higher priority (10 > 8), it should be scheduled first if workers are constrained.
    // - JobD runs only after both JobB and JobC are completed.

    std::vector<std::shared_ptr<Job>> jobs = {
        std::make_shared<Job>("JobA", 2, 10, 5, std::vector<std::string>{}),
        std::make_shared<Job>("JobB", 3, 15, 10, std::vector<std::string>{"JobA"}),
        std::make_shared<Job>("JobC", 1, 12, 8, std::vector<std::string>{"JobA"}),
        std::make_shared<Job>("JobD", 2, 20, 15, std::vector<std::string>{"JobB", "JobC"})
    };

    // Run the simulation using 2 parallel workers
    ExecutionEngine engine(2, jobs);
    engine.execute();

    return 0;
}
#ifndef JOB_H
#define JOB_H

#include "Types.h"
#include <string>
#include <vector>

class Job {
private:
    std::string id;
    int duration;
    int deadline;
    int priority;
    std::vector<std::string> dependencies;
    JobState state;
    int remainingTime;

public:
    Job(std::string id, int duration, int deadline, int priority, std::vector<std::string> dependencies);

    // Getters
    std::string getId() const;
    int getDuration() const;
    int getDeadline() const;
    int getPriority() const;
    std::vector<std::string> getDependencies() const;
    JobState getState() const;
    int getRemainingTime() const;

    // Setters
    void setState(JobState newState);
    bool decrementRemainingTime();
};

#endif // JOB_H
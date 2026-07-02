#include "Job.h"

// Constructor implementation
Job::Job(std::string id, int duration, int deadline, int priority, std::vector<std::string> dependencies)
    : id(id), duration(duration), deadline(deadline), priority(priority), 
      dependencies(dependencies), state(JobState::BLOCKED), remainingTime(duration) {}

std::string Job::getId() const {
    return id;
}

int Job::getDuration() const {
    return duration;
}

int Job::getDeadline() const {
    return deadline;
}

int Job::getPriority() const {
    return priority;
}

std::vector<std::string> Job::getDependencies() const {
    return dependencies;
}

JobState Job::getState() const {
    return state;
}

int Job::getRemainingTime() const {
    return remainingTime;
}

void Job::setState(JobState newState) {
    state = newState;
}

bool Job::decrementRemainingTime() {
    if (remainingTime > 0) {
        remainingTime--;
    }
    return remainingTime == 0;
}
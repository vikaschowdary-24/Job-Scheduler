#include "Worker.h"

// Constructor implementation
Worker::Worker(int id)
    : workerId(id), state(WorkerState::IDLE), currentJob(nullptr) {}

int Worker::getWorkerId() const {
    return workerId;
}

WorkerState Worker::getState() const {
    return state;
}

std::shared_ptr<Job> Worker::getCurrentJob() const {
    return currentJob;
}

void Worker::assignJob(std::shared_ptr<Job> job) {
    currentJob = job;
    state = WorkerState::BUSY;
    currentJob->setState(JobState::RUNNING);
}

bool Worker::tick() {
    // If the worker is busy, decrement remaining time of the active job
    if (state == WorkerState::BUSY && currentJob != nullptr) {
        if (currentJob->decrementRemainingTime()) {
            currentJob->setState(JobState::COMPLETED);
            return true; // Job completed during this tick
        }
    }

    return false;
}

void Worker::release() {
    currentJob = nullptr;
    state = WorkerState::IDLE;
}
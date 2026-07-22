#include <gtest/gtest.h>
#include "Worker.h"
#include "Job.h"

TEST(Worker, ConstructorInitialisesResourcesCorrectly) {

    Worker worker("worker1", 8, 2, 16000);
    EXPECT_EQ(worker.getWorkerID(), "worker1");
    EXPECT_EQ(worker.getAvailableCPUs(), 8);
    EXPECT_EQ(worker.getAvailableGPUs(), 2);
    EXPECT_EQ(worker.getAvailableMem(), 16000);
    EXPECT_EQ(worker.getState(), WorkerState::Idle);

}

TEST(Worker, AcceptsJobWithEnoughResources) {
    Worker worker("worker1", 8, 2, 16000);
    Job job("job1", 1, 4000, 2, "payload");
    bool result = worker.executeJob(job);
    EXPECT_TRUE(result);
    EXPECT_EQ(worker.getState(), WorkerState::Busy);
    EXPECT_EQ(worker.getCurrentJobID(), "job1");
    EXPECT_EQ(worker.getAvailableCPUs(), 6);
    EXPECT_EQ(worker.getAvailableGPUs(), 1);
    EXPECT_EQ(worker.getAvailableMem(), 12000);
}


TEST(Worker, RejectsJobWithInsufficientResources) {
    Worker worker("worker1", 4, 1, 8000);
    Job job("job1", 2, 10000, 8, "payload");
    bool result = worker.executeJob(job);
    EXPECT_FALSE(result);
    EXPECT_EQ(worker.getState(), WorkerState::Idle);
    EXPECT_EQ(worker.getCurrentJobID(), "");
    EXPECT_EQ(worker.getAvailableCPUs(), 4);
    EXPECT_EQ(worker.getAvailableGPUs(), 1);
}


TEST(Worker, CompletingJobReleasesResources) {

    Worker worker("worker1", 8, 2, 16000);
    Job job("job1", 1, 4000, 2, "payload");
    worker.executeJob(job);
    bool result = worker.completeJob(job);
    EXPECT_TRUE(result);
    EXPECT_EQ(worker.getState(), WorkerState::Idle);
    EXPECT_EQ(worker.getCurrentJobID(), "");
    EXPECT_EQ(worker.getAvailableCPUs(), 8);
    EXPECT_EQ(worker.getAvailableGPUs(), 2);
    EXPECT_EQ(worker.getAvailableMem(), 16000);

}


TEST(Worker, CannotCompleteUnknownJob) {

    Worker worker("worker1", 8, 2, 16000);
    Job job("job1", 1, 4000, 2, "payload");
    bool result = worker.completeJob(job);
    EXPECT_FALSE(result);
}
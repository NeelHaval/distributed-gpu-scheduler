#include <gtest/gtest.h>
#include "Job.h"

TEST(Job, ConstructorAssignsResourcesCorrectly) {

    Job job("Job1", 2, 4096, 4, "payload");
    EXPECT_EQ(job.getJobID(), "Job1");
    EXPECT_EQ(job.getRequiredGPUs(), 2);
    EXPECT_EQ(job.getRequiredMem(), 4096);
    EXPECT_EQ(job.getRequiredCPUs(), 4);

}

TEST(Job, UpdateStateChangesState) {

    Job job("job1",2,4096,4,"payload");
    job.updateState(JobState::Running);
    EXPECT_EQ(job.getState(), JobState::Running);

}

TEST(Job, MarkCompletedSetsCompletedState) {
    Job job("job1",2,4096,4,"payload");
    job.markCompleted();
    EXPECT_EQ(job.getState(), JobState::Completed);
}

TEST(Job, MarkJobFailed) {

    Job job("job1",2,4096,4,"payload");
    job.markFailed();
    EXPECT_EQ(job.getState(), JobState::Failed);

}

TEST(Job, AssignWorkerStoresWorkerID) {
    Job job("job1",2,4096,4,"payload");
    job.assignWorker("worker3");
    EXPECT_EQ(job.getWorkerID(), "worker3");
}

TEST(Job, RecoverJobAfterSerializationDeserialization) {

    Job job("job1",2,4096,4,"payload");
    std::string serializedJob = job.serialize();
    Job restored = Job::deserialize(serializedJob);
    EXPECT_EQ(restored.getJobID(), "job1");
    EXPECT_EQ(restored.getRequiredGPUs(), 2);
    EXPECT_EQ(restored.getRequiredMem(), 4096);
    EXPECT_EQ(restored.getRequiredCPUs(), 4);

}

TEST(Job, MultipleJobsRemainIndependent) {
    Job first("job1",1,1000,2,"A");
    Job second("job2",4,8000,8,"B");
    EXPECT_EQ(first.getJobID(), "job1");
    EXPECT_EQ(second.getJobID(), "job2");
    EXPECT_NE(first.getJobID(), second.getJobID());
}

TEST(Job, MultipleStateTransitions) {

    Job job("job1",2,4096,4,"payload");
    job.updateState(JobState::Queued);
    EXPECT_EQ(job.getState(), JobState::Queued);
    job.updateState(JobState::Running);
    EXPECT_EQ(job.getState(), JobState::Running);
    job.markCompleted();
    EXPECT_EQ(job.getState(), JobState::Completed);

}
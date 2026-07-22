#include <gtest/gtest.h>
#include "Scheduler.h"
#include "Worker.h"
#include "Job.h"



TEST(Scheduler, InitiallyHasNoWorkers) {

    Scheduler scheduler;
    EXPECT_EQ(scheduler.workerCount(),0);

}


TEST(Scheduler, RegistersWorker) {

    Scheduler scheduler;
    Worker worker("worker1",8,2,16000);
    scheduler.registerWorker(worker);
    EXPECT_EQ(scheduler.workerCount(),1);

}



TEST(Scheduler, DoesNotRegisterDuplicateWorkers) {

    Scheduler scheduler;
    Worker worker1("worker1",8,2,16000);
    scheduler.registerWorker(worker1);
    scheduler.registerWorker(worker1);
    EXPECT_EQ(scheduler.workerCount(),1);

}



TEST(Scheduler, SubmitJobAddsToQueue) {

    Scheduler scheduler;
    Job job("job1",1,4000,2,"payload");
    scheduler.submitJob(job);
    EXPECT_EQ(scheduler.queueSize(),1);

}



TEST(Scheduler, SchedulesJobToAvailableWorker) {

    Scheduler scheduler;
    Worker worker("worker1",8,2,16000);
    scheduler.registerWorker(worker);
    Job job("job1",1,4000,2,"payload");
    scheduler.submitJob(job);
    scheduler.schedule();
    EXPECT_EQ(scheduler.queueSize(),0);
    const Job* running = scheduler.getRunningJob("job1");
    ASSERT_NE(running, nullptr);
    EXPECT_EQ(running->getWorkerID(), "worker1");

}



TEST(Scheduler, LeavesJobQueuedIfNoWorkerAvailable) {

    Scheduler scheduler;
    Job job("job1",1,4000,2,"payload");
    scheduler.submitJob(job);
    scheduler.schedule();
    EXPECT_EQ(scheduler.queueSize(),1);

}



TEST(Scheduler, CannotScheduleJobWithoutResources) {

    Scheduler scheduler;
    Worker worker("worker1",2,0,1000);
    scheduler.registerWorker(worker);
    Job job("job1",1,4000,8,"payload");
    scheduler.submitJob(job);
    scheduler.schedule();
    EXPECT_EQ(scheduler.queueSize(),1);

}
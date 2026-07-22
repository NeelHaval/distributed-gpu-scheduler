# Engineering Decisions

## 23/06/2026

- Initial architecture created

## 15/07/2026

- For time measurements steady_clock was used instead of system_clock because steady_clock
avoids jumping between times whereas system_clock may go backwards resulting in
mismatches between the time algorithms expect and that which they recieve.

- Delimited String was selected as the initial choice of serialization and
deserialization as it is simple and dependency free. This makes debugging
quicker. Later date, different methods may be considered if they allow for higher
performance.

## 18/07/2026

- Workers do not own Job objects. The scheduler maintains job ownership and workers maintain execution state and resource availability.

## 19/07/2026

### Why does the Scheduler copy submitted jobs?

When a client submits a job, the scheduler creates and stores its own copy rather than modifying the client's original object.

**Reasons:**
- **Clear ownership:** Once submitted, the scheduler owns the job lifecycle.
- **Avoid shared mutable state:** The client and scheduler should not both modify the same `Job` instance, as this can lead to inconsistent state and bugs.
- **Closer to a real distributed system:** In later phases, jobs will be transmitted over the network, meaning the scheduler naturally receives a serialized copy rather than the original object.

This allows the scheduler to safely update the job's state (e.g., `Queued`, `Running`, `Completed`) without affecting the client's original copy.

## Resource Allocation and Future Thread Safety

### Current Phase 1 Approach

Currently, resource allocation is handled using a defensive two-stage validation process:

1. The Scheduler searches for a Worker with sufficient available resources.
2. The Worker performs its own resource check before accepting the job.
3. Only after validation does the Worker reserve the required resources.

This prevents invalid allocations caused by stale scheduler information and ensures that the Worker remains the final authority over its own resource state.

However, this approach does **not completely prevent race conditions** in a multi-threaded environment because the resource check and resource assignment are separate operations.

Example:

- Thread 1:
Check available CPUs -> enough resources

- Context switch

- Thread 2:
Allocate resources

- Context switch

- Thread 1:
Allocate resources using outdated information


This can lead to inconsistent resource tracking.

---

### Future Improvement: Mutex-Based Resource Reservation

When the scheduler becomes multi-threaded, resource allocation should be made atomic using a mutex.

A **mutex** (mutual exclusion lock) is a synchronisation mechanism that allows only one thread to access a protected section of code at a time.

For resource allocation, the Worker would:

1. Acquire the mutex lock.
2. Check available resources.
3. Reserve resources if available.
4. Release the mutex lock.

Example:

- Acquire lock

- Check resources

- Assign resources

- Release lock


This ensures that no other thread can modify the Worker resources between the check and assignment operations.

The future goal is to replace the current:

- checkResources()
- assignResources()

pattern with an atomic:

- reserveResources()


operation protected by a mutex.

This provides safe concurrent scheduling while preserving the current separation of responsibilities:
- Scheduler decides job placement
- Worker validates and protects its own resources

## 21/07/2026

### Decision: Adopt GoogleTest for unit testing

Context:<br>
The scheduler consists of multiple C++ components (`Job`, `Worker`, and `Scheduler`) which will become increasingly complex as networking and fault tolerance are introduced.

Decision:<br>
GoogleTest was selected as the unit testing framework.

Reasons:<br>
- Native C++ support
- Industry-standard framework
- Integrates with CMake and CTest
- Provides a foundation for future mocking and integration testing

### Consequences

**Positive:**
- Automated regression testing
- Consistent testing workflow
- Easy integration into future CI/CD pipelines

**Negative:**
- Adds an external dependency

---

### Decision: Test components independently before distributed integration

Context:<br>
The system will eventually include distributed behaviour such as networking, worker failures, and job recovery. These features introduce more complex failure modes.

Initial testing focuses on deterministic unit tests for individual components.

Current tests validate:
- Job state transitions
- Worker resource allocation
- Scheduler job assignment

Integration and fault-injection testing will be introduced as distributed features are implemented.

### Consequences

**Positive:**
- Faster debugging
- Reliable regression testing
- Provides a stable foundation before introducing distributed complexity

**Negative:**
- Current tests do not validate network communication or multi-node behaviour

## 22/07/2026

Modified scheduler tests to verify that it keeps and modifies only its copy
of a running job. This means that the original job object submitted by the client 
remains unmodified.

## Ongoing decisions:

- C++ networking library?
- HTTP vs gRPC?
- Threading model?
- Persistence strategy?
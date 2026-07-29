# Architecture

## Goal:

Build a distributed GPU scheduler which assigns jobs to worker nodes based on
available resources.

## Components:

### Scheduler:

Responsibilities:

- Recieve jobs
- Track workers
- Allocate resources
- Assign jobs
- Detect worker failures

### Worker:

Responsibilities:

- Register with scheduler
- Excecute jobs
- Report status
- Send hearbeats

### Client:

Responsibilities:

- Submit jobs
- Query status

## Resources:

Each worker reports:

- CPU core
- Memory
- GPU's

## Job lifecycle:

- Submission
- Scheduling
- Execution
- Completion

## Future features:

- Priority scheduling
- Fair scheduling
- GPU topology awareness
- NUMA awareness

### Future serialization methods:
- Protocol Buffers (Google Protobuf)
- FlatBuffers
- Cap'n Proto

## Architecture Diagram

```mermaid
graph TD
    C[Client] -->|submit job| S[Scheduler]

    S -->|assign job| W1[Worker 1]
    S -->|assign job| W2[Worker 2]

    W1 -->|heartbeat| S
    W2 -->|heartbeat| S

    S -->|track status| DB[(Job / Worker State)]
```

## Job States

# Job State Definitions

A Job moves through a lifecycle controlled by the Scheduler. 
Phase 1 only requires: `Submitted → Queued → Running → Completed`.

| State | Meaning |
|---|---|
| **Submitted** | Job has been created by the client and received by the scheduler. |
| **Queued** | Job is waiting in the scheduler queue for available resources. |
| **Scheduling** | Scheduler is searching for a suitable worker. |
| **Assigned** | Scheduler has selected a worker for the job. |
| **Dispatching** | Job information is being sent to the selected worker. |
| **Running** | Worker has accepted the job and execution is in progress. |
| **Retrying** | Job failed temporarily and is being rescheduled. |
| **Failed** | Job cannot complete successfully after failure handling. |
| **Completed** | Job finished execution successfully. |

# Initialising new socket to allow TCP communication

```mermaid
sequenceDiagram
    participant App as Worker Application
    participant Client as Client Object
    participant Socket as Socket Object
    participant WinSock as Winsock API
    participant Kernel as Windows Networking Stack

    App->>Client: Create Client object
    Client->>Socket: Construct internal Socket object
    Socket->>Socket: Set socketFD = -1

    Note over Socket: Socket object exists,<br/>but no OS socket exists yet

    App->>Client: connect("127.0.0.1", 5000)
    Client->>Socket: connect(ip, port)

    Socket->>WinSock: socket(AF_INET, SOCK_STREAM, 0)
    WinSock->>Kernel: Request TCP socket resource
    Kernel-->>WinSock: Create socket handle
    WinSock-->>Socket: Return socketFD

    Socket->>Socket: Store handle in socketFD

    Socket->>WinSock: connect(socketFD, serverAddress)
    WinSock->>Kernel: Establish TCP connection

    Note over Kernel: TCP handshake occurs<br/>(SYN, SYN-ACK, ACK)

    Kernel-->>WinSock: Connection established
    WinSock-->>Socket: Success

    Note over Socket: Socket now owns a valid<br/>TCP connection

    Socket->>WinSock: send(data)
    WinSock->>Kernel: Transmit TCP packets

    Kernel-->>WinSock: Deliver data to destination

    WinSock-->>Socket: receive(data)

    Note over Socket: Socket can now exchange<br/>data with scheduler server

```
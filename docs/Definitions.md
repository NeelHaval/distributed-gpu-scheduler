# Definitions

## Worker:

A worker is a compute node in the cluster. It advertises its available resources (CPU, GPU, memory) to the scheduler, receives jobs to execute, periodically reports its health via heartbeats, and returns job completion or failure information. The scheduler treats workers as interchangeable execution nodes and makes placement decisions based on their available resources.
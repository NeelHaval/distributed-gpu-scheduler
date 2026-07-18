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

Ongoing decisions:

- C++ networking library?
- HTTP vs gRPC?
- Threading model?
- Persistence strategy?
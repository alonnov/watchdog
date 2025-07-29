# Watchdog Project

A C implementation of a watchdog system for process monitoring with internal task scheduling for IPC (Inter-Process Communication).

## Overview

This project implements a watchdog system that monitors application health and can restart applications if they become unresponsive. The watchdog uses an internal task scheduler for managing IPC operations and timeout checks.

**Main Components:**
- **Watchdog**: The primary system for monitoring and restarting applications
- **Task Scheduler**: Internal component used for managing IPC operations and timeout scheduling

## Project Structure

```
wd/
├── include/          # Header files
│   ├── dvector.h         # Dynamic vector implementation
│   ├── heap.h            # Heap data structure
│   ├── priority_queue.h  # Priority queue implementation
│   ├── scheduler.h       # Task scheduler API
│   ├── task.h            # Task management
│   ├── uid.h             # Unique identifier system
│   ├── wd.h              # Watchdog API
│   └── wdlib.h           # Internal watchdog library
└── src/              # Source files
    ├── dvector.c         # Dynamic vector implementation
    ├── heap.c            # Heap operations
    ├── priority_queue.c  # Priority queue operations
    ├── scheduler.c       # Scheduler implementation
    ├── task.c            # Task management
    ├── uid.c             # UID generation and comparison
    ├── wd_main.c         # Watchdog main process
    ├── wd.c              # Watchdog implementation
    └── wdlib.c           # Internal watchdog functions
```

## Features

### Watchdog System
- **Process Monitoring**: Monitors application health and responsiveness
- **Automatic Restart**: Restarts applications that become unresponsive
- **Configurable Timeouts**: Adjustable monitoring intervals and thresholds
- **Graceful Shutdown**: Proper cleanup when stopping the watchdog
- **IPC Management**: Internal task scheduler for managing inter-process communication

## API Documentation

### Watchdog API

#### `WDStart(size_t interval, size_t threshold, int argc, char* argv[])`
Starts the watchdog for the calling process.
- **Parameters**:
  - `interval` - timeout check interval in seconds (must be > 1)
  - `threshold` - number of failed checks before action
  - `argc` - number of command line arguments
  - `argv` - array of command line arguments
- **Returns**: `SUCCESS` on success, `FAILURE` on failure
- **Time Complexity**: O(1)
- **Space Complexity**: O(1)

#### `WDStop(void)`
Stops the active watchdog.
- **Time Complexity**: O(1)
- **Space Complexity**: O(1)


## Building the Project

### Prerequisites
- GCC compiler
- Make utility
- Standard C libraries

### Compilation
```bash
# Compile all source files into a single executable
gcc -o watchdog.out src/*.c -I./include

# Or compile individual components
gcc -c src/*.c -I./include
gcc -o watchdog.out *.o
```

## Usage Examples

### Watchdog Example
```c
#include "wd.h"

int main(int argc, char* argv[]) {
    // Start watchdog with 2-second intervals and 3-timeout threshold
    WDStart(2, 3, argc, argv);
    
    // Your application code here
    
    // Stop the watchdog
    WDStop();
    
    return 0;
}
```

## Data Structures

### Watchdog System
- **Process Monitoring**: Tracks application health and responsiveness
- **IPC Management**: Uses internal scheduler for timeout and communication tasks
- **Restart Mechanism**: Handles application restart when unresponsive

### Internal Priority Queue (Heap-based)
- **Implementation**: Binary heap with customizable comparison function
- **Operations**: Enqueue, Dequeue, Remove by criteria
- **Time Complexity**: O(log n) for enqueue/dequeue, O(n) for remove
- **Usage**: Internal component for managing IPC tasks and timeout scheduling

## Error Handling

The project includes comprehensive error handling:
- **Memory Allocation**: Checks for malloc failures
- **Invalid Parameters**: Uses assertions for parameter validation
- **Task Execution**: Handles operation function failures
- **Watchdog Timeouts**: Graceful handling of process monitoring failures

## Performance Characteristics

- **Watchdog Operations**: O(1) for start/stop operations
- **IPC Management**: Internal scheduler operations are O(log n) or O(1)
- **Memory Usage**: Efficient memory management with proper cleanup
- **Concurrent Access**: Not thread-safe (single-threaded design)
- **Scalability**: Suitable for monitoring single applications with internal task management

## Testing

The project includes comprehensive error handling and can be tested by:
- Running the watchdog main process (`wd_main.c`)
- Testing individual components through the API
- Creating test applications that use the watchdog functionality

## License

This project was created as an educational exercise by Alon Nov.

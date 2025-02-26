#Daniel Frederick - Project 4 - Dining Philosophers Problem

Adapted from Dijkstra's Solution in c++20
https://en.wikipedia.org/wiki/Dining_philosophers_problem

Philosophers either acquire both forks simultaneously or wait, ensuring that no philosopher holds only one fork outside of their critical section. Each philosopher functions as an individual thread and moves cyclically between the states of thinking, being hungry, and eating. A shared mutex protects the critical sections to ensure that state transitions occur without interruption. Additionally, binary semaphores are used as signaling mechanisms to coordinate access to the forks. This design guarantees that a philosopher can only begin eating when neither of their adjacent philosophers is eating, thereby maintaining mutual exclusion and preventing starvation.

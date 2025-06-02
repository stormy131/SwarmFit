# PSOptim

### Project description
This project aims to develop a minimalistic C++ library for "function optimization" problem (finding 
the global minimum of a user-provided objective function). The library is intended for researchers, engineers, and 
developers solving complex optimization problems.

The library is designed to efficiently find the global minimum of a user-defined objective function. It works on 
arbitrary bounded dimensions, making it adaptable to a wide range of optimization problems. For solving such task, 
the proposed solution employs Evolutionary Algorithms, namely Particle Swarm Optimization (PSO) as its 
primary method for solving stated optimization problem. Efficiency is a core feature of the library, achieved through parallelized 
computations for updating the state of particles, enabling faster performance on large-scale problems. Additionally, 
users can configure key algorithm parameters and stopping criteria. Hence, the optimization process is implemented 
in a way, that lets user configure it to their specific needs.

### Particle Swarm Optimization
[Particle Swarm Optimization (PSO)](https://en.wikipedia.org/wiki/Particle_swarm_optimization#) is a population-based
optimization technique inspired by the social behavior of birds and fish. It explores the search space using a swarm 
of particles, each representing a potential solution. Particles adjust their positions based on their own experience 
and the collective knowledge of the swarm to converge towards the optimal solution. PSO is widely used for solving 
non-linear, multidimensional, and computationally expensive optimization problems.

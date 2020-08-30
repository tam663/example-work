# Example Work Portfolio - Tom McIver

This repository contains the following files:


## C++
### 1) Ising Crystal Model

- This folder contains the full implementation and analysis of a Monte Carlo Markov Chain simulation of a ferromagnetic crystal. The simulation was primarily handled using a "Crystal" class which contained 
functions to measure and keep track of the physical parameters of interest, and these can be viewed in the files [Crystal.cpp](/c++/Ising-Crystal-Model/Crystal.cpp) and [Crystal.hpp](/c++/Ising-Crystal-Model/Crystal.hpp).
The four other files contain the main() functions for investigating specific properties of the simulated crystal under different conditions. The 
[Autocorrelation.cpp](/c++/Ising-Crystal-Model/Autocorrelation.cpp) and [RunTimes.cpp](/c++/Ising-Crystal-Model/RunTimes.cpp) files record properties of the simulation efficiency, while [TemperatureDependence.cpp](/c++/Ising-Crystal-Model/RunTimes.cpp) and 
[Hysteresis.cpp](/c++/Ising-Crystal-Model/Hysteresis.cpp) record the simulated physical behaviour of the lattice with respect to temperature and cycling conditions.
- A number of optimisations of the implementation were necessary for these results to be produced. In particular _**use of the 'futures' template**_
class in the main() function of TemperatureDependence.cpp, Runtimes.cpp and Autocorrelation.cpp produce order of **magnitude speed-ups in 
the run time** for the simulation. Other significant speed-ups were achieved using various **vector measuring techniques** which allowed for the 
recording of the simulated physical properties to become an O(1) operation rather than O(n2).

### 2) Monte Carlo Simulation of the Ising Model pdf
- This is the [full report](/c++/Monte-Carlo-Simulation-of-the-Ising-Model.pdf) for this project. Section 2 contains analysis the implementation and optimisation of the program, while 
section 3 deals with the physical results of the simulation. In short, speed-ups of approximately 95% were achieved in the 
implementation of the simulation by using methods of parallelisation and vectorised measuring. This allowed for larger
lattices to be simulated, and thus more complex physical behaviours to be studied.
- In terms of the physics studied, the expected phase change behaviour was sucessfully observed, which can be immediately
seen in the [plots](/c++/Figures) of magnetisation, energy and heat capacity. Second order behaviour was also sucessfully simulated, and in 
particular, hysteresis behaviour was observed.

### 3) Figures
- This [folder](/c++/Figures) contains figures which are not contained within the pdf write-up. In particular, there are a number of more specific plots that show
the error and measurements for the lattice under secificed conditions that may illuminate the results as outline in the report.


       

# Example Work Portfolio - Tom McIver

This repository contains two folders, one containing Python projects and the other C++. 


### Python :

1) Machine Learning for Fantasy Football: 
This project utilises data from the API of 'Premier League Fantasy Football,' an online game in which players have to try and 
predict the results of football matches (see full rules [here](https://fantasy.premierleague.com/help/rules)). Football is notoriously
difficult to predict sport, which is in part a cause of its huge popularity. Many avid supports claim that they can predict the outcomes
of matches however, and there are [millions of players](https://www.wired.co.uk/article/fantasy-premier-league) of the premier league fantasy football, and even more for the equivalent 
[american football versions](https://www.washingtonpost.com/news/made-by-history/wp/2017/09/10/the-dark-side-of-fantasy-football/). But is there any 
real behviour that is predictable in football? In this project the statistics of football matches are analysed and predictive models 
are produced using machine learning techniques.
   
   In brief, it was found that football matches can been sucessfully modelled using poisson distributions, and that the preformance 
of individual players is predictable. The folder [Statistical Season Analysis](/Python/Machine-Learning-for-Fantasy-Football/Statistical-Season-Analysis) contains the modelling of matches using skellam 
distributions, and the plots of the produced probability distributions can be found in the [Jupyter Notebook](/Python/Machine-Learning-for-Fantasy-Football/Statistical-Season-Analysis/Predicting-Second-Season.ipynb). 

   In the 'Fantasy Football
Predictions' Jupyter Notebook the analysis of the footballer performance data from the Premier League Fantasy Football API can be
found. Here, two predictive models are also built-- one is a performance 'direction' indicator (will a  footballer perform better or worse than
the previous week given last week's performance) and a regression model to predict the footballer's points in the next match given 
this weeks performance.
   Both models built in this project use a recurrent network structure with LTSM units being used. The direction model was able to achieve
accuracies of 97% while the root mean square error of the regression model was approximately 4 points, which corresponds to an 
average percentage error of 30%. Although this is a reasonably high error figure, given the random nature of football it is not so bad. 
Addionally, even with this large value, modelling the predictions as gaussian distributed could produce a quite useful model, and this 
represents the next steps to be taken in the project.


2) Galaxy Orbits Simulation:
This folder contains the full write-up and program of a project to investigate the formation of galactic structures. In particular, the formation
of galaxy tails and spirals were sucessfully reproduced. The project was implemented in python, and a Runge-Kutta integrator class was 
written to carry out the required computations.


### C++ :
1) Ising Crystal Model
This folder contains the full implementation of the model. The simulation was primarily handled using a "Crystal" class which contained 
functions to measure and keep track of the physical parameters of interest, and these can be viewed in the files Cyrstal.hpp and Crystal.hpp.
The four other files contain the main() functions for investigating specific properties of the simulated crystal under different conditions. The 
Autocorrelation.cpp and RunTimes.cpp files record properties of the simulation efficiency, while TemperatureDependence.cpp and 
Hysteresis.cpp record the simulated physical behaviour of the lattice with respect to temperature and cycling conditions.
A number of optimisations of the implementation were necessary for these results to be produced. In particular _**use of the 'futures' template**_
class in the main() function of TemperatureDependence.cpp, Runtimes.cpp and Autocorrelation.cpp produce order of magnitude speed-ups in 
the run time for the simulation. Other significant speed-ups were achieved using various vector measuring techniques which allowed for the 
recording of the simulated physical properties to become an O(1) operation rather than )(n2).
2) Monte Carlo Simulation of the Ising Model pdf
This is the full report for this project. Section 2 contains various results regarding the implementation and optimisation of the program, while 
section 3 deals with the physical results of the simulation.
3) Figures
This folder contains figures which are not contained within the pdf write-up. In particular, there are a number of more specific plots that show
the error and measurements for the lattice under secificed conditions that may illuminate the results as outline in the report.
       

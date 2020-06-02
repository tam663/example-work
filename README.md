This repository contains examples of python and c++ work. The Python folder contains:

1) Fantasy football machine learning: 
This project uses the API from the website for the Premier League Fantasy Football league, which
provides weekly performance data on players in the Premier League. Using this data, in 
combination with historic performance data from previous seasons, a predictive model was produced
using Deep Learning techniques which was able to achieve accuracies in the range of 98%. The 
scripts 'new_data_loader', 'Normalise_new_data', 'data_sequencer', 'data_scaling' and 
'data_processor' are used to preprocess the input data and historic data. The output from these
 scripts is then used to train the predictive model using 'neural_network.' 'Prediction_evaluator,'
'Return_predictions' and 'Save_predictions' are then used to apply the model to newly received data
and save the predictions in csv form. Pandas and TensorFlow modules are used extensively here, in 
addition to Numpy. The ipython notebook in this folder walks through the work found in the .py files, and demonstrates the work
most clearly.
   Additionally, the folder 'Statistical Analysis of Season' contains statistical analysis of the performance of teams during one
football season, and prediction of the performance next year based on this, including computation of the associated
probability distributions.


2) Galaxy Orbits Simulation:
This folder contains a project which investigated the importance of gravity in the longterm evolution
of galactic structures. In particular, Numpy was used to build a python class model of the gravitational
interaction between masses in which Runge-Kutta methods were applied. A script of the final programme is
included in addition to the project report and an animation produced by the script.


The C++ folder contains:
1) The complete program and write-up of a research project to investigate the simulation of ferromagnetic properties 
    using a Monte Carlo Markov algorithm. In this project a model of a crystal was implemented using a C++ class, and 
    this can be found in the files 'Crystal.cpp' and 'Crystal.hpp'. The four other scripts are programs to record the behaviour 
    of the simulated lattice under various constraints. Specifically, the 'Temperature_dependence.cpp' file and 'Hysteresis.cpp' 
    file are for recorded the physical bahviour simulated, while 'Autocorrelation.cpp' and 'Runtimes.cpp' are for analysis of
    the implementation.
       Various packages from the C++ standard libray are used in this project, for example the use of the 'random' and 'future'
    packages were of particular importance for the program to give accurate results and prackical run times. Specifically, using
    random meant that the cruical Monte Carlo step of the implemented aglgorithm was truly random, while using futures to 
    aysncronously run the simulation gave order of magnitude speed-ups of the run time.
    In the pdf for the project and in the figures folder the results of this porject are presented. In brief, the behaviour of ferromagnets 
    was sucessfully reproduced in all crucial aspects, including the expected phase change behaviour and hysteresis behaviour. The 
    simulated bahviour was also found to converge to the analytical results as the size of the simulated lattice was increased as 
    expected. Further analysis of the results and discussion work which can be carried out using this work as a starting point
    can be found in the accompanying pdf write-up.
       

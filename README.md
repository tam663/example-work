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
1) Complete program including all the main() files used to generate simulation results and the .cpp and header files
 in which the crystal model was implemented
 
 2) The report for the simulated model including an analysis of the implemeted algorithm and plots of the 
 simulation results.

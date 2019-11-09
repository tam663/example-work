This repository contains the following projects:

1) Fantasy football machine learning: 
This project uses the API from the website for the Premier League Fantasy Football league, which
provides weekly performance data on players in the Premier League. Using this data, in 
combination with historic performance data from previous seasons, a predictive model was produced
using Deep Learning techniques which was able to achieve accuracies in the range of 80%. The 
scripts 'new_data_loader', 'Normalise_new_data', 'data_sequencer', 'data_scaling' and 
'data_processor' are used to preprocess the input data and historic data. The output from these
 scripts is then used to train the predictive model using 'neural_network.' 'Prediction_evaluator,'
'Return_predictions' and 'Save_predictions' are then used to apply the model to newly received data
and save the predictions in csv form. Pandas and TensorFlow modules are used extensively here, in 
addition to Numpy.


2) Orbits:
This folder contains a project which investigated the importance of gravity in the longterm evolution
of galactic structures. In particular, Numpy was used to build a python class model of the gravitational
interaction between masses in which Runge-Kutta methods were applied. A script of the final programme is
included in addition to the project report and an animation produced by the script.

3) Neural Network:
This folder contains a script in which a neural network is implemented using only Numpy. The resulting
network was able to achieve recognition accuracies of handwritten digits of approximately 90%. The aim
of this project was not to produce a practical implementation of a neural network but rather to develop
a more comprehensive understanding of the underlying mathematics of Deep Learning techniques.

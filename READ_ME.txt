This repository contains the following projects:

1) Fantasy football machine learning: This project uses the API from the website for the Premier League Fantasy Football league, which provides weekly performance data on players in the Premier League. Using this data, in combination with historic performance data from previous seasons, a predictive model was produced using deep learning techniques which was able to achieve accuracies in the range of 80%. Primarily, performance in the next week and the future price of players were predicted. The scripts 'new_data_loader', 'Normalise_new_data', 'data_sequencer', 'data_scaling' and 'data_processor' are used to preprocess the input data and historic data. The output from these scripts is then used to train the predictive model using 'neural network.' 'Prediction_evaluator', 'Return_predictions' and 'Save_predictions' are then used to apply the model to newly received data and save the predictions in csv form.
   Pandas and TensorFlow modules are used extensively here, in addition to Numpy.


2) Orbit Calculator. Example of object orientated programming and implementation of mathematically complex algorithms- specifically a Runge-Kutta fourth order integration technic. Folder includes the write-up for which the program was written in addition to the script and an animation of the motion computed by the script.

3) Neural Network using Numpy. Demonstration of implementation of tensoral algorithms.
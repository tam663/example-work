import requests
import json
import os
import datetime
import pandas as pd
import pickle
from collections import deque
import random
import time
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, LSTM, BatchNormalization
from tensorflow.keras.callbacks import TensorBoard, ModelCheckpoint
from pymongo import MongoClient
from new_data_loader import Update_files
from data_processer import Preprocess_i
from neural_network import Network
from Save_predictions import Predictions_to_mongodb_and_csv

from Normalise_new_data import Normalise_NEW_DATA

TIME_PERIOD, EPOCHS, BATCH_SIZE, LTSM_SHAPE = 3, 20, 32, 128
PREDICTION_GAMEWEEK = 7
Update_files()
Normalise_NEW_DATA()
""" Only use 'Network' if new model is to be defined. Else, supply the list Models to the prefiction function. """
# Network(directory, TIME_PERIOD, EPOCHS, BATCH_SIZE, LTSM_SHAPE)
Models = [
    ["models/pb_3_20_32_128_1567239826.1518998", "PB3", ["bps", "ict_index", "minutes"], 3],
    ["models/tvb_3_20_32_128_1566912317", "TVB", ["transfers_in", "transfers_out", "minutes"], 3]
]
Predictions_to_mongodb_and_csv(PREDICTION_GAMEWEEK, Models)

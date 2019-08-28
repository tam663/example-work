"""(6) Testing of model on newly collected data from the current premier league season. """

import tensorflow as tf
import os
import pandas as pd
import numpy as np
from single_file_preprocessor import next_week_prediction

max_in, max_out, max_value = 843341, 1174804, 136


def next_week_prediction(directory, filename, model):
    df = pd.read_csv(f"{directory}/{filename}", index_col='Gameweek')
    input_df = pd.DataFrame()
    input_df["transfers_in"] = df["transfers_in"].div(max_in)
    input_df["transfers_out"] = df["transfers_out"].div(max_out)
    input_df["minutes"] = df["minutes"].div(90)
    IN = np.array(input_df.values)
    IN_reshaped = np.reshape(IN, (1, 3, 3))
    prediction = model.predict_classes(IN_reshaped)
    return prediction


model = tf.keras.models.load_model("models/tvb_3_20_32_128_1566912317")
directory = "NEW_DATA"

definitions = ["worse", "better"]
total_better = 0
total_worse = 0

for filename in os.listdir(directory):
    if filename.endswith(".csv"):
        # print(filename)
        predict = next_week_prediction(directory, filename, model)
        # print(definitions[int(predict[0])])
        if definitions[int(predict[0])] == "worse":
            total_worse += 1
        if definitions[int(predict[0])] == "better":
            total_better += 1

print("worse:", total_worse)
print("better:", total_better)

"""

   (7) Functions defined which return a performance prediction, for an
   individual player, for the next week given an input secquence of
   performance during the previous weeks. Use "next_week_prediction()"
   for general usage. The other functions are designed to deal with specific
   models.

"""

import pandas as pd
import numpy as np


def next_week_prediction(directory, filename, model, paramaters, time_period, pred_week):
    df = pd.read_csv(f"{directory}/{filename}", index_col='Gameweek')
    input_df = df[paramaters].iloc[pred_week - time_period:pred_week]
    IN = np.array(input_df.values)
    try:
        IN_reshaped = np.reshape(IN, (1, time_period, len(paramaters)))
        prediction = model.predict_classes(IN_reshaped)  # prediction is an array of dimension=1
        element_id = df["id"].iloc[0]
        element_name = f"{df['first_name'].iloc[0]} {df['second_name'].iloc[0]}"
        return prediction, element_id, element_name
    except ValueError:
        element_id = df["id"].iloc[0]
        element_name = f"{df['first_name'].iloc[0]} {df['second_name'].iloc[0]}"
        return np.nan, element_id, element_name


def next_week_prediction_TVB3(directory, filename, model):
    max_in, max_out, max_value = 843341, 1174804, 136
    df = pd.read_csv(f"{directory}/{filename}", index_col='Gameweek')
    input_df = pd.DataFrame()
    input_df["transfers_in"] = df["transfers_in"].iloc[-3:].div(max_in)
    input_df["transfers_out"] = df["transfers_out"].iloc[-3:].div(max_out)
    input_df["minutes"] = df["minutes"].iloc[-3:].div(90)
    IN = np.array(input_df.values)
    IN_reshaped = np.reshape(IN, (1, 3, 3))
    prediction = model.predict_classes(IN_reshaped)  # prediction is an array of dimension=1
    element_id = df["id"].iloc[0]
    element_name = f"{df['first_name'].iloc[0]} {df['second_name'].iloc[0]}"
    return prediction, element_id, element_name


def next_week_prediction_PB3(directory, filename, model):

    df = pd.read_csv(f"{directory}/{filename}", index_col='Gameweek')
    input_df = pd.DataFrame()
    input_df["bps"] = df["bps"].iloc[-3:].div(max_bps)
    input_df["ict_index"] = df["ict_index"].iloc[-3:].div(max_ict)
    input_df["minutes"] = df["minutes"].iloc[-3:].div(90)
    IN = np.array(input_df.values)
    IN_reshaped = np.reshape(IN, (1, 3, 3))
    prediction = model.predict_classes(IN_reshaped)  # prediction is an array of dimension=1
    element_id = df["id"].iloc[0]
    element_name = f"{df['first_name'].iloc[0]} {df['second_name'].iloc[0]}"
    return prediction, element_id, element_name

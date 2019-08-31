"""
(6) Testing of model on newly collected data from the current premier league season.
N.B. in the current code the length of the time series input data is hard-coded, and this may need to be
change in future if further deeplarning models are produced requiring different length of input data.
 """
import tensorflow as tf
import os
import pandas as pd
import numpy as np
from pymongo import MongoClient
from Return_predictions import next_week_prediction


def Predictions_to_mongodb_and_csv(Gameweek_number, model_name_list):

    client = MongoClient('localhost', 27017)
    db = client.FantasyFootball
    gameweek = db[f"predictions_gw{Gameweek_number}"]
    print(type(gameweek))
    model_labels = []
    for single_model in model_name_list:
        single_model[0] = tf.keras.models.load_model(f"{single_model[0]}")
        model_labels.append(single_model[1])
    directory = "NEW_DATA_normalised"
    definitions = ["worse", "better"]
    player_list = []
    names = ["Player", "ID"] + model_labels
    data = []
    for filename in os.listdir(directory):
        if filename.endswith(".csv"):
            player_data = {}
            predictions = []
            for single_model in model_name_list:
                predict, element_id, element_name = next_week_prediction(directory, filename, single_model[0], single_model[2], single_model[3])
                player_data.update({f"{single_model[1]} prediction": definitions[int(predict[0])]})
                predictions.append(definitions[int(predict[0])])
            player_data.update({
                "Player": f"{element_name}",
                "ID": int(element_id)
            })
            player_list.append(player_data)
            total_data = [element_name, element_id] + predictions
            data.append(total_data)
    # print(player_list)
    # print("dataframe:", data)
    result = gameweek.insert_many(player_list)
    print(result)
    df = pd.DataFrame(np.array(data), columns=names)
    with open(f"model_predictions_GW4_two.csv", 'w') as file:
        df.to_csv(file, header=True, mode='w', index=False)


Models = [
    ["models/pb_3_20_32_128_1567239826.1518998", "PB3", ["bps", "ict_index", "minutes"], 3],
    ["models/tvb_3_20_32_128_1566912317", "TVB", ["transfers_in", "transfers_out", "minutes"], 3]
]

Predictions_to_mongodb_and_csv(5, Models)

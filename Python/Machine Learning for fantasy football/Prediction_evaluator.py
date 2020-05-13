"""
    (9) Script to compare the predicted performance and actual performance in a given week.

"""

import pandas as pd
import numpy as np


def classify(current, future):
    if float(future) > float(current):
        return definitions[1]
    else:
        return definitions[0]


Gameweek_number = 4
definitions = ["worse", "better"]
GW3 = pd.read_csv("Gameweeks/GW3_player_data.csv")  # , index_col='id')
GW4 = pd.read_csv("Gameweeks/GW4_player_data.csv")  # , index_col='id')
GW4P = pd.read_csv("Gameweeks/GW4_player_predictions.csv")
holder = ["" for i in range(len(GW4P))]
GW4P["actual"] = holder


for index, dat in GW4P.iterrows():
    print(dat)
    print(GW4P.iloc[index].loc["actual"])
    identity = dat.loc["ID"]
    start_row = GW3.loc[GW3["id"] == identity]
    end_row = GW4.loc[GW4["id"] == identity]
    if start_row.empty == False and end_row.empty == False:
        value = classify(start_row["total_points"].iloc[0], end_row["total_points"].iloc[0])
        GW4P.at[index, "actual"] = value
    else:
        GW4P.at[index, "actual"] = "N/A"
    # print(GW4P.iloc[index].loc["actual"])

with open(f"Gameweeks/model_predictions_GW{Gameweek_number}.csv", 'w') as file:
    GW4P.to_csv(file, header=True, mode='w', index=False)

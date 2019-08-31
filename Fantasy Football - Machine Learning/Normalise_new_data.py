import pandas as pd
import numpy as np
import os

max_in, max_out, max_value, max_bps, max_ict = 843341, 1174804, 136, 114, 31.1
cols = ["transfers_in", "transfers_out", "now_cost", "bps", "ict_index", "minutes"]
normal_values = [843341, 1174804, 136, 114, 31.1, 90]

for filename in os.listdir("NEW_DATA"):
    if filename.endswith(".csv"):
        df = pd.read_csv(f"NEW_DATA/{filename}", index_col='Gameweek')
        for col, number in zip(cols, normal_values):
            df[col] = df[col].div(number)
        with open(f"NEW_DATA_normalised/{filename}", mode='w') as file:
            df.to_csv(file, header=True, mode='w', index=True)

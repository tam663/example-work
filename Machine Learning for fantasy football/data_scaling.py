"""(3) Normalize data for model training. """

import pandas as pd
import numpy as np
import os
import datetime


def classify(current, future):
    if float(future) > float(current):
        return 1
    else:
        return 0


def Scale_columns(network_type):
    new_directory = f"new_scaled_data_{network_type}_{datetime.date.today()}"
    directory = os.path.dirname(os.path.abspath(__file__))
    os.mkdir(f"{directory}/{new_directory}")
    count = 0
    rounds_array = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38]
    # Fix incorrect indexing in historic data:
    for filename in os.listdir(f"{directory}/processed_data_1"):
        if filename.endswith(".csv"):
            df = pd.read_csv(f"{directory}/processed_data_1/{filename}")
            if len(df["round"]) == 38:
                if df["round"].iloc[-1] != 38:
                    df["round"].iloc[-1] = 38
            difference_length = 38 - len(df["round"])
            if len(df["round"]) == 38:
                df["round"] = rounds_array
            else:
                df["round"] = rounds_array[difference_length:]
            df.to_csv(f"{directory}/processed_data_1/{filename}", header=True, mode='w', index=False)
            count += 1
        else:
            count += 1
            continue

    if network_type == 'transfers_value_based':
        """
        Determine the maximum values for 'transfers_in,' 'transfers_out' and 'value.'
        """
        max_value = 0
        max_in = 0
        max_out = 0
        for filename in os.listdir(f"{directory}/processed_data_1"):
            if filename.endswith(".csv") or filename.endswith(".py"):
                df = pd.read_csv(f"{directory}/processed_data_1/{filename}")
                print(count, filename)
                if df.at[df['transfers_in'].idxmax(axis=1), "transfers_in"] > max_in:
                    print(df['transfers_in'][df['transfers_in'].idxmax(axis=1)])
                    max_in = df.at[df['transfers_in'].idxmax(axis=1), "transfers_in"]
                if df.at[df['transfers_out'].idxmax(axis=1), "transfers_out"] > max_out:
                    max_out = df.at[df['transfers_out'].idxmax(axis=1), "transfers_out"]
                # if df.at[df["value"].idxmax(), "value"] > max_value:
                #     max_value = df.at[df["value"].idxmax(), "value"]

                count += 1
            else:
                count += 1
                continue

        # max_in, max_out, max_value = 843341, 1174804, 136
        # Scale the data
        for filename in os.listdir(f"{directory}/processed_data_1"):
            if filename.endswith(".csv"):
                print(filename)
                df = pd.read_csv(f"{directory}/processed_data_1/{filename}", index_col='round')
                df["future_price"] = df['value'].shift(+1)
                df["price_change"] = df["future_price"] - df["value"]
                df["total_points_next"] = df["total_points"].shift(-1)
                df["target_points"] = list(map(classify, df["total_points_next"], df["total_points"]))
                df["target_price"] = list(map(classify, df['value'].shift(-1), df["value"]))
                new_df = pd.DataFrame()
                new_df["transfers_in"] = df["transfers_in"].div(max_in)
                new_df["transfers_out"] = df["transfers_out"].div(max_out)
                # new_df["value"] = df["value"].div(max_value)
                new_df["minutes"] = df["minutes"].div(90)
                # new_df["target_price"] = df["target_price"]
                new_df["target_points"] = df["target_points"]
                # new_df["price_change"] = df["price_change"]

                if new_df["minutes"][new_df["minutes"] > 0].count() > 10:
                    with open(f"{directory}/{new_directory}/{filename}", 'w') as file:
                        new_df.to_csv(file, header=True, mode='w', index=True)
            else:
                continue
        print("data scaled with columns: 'transfers_in    transfers_out    minutes    target_points  ' ")
        return new_directory

    elif network_type == 'performance_based':
        """
        Determine the maximum values for 'tbps' and 'ICT index.'
        """
        max_bps = 0
        max_ict = 0
        for filename in os.listdir(f"{directory}/processed_data_1"):
            if filename.endswith(".csv") or filename.endswith(".py"):
                df = pd.read_csv(f"{directory}/processed_data_1/{filename}")
                print(count, filename)
                if df.at[df['bps'].idxmax(axis=1), "bps"] > max_bps:
                    print(df['bps'][df['bps'].idxmax(axis=1)])
                    max_bps = df.at[df['bps'].idxmax(axis=1), "bps"]
                if df.at[df['ict_index'].idxmax(axis=1), "ict_index"] > max_ict:
                    max_ict = df.at[df['ict_index'].idxmax(axis=1), "ict_index"]

                count += 1
            else:
                count += 1
                continue

        #max_bps, max_ict = 114, 31.1
        # Scale the data
        for filename in os.listdir(f"{directory}/processed_data_1"):
            if filename.endswith(".csv"):
                print(filename)
                df = pd.read_csv(f"{directory}/processed_data_1/{filename}", index_col='round')
                df["total_points_next"] = df["total_points"].shift(-1)
                df["target_points"] = list(map(classify, df["total_points_next"], df["total_points"]))
                new_df = pd.DataFrame()
                new_df["bps"] = df["bps"].div(max_bps)
                new_df["ict_index"] = df["ict_index"].div(max_ict)
                new_df["minutes"] = df["minutes"].div(90)
                new_df["target_points"] = df["target_points"]

                if new_df["minutes"][new_df["minutes"] > 0].count() > 10:
                    with open(f"{directory}/{new_directory}/{filename}", 'w') as file:
                        new_df.to_csv(file, header=True, mode='w', index=True)
            else:
                continue
        print("data scaled with columns: 'bps, ict_index, minutes target_points' ")
        return new_directory

    else:
        print("no network typed passed")

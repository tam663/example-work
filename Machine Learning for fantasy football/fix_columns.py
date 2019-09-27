'''

(10). The Fantasy Football API only holds data from the previous gameweek. If data is not retrived
      from the API during a given week use this script to update the data from a file, if the
      gameweek data is available from another source- pass the location of the missing gameweek
      data to the function via the paramater 'missing_data_location.' The script as below retrives
      data for gameweek 5.

'''

import pandas as pd
import numpy as np
import os
import math


def fix_data(missing_data_location):
    GAMEWEEK_5 = pd.read_csv(missing_data_location, index_col="name")
    # print(GAMEWEEK_5.head())
    not_updated = []
    for filename in os.listdir("NEW_DATA"):
        try:
            print(filename)
            data = pd.read_csv(f"NEW_DATA/{filename}", index_col="Gameweek")
            condition_1 = 5 in data.index
            if condition_1 == False:
                cols_mine = data.columns.values.tolist()
                name = filename[:-4]
                condition_2 = name in GAMEWEEK_5.index
                if condition_2 == True:
                    five = GAMEWEEK_5.loc[f"{name}"]
                    new_dat = five[cols_mine]
                    new_dataa = new_dat.rename(5)
                    result = data.append(new_dataa)
                    result.sort_index(inplace=True)
                    try:
                        result.at[5, "minutes"] = result.loc[5].loc["minutes"] + result.loc[4].loc["minutes"]
                        for index, value in result.loc[5].iteritems():
                            if math.isnan(value):
                                result.at[5, index] = result.at[4, index]
                    except:
                        pass
                    with open(f"NEW_DATA/{filename}", mode='w') as file:
                        result.to_csv(file, header=True, mode='w', index=True)
                print("data updated")

            else:
                print("data not updated")
        except UnicodeDecodeError:
            not_updated.append(filename)
            print(f"Error when reading {filename}")

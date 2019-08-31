''' (2) Script to produce a set of data from which a ML model can be trained. All useless data is removed from the historic data.'''
import json
import os
import datetime
import pandas as pd
years = [[16, 17], [17, 18], [18, 19]]
for year in years:
      data = pd.read_csv(f'raw_data/20{year[0]}-{year[1]}/player_idlist.csv')
      for i in range(len(data)):
            player = pd.read_csv(f'raw_data/20{year[0]}-{year[1]}/players/{data.iloc[i][0]}_{data.iloc[i][1]}_{data.iloc[i][2]}/gw.csv', index_col='round')
            player.drop(["assists",
                         "attempted_passes",
                         "big_chances_created",
                         "big_chances_missed",
                         "bonus",
                         "clearances_blocks_interceptions",
                         "completed_passes",
                         "dribbles",
                         "ea_index",
                         "element",
                         "errors_leading_to_goal",
                         "errors_leading_to_goal_attempt",
                         "fixture",
                         "fouls",
                         "key_passes",
                         "kickoff_time",
                         "kickoff_time_formatted",
                         "loaned_in",
                         "loaned_out",
                         "offside",
                         "open_play_crosses",
                         "opponent_team",
                         "penalties_conceded",
                         "tackled",
                         "tackles",
                         "target_missed",
                         "team_a_score",
                         "team_h_score",
                         "winning_goals",
                         "was_home",
                         "selected",
                         "transfers_balance",
                         "id",
                         "recoveries"
                         ], axis=1, inplace=True)
            with open(f"processed_data_1/{data.iloc[i][0]}_{data.iloc[i][1]}_{year[0]}.csv", 'w') as file:
                  player.to_csv(file, header=True, mode='w')

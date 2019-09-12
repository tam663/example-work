'''

    (1) Script to retrive player data from the Fantasy Premier League API
    and update a data files containing player performance data.

'''
import requests
import json
import os
import datetime
import pandas as pd
import pickle

"""
https://fantasy.premierleague.com/api/my-team/2634476/ -> specifc fantasy team's players

"""


def Update_files():
    url = "https://fantasy.premierleague.com/api/bootstrap-static/"
    r = requests.get(url)
    data = json.loads(r.text)           # Convert data to python dict and then retrive data with the dict:

    gameweeks = data['events']          # 'gameweeks' data contains average fantasy team score, the dream team score, and the most select, most transfered in, most captian
    # vice-captained and highest sccoring players, with the players being reference using the 'id' tag.
    teams = data['teams']
    players = data['elements']
    for gameweek in gameweeks:
        del gameweek['chip_plays']       # Don't need this data and it is problematic to format

    for event in gameweeks:              # Get current week.
        if event['finished'] == True:
            current_gameweek = event['id']
        else:
            if event["finished"] == False:
                break

    for player in players:
        del (player["chance_of_playing_next_round"],
             player["chance_of_playing_this_round"],
             player["cost_change_event"],
             player["cost_change_event_fall"],
             player["cost_change_start"],
             player["cost_change_start_fall"],
             player["value_form"],
             player["value_season"],
             player["web_name"],
             player["transfers_out_event"],
             player["transfers_in_event"],
             player["status"],
             player["squad_number"],
             player["special"],
             player["photo"],
             player["news_added"],
             player["news"],
             player["ep_this"],
             player["ep_next"])
        player["Gameweek"] = current_gameweek

    # Save this week's data as a csv and the original json formated data to txt.
    player_df = pd.DataFrame(data=players)
    gameweeks_df = pd.DataFrame(data=gameweeks)
    gameweeks_df.to_csv("Gameweeks/Overall_FF_stats.csv", header=True, mode='w', index=False)
    player_df.to_csv(f'Gameweeks/GW{current_gameweek}_player_data.csv', header=True, mode='w', index=False)
    pickle.dump(r, open(f"Gameweeks/GW{current_gameweek}_player_data.txt", 'wb'))
    # Open with p = pickle.load(open(f"Gameweeks/GW{cuurent_gameweek}_test.txt", 'rb')) & data = json.loads(p.text)
    # Append player data to player files:
    for index, player in player_df.iterrows():
        if os.path.isfile(f'NEW_DATA/{player["first_name"]}_{player["second_name"]}_{player["id"]}.csv') == True:
            total_data = pd.read_csv(f'NEW_DATA/{player["first_name"]}_{player["second_name"]}_{player["id"]}.csv')
            if player["Gameweek"] > total_data["Gameweek"].iloc[-1]:
                result = total_data.append(player)
                result.to_csv(f'NEW_DATA/{player["first_name"]}_{player["second_name"]}_{player["id"]}.csv', header=True, mode='w', index=False)
        else:
            save_data = pd.DataFrame([player])
            save_data.to_csv(f'NEW_DATA/{player["first_name"]}_{player["second_name"]}_{player["id"]}.csv', header=True, mode='w', index=False)


Update_files()

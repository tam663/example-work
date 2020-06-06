import pandas as pd
import numpy as np
pd.options.mode.chained_assignment = None  # default='warn'


def individual_team_performance(variables, Gameweeks):
    '''
    Function which returns a list of dataframes, which each contains the perfromance of each team for each gameweek.
    These dataframes can then be used to train a multivariable regression model, based
    of the previous weeks performance and betting odds.
    '''
    team_list = []
    ind_variables = ["Position"] + variables

    for team in list(team_names["TeamID"].values):
        df = pd.DataFrame(index=Gameweeks, columns=ind_variables)
        df["FormPrev3"].values[:] = "___"
        df["FormPrev5"].values[:] = "_____"
        df["HomeFormPrev3"].values[:] = "___"
        df["HomeFormPrev5"].values[:] = "_____"
        df["AwayFormPrev3"].values[:] = "___"
        df["AwayFormPrev5"].values[:] = "_____"
        df.fillna(0, inplace=True)
        team_list.append(df)
    return team_list, ind_variables


def make_table(variables, indices):
    '''
    Produce empty table for results to be put into.
    '''
    table = pd.DataFrame(columns=variables, index=indices)  # index of table is each team's ID
    table["FormPrev3"].values[:] = "___"
    table["FormPrev5"].values[:] = "_____"
    table["HomeFormPrev3"].values[:] = "___"
    table["HomeFormPrev5"].values[:] = "_____"
    table["AwayFormPrev3"].values[:] = "___"
    table["AwayFormPrev5"].values[:] = "_____"
    table.fillna(0, inplace=True)
    table["Position"] = list(indices)
    return table


def run_season(Season_Results, empty_table, team_list, indices, column_list):
    '''
    Loop through all 54 gameweeks, and use the results dataframe to fill in the empty table.
    '''

    league_won = 0
    for i in range(1, 55):
        week_results = Season_Results.loc[Season_Results["Gameweek"] == i]
        for index, row in week_results.iterrows():
            teamH = row.loc["HomeTeamID"]
            teamA = row.loc["AwayTeamID"]
            '''
            The section section below could be modified to make it more modular by using the 'variables' list, which is a list of the column lables used to build the table variable; a loop through the variables list would be perferable to what is below, however there is quite a lot
            of variation as to how each variable is incremented so this may not be possible.
            '''
            if row.loc["HomeScore"] == row.loc["AwayScore"]:
                empty_table["Points"][teamH] = empty_table.loc[teamH]["Points"] + 1
                empty_table["GoalsFor"][teamH] = empty_table.loc[teamH]["GoalsFor"] + row.loc["HomeScore"]
                empty_table["GoalsAgainst"][teamH] = empty_table.loc[teamH]["GoalsAgainst"] + row.loc["AwayScore"]
                empty_table["FormPrev3"][teamH] = empty_table.loc[teamH]["FormPrev3"][1:] + 'D'
                empty_table["FormPrev5"][teamH] = empty_table.loc[teamH]["FormPrev5"][1:] + 'D'
                empty_table["HomeFormPrev3"][teamH] = empty_table.loc[teamH]["HomeFormPrev3"][1:] + 'D'
                empty_table["HomeFormPrev5"][teamH] = empty_table.loc[teamH]["HomeFormPrev5"][1:] + 'D'

                empty_table["Points"][teamA] = empty_table.loc[teamA]["Points"] + 1
                empty_table["GoalsFor"][teamA] = empty_table.loc[teamA]["GoalsFor"] + row.loc["AwayScore"]
                empty_table["GoalsAgainst"][teamA] = empty_table.loc[teamA]["GoalsAgainst"] + row.loc["HomeScore"]
                empty_table["FormPrev3"][teamA] = empty_table.loc[teamA]["FormPrev3"][1:] + 'D'
                empty_table["FormPrev5"][teamA] = empty_table.loc[teamA]["FormPrev5"][1:] + 'D'
                empty_table["AwayFormPrev3"][teamA] = empty_table.loc[teamA]["AwayFormPrev3"][1:] + 'D'
                empty_table["AwayFormPrev5"][teamA] = empty_table.loc[teamA]["AwayFormPrev5"][1:] + 'D'

                empty_table["GamesPlayed"][teamH] = i
                empty_table["GamesPlayed"][teamA] = i

            if row.loc["HomeScore"] > row.loc["AwayScore"]:
                empty_table["Points"][teamH] = empty_table.loc[teamH]["Points"] + 3
                empty_table["GoalDifference"][teamH] = empty_table.loc[teamH]["GoalDifference"] + (row.loc["HomeScore"] - row.loc["AwayScore"])
                empty_table["GoalsFor"][teamH] = empty_table.loc[teamH]["GoalsFor"] + row.loc["HomeScore"]
                empty_table["GoalsAgainst"][teamH] = empty_table.loc[teamH]["GoalsAgainst"] + row.loc["AwayScore"]
                empty_table["FormPrev3"][teamH] = empty_table.loc[teamH]["FormPrev3"][1:] + 'W'
                empty_table["FormPrev5"][teamH] = empty_table.loc[teamH]["FormPrev5"][1:] + 'W'
                empty_table["HomeFormPrev3"][teamH] = empty_table.loc[teamH]["HomeFormPrev3"][1:] + 'W'
                empty_table["HomeFormPrev5"][teamH] = empty_table.loc[teamH]["HomeFormPrev5"][1:] + 'W'

                empty_table["GoalDifference"][teamA] = empty_table.loc[teamA]["GoalDifference"] + (row.loc["AwayScore"] - row.loc["HomeScore"])
                empty_table["GoalsFor"][teamA] = empty_table.loc[teamA]["GoalsFor"] + row.loc["AwayScore"]
                empty_table["GoalsAgainst"][teamA] = empty_table.loc[teamA]["GoalsAgainst"] + row.loc["HomeScore"]
                empty_table["FormPrev3"][teamA] = empty_table.loc[teamA]["FormPrev3"][1:] + 'L'
                empty_table["FormPrev5"][teamA] = empty_table.loc[teamA]["FormPrev5"][1:] + 'L'
                empty_table["AwayFormPrev3"][teamA] = empty_table.loc[teamA]["AwayFormPrev3"][1:] + 'L'
                empty_table["AwayFormPrev5"][teamA] = empty_table.loc[teamA]["AwayFormPrev5"][1:] + 'L'

                empty_table["GamesPlayed"][teamH] = i
                empty_table["GamesPlayed"][teamA] = i

            if row.loc["HomeScore"] < row.loc["AwayScore"]:
                empty_table["GoalDifference"][teamH] = empty_table.loc[teamH]["GoalDifference"] + (row.loc["HomeScore"] - row.loc["AwayScore"])
                empty_table["GoalsFor"][teamH] = empty_table.loc[teamH]["GoalsFor"] + row.loc["HomeScore"]
                empty_table["GoalsAgainst"][teamH] = empty_table.loc[teamH]["GoalsAgainst"] + row.loc["AwayScore"]
                empty_table["FormPrev3"][teamH] = empty_table.loc[teamH]["FormPrev3"][1:] + 'L'
                empty_table["FormPrev5"][teamH] = empty_table.loc[teamH]["FormPrev5"][1:] + 'L'
                empty_table["HomeFormPrev3"][teamH] = empty_table.loc[teamH]["HomeFormPrev3"][1:] + 'L'
                empty_table["HomeFormPrev5"][teamH] = empty_table.loc[teamH]["HomeFormPrev5"][1:] + 'L'

                empty_table["Points"][teamA] = empty_table.loc[teamA]["Points"] + 3
                empty_table["GoalDifference"][teamA] = empty_table.loc[teamA]["GoalDifference"] + (row.loc["AwayScore"] - row.loc["HomeScore"])
                empty_table["GoalsFor"][teamA] = empty_table.loc[teamA]["GoalsFor"] + row.loc["AwayScore"]
                empty_table["GoalsAgainst"][teamA] = empty_table.loc[teamA]["GoalsAgainst"] + row.loc["HomeScore"]
                empty_table["FormPrev3"][teamA] = empty_table.loc[teamA]["FormPrev3"][1:] + 'W'
                empty_table["FormPrev5"][teamA] = empty_table.loc[teamA]["FormPrev5"][1:] + 'W'
                empty_table["AwayFormPrev3"][teamA] = empty_table.loc[teamA]["AwayFormPrev3"][1:] + 'W'
                empty_table["AwayFormPrev5"][teamA] = empty_table.loc[teamA]["AwayFormPrev5"][1:] + 'W'

                empty_table["GamesPlayed"][teamH] = i
                empty_table["GamesPlayed"][teamA] = i
        empty_table.sort_values("Points", ascending=False, inplace=True)
        empty_table["Position"] = indices
        difference_1_2 = empty_table.loc[empty_table["Position"][:2].index[0]]["Points"] - empty_table.loc[empty_table["Position"][:2].index[1]]["Points"]

        if league_won == 0:
            if difference_1_2 > 3 * (55 - i):
                league_won = i  # week number in which league was won

        for n, team in enumerate(team_list):
            '''
            The value (n+1) is the team id --> use this to reference current week values
            from the empty_table dataframe.
            Use the variable i to reference a particular gameweek in the team dataframe.
            '''
            for col in column_list:
                if col == "Position":
                    team[col][i] = empty_table.loc[n + 1].index
                team[col][i] = empty_table.loc[n + 1][col]

    return empty_table, league_won


def compare(odds, results):

    odds["difference"] = odds["Home"] - odds["Away"]
    '''
    Values in odds["difference"]:
    negative ==> expected home win
    positive ==> expected away win
    '''
    results["difference"] = odds["difference"].copy()
    results["results"] = 0
    results.loc[results['HomeScore'] > results["AwayScore"], 'results'] = "Home"
    results.loc[results['HomeScore'] < results["AwayScore"], 'results'] = "Away"
    results.loc[results['HomeScore'] == results["AwayScore"], 'results'] = "Draw"
    results.sort_values("difference", inplace=True)  # results sorted by difference from most negative to most positive
    away = results.loc[results["results"] == "Away"]
    home = results.loc[results["results"] == "Home"]
    draw = results.loc[results["results"] == "Draw"]
    '''
    Now compare the most unexpected away and home wins, and draws, given the odds:
    '''
    o = max(abs(home.iloc[-1]["difference"]), abs(away.iloc[0]["difference"]))

    if o == abs(home.iloc[-1]["difference"]):
        category = "home Win"
        typee = home
        biggest_upset = home.iloc[-1]["MatchID"]
    elif o == abs(away.iloc[0]["difference"]):
        category = "away Win"
        typee = away
        biggest_upset = away.iloc[0]["MatchID"]

    biggest_draw_upset = max(abs(draw.iloc[-1]["difference"]), abs(draw.iloc[0]["difference"]))
    # print(typee.loc[typee["MatchID"]==biggest_upset]["difference"].values[0])
    b = max(abs(typee.loc[typee["MatchID"] == biggest_upset]["difference"].values[0]), biggest_draw_upset)
    if b == abs(typee.loc[typee["MatchID"] == biggest_upset]["difference"].values[0]):
        pass
    elif b == biggest_draw_upset:
        category = "draw"
        typee = draw
        try:
            biggest_upset = draw.loc[draw["difference"] == b]["MatchID"].values[0]
        except:
            biggest_upset = draw.loc[draw["difference"] == -b]["MatchID"].values[0]
    return biggest_upset, category, typee.loc[typee["MatchID"] == biggest_upset]


if __name__ == "__main__":
    # data files load:
    betting_odds = pd.read_csv("odds.csv")
    match_results = pd.read_csv("results.csv")
    fixtures = pd.read_csv("fixtures.csv")
    team_names = pd.read_csv("teams.csv")
    starting = pd.read_csv("startingXI.csv")
    players = pd.read_csv("players.csv")

    variables = ["Points", "GamesPlayed", "GoalDifference", "GoalsFor", "GoalsAgainst", "FormPrev3", "FormPrev5", "HomeFormPrev3", "HomeFormPrev5", "AwayFormPrev3", "AwayFormPrev5"]

    table_indices = list(team_names["TeamID"].values)
    Gameweeks = list(range(1, 55))
    Year1_results = match_results.loc[match_results["SeasonID"] == 1]

    teams, team_columns = individual_team_performance(variables, Gameweeks)
    table = make_table(variables, table_indices)
    table_end, league_won_GW = run_season(Year1_results, table, teams, table_indices, team_columns)
    biggest_upset, upset_type, upset_match = compare(betting_odds, match_results)
    home_upset_team = team_names.loc[team_names["TeamID"] == upset_match["HomeTeamID"].values[0]]["TeamName"].values[0]
    away_upset_team = team_names.loc[team_names["TeamID"] == upset_match["AwayTeamID"].values[0]]["TeamName"].values[0]
    print(table_end)
    print(f" The league was won in gameweek {league_won_GW}, and the final table standings are as above.")
    print(f" The biggest upset during the season was the {upset_type} in match {biggest_upset}, between {home_upset_team}, who were at home, and {away_upset_team}.")

'''

    (4) Function defined which returns a tuple (x,Y) which contains input features
    and corresponding output label. In the current form the features are the minutes
    played, and transfers in and out for the previous {length} weeks of games played
    and the label is whether or not the points achieved in the next week increases
    or decreases.

'''

import pandas as pd
import numpy as np
import os
from collections import deque
import random

passed_dir = 'processed_data_2'


def Sequence_data(passed_directory, length):
    SEQ_LEN = length
    sequential_data = []  # this is a list that will CONTAIN the sequences
    directory = os.path.dirname(os.path.abspath(__file__))
    for filename in os.listdir(f"{directory}/{passed_directory}"):
        if filename.endswith(".csv"):
            df = pd.read_csv(f"{directory}/{passed_directory}/{filename}", index_col="round")
            # input_df = df[["transfers_in", "transfers_out", "minutes", "target_points"]] #performance predictior network from "transfers_in", "transfers_out", "minutes"
            input_df = df[['bps', 'ict_index', 'minutes', 'target_points']]  # performance predictior network from 'bps', 'ict_index', 'minutes'
            prev_days = deque(maxlen=SEQ_LEN)  # These will be our actual sequences. They are made with deque, which keeps the maximum length by popping out older values as new ones come in
            for i in input_df.values:  # iterate over the values
                prev_days.append([n for n in i[:-1]])  # store all but the target
                if len(prev_days) == SEQ_LEN:  # make sure we have 60 sequences!
                    sequential_data.append([np.array(prev_days), i[-1]])  # append those bad boys!
    random.shuffle(sequential_data)
    # Balance data.
    up_count = 0
    down_count = 0
    up_data = []
    down_data = []
    for dat in sequential_data:
        if dat[1] == 1:
            up_count += 1
            up_data.append(dat)
        if dat[1] == 0:
            down_count += 1
            down_data.append(dat)
        else:
            pass
    lower = min(up_count, down_count)
    up_data = up_data[:lower]
    down_data = down_data[:lower]
    print(len(down_data), len(up_data))
    training_total = up_data + down_data
    random.shuffle(training_total)
    # Separate the inputs and outputs for the network.
    x = []
    Y = []
    for features, result in training_total:
        x.append(features)
        Y.append(result)

    return np.array(x), Y

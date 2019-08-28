"""(5) definition of neural network model. """

import time
from data_sequencer import Load_data
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, LSTM, BatchNormalization
from tensorflow.keras.callbacks import TensorBoard
from tensorflow.keras.callbacks import ModelCheckpoint

# Variables
# TIME_PERIOD = 3
# EPOCHS = 10  # how many passes through our data
# BATCH_SIZE = 64


def Network(TIME_PERIOD, EPOCHS, BATCH_SIZE, LTSM_SHAPE):
    # Get data:
    IN, OUT = Load_data('processed_data_2', TIME_PERIOD)
    length = len(OUT)
    train_x = IN[:int(0.9 * length)]
    validation_x = IN[int(0.9 * length):]
    train_y = OUT[:int(0.9 * length)]
    validation_y = OUT[int(0.9 * length):]

    # Define Network & callback:
    NAME = f"tvb_{TIME_PERIOD}_{EPOCHS}_{BATCH_SIZE}_{LTSM_SHAPE}_{time.time()}"
    ternsorboard = TensorBoard(log_dir=f"logs/{NAME}")

    model = Sequential()
    model.add(LSTM(LTSM_SHAPE, input_shape=(train_x.shape[1:]), return_sequences=True))
    model.add(Dropout(0.2))
    model.add(BatchNormalization())  # normalizes activation outputs, same reason you want to normalize your input data.

    model.add(LSTM(LTSM_SHAPE, return_sequences=True))
    model.add(Dropout(0.1))
    model.add(BatchNormalization())

    model.add(LSTM(LTSM_SHAPE))
    model.add(Dropout(0.2))
    model.add(BatchNormalization())

    model.add(Dense(32, activation='relu'))
    model.add(Dropout(0.2))

    model.add(Dense(2, activation='softmax'))

    opt = tf.keras.optimizers.Adam(lr=0.001, decay=1e-6)
    model.compile(loss='sparse_categorical_crossentropy', optimizer=opt, metrics=['accuracy'])

    history = model.fit(train_x, train_y, batch_size=BATCH_SIZE, epochs=EPOCHS, validation_data=(validation_x, validation_y), callbacks=[ternsorboard])
    print('\nhistory dict:', history.history)

    # Score model
    score = model.evaluate(validation_x, validation_y, verbose=0)
    print('Test loss:', score[0])
    print('Test accuracy:', score[1])
    # Save model
    model.save(f"models/{NAME}")


# TIME_PERIOD, EPOCHS, BATCH_SIZE, LTSM_SHAPE = 5, 20, 32, 128
# Network(TIME_PERIOD, EPOCHS, BATCH_SIZE, LTSM_SHAPE)

# for TIME_PERIOD in [3,5,10]:
#     for EPOCHS in [5,10,20]:
#         for BATCH_SIZE in [16,32,64,128]:
#             for LTSM_SHAPE in [64,128,254]:
#                 Network(TIME_PERIOD, EPOCHS, BATCH_SIZE, LTSM_SHAPE)

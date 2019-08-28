import numpy as np
import sys
import random
import gzip
import pickle
np.set_printoptions(threshold=sys.maxsize)

""" Basic neural network implemented based on the algorithms outlined at http://neuralnetworksanddeeplearning.com/index.html. The
network as implemented below is trained using the MNIST data base. Maximum accuracy achieved is 87% for the current initial conditions.
The algorithms 'forwardpropagation,' 'backwardpropagation' and 'analyse' have been implemented independently, using the literature
in the above weblink, while the 'load_data' and mini_batch prparation has been borrowed from the code on this link. In this implementation
the accuracy of the resulting netowrl is severly limited by the time required for computation, which is large due to the lack of use
more efficient matrix-manipulation modules than numpy.  Hence the implementation is simply meant as an exercise to demonstrate
the basoc theory of neural-networks."""


class neuralnetwork():

    def __init__(self, network_shape):
        self.weights = []
        self.biases = []
        self.no_layers = len(network_shape)
        self.shape = network_shape

    def initiate_network(self):
        for i in range(len(self.shape) - 1):
            self.weights.append(np.random.randn(self.shape[i + 1], self.shape[i]))
        for i in range(len(self.shape) - 1):
            self.biases.append(np.random.randn(self.shape[i + 1], 1))

    def stocastic_gradient_descent(self, training_dat, epochs, step_size, mini_batch_size, test_dat):
        # Convert the training labels (which are initally given as a digit 0-9) in the sample into a vector form, where the value of
        # ith indice is set to one and i is the number label for the image.

        for i in range(epochs):
            print("epoch:", i)
            # Data containers for adjusting the weights and biases.
            delta_b = [np.zeros(b.shape) for b in self.biases]
            delta_w = [np.zeros(w.shape) for w in self.weights]
            delta_ws = [np.zeros(w.shape) for w in self.weights]

            n = len(training_dat)
            random.shuffle(training_dat)
            mini_batches = [training_dat[k:k + mini_batch_size]for k in range(0, 1 * mini_batch_size, mini_batch_size)]
            for mini_batch in mini_batches:

                for x, y in mini_batch:
                    # Carry out forward then backward propagation for each sample
                    z_values, acivity_out, activations = self.forwardpropagation(x, training=True)
                    error = self.backpropagation(acivity_out, z_values, y)
                    # Calculate changes to delta
                    for k in range(len(delta_ws)):
                        for l in range(len(error[k])):
                            for m in range(len(activations[k])):
                                delta_ws[k][l][m] = activations[k][m] * error[k][l]
                    delta_w = [dw + dws for dw, dws in zip(delta_w, delta_ws)]
                    delta_b = [db + dbs for db, dbs in zip(delta_b, error)]
                # Adjust weights and biases:
                self.weights = [w - (step_size / mini_batch_size) * dw for w, dw in zip(self.weights, delta_w)]
                self.biases = [b - (step_size / mini_batch_size) * db for b, db in zip(self.biases, delta_b)]
            self.analyse(test_dat)
        print("Network trained.")

    def forwardpropagation(self, image_in, training):
        zs = []
        activations = []
        activations.append(image_in)
        # 1st layer of network takes image_in as input:
        wx = np.dot(self.weights[0], image_in)
        z = [x + y for x, y in zip(wx, self.biases[0])]
        zs.append(z)
        activation = [sigmoid(x) for x in z]
        activations.append(activation)
        # Further layers take the 'activation' of the previous layer as input:
        for i in range(1, self.no_layers - 1):
            wx = np.dot(self.weights[i], activation)
            z = [x + y for x, y in zip(wx, self.biases[i])]
            zs.append(z)
            activation = [sigmoid(x) for x in z]
            activations.append(activation)
        # Return appropriate arrays depending on where 'forwardpropagation' is being called.
        if training == True:
            return zs, activation, activations

        if training == False:
            return activation

    def backpropagation(self, acivity_out, z_values, label):
        # Implements the backpropagation algorithm. The maths is not explain in detail here as it is
        # relatively involved. See link at top of script for more.
        # Error in final layer of network:
        error = []
        difference = [cost_derivative(y, a)for y, a in zip(label, acivity_out)]
        diferential = [dif_sigmoid(x) for x in z_values[-1]]
        delta = np.multiply(difference, diferential)
        error.append(delta)
        error_previous_layer = delta
        # Error in ith layer:
        for i in range(2, self.no_layers):
            difference = np.dot(self.weights[-i + 1].transpose(), error_previous_layer)
            diferential = [dif_sigmoid(x) for x in z_values[-i]]
            delta = np.multiply(difference, diferential)
            error.insert(0, delta)
            error_previous_layer = delta
        return error

    def sample_data(self, training_i, training_l, mini_batch_size):
        # Return a random selection of the training data passed to 'stocastic_gradient_descent' which
        # will be used to adjust the network.
        sample_i = []
        sample_l = []
        number = np.random.rand(2 * mini_batch_size)
        for i in range(2 * mini_batch_size):
            num = int(len(training_i) * number[i])
            sample_i.append(training_i[num])
            sample_l.append(training_l[num])
        sample_ia = np.array(sample_i)
        sample_la = np.array(sample_l)
        return sample_ia, sample_la

    def analyse(self, data_in):
        right = 0
        wrong = 0
        # Input data to network:
        for x, y in data_in:
            out = self.forwardpropagation(x, training=False)
            out_list = list(out)
            if out.index(max(out_list)) != y:
                wrong += 1
            else:
                right += 1
        print("Data classified. Percentage accuracy = {}%".format(100 * right / (wrong + right)))


''' load_data is used to retrive the handwritten number samples form the MNIST database.The data is
returned as a tuple for the training, validation and test data respectively. The first
tuple is an array containing the handwritten image data and is of shape (50000, 784), (10000, 784)
and (10000, 784) for the training, validation and test data respectively. The second tuple is of shape
(50000,), (10000,) and (10000,) for each data set, and it contains labels indicating which number from
0-9 is shown in the corresponding image.'''


def load_data():

    with gzip.open('/Users/tommciver/Documents/python_folder/AI/mnist.pkl.gz', 'rb') as f:
        file_content = f.read()
        training_data, validation_data, test_data = pickle.loads(file_content, encoding='latin1')

    training_inputs = [np.reshape(x, (784, 1)) for x in training_data[0]]
    training_results = [vectorized_result(y) for y in training_data[1]]
    training_data = zip(training_inputs, training_results)
    validation_inputs = [np.reshape(x, (784, 1)) for x in validation_data[0]]
    validation_data = zip(validation_inputs, validation_data[1])
    test_inputs = [np.reshape(x, (784, 1)) for x in test_data[0]]
    test_data = zip(test_inputs, test_data[1])
    return (training_data, validation_data, test_data)
    # return training_data, validation_data, test_data


def vectorized_result(j):
    """Return a 10-dimensional unit vector with a 1.0 in the jth
    position and zeroes elsewhere.  This is used to convert a digit
    (0...9) into a corresponding desired output from the neural
    network."""
    e = np.zeros((10, 1))
    e[j] = 1.0
    return e


def sigmoid(x):
    return 1 / (1 + np.exp(-x))


def dif_sigmoid(x):
    return sigmoid(x) * (1 - sigmoid(x))


def cost_function(desired_out, out):
    return (out - desired_out)**2 / 2


def cost_derivative(desired_out, out):
    return (out - desired_out)


random.seed(1)
net = neuralnetwork([784, 50, 10])
net.initiate_network()
training_data, validation_data, test_data = load_data()
print(type(training_data))
# net.analyse(test_images, test_labels)
net.stocastic_gradient_descent(list(training_data), 2000, 3, 10, list(test_data))  # epochs, step_size, mini_batch_size

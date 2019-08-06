import numpy as np
import random
import gzip
import pickle


class neuralnetwork():

    def __init__(self, network_shape):
        self.weights = []
        self.biases = []
        self.no_layers = len(network_shape)  # - 1  # Network includes the unput layer which is not counted.
        self.shape = network_shape

    def initiate_network(self):
        for i in range(len(self.shape) - 1):
            self.weights.append(np.random.randn(self.shape[i + 1], self.shape[i]))
        for i in range(len(self.shape) - 1):
            self.biases.append(np.random.randn(self.shape[i + 1], 1))

    def stocastic_gradient_descent(self, training_i, training_l, epochs, step_size, sample_size):
        for i in range(epochs):
            # print('epoch:', i)
            # Randomly choose a selection of data from the input images and labels to train the network via stocstic gradient descent.
            sample_i, sample_l = self.sample_data(training_i, training_l, sample_size)
            for i in range(len(sample_l)):
                print(sample_l[i])
            # Convert the training labels (which are initally given as a digit 0-9) in the sample into a vector form, where the value of
            # ith indice is set to one and i is the number label for the image.
            sample_l_vector = []
            for i in range(len(sample_l)):
                empty = np.zeros(10)
                empty[sample_l[i]] = 1
                sample_l_vector.append(empty)

            # Data containers for adjusting the weights and biases.
            delta_b = []
            delta_w = []
            delta_ws = []
            for i in range(len(self.shape) - 1):
                delta_w.append(np.random.randn(self.shape[i + 1], self.shape[i]))
            for i in range(len(self.shape) - 1):
                delta_b.append(np.random.randn(self.shape[i + 1], 1))
            for i in range(len(self.shape) - 1):
                delta_ws.append(np.random.randn(self.shape[i + 1], self.shape[i]))

            # Carry out forward then backward propagation for each sample
            for k in range(sample_size):
                # print("higher iteration:", k)
                z_values, acivity_out, activations = self.forwardpropagation(sample_i[k], training=True)
                error = self.backpropagation(acivity_out, z_values, sample_l_vector[k])
                activations.insert(0, sample_i[k])

                for i in range(1, self.no_layers - 1):
                    delta_ws[i] = [d * a for d, a in zip(activations[i - 1], error[i])]
                delta_w = [dw + dws for dw, dws in zip(delta_w, delta_ws)]
                delta_b = [db + dbs for db, dbs in zip(delta_b, error)]

        # Adjust weights and biases:
        self.weights = [w - dw * (step_size / sample_size) for w, dw in zip(self.weights, delta_w)]
        self.biases = [b - db * (step_size / sample_size) for b, db in zip(self.biases, delta_b)]
        print("Network trained.")

    def forwardpropagation(self, image_in, training):
        zs = []
        activations = []
        # 1st layer of network takes image_in as input:
        wx = np.dot(self.weights[0], image_in)
        z = [sum(x) for x in zip(wx, self.biases[0])]
        zs.append(z)
        activation = [sigmoid(x) for x in z]
        activations.append(activation)

        for i in range(1, self.no_layers - 1):
            # print("iteration:", i)
            wx = np.dot(self.weights[i], activation)
            # print(np.shape(wx))
            # print(np.shape(self.biases[i]))
            z = [sum(x) for x in zip(wx, self.biases[i])]
            zs.append(z)
            activation = [sigmoid(x) for x in z]
            activations.append(activation)
        if training == True:
            return zs, activation, activations
        if training == False:
            return activation

    def backpropagation(self, acivity_out, z_values, label):
        error = []
        # Error in final error:
        difference = [np.subtract(x, y) for x, y in zip(acivity_out, label)]
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

    def sample_data(self, training_i, training_l, sample_size):
        sample_i = []
        sample_l = []
        number = np.random.rand(sample_size)
        for i in range(sample_size):
            sample_i.append(training_i[int(len(training_i) * number[i])])
            sample_l.append(training_l[int(len(training_i) * number[i])])
        sample_ia = np.array(sample_i)
        sample_la = np.array(sample_l)
        return sample_ia, sample_la

    def analyse(self, data_in, labels):
        right = 0
        wrong = 0
        # Input data to network:
        for j in range(len(data_in)):

            out = self.forwardpropagation(data_in[j], training=False)
            # for i in range(len(out)):
            #     if out[i] != np.max(out):
            #         out[i] = 0
            #     else:
            #         out[i] = 1
            out_list = list(out)
            # print(out.index(max(out_list)), labels[j])
            if out.index(max(out_list)) != labels[j]:
                wrong += 1
            else:
                right += 1
        print("Data classified. Percenage accuracy = {}%".format(100 * right / (wrong + right)))


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

    return training_data, validation_data, test_data


def sigmoid(x):
    return 1 / (1 + np.exp(-x))


def dif_sigmoid(x):
    return sigmoid(x) * (1 - sigmoid(x))


def cost_function(desired_out, out):
    return (desired_out - out)**2 / 2


def cost_derivative(desired_out, out):
    return (out - desired_out)


net = neuralnetwork([784, 100, 10])
net.initiate_network()
training_data, validation_data, test_data = load_data()
training_images, training_labels = training_data
validation_images, validation_labels = validation_data
test_images, test_labels = test_data
net.stocastic_gradient_descent(training_images, training_labels, 1, 3, 100)
# print(test_images[0])
net.analyse(training_images, training_labels)

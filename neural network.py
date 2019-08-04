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
            self.biases.append(np.random.randn(self.shape[i + 1]))

    def stocastic_gradient_descent(self, training_i, training_l, test_i, test_l, epochs, step_size, sample_size):
        for i in range(epochs):
            sample_i, sample_l = self.sample_data(training_i, training_l, sample_size)
            # Use backpropogation algorithm to calculate the change in weights and biases from each data set in
            # 'sample_i' and then take the average of these values.
            sample_l_vector = []
            for i in range(len(sample_l)):
                empty = np.zeros(10)
                empty[sample_l[i]] = 1
                sample_l_vector.append(empty)
            for k in range(sample_size):
                print(k)
                layer_acivity_out = []
                z_values = []
                delta_b = []
                delta_w = []
                for i in range(len(self.shape) - 1):
                    delta_w.append(np.random.randn(self.shape[i + 1], self.shape[i]))
                for i in range(len(self.shape) - 1):
                    delta_b.append(np.random.randn(self.shape[i + 1]))
                # First layer of neural network returns simply the image data as output
                layer_acivity_out.append(sample_i[k])
                # Second layer input is the product of the weighting factor and first layer output plus the bias
                z_values.append(np.dot(self.weights[0], sample_i[k]) + self.biases[0])
                layer_acivity_out.append(sigmoid(z_values[0]))

                # For a general layer j the input is the output activation from the previous layer:
                for j in range(1, self.no_layers - 1):
                    z = np.dot(self.weights[j], layer_acivity_out[j]) + self.biases[j]
                    layer_acivity_out.append(sigmoid(z))
                    z_values.append(z)

                delta_w_s, delta_b_s = self.backpropagation(sample_l_vector[k], layer_acivity_out, z_values)
                delta_w = [dw + dws for dw, dws in zip(delta_w, delta_w_s)]
                delta_b = [db + dbs for db, dbs in zip(delta_b, delta_b_s)]

            # Adjust weights and biases:
            self.weights = [w + dw * (step_size / sample_size) for w, dw in zip(self.weights, delta_w)]
            self.biases = [b + db * (step_size / sample_size) for b, dw in zip(self.biases, delta_b)]
        print('network trained')

    def backpropagation(self, sample_l, layer_acivity_out, z_values):
        delta_b = []
        delta_w = []
        for i in range(len(self.shape) - 1):
            delta_w.append(np.random.randn(self.shape[i + 1], self.shape[i]))
        for i in range(len(self.shape) - 1):
            delta_b.append(np.random.randn(self.shape[i + 1]))

        print(np.shape(layer_acivity_out[-1]))
        delta_b[-1] = cost_derivative(sample_l, layer_acivity_out[-1]) * dif_sigmoid(z_values[- 1])
        delta_w[-1] = np.dot(delta_b[-1], layer_acivity_out[-2].transpose())

        for i in range(2, self.no_layers):
            print(i)
            error_previous_layer = np.multiply(cost_derivative(layer_acivity_out[-i + 1], layer_acivity_out[-i + 1]), dif_sigmoid(z_values[-i + 1]))
            delta_b[-i] = np.multiply(np.dot(self.weights[-i + 1].transpose(), error_previous_layer), dif_sigmoid(z_values[-i]))
            delta_w[-i] = np.multiply(np.dot(self.weights[-i + 1].transpose(), error_previous_layer), dif_sigmoid(z_values[-i])) * layer_acivity_out[-i]
        for i in range(2):
            print('w:', np.shape(delta_w[i]), np.shape(self.weights[i]))
            print('b', np.shape(delta_b[i]), np.shape(self.biases[i]))
        return delta_w, delta_b

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

net.stocastic_gradient_descent(training_images, training_labels, test_images, test_labels, 10, 1, 10)

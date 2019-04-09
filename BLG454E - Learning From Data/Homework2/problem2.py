# Kadir Emre Oto (150140032)
# BLG454E: Homework2 - Problem2
# Example Usage: python3 problem2.py

import numpy
from collections import Counter


class Classifier:
    def __init__(self, k=10):
        self.k = k
        self.fold_size = 0

        self.y = []  # holds real class values as string
        self.r = []  # holds real class values as matrix
        self.data = []  # holds features

        self.class_count = 0
        self.feature_count = 0

        self.class_index = {}  # holds the integer indices of class labels

        self.w = None
        self.accuracy = 0

    def load(self, filename):
        """takes a file to load the data"""

        with open(filename, 'r') as train:
            for line in train:
                if not line.strip():  # skip if line is empty
                    continue

                x1, x2, x3, x4, y = line.split(',')
                x1, x2, x3, x4 = map(float, (x1, x2, x3, x4))  # casting the features

                self.y.append(y)
                self.data.append([x1, x2, x3, x4])

                if y not in self.class_index:
                    self.class_count += 1
                    self.class_index[y] = len(self.class_index)

        for y in self.y:  # construct r matrix
            array = [0.] * self.class_count
            array[self.class_index[y]] = 1.

            self.r.append(array)

        self.feature_count = len(self.data[0])  # calculate feature_count
        self.fold_size = len(self.data) // self.k  # calculate fold size

    def fit(self, learning_rate=0.1, iteration_count=500):
        accuracy = 0
        results = {}  # for PART-A, holds number of predictions of classes

        w = numpy.random.rand(self.class_count, len(self.data[0]))  # create weight matrix with random values

        for index in range(0, len(self.data), self.fold_size):
            train = self.data[:index] + self.data[index + self.fold_size:]  # choose training data
            r_train = self.r[:index] + self.r[index + self.fold_size:]  # and its real class

            for _ in range(iteration_count):
                d_w = numpy.zeros((self.class_count, self.feature_count))  # declare delta w matrix with zeros

                for i in range(len(train)):
                    x = numpy.array(train[i])  # find current instance
                    r = numpy.array(r_train[i])  # and its real class

                    o = numpy.dot(w, x)
                    y = numpy.exp(o - o.max())  # to avoid RuntimeWarning: invalid value encountered in true_divide
                    # maximum value is subtracted

                    y /= y.sum()

                    f = (r - y).reshape(self.class_count, 1)
                    s = x.reshape(1, self.feature_count)

                    d_w += numpy.dot(f, s)  # re-calculate delta w matrix

                w += learning_rate * d_w  # edit w matrix

            test = self.data[index: index + self.fold_size]  # choose test data
            r_test = self.r[index: index + self.fold_size]  # and its real class

            for i in range(len(test)):
                x = numpy.array(test[i])  # find current instance
                r = numpy.array(r_test[i])  # and its real class

                o = numpy.dot(w, x)
                y = numpy.exp(o - o.max())
                y /= y.sum()

                if numpy.argmax(r) not in results:
                    results[numpy.argmax(r)] = Counter()
                results[numpy.argmax(r)][numpy.argmax(y)] += 1

                # increase accuracy if prediction is correct
                accuracy += (numpy.argmax(y) == numpy.argmax(r)) / (len(test) * self.k)

        if accuracy > self.accuracy:  # if current accuracy is better, change the weight matrix to use it later
            self.accuracy = accuracy
            self.w = w

        print(results)  # for PART-A
        return accuracy


if __name__ == '__main__':
    classifier = Classifier()
    classifier.load("iris.data.txt")

    ITERATION_COUNT = 250
    for p in range(-2, 2):
        lr = 10 ** p  # learning rate
        print('Learning rate: {}, Iteration Count: {}, Accuracy: {}'.format(lr, ITERATION_COUNT,
                                                                            classifier.fit(lr, ITERATION_COUNT)))

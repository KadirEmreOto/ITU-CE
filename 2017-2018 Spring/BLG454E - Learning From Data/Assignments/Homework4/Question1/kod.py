# Kadir Emre Oto (150140032)
# BLG454E: Homework4 - Problem1
# Example Usage: python3 kod.py

import csv
import numpy
import random


class Classifier:
    def __init__(self, unit=10):
        self.x = []  # holds features
        self.r = []  # holds real class values as matrix

        self.w = []  # random values between (-0.01, 0.01)
        self.v = []  # random values between (-0.01, 0.01)

        self.unit = unit

    def train(self, filename):
        """takes a file to load the data"""

        with open(filename, 'r') as train:
            train.readline()

            for line in csv.reader(train):
                self.x.append(list(map(int, line[:-1])))
                self.r.append([0]*10)
                self.r[-1][int(line[-1])] = 1

        self.w = numpy.random.rand(len(self.x[0]), self.unit) / 50 - 0.01
        self.v = numpy.random.rand(self.unit, 10) / 50 - 0.01

        self.x = numpy.array(self.x)
        self.r = numpy.array(self.r)

    def test(self, filename):
        x = []
        r = []

        c = 0
        with open(filename, 'r') as test:
            test.readline()

            for line in csv.reader(test):
                x.append(list(map(int, line[:-1])))
                r.append(int(line[-1]))

                if c > 10:
                    break
                c += 1

        y = self.predict(numpy.array(x))
        true = 0

        for i in range(len(r)):
            true += r[i] == y[i]

        return float(true) / len(r)  # accuracy

    @staticmethod
    def sigmoid(n):
        return 1 / (1 + numpy.exp(-n))

    def fit(self, learning_rate=0.01, iteration_count=1):
        error = 0

        for it in range(iteration_count):
            error = 0
            indices = list(range(self.x.shape[0]))
            random.shuffle(indices)

            for i in indices:
                x = self.x[i]
                r = self.r[i]

                z = Classifier.sigmoid(numpy.dot(self.w.T, x))  # (10, 64) * (64,1) -> (10,1)
                y = numpy.dot(self.v.T, z)  # (10, 10) * (10,1) -> (10,1)

                y = numpy.exp(y - y.max())
                y /= y.sum()

                self.w += (learning_rate * (((r - y) @ self.v.T) * (z * (1 - z))).reshape(-1, 1) @ x.reshape(1, -1)).T
                self.v += (learning_rate * (r - y).reshape(-1, 1) @ z.reshape(1, -1)).T

                error -= r @ numpy.log(y)

        return error

    def predict(self, x):
        z = Classifier.sigmoid(numpy.dot(self.w.T, x.T))  # (10, 64) * (64,1) -> (10,1)
        y = numpy.dot(self.v.T, z).T

        return numpy.argmax(y, axis=1)


if __name__ == '__main__':
    for count in [200]:
        classifier = Classifier()
        classifier.train("dataTrain.csv")
        error = classifier.fit(iteration_count=count)
        accuracy = classifier.test("dataTest.csv")

        print("Iteration Count:", count)
        print("Cross Entropy Error:", error)
        print("Accuracy:", accuracy)
        print("-"*10)

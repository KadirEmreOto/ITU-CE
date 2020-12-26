# Kadir Emre Oto (150140032)
# BLG454E: Homework2 - Problem1
# Example Usage: python3 problem1.py

import numpy
from math import log
import matplotlib.pyplot as plt


class Classifier:
    def __init__(self):
        self.class_counter = {}  # holds how many class instances exist
        self.x = {}  # holds x1 and x2 values for each class: self.x[class1] = [[x1 ..], [x2 ..]]

        self.cov_cache = {}  # this dictionaries reduce the complexity by dp (memoization)
        self.mean_cache = {}

    def train(self, filename):
        """takes a file to train the discriminant function"""

        with open(filename, 'r') as train:
            for index, line in enumerate(train):
                if index != 0:  # skip header
                    x1, x2, y = map(float, line.split(','))
                    y = int(y)

                    if y not in self.class_counter:  # it means this class is new
                        self.class_counter[y] = 0
                        self.x[y] = [[], []]

                    self.class_counter[y] += 1
                    self.x[y][0].append(x1)
                    self.x[y][1].append(x2)

        for i in range(len(self.x)):
            self.x[i] = numpy.column_stack(self.x[i])

    def test(self, filename):  # PART-C
        """takes a file to test the accuracy of discriminant function"""

        total = 0
        correct = 0

        with open(filename, 'r') as test:
            for index, line in enumerate(test):
                if index != 0:  # skip header
                    x1, x2, y = map(float, line.split(','))

                    total += 1
                    correct += self.guess([x1, x2]) == y

        return correct / total

    def covariance(self, i):
        """calculates the covariance matrix given class label"""

        if i in self.cov_cache:  # memoization
            return self.cov_cache[i]

        x = self.x[i] - self.x[i].mean(axis=0)
        self.cov_cache[i] = numpy.dot(x.T, x.conj()) / (len(x) - 1)  # save the result
        return self.cov_cache[i]

    def mean(self, i):
        """calculates the mean values of features for given class"""

        if i in self.mean_cache:  # memoization
            return self.mean_cache[i]

        self.mean_cache[i] = self.x[i].mean(axis=0)  # save the result
        return self.mean_cache[i]

    def g(self, x, i):  # PART-A
        """
        calculates the discriminant function

        :param x: x values
        :param i: class label
        """

        s = self.covariance(i)
        u = self.mean(i)

        # maybe numpy.linalg.inv(s) this value can be cached too, inverse operation has high complexity
        return -0.5 * numpy.matmul(numpy.matmul((x - u).T, numpy.linalg.inv(s)), (x - u)) - 0.5 * \
            log(numpy.linalg.det(s)) + log(self.class_prob(i))

    def class_prob(self, c):
        """calculates the probability of given class by using class counter"""

        return self.class_counter.get(c, 0) / sum(self.class_counter.values())  # if too many class exist, the sum -
        # value can be pre-calculated

    def guess(self, x: list):
        """takes x values and guesses their most likely class"""

        best = float('-inf')
        _class = None

        for i in self.x:
            d = self.g(x, i)

            if d > best:
                best = d
                _class = i

        return _class

    def plot(self, sensitivity=0.05):  # PART-B
        """
        draws the decision boundaries

        :param sensitivity: accrual of x and y values, so small is better
        """
        x_axis = numpy.arange(-6, 5, sensitivity)  # boundaries of x
        y_axis = numpy.arange(-2, 14, sensitivity)  # boundaries of y
        x_axis, y_axis = numpy.meshgrid(x_axis, y_axis)

        z = numpy.array([self.guess([x, y]) for x, y in numpy.c_[x_axis.ravel(), y_axis.ravel()]])

        plt.pcolormesh(x_axis, y_axis, z.reshape(x_axis.shape), cmap=plt.cm.winter)
        plt.show()


if __name__ == '__main__':
    classifier = Classifier()
    classifier.train("datatrain.csv")
    accuracy = classifier.test("datatest.csv")
    print('Accuracy: {}'.format(accuracy))
    classifier.plot()

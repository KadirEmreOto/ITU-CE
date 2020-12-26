# Kadir Emre Oto (150140032)
# BLG454E: Homework3 - PCA
# Example Usage: python3 pca.py
# PS: The program is not well-tested :(

import csv
import numpy


class PCA:
    def __init__(self):
        self.X = []
        self.Y = []

    def load(self, filename):
        with open(filename, "r") as data:
            for line in csv.reader(data):
                line = list(map(int, line))
                self.X.append(line[:-1])
                self.Y.append(line[-1])

        self.X = numpy.array(self.X)
        self.Y = numpy.array(self.Y)

    def fit(self, scale=2):
        X = self.X - self.X.mean(axis=0)

        sigma = numpy.cov(self.X)
        w, v = numpy.linalg.eig(sigma)  # w: eigenvalues, v: eigenvectors

        indices = numpy.argsort(w)[::-1]
        vectors = v[:, indices]
        values = w[indices]

        vectors = vectors[:, :scale]
        return numpy.dot(vectors.T, X).T, values, vectors


if __name__ == '__main__':
    pca = PCA()
    pca.load("data.txt")
    pca.fit(scale=2)

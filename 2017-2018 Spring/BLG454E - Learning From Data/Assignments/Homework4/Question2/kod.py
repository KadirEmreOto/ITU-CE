# Kadir Emre Oto (150140032)
# BLG454E: Homework4 - Problem2
# Example Usage: python3 kod.py

import csv
import numpy
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA

from cluster import KMeans  # my kmeans library


def load(filename):
    x = []

    with open(filename, "r") as file:
        file.readline()

        for line in csv.reader(file):
            x.append(list(map(int, line)))
            del x[-1][0]

    return x


def plot(cluster):
    """
    draws the decision boundaries

    :param cluster:
    :return:
    """

    x_axis = numpy.arange(cluster.borders[0][0]-1, cluster.borders[0][1]+1, 0.1)  # boundaries of x
    y_axis = numpy.arange(cluster.borders[1][0]-1, cluster.borders[1][1]+1, 0.1)  # boundaries of y
    x_axis, y_axis = numpy.meshgrid(x_axis, y_axis)

    z = numpy.array([cluster.predict([x, y]) for x, y in numpy.c_[x_axis.ravel(), y_axis.ravel()]])

    plt.pcolormesh(x_axis, y_axis, z.reshape(x_axis.shape), cmap=plt.cm.Paired)
    plt.show()


if __name__ == '__main__':
    data = load("Cluster.csv")

    pca = PCA(n_components=2)
    data = pca.fit_transform(data)

    results = {}
    for n in [1, 5, 10, 20]:
        kmeans = KMeans(n_clusters=n).fit(data)
        print("k:", n, "\terror:", kmeans.cluster_error_)

        results[kmeans] = kmeans.cluster_error_, kmeans

    plot(min(results, key=lambda x: results[x]))

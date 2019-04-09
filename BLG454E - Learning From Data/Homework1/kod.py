# Kadir Emre Oto (150140032) - Python 2

from math import pi, e, sqrt, exp
import matplotlib.pyplot as plt


def P(x):
    return 1. / (std * sqrt(2*pi)) * exp(-pow(x - mean, 2) / (2*variance))


with open("data.txt", "r") as f:
    data = [float(x) for x in f]


mean = sum(data) / len(data)
variance = sum((x - mean)**2 for x in data) / len(data)
std = variance ** 0.5

x = map(lambda x: x / 100., range(1800))
probs = map(P, x)

plt.hist(data, 30, normed=1, facecolor='g', alpha=0.75)
plt.plot(x, probs, 'k', linewidth=2)

title = "mean = {:.4f}, variance = {:.4f} std = {:.4f}".format(mean, variance, std)
plt.title(title)

plt.show()
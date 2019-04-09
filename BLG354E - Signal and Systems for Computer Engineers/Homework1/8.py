from math import cos, sin, pi
import matplotlib.pyplot as plt


def f(t):
    return 2 + 4 * cos(500*pi*t + 5/4*pi) + 3 * (cos(250*pi*(t - pow(10, -3))))


x = list(range(10000))
y = list(map(f, x))

plt.plot(x, y)
plt.title("Homework")
plt.show()

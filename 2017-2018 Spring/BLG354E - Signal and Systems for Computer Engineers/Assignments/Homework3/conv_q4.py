# Kadir Emre Oto (150140032)
# BLG354E - Homework3 - Question 4

import numpy as np
import matplotlib.pyplot as plt


fg, ax = plt.subplots(1, 1)

n = 1000
t = np.linspace(0, 1, n)
T = t[1] - t[0]

x = np.where(t <= 1, 1, 0)
y = x

ax.plot(t, x, label="$x$")
m = n

for i in range(1, 11):
    t = np.linspace(0, 2*i, n + m - 1)  # calculate new linspace

    y = np.convolve(y, x) * T  # y = x ^ i
    ax.plot(t, y, label="$x ^ {%s}$" % i)
    m = n + m - 1  # new size of linspace

ax.legend(loc='best')
ax.grid(True)

fg.canvas.draw()
plt.show()

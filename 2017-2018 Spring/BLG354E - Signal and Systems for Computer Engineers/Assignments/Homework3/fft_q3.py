# Kadir Emre Oto (150140032)
# BLG354E - Homework3 - Question 3
# Reference: https://gist.github.com/bistaumanga/5682774

import sys
import time
import random
from cmath import exp, pi, isclose
from numpy.fft import fft  # built-in fft function


def omega(p, q):
    """ The omega term in DFT and IDFT formulas"""
    return exp((2.0 * pi * 1j * q) / p)


def myfft(x):
    """ FFT of 1-d signals
    usage : X = fft(x)
    where input x = list containing sequences of a discrete time signals
    and output X = dft of x """

    size = len(x)  # size of array
    if size == 1:  # if array size is 1, return the array itself
        return x

    even, odd = myfft(x[0::2]), myfft(x[1::2])  # find even and odd parts of array and call ff
    combined = [0] * size  # create an empty array to hold combination of even and odd parts

    for m in range(size // 2):  # combine even and odd parts
        combined[m] = even[m] + omega(size, -m) * odd[m]
        combined[m + size // 2] = even[m] - omega(size, -m) * odd[m]

    return combined


def issame(ar1, ar2):
    for i in range(len(ar1)):
        if not isclose(ar1[i], ar2[i]):  # results can be double, so direct comparison is not accurate
            return False

    return True


if __name__ == '__main__':
    n = int(sys.argv[-1])  # take the input size from command-line
    array = [random.randint(0, 1) for i in range(n)]  # create a random array with zeros and ones

    start = time.time()
    f1 = list(map(abs, fft(array)))
    mid = time.time()
    f2 = list(map(abs, myfft(array)))
    end = time.time()

    print("Array size = {}".format(n))
    print("Built-in fft algorithm tooks {} seconds".format(mid - start))
    print("My fft algorithm tooks {} seconds".format(end - mid))
    print("Results are{} the same".format(" not" if not issame(f1, f2) else ""))
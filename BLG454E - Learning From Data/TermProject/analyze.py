import csv
import random


def analyze(filename):
    indices = [3, 6, 7, 8]
    f = [[] for i in range(len(indices))]

    with open(filename, 'r') as data:
        h = data.readline().split(',')

        for line in csv.reader(data):
            for j, i in enumerate(indices):
                f[j].append(float(line[i]))

    for j, i in enumerate(indices):
        print(h[i], len(f[j]), min(f[j]), max(f[j]))


def correct(filename):
    pass


# analyze("test.txt")

print("orderItemID,returnShipment")
for i in range(1, 50079):
	print('{},{}'.format(i, random.randint(0, 1)))

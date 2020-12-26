import sys
import random


N = int(sys.argv[-1])
print N

for i in xrange(N):
	x, y, z = random.randint(1, 10000), random.randint(1, 10000), random.randint(1, 10000)
	print x, y, z
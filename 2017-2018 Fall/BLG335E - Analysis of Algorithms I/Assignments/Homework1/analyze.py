
counter = [set() for i in xrange(6)]
maxxxxx = [0 for i in xrange(6)]
filepath = 'hs-sets/hs-set-1M.txt'
lines = set()
count = 0

with open(filepath, 'r') as file:
	for line in file:
		lines.add(line)
		count += 1
		a = map(lambda x: x.strip(), line.split('\t'))
		for i in xrange(len(a)):
			counter[i].add(a[i])
			maxxxxx[i] = max(maxxxxx[i], len(a[i]))

for i in xrange(6):
	print 'c:', len(counter[i]), 'm:', maxxxxx[i]

print 'Line ->', len(lines)
print count
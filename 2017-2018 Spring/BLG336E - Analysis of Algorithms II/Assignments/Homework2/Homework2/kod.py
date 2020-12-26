
def dist(u, v):
	d = (u[0] - v[0]) ** 2
	d += (u[1] - v[1]) ** 2
	d += (u[2] - v[2]) ** 2

	return d ** 0.5

N = int(raw_input())
P = [map(int, raw_input().split()) for i in xrange(N)]

ans = None
best = 1e10

for i in xrange(N):
	for j in xrange(i+1, N):
		d = dist(P[i], P[j])
		if d < best:
			best = d
			ans = P[i], P[j]

print ans, best
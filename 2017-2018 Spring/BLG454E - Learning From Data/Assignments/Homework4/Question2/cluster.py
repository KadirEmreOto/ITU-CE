import random


class Mean:
    def __init__(self, axis=None):
        self.axis = axis
        self.points = []

    def add_point(self, point):
        self.points.append(point)

    def clear_points(self):
        self.points = []

    def recalculate_axis(self):
        if not self.points:
            return

        x = 0.
        y = 0.

        for point in self.points:
            x += point[0]
            y += point[1]

        x /= len(self.points)
        y /= len(self.points)

        if (x, y) == self.axis:
            return False

        self.axis = (x, y)
        return True

    def sum_of_squared_error(self):
        error = 0

        for point in self.points:
            error += self.distance(point)

        return error

    def distance(self, point):
        return (self.axis[0] - point[0]) ** 2 + (self.axis[1] - point[1]) ** 2


class KMeans:
    def __init__(self, n_clusters=2):
        self.n_clusters = n_clusters

        self.data = []
        self.means = []
        self.borders = [[float('inf'), float('-inf')], [float('inf'), float('-inf')]]

    def fit(self, data, max_iteration=100):
        self.data = data

        for x, y in data:
            self.borders[0][0] = int(min(self.borders[0][0], x))
            self.borders[0][1] = int(max(self.borders[0][1], x))

            self.borders[1][0] = int(min(self.borders[1][0], y))
            self.borders[1][1] = int(max(self.borders[1][1], y))

        for i in range(self.n_clusters):
            x = random.randint(*self.borders[0])
            y = random.randint(*self.borders[1])

            self.means.append(Mean(axis=(x, y)))

        while max_iteration > 0 and self.group():
            max_iteration -= 1

        return self

    def group(self):
        for mean in self.means:
            mean.clear_points()

        for point in self.data:
            label = self.predict(point)
            self.means[label].add_point(point)

        flag = False
        for mean in self.means:
            if mean.recalculate_axis():
                flag = True

        return flag

    @property
    def cluster_centers_(self):
        centers = []
        for mean in self.means:
            centers.append(mean.axis)
        return centers

    @property
    def cluster_errors_(self):
        errors = []
        for mean in self.means:
            errors.append(mean.sum_of_squared_error())
        return errors

    @property
    def cluster_error_(self):
        error = 0
        for mean in self.means:
            error += mean.sum_of_squared_error()
        return error

    def predict(self, point):
        label = 0
        dist = float('inf')

        for i, m in enumerate(self.means):
            d = m.distance(point)
            if d < dist:
                dist = d
                label = i

        return label


if __name__ == '__main__':
    kmeans = KMeans()
    points = [[1, 2], [1, 4], [1, 0], [4, 2], [4, 4], [4, 0]]
    kmeans.fit(points)

    print(kmeans.cluster_centers_)

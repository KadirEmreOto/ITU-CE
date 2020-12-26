import csv
from sklearn.neural_network import MLPClassifier



class Classifier:
    def __init__(self):
        self.x = []
        self.y = []
        self.classifier = MLPClassifier(solver='lbfgs', alpha=1e-5, hidden_layer_sizes=(5, 2), random_state=1)

    def fit(self):
        self.classifier.fit(self.x, self.y)

    def load(self, filename):
        with open(filename, 'r') as data:
            data.readline()

            for line in csv.reader(data):
                self.convert(line)
                self.x.append(line[:-1])
                self.y.append(line[-1])

    def test(self, filename):
        features = []
        with open(filename, 'r') as data:
            data.readline()

            for line in csv.reader(data):
                self.convert(line)

                features.append(line)

        results = self.classifier.predict(features)

        print("orderItemID,returnShipment")
        for i in range(len(features)):
            print("{},{}".format(features[i][0], results[i]))

    @staticmethod
    def convert(rows):
        rows[0] = int(rows[0])
        rows[1] = int(rows[1].replace("-", "")) if len(rows[1]) > 2 else 0
        rows[2] = int(rows[2].replace("-", "")) if len(rows[2]) > 2 else 0
        rows[3] = int(rows[3])
        rows[4] = hash(rows[4])
        rows[5] = hash(rows[5])
        rows[6] = int(rows[6])
        rows[7] = float(rows[7])
        rows[8] = int(rows[8])
        rows[9] = hash(rows[9])
        rows[10] = int(rows[10].replace("-", "")) if len(rows[10]) > 2 else -1
        rows[11] = hash(rows[11])
        rows[12] = int(rows[12].replace("-", "")) if len(rows[12]) > 2 else -1

        if len(rows) == 14:
            rows[13] = int(rows[13])



if __name__ == '__main__':
    classifier = Classifier()
    classifier.load("train.txt")
    classifier.fit()
    classifier.test("test.txt")

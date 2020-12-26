from sklearn.neural_network import MLPClassifier
from sklearn.svm import SVC, LinearSVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.metrics import classification_report,confusion_matrix

import numpy as np
import matplotlib.pyplot as plt

import math
import csv
from datetime import datetime
from datetime import timedelta
from random import shuffle
from collections import defaultdict

class Classifier:
    def __init__(self, train_data, submission_data):
        self.train_data = train_data
        self.submission_data = submission_data
    
    @staticmethod
    def convert_age(age):
        ages = [13, 18, 24, 30, 40, 60]
        for i in range(6):
            if age <= ages[i]:
                break
        return i
    
    @staticmethod
    @np.vectorize
    def convert_delivery(time):
        times = [1, 2, 3, 7, 14, 30, 60]
        for i in range(7):
            if time <= times[i]:
                break
        return i

    @staticmethod
    def check_day(order_date, delivery_date):
        if (order_date - datetime(order_date.year, 2, 14)).days <= 30 and (delivery_date - datetime(order_date.year, 2, 14)).total_seconds() > 0:
            return 1
        return 0

    def process(self, X, y):
        X = np.vstack(X)
        scaler = StandardScaler()
        scaler.fit(X)
        X = scaler.transform(X)

        # pca = PCA(n_components=22)
        # X = pca.fit_transform(X)
        # N_features = X.shape[1]

        return X, y

    def train(self):
        X, y = self.load_data()

        self.X, self.y = self.process(X, y)

        X_train, X_test, y_train, y_test = train_test_split(self.X, self.y, test_size=0.2)

        # self.model = MLPClassifier(hidden_layer_sizes=(100, 50, 50), solver='adam', verbose=True, alpha=0.001)
        self.model = RandomForestClassifier(n_estimators=400, verbose=True, max_depth=10, n_jobs=10)
        # self.model = SVC(verbose=True)
        # self.model = LinearSVC(verbose=True)

        print(self.model.fit(X_train, y_train))

        print(self.model.feature_importances_)

        predictions = self.model.predict(X_test)

        print(confusion_matrix(y_test, predictions))
        print(classification_report(y_test, predictions))

        score = self.model.score(X_test, y_test)
        print(score)
        self.plot_importances()
        return score
        
    def create_submission(self, score):
        X_test, y_test = self.read_data(self.submission_data, test=True)

        X_test, y_test = self.process(X_test, y_test)

        predictions = self.model.predict(np.array(X_test))

        with open('submission-{}.txt'.format(score), 'w') as f:
            f.write('orderItemID,returnShipment\n')
            f.write('\n'.join((str(x) + ',' + str(float(y))
                               for x, y in zip(range(1, len(X_test) + 1), predictions))))

    def load_data(self):
        try:
            X, y = np.load('data.npy')
        except:
            X, y = self.read_data(self.train_data)
            np.save('data.npy', (X, y))

        self.X = X
        self.y = y
        return X, y

    def plot_importances(self):
        
        importances = self.model.feature_importances_
        std = np.std([tree.feature_importances_ for tree in self.model.estimators_], axis=0)
        indices = np.argsort(importances)[::-1]

        # Plot the feature importances of the forest
        plt.figure()
        plt.title("Feature importances")
        plt.bar(range(self.X.shape[1]), importances[indices],
                color="r", yerr=std[indices], align="center")
        plt.xticks(range(self.X.shape[1]), indices)
        plt.xlim([-1, self.X.shape[1]])
        plt.show()

    @staticmethod
    def risk_by(df, attribute):
        r1 = defaultdict(int)
        r0 = defaultdict(int)
        r1.update(df[df['returnShipment'] == 1].groupby(attribute).size())
        r0.update(df[df['returnShipment'] == 0].groupby(attribute).size())

        risks = defaultdict(int)

        for i in range(1, 3072):
            risks[i] = 1 - ((r0[i] + 50) / (r0[i] + r1[i] + 100))
        
        return risks
    
    def read_data(self, filename, test=False):
        with open(filename) as f:
            X = []
            y = []

            names = f.readline()

            X = np.array(list(csv.reader(f)))

            print('Read X:', X.shape)

            if not test:
                y = X[:, -1]
                X = X[:, :-1]

            mean = [2.41475409e+05,   1.86300249e+10,   1351045637,   1.40798420e+03,
                    2.86151477e+18,   1.77485647e+18,   2.98725436e+01,   6.99355493e+01,
                    3.24335507e+04,   4.44847020e+17,   170449750,  -1.87869032e+18,
                    1.86031415e+10]

            sizes = {size:ix for ix, size in enumerate(set(X[:, 4]))}
            colors = {color:ix for ix, color in enumerate(set(X[:, 5]))}
            salutations = {salutations: ix for ix, salutations in enumerate(set(X[:, 9]))}
            states = {state:ix for ix, state in enumerate(set(X[:, 11]))}

            colors['?'] = colors['black']

            add = []
            delivery_time = np.array([])
            age = np.array([])
            member_age = np.array([])
            special = np.array([])
            
            import pandas as pd
            df = pd.read_csv("train.txt")
            
            risk_by_item = self.risk_by(df,'itemID')
            risk_by_customer = self.risk_by(df,'customerID')
            risk_by_manufacturer = self.risk_by(df,'manufacturerID')

            for x in X:
                tmp = np.array([])
                tmp = np.append(tmp, x[1].split('-'))

                if x[2] != '?' and datetime(2012, 1, 1) < datetime.strptime(x[2], '%Y-%m-%d') < datetime(2014, 1, 1):
                    tmp = np.append(tmp, x[2].split('-'))
                else:
                    tmp = np.append(tmp, datetime.fromtimestamp(mean[2]).strftime('%Y-%m-%d').split('-'))
                    x[2] = datetime.fromtimestamp(mean[2]).strftime('%Y-%m-%d')

                x[4] = sizes[x[4]]
                x[5] = colors[x[5]]
                x[9] = salutations[x[9]]

                if x[10] != '?' and datetime(1970, 1, 1) < datetime.strptime(x[10], '%Y-%m-%d') < datetime.now():
                    tmp = np.append(tmp, x[10].split('-'))
                else:
                    tmp = np.append(tmp, datetime.fromtimestamp(mean[10]).strftime('%Y-%m-%d').split('-'))
                    x[10] = datetime.fromtimestamp(mean[2]).strftime('%Y-%m-%d')

                x[11] = states[x[11]]

                tmp = np.append(tmp, (*map(int, x[12].split('-')),
                                    self.convert_delivery((datetime.strptime(x[2], '%Y-%m-%d') - datetime.strptime(x[1], '%Y-%m-%d')).days),
                                    self.convert_age((datetime.now() - datetime.strptime(x[10], '%Y-%m-%d')).days // 365),
                                    (datetime.now() - datetime.strptime(x[12], '%Y-%m-%d')).days,
                                    self.check_day(datetime.strptime(x[1], '%Y-%m-%d'), datetime.strptime(x[2], '%Y-%m-%d')),
                                    risk_by_item[int(x[3])],
                                    risk_by_customer[int(x[8])],
                                    risk_by_manufacturer[int(x[6])]))
                add.append(tmp)
                

            # delivery_time = np.array([self.convert_delivery((datetime.strptime(x[2], '%Y-%m-%d') - datetime.strptime(x[1], '%Y-%m-%d')).days) for x in X])
            # age = np.array([self.convert_age((datetime.now() - datetime.strptime(x, '%Y-%m-%d')).days // 365) for x in X[:, 10]])
            # member_age = np.array([(datetime.now() - datetime.strptime(x[12], '%Y-%m-%d')).days for x in X])
            # special = np.array([self.check_day(datetime.strptime(x[1], '%Y-%m-%d'), datetime.strptime(x[2], '%Y-%m-%d')) for x in X])
            
            X = np.delete(X, [0, 1, 2, 8, 10, 12], axis=1)
            X = np.hstack((X, np.array(add)))
            print('New X', X.shape)

            X = list(X.astype(float))
            return X, y

    
if __name__ == '__main__':

    clf = Classifier('train.txt', 'test.txt')
    score = clf.train()
    clf.create_submission(score)

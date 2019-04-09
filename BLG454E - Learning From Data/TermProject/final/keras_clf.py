from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
import numpy as np
import csv
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.utils import np_utils
from datetime import datetime, timedelta
from collections import defaultdict

weight_file = 'model.weights.{epoch:03d}-{val_acc:.3f}.hdf5'
N_features = 14

class Classifier:
    def __init__(self, train_data, submission_data):
        self.train_data = train_data
        self.submission_data = submission_data
        
    def load_weights(self):
        self.model.load_weights('model.weights.050-0.635.hdf5')

    def get_model(self):
        model = Sequential()
        model.add(Dense(1024, input_shape=(26,)))
        model.add(Activation('relu'))
        model.add(Dropout(0.5))
        model.add(Dense(1024))
        model.add(Activation('relu'))
        model.add(Dropout(0.5))
        model.add(Dense(512))
        model.add(Activation('relu'))
        model.add(Dropout(0.5))
        model.add(Dense(256))
        model.add(Activation('relu'))
        model.add(Dropout(0.5))
        model.add(Dense(1))
        model.add(Activation('sigmoid'))
        return model

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

        X_train, X_test, y_train, y_test = train_test_split(self.X, self.y)

        mcCallBack = keras.callbacks.ModelCheckpoint(weight_file, monitor='val_acc', verbose=0, save_best_only=False,
                                                    save_weights_only=False, mode='auto', period=1)

        self.model = self.get_model()
        self.model.compile(loss='binary_crossentropy',
                           optimizer=keras.optimizers.Adam(),
                           metrics=['accuracy'])
        try:
            self.model.fit(X_train, y_train,
                            batch_size=512,
                            epochs=100,
                            shuffle=True,
                            callbacks=[mcCallBack],
                            validation_data=[X_test, y_test])
        except:
            print('Cancelled')
        
    def create_submission(self):
        X_test, y_test = self.read_data(self.submission_data, test=True)

        X_test, y_test = self.process(X_test, y_test)

        predictions = self.model.predict(np.array(X_test))
        
        with open('submission.txt', 'w') as f:
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
            print(X[0:5])
            X = list(X.astype(float))
            return X, y



if __name__ == '__main__':
    clf = Classifier('train.txt', 'test.txt')
    clf.train()
    clf.create_submission()
    
    




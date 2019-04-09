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

weight_file = 'model.weights.{epoch:03d}-{val_acc:.3f}.hdf5'
N_features = 15


class Classifier:
    def __init__(self, train_data, submission_data):
        self.train_data = train_data
        self.submission_data = submission_data
        
    def load_weights(self):
        self.model.load_weights('model.weights.050-0.635.hdf5')

    def get_model(self):
        model = Sequential()
        model.add(Dense(50, input_shape=(14,)))
        model.add(Activation('relu'))
        model.add(Dropout(0.3))
        model.add(Dense(10))
        model.add(Activation('relu'))
        model.add(Dropout(0.3))
        model.add(Dense(1))
        model.add(Activation('sigmoid'))
        return model

    def process(self, X, y):
        X = np.vstack(X)

        delivery_time = X[:, 2] - X[:, 1]
        age = np.array([(datetime.fromtimestamp(x) - datetime.now()).days // 365 for x in X[:, 10]])
        member_age = np.array([(datetime.fromtimestamp(x[1]) - datetime.fromtimestamp(x[12])).days for x in X])

        X = np.hstack((X[:, 1:], delivery_time.reshape(-1, 1), age.reshape(-1, 1), member_age.reshape(-1, 1)))

        scaler = StandardScaler()
        scaler.fit(X)
        X = scaler.transform(X)

        pca = PCA(n_components='mle', svd_solver='full')
        X = pca.fit_transform(X)
        N_features = X.shape[1]

        return X, y

    def train(self):
        X, y = self.load_data()

        self.X, self.y = self.process(X, y)

        X_train, X_test, y_train, y_test = train_test_split(self.X, self.y, test_size=0.1)

        mcCallBack = keras.callbacks.ModelCheckpoint(weight_file, monitor='val_acc', verbose=0, save_best_only=False,
                                                    save_weights_only=False, mode='auto', period=1)

        self.model = self.get_model()
        self.model.compile(loss='binary_crossentropy',
                           optimizer=keras.optimizers.Adam(lr=0.001),
                           metrics=['accuracy'])

        self.model.fit(X_train, y_train,
                        batch_size=64,
                        epochs=50,
                        shuffle=True,
                        callbacks=[mcCallBack],
                        validation_data=[X_test, y_test])

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
            X, y = self.read_data(train_data)
            np.save('data.npy', (X, y))

        self.X = X
        self.y = y
        return X, y

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

            mean = [2.41475409e+05,   1.86300249e+10,   1.86236642e+10,   1.40798420e+03,
                    2.86151477e+18,   1.77485647e+18,   2.98725436e+01,   6.99355493e+01,
                    3.24335507e+04,   4.44847020e+17,   1.70987542e+10,  -1.87869032e+18,
                    1.86031415e+10]

            for x in X:
                x[1] = datetime.strptime(x[1], '%Y-%m-%d').timestamp()

                if x[2] != '?' and datetime(2012, 1, 1) < datetime.strptime(x[2], '%Y-%m-%d') < datetime(2014, 1, 1):
                    x[2] = datetime.strptime(x[2], '%Y-%m-%d').timestamp()
                else:
                    x[2] = mean[2]

                x[4] = hash(x[4])
                x[5] = hash(x[5]) if x[5] != '?' else hash('black')
                x[9] = hash(x[9])

                if x[10] != '?' and datetime(1970, 1, 1) < datetime.strptime(x[10], '%Y-%m-%d') < datetime.now():
                    x[10] = datetime.strptime(x[10], '%Y-%m-%d').timestamp()
                else:
                    x[10] = mean[10]
                x[11] = hash(x[11])
                x[12] = datetime.strptime(x[12], '%Y-%m-%d').timestamp()

            X = list(X.astype(float))
            return X, y


if __name__ == '__main__':
    clf = Classifier('train.txt', 'test.txt')
    clf.train()
    clf.create_submission()
    
    




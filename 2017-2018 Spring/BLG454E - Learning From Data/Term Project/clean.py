import csv
import numpy as np
from datetime import datetime


def convert_age(age):
    ages = [13, 18, 24, 30, 40, 60]
    for i in range(6):
        if age <= ages[i]:
            break
    return i

@np.vectorize
def convert_delivery(time):
    times = [1, 2, 3, 7, 14, 30, 60]
    for i in range(7):
        if time <= times[i]:
            break
    return i

def read_data(filename, test=False):
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

        sizes = {size:ix+1 for ix, size in enumerate(set(X[:, 4]))}
        colors = {color:ix+1 for ix, color in enumerate(set(X[:, 5]))}
        salutations = {salutations: ix+1 for ix, salutations in enumerate(set(X[:, 9]))}
        states = {state:ix+1 for ix, state in enumerate(set(X[:, 11]))}

        colors['?'] = colors['black']

        add = []

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
            tmp = np.append(tmp, x[12].split('-'))
            add.append(tmp)


        delivery_time = np.array([convert_delivery((datetime.strptime(x[2], '%Y-%m-%d') - datetime.strptime(x[1], '%Y-%m-%d')).days) for x in X])
        age = np.array([convert_age((datetime.now() - datetime.strptime(x, '%Y-%m-%d')).days // 365) for x in X[:, 10]])
        member_age = np.array([(datetime.now() - datetime.strptime(x[12], '%Y-%m-%d')).days for x in X])
        
        X = np.delete(X, [1, 2, 10, 12], axis=1)
        X = np.hstack((X[:, 1:], delivery_time.reshape(-1, 1), age.reshape(-1, 1), member_age.reshape(-1, 1), np.array(add)))

        print('New X', X.shape)

        X = list(X.astype(float))
        return X, list(y)


limit = 1000

X, Y = read_data("train.txt")
X, Y, X_ac, Y_ac = X[:-limit], Y[:-limit], X[-limit:], Y[-limit:]
X_test, Y_test = read_data("test.txt", True)

print(len(X), len(Y), len(X_ac), len(Y_ac), len(X_test), len(Y_test))

with open("cleaned_train.txt", "w") as f:
    f.write("orderItemID,orderDate,deliveryDate,itemID,size,color,manufacturerID,price,customerID,salutation,dateOfBirth,state,creationDate,returnShipment\n")
    for i in range(1, len(X) + 1):
        f.write("{},{}\n".format(','.join(map(str, X[i-1])), Y[i-1]))

with open("cleaned_test.txt", "w") as f:
    f.write("orderItemID,orderDate,deliveryDate,itemID,size,color,manufacturerID,price,customerID,salutation,dateOfBirth,state,creationDate,returnShipment\n")
    for i in range(1, len(X_ac) + 1):
        f.write("{},{}\n".format(','.join(map(str, X_ac[i-1])), Y_ac[i-1]))

with open("cleaned_sub.txt", "w") as f:
    f.write("orderItemID,orderDate,deliveryDate,itemID,size,color,manufacturerID,price,customerID,salutation,dateOfBirth,state,creationDate\n")
    for i in range(1, len(X_test) + 1):
        f.write("{}\n".format(','.join(map(str, X_test[i-1]))))



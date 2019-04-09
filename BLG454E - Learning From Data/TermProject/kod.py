import csv
from datetime import datetime
import tensorflow as tf
import numpy as np


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
            y = [[(int(i) ^ 1), int(i)] for i in y]

        X = list(X.astype(float))
        return X, y


# Generate tain and test data
X, Y = read_data("cleaned_train.txt")
X_ac, Y_ac = read_data("cleaned_test.txt")
X_test, Y_test = read_data("cleaned_sub.txt", True)

learning_rate = 0.001
training_epochs = 7
batch_size = 100
display_step = 1


# Network Parameters
n_hidden_1 = 10 # 1st layer number of features
n_hidden_2 = 10 # 2nd layer number of features
n_input = 23 # Number of feature
n_classes = 2 # Number of classes to predict


# tf Graph input
x = tf.placeholder("float", [None, n_input])
y = tf.placeholder("float", [None, n_classes])

# Create model
def multilayer_perceptron(x, weights, biases):
    # Hidden layer with RELU activation
    layer_1 = tf.add(tf.matmul(x, weights['h1']), biases['b1'])
    layer_1 = tf.nn.relu(layer_1)
    # Hidden layer with RELU activation
    layer_2 = tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
    layer_2 = tf.nn.relu(layer_2)
    # Output layer with linear activation
    out_layer = tf.matmul(layer_2, weights['out']) + biases['out']
    return out_layer

# Store layers weight & bias
weights = {
    'h1': tf.Variable(tf.random_normal([n_input, n_hidden_1])),
    'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
    'out': tf.Variable(tf.random_normal([n_hidden_2, n_classes]))
}

biases = {
    'b1': tf.Variable(tf.random_normal([n_hidden_1])),
    'b2': tf.Variable(tf.random_normal([n_hidden_2])),
    'out': tf.Variable(tf.random_normal([n_classes]))
}

# Construct model
pred = multilayer_perceptron(x, weights, biases)

# Define loss and optimizer
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=y))
optimizer = tf.train.AdagradOptimizer(learning_rate=learning_rate).minimize(cost)

# Initializing the variables
init = tf.global_variables_initializer()

with tf.Session() as sess:
    sess.run(init)
    # Training cycle
    for epoch in range(training_epochs):
        avg_cost = 0.
        total_batch = int(len(X)/batch_size)
        X_batches = np.array_split(X, total_batch)
        Y_batches = np.array_split(Y, total_batch)
        # Loop over all batches
        for i in range(total_batch):
            batch_x, batch_y = X_batches[i], Y_batches[i]
            # Run optimization op (backprop) and cost op (to get loss value)
            _, c = sess.run([optimizer, cost], feed_dict={x: batch_x,
                                                          y: batch_y})
            # Compute average loss
            avg_cost += c / total_batch
        # Display logs per epoch step
        if epoch % display_step == 0:
            print("Epoch:", '%04d' % (epoch+1), "cost=", "{:.9f}".format(avg_cost))
    print("Optimization Finished!")

    correct_prediction = tf.equal(tf.argmax(pred, 1), tf.argmax(y, 1))
    # Calculate accuracy
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))
    print("Accuracy:", accuracy.eval({x: X_ac, y: Y_ac}))

    result = sess.run(pred, feed_dict={x: X_test})
    with open("submission.csv", "w") as f:
        f.write("orderItemID,returnShipment\n")
    
        for i in range(len(X_test)):
            f.write("{},{}\n".format(i+1, 1 if result[i][0] < result[i][1] else 0))


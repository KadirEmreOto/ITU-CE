
def my_conv(x, h):
    m = [[0] * (len(x) + len(h) - 1) for _ in range(len(h))]
    y = [0] * (len(x) + len(h) - 1)

    for i in range(len(h)):
        for j in range(len(x)):
            m[i][i+j] = h[i] * x[j]

    for i in range(len(x) + len(h) - 1):
        for j in range(len(h)):
            y[i] += m[j][i]

    return y


if __name__ == '__main__':
    X = [2, 4, 6, 4, 2]
    H = [3, -1, 2, 1]

    print(my_conv(X, H))


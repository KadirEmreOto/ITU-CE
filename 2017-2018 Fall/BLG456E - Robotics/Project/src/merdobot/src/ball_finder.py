#!/usr/bin/python
from scipy.ndimage import measurements, morphology
from numpy import array, ones
from PIL import Image
from listener import Listener

listener = Listener()
listener.start()


def is_grey(pixel):
    return abs(pixel[0]-pixel[1]) <= 10 and abs(pixel[1]-pixel[2]) <= 10


def is_equal(a, b):
    return (a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) + (a[2]-b[2])*(a[2]-b[2]) < 900


def ball_control(image_path):
    # http://en.wikipedia.org/wiki/Mathematical_morphology
    # load image and threshold to make sure it is binary
    img = Image.open(image_path)
    width, height = img.size
    pixels = list(img.getdata())
    pixels = [pixels[i * width:(i + 1) * width] for i in range(height)]
    colours = {(10, 10, 10): 0}

    for i in xrange(height):
        for j in xrange(width):
            if is_grey(pixels[i][j]):
                continue
            if pixels[i][j] not in colours:
                colours[pixels[i][j]] = 1
            else:
                colours[pixels[i][j]] += 1

    colour = max(colours, key=colours.get)

    im = array(Image.open(image_path).convert('L'))
    im = 1*(im < 128)

    labels, nbr_objects = measurements.label(im)

    # morphology - opening to separate objects better
    im_open = morphology.binary_opening(im, ones((9, 5)), iterations=2)

    labels_open, nbr_objects_open = measurements.label(im_open)

    found = False

    if nbr_objects > 0:
        if colour[1] + colour[2] < colour[0] and listener.color == 'red':
            found = True
        if colour[0] + colour[2] < colour[1] and listener.color == 'green':
            found = True
        if colour[0] + colour[1] < colour[2] and listener.color == 'blue':
            found = True
        if colour[0] > 75 and colour[1] > 75 and colour[2] < 50 and listener.color == 'yellow':
            found = True
        if colour[0] > 75 and colour[2] > 75 and colour[1] < 50 and listener.color == 'pink':
            found = True

    if found:
        x = 0
        cnt = 0
        for i in xrange(height):
            for j in xrange(width):
                if is_grey(pixels[i][j]):
                    continue
                if is_equal(pixels[i][j], colour):
                    x += j
                    cnt += 1
        return True, (float(x)/cnt)/width, float(cnt)/(height*width)

    return False, None, None

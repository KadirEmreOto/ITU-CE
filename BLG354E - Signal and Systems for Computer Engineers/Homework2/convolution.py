
import numpy
import scipy.signal
import matplotlib.image as mpimg
from skimage.color import rgb2gray

K = numpy.array([[1 / 9.] * 3 for i in range(3)])
img = mpimg.imread('noisyCameraman.png')

convolved = scipy.signal.convolve2d(rgb2gray(img), K, mode='same', boundary='fill', fillvalue=0)
scipy.misc.imsave('result.png', convolved)


# BLG354E - Homework-4
# Kadir Emre Oto (150140032)
# Run: python3 kod.py

import numpy
import matplotlib.pyplot as plt

from scipy import signal
from scipy.io import wavfile, loadmat


class AMCom:
    def __init__(self, mat, record):
        self.mat = loadmat(mat)
        self.frequency_response = signal.freqz(self.mat['b'][0])

        self.sampling_frequency, self.signal_data = wavfile.read(record)
        self.cut_signal(28000, 58000)

    def cut_signal(self, begin, end):
        self.signal_data = self.signal_data[begin:end]

    def filter(self, x):
        return signal.convolve(x, self.mat['b'][0])

    def modulate(self, x, w, q=0):
        """
        modulator (at the same time demodulator) function

        :param x: signal data
        :param w: frequency
        :param q: phase
        :return: output
        """

        y = [numpy.cos(2 * numpy.pi * w * (i / self.sampling_frequency) + q) for i in range(0, len(x))]
        return signal.convolve(x, y)

    def plot(self):
        filtered = self.filter(self.signal_data)
        modulated = self.modulate(filtered, 8500)

        demodulated_1 = self.modulate(modulated, 8500, numpy.pi)
        demodulated_2 = self.modulate(modulated, 8510, 0)

        # filtered signal and demodulated signal with phase
        plt.figure(figsize=(12, 7))
        plt.xlim(0, 30000)
        plt.title("Filtered signal and demodulated signal with phase (\pi / 4)")
        plt.ylabel('Amplitude')

        plt.plot(filtered)
        plt.plot(self.filter(demodulated_1) * 4)

        plt.savefig('figure-1.png', dpi=500)

        # filtered signal and demodulated signal with different frequency
        plt.clf()  # clear figure
        plt.figure(figsize=(12, 7))

        plt.xlim(0, 30000)
        plt.title("Filtered signal and demodulated signal with different frequency")
        plt.ylabel('Amplitude')

        plt.plot(filtered)
        plt.plot(self.filter(demodulated_2) * 4)

        plt.savefig('figure-2.png', dpi=500)

        # spectrogram of filtered signal
        plt.clf()  # clear figure
        plt.figure(figsize=(12, 7))

        plt.xlim(0, 30000)
        plt.title("Spectrogram of filtered signal")
        plt.ylabel('Frequency')

        plt.specgram(filtered, Fs=self.sampling_frequency)
        plt.savefig('figure-3.png', dpi=500)


if __name__ == '__main__':
    amcom = AMCom(mat="firFilter_b_Coeffs.mat", record="helloworld.wav")
    amcom.plot()

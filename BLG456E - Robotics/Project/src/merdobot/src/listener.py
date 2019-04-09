import os
import speech_recognition

from threading import Thread


# https://pypi.python.org/pypi/SpeechRecognition/
class Listener(object):
    color = None
    stop = False

    def __init__(self):
        self.recognizer = speech_recognition.Recognizer()
        self.microphone = speech_recognition.Microphone() 

        with self.microphone as source:  # for better results
            self.recognizer.adjust_for_ambient_noise(source)

        self.colors = {'red', 'yellow', 'green', 'blue', 'pink'}

    def start(self):
        thread = Thread(target=self.run)
        thread.start()

    def run(self): 
        while not self.stop:
            self.display()
            print('What is your command?')

            with self.microphone as source:
                audio = self.recognizer.listen(source)

            try:  # also we can run this try-except block in a thread for concurent commands 
                value = self.recognizer.recognize_google(audio).lower()  # we can also use additional apis: bing, google_cloud, sphinx..
                print (value)
                
                if 'turn' in set(value.split()) and 'off' in set(value.split()):  # if user says 'turn off', it breaks the loop
                    print('Turning off..')
                    break

                elif 'cancel' in value:
                    self.color = None

                else:
                    for color in self.colors:
                        if color in value:
                            self.color = color

            except:
                print('Command cannot be recognized!')

    def display(self):
        print('Current Task:', 'getting a {} object'.format(self.color) if self.color else '-')


if __name__ == '__main__':
    listener = Listener()
    listener.start()
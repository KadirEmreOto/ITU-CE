# BLG433E - Computer Communication Project 1
# Kadir Emre Oto (150140032)

import os
import time
import random

from base import Socket
from socket import socket, AF_INET, SOCK_STREAM, error
from threading import Thread

from tkinter import X, Y, END, LEFT, DISABLED, NORMAL
from tkinter import Tk, Frame, Entry, Scrollbar, Text, Label, Button

colors = dict(red='#fc625d', blue='#5ab6c1', green='#4bfc10',
              orange='#ffee00', gray='#5c636f', pink='#c57bdb')


class Interface(Socket):
    def __init__(self):
        Socket.__init__(self)
        self.server = socket(AF_INET, SOCK_STREAM)
        self.connected = False

        self.window = Tk()
        self.window.title('KEO Chat')
        self.window.resizable(width=False, height=False)

        login_frame = Frame(self.window, bd=10)
        login_frame.pack(pady=5, padx=10, fill=X)

        host_label = Label(login_frame, text='Server')
        host_label.grid(row=0, column=0)

        self.host_entry = Entry(login_frame, width=30, justify='center')
        self.host_entry.grid(row=0, column=1)
        self.host_entry.insert(END, '127.0.0.1:5368')

        user_label = Label(login_frame, text='Username')
        user_label.grid(row=1, column=0)

        self.user_entry = Entry(login_frame, width=30, justify='center')
        self.user_entry.grid(row=1, column=1)
        self.user_entry.insert(END, os.environ.get('USER', f'Unknown-{random.randint(1000, 9999)}'))

        self.connect_button = Button(login_frame, text='Connect')
        self.connect_button.grid(row=2, column=1)
        self.connect_button['command'] = self.connect

        frame = Frame(self.window, bd=10)
        frame.pack(pady=5, padx=10, fill=X)

        scroll = Scrollbar(frame)
        self.text = Text(frame, width=50, height=30,
                         highlightcolor=colors['gray'],
                         highlightbackground=colors['gray'],
                         highlightthickness=1)
        self.text.configure(yscrollcommand=scroll.set,
                            state=DISABLED)

        scroll.configure(command=self.text.yview)

        self.text.pack(side=LEFT)
        scroll.pack(side=LEFT, fill=Y)

        self.entry = Entry(self.window)
        self.entry.pack(pady=10, padx=16, fill=X)
        self.entry.bind('<Return>', self.send_message)

    def connect(self):
        host = self.host_entry.get()

        if self.connected:
            self.server.close()  # disconnect
            self.server = socket(AF_INET, SOCK_STREAM)  # reset the connection
            self.connected = False
            self.insert("You disconnected")

            self.connect_button['text'] = 'Connect'
            self.host_entry.configure(background="white")
            self.user_entry.configure(state=NORMAL)

        elif host.count(':') != 1:  # wrong ip format
            self.host_entry.delete(0, END)
            self.host_entry.insert(END, '127.0.0.1:5368')

        else:
            addr, port = host.split(':')

            try:
                if not self.user_entry.get():
                    self.user_entry.insert(END, os.environ.get('USER', f'Unknown-{random.randint(1000, 9999)}'))

                self.server.connect((addr, int(port)))  # connect to server
                self.send(self.server, self.user_entry.get())  # send the username

            except Exception as e:
                self.server = socket(AF_INET, SOCK_STREAM)  # reset the socket object
                self.insert(str(e))
                return

            self.connected = True
            self.connect_button['text'] = 'Disconnect'
            self.insert("You connected")

            self.host_entry.configure(background=colors['green'])
            self.user_entry.configure(state=DISABLED)

            thread = Thread(target=self.listen, daemon=True)  # listen to the server in parallel
            thread.start()

    def listen(self):
        while True:
            try:
                message = self.recv(self.server)
            except error:  # it means the connection is closed
                break

            self.insert(message)

    def insert(self, message):
        self.text.configure(state=NORMAL)
        for line in message.split("\n"):
            self.text.insert(END, time.strftime(f'[%H:%M:%S] {line}\n'))
        self.text.configure(state=DISABLED)  # make the text area disable

    def send_message(self, event):
        message = event.widget.get()
        event.widget.delete(0, END)

        if self.connected:
            self.send(self.server, message)  # send the server the message
            self.insert(f"You: {message}")

        else:
            self.insert('Please connect first!')

    def main(self):
        self.window.mainloop()
        self.server.close()  # after window exit event


if __name__ == '__main__':
    interface = Interface()
    interface.main()

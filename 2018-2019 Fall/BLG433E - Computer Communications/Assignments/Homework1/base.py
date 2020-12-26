import socket


class Socket(object):
    def __init__(self, buffer_size=1024):
        self.buffer_size = buffer_size

    def recv(self, connection):
        """
        gets a message from given connection

        :param connection: socket object
        :return: message from the given connection
        """
        while True:
            try:
                message = connection.recv(self.buffer_size).decode().strip()
            except socket.timeout:
                continue

            if not message:
                raise socket.error

            return message

    @staticmethod
    def send(connection, message):
        """
        sends the given message to the given connection

        :param connection: socket object
        :param message: message to send
        """
        connection.sendall(message.encode())

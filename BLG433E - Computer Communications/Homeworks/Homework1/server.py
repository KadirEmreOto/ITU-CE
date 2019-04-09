# BLG433E - Computer Communication Project 1
# Kadir Emre Oto (150140032)

from base import Socket
from socket import socket, AF_INET, SOCK_STREAM, error
from threading import Thread


class Server(Socket):
    def __init__(self, host="", port=5368):
        Socket.__init__(    self)

        self.server = socket(AF_INET, SOCK_STREAM)  # TCP/IP connection
        self.server.bind((host, port))
        self.server.listen(1)

        self.host = host
        self.port = port
        self.flag = True

        self.clients = {}  # hold <connection, username> pairs in dictionary

    def run(self, daemon=10):
        for i in range(daemon):  # accept connections in parallel
            thread = Thread(target=self.accept)
            thread.start()

    def accept(self):
        while self.flag:
            connection, addr = self.server.accept()
            connection.settimeout(10)
            username = self.recv(connection)  # get the username immediately

            self.clients[connection] = username  # save the connection
            print(f'[+] connected: {username} (Active: {len(self.clients)})')

            self.server.listen(len(self.clients) + 1)

            for other in self.clients:
                if other != connection:
                    self.send(other, f"{username} connected")  # send all other connections the new connection
                    self.send(connection, f"Active: {self.clients[other]}\n")  # send the new connection others

            # listen all messages from this connection in parallel
            thread = Thread(target=self.listen, args=(connection, ), daemon=True)
            thread.start()

    def listen(self, connection):
        while True:
            try:
                message = self.recv(connection)
            except error:  # it means the connection is closed or broken pipe error
                print(f"[-] disconnected: {self.clients[connection]} (Active: {len(self.clients)-1})")

                connection.close()

                for other in self.clients:
                    if other != connection:  # notify others
                        self.send(other, f"{self.clients[connection]} disconnected")

                del self.clients[connection]
                self.server.listen(len(self.clients) + 1)

                break

            for conn in list(self.clients.keys()):
                if conn != connection:  # send all clients the message
                    self.send(conn, f'{self.clients[connection]}: {message}')


if __name__ == '__main__':
    server = Server()
    server.run()

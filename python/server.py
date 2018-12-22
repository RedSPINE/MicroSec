# coding: utf-8

import socket
import subprocess
import pipes

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.bind(('', 3111))
path_to_c_client = "../c/client"

while True:
        socket.listen(3)
        client, address = socket.accept()
        print (str(address)+" connected")

        received = client.recv(255)
        received = received.split()

        print("First arg : " + str(received[0], 'utf-8'))
        print("Second arg : " + str(received[1], 'utf-8'))

        subprocess.run(path_to_c_client + " " + str(received[0], 'utf-8') + " " + str(received[1], 'utf-8'))


print ("Close")
client.close()
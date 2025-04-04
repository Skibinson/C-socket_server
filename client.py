import socket
import readline
from datetime import datetime 
logfile = open("cfile.log", 'a')
conf = open("client.conf", "r")

ipaddr = conf.readline().split()[1] 
port = int(conf.readline().split()[1])
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

if (clientsocket.connect((ipaddr, port))==None):
    logfile.write("Connection time: " + str(datetime.now())[:-7] + '\n')
    logfile.write("Server address: " + ipaddr + ':' + str(port) + '\n')

string = input()
length = len(string)
bytestring = bytes(string, "utf-8")
clientsocket.send(bytestring)
data = (clientsocket.recv(1024)).decode("utf-8")

logfile.write("Sended string: " + string + " Time: " + str(datetime.now())[:-7] + '\n')
logfile.write("Received string: " + data + " Time: " + str(datetime.now())[:-7] + '\n\n')

print(data)

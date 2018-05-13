import socket
import sys
from thread import *

if len(sys.argv) <2:
    print "missing arg PORT"
    sys.exit(0)

HOST = ''
PORT = int(sys.argv[1])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    sock.bind((HOST, PORT))
except socket.error as msg:
    print "bind fail error "+str(msg[0])+" message "+str(msg[1])
print "Bind complete"

sock.listen(10)

class Client(object):
    def __init__(self, conn):
        self.conn = conn
        self.Run()

    def Run(self):
        #send size of index data
        #self.conn.send("Welcome")
        self.Send("Welcome".encode("utf-8"))
        self.Send("Welcome2".encode("utf-8"))
        while True:
            data = self.conn.recv(1024)
            print data
            if not data:
                break
            #self.conn.sendall("k")

        self.conn.close()
        print"some one left"

    def Send(self, data):
        print"send 1"
        self.conn.sendall((str(len(data))+"_").encode('utf-8'))
        print"recv1 "
        self.conn.recv(1)
      
        print"send 2"
        self.conn.sendall(data)       
        print"recv2 "
        self.conn.recv(1)

while 1:
    conn,addr = sock.accept()
    print "Connected with "+str(addr[0])+":"+str(addr[1])
    start_new_thread(Client, (conn,))

sock.close()

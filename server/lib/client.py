import socket;
import sys;
import multiprocessing

def getLenStr(string):
    nameLen = ""
    if len(string) <10:
        nameLen="0"
    nameLen+=str(len(string))
    return nameLen
   
def listener(connection):
    while 1:
        data, addr = connection.recvfrom(1054)

        print data
        if data[0:3] =="-n-":
            print data[5:]+" has Connected"
        elif data[0:3] =="-m-":
            print data[5:]
        elif  data[0:3] =="-e-":
            print data[5:]+" has Disconnected"

    return
def tcpListener(connection):
    while 1:
        data = connection.recv(1054)
        print "tcp : "+data
    return

class Client(object):
    def __init__(self):
        print len(sys.argv)
        if len(sys.argv) < 3:
            print "USAGE : host port\n"
            sys.exit(0)

        self.HOST = sys.argv[1]
        self.PORT = (int)(sys.argv[2])
        try:
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.conn.bind(("localhost", self.PORT-1))
        except socket.error:
            print "failed to bind"
            sys.exit()
        self.name  = raw_input("Enter your name ")

        #data, addr = self.conn.recvfrom(1054)
        #print data   
        self.listenP = multiprocessing.Process(target = listener, args = (self.conn,)) 
        self.listenP.start()
        # TCP SOCKET
        
        try:
            self.tcpConn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error:
            print ("failed to bind socket")
            sys.exit()
        self.tcpConn.connect((self.HOST, self.PORT))
        
        self.tcpConn.sendall("00"+getLenStr(self.name)+self.name)
                
        self.listenTCP = multiprocessing.Process(target = tcpListener, args = (self.tcpConn,)) 
        self.listenTCP.start()
    def sendTcp(self, data):
        self.tcpConn.sendall(data)
    def main(self):
        while 1:
            inputstr = self.name+": "+raw_input("")
            self.conn.sendto("-m-"+getLenStr(inputstr)+inputstr, (self.HOST, self.PORT))

    def end(self):
        self.listenP.terminate()
        self.listenP.join()
        self.listenTCP.terminate()
        self.listenTCP.join()

        self.conn.sendto("01"+getLenStr(self.name)+self.name, (self.HOST, self.PORT))
        sys.exit(0)

client = Client()
try:
    client.main()
    p.start()
       
except KeyboardInterrupt:
    client.end()

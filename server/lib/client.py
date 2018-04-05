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
        if data[0:3] =="-n-":
            print data[5:]+" has Connected"
        elif data[0:3] =="-m-":
            print data[5:]
        elif  data[0:3] =="-e-":
            print data[5:]+" has Disconnected"

    return

class Client(object):
    def __init__(self):
        print len(sys.argv)
        if len(sys.argv) < 3:
            sys.exit()

        self.HOST = sys.argv[1]
        self.PORT = (int)(sys.argv[2])
        try:
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.conn.bind(("localhost", self.PORT))
        except socket.error:
            print "failed to bind"
            sys.exit()
        self.name  = raw_input("Enter your name ")
        self.conn.sendto("-n-"+getLenStr(self.name)+self.name, (self.HOST, 5555))

        data, addr = self.conn.recvfrom(1054)
        print data   
        self.listenP = multiprocessing.Process(target = listener, args = (self.conn,)) 
        self.listenP.start()


    def main(self):
        while 1:
            inputstr = self.name+": "+raw_input("")
            self.conn.sendto("-m-"+getLenStr(inputstr)+inputstr, (self.HOST, 5555))

    def end(self):
        self.listenP.terminate()
        self.listenP.join()
        
        self.conn.sendto("-e-"+getLenStr(self.name)+self.name, (self.HOST, 5555))
        sys.exit(0)

client = Client()    
try:
    client.main()
except KeyboardInterrupt:
    client.end()
    

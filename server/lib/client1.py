import socket;
import sys;
from multiprocessing import Process
import signal,sys
from random import randint
import time
import math

def getLenStr(string):
    nameLen = ""
    if len(string) <10:
        nameLen="0"
    nameLen+=str(len(string))
    return nameLen
   
if len(sys.argv) < 3:
    print ("USAGE : host port\n")
    sys.exit(0)
masterPort = 0
masterPort = (int)(sys.argv[2])

class Client(object):
    def __init__(self, mp):
        self.HOST = sys.argv[1]
        self.POS = [0, 0, 0] 
        self.TARGET = [50, 0, 10]
        self.PORT = mp
        self.chatTxt=["Beep Boop Im a Bot!",
                        "Hello World!",
                        "Top Kek",
                        "Harrassment Sunstrike",
                        "I can't let you do that dave...",
                        "My name Bot, Ro Bot."]
        try:
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.conn.bind(("192.168.1.10", self.PORT))
        except socket.error:
            print ("failed to bind1 " + str(self.PORT))
            sys.exit(0)
        self.name  = 'bot'#input("Enter your name ")
        self.login = "bot" + str(mp)
        lmsg = self.name+"|"+self.login
        self.conn.sendto(("00"+getLenStr(lmsg)+lmsg).encode('utf-8'), (self.HOST, 5556))
        data, addr = self.conn.recvfrom(1054)
        data = data.decode('utf-8')
        self.id = data[2:];
        print('creating bot '+str(mp) + ' : ' +self.id)

    def dist(self, a, b):
        arr = [a[0] - b[0], a[1] - b[1], a[2] - b[2]]
        return math.sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]  )

    def dir2t(self, a, b):
        arr = [a[0] - b[0], a[1] - b[1], a[2] - b[2]]
        length = self.dist(a, b)
        arr[0] = arr[0] / length 
        arr[1] = arr[1] / length
        arr[2] = arr[2] / length
        return arr

    def sendMsg(self, msg):
        self.conn.sendto("03"+getLenStr(msg)+msg, (self.HOST, 5556))

    def main(self):

        '''rInt = randint(0,100)
        if rInt >= 50 and rInt <=60:
            self.sendMsg(self.chatTxt[randint(0, len(self.chatTxt)-1)])'''

        if self.dist(self.POS, self.TARGET) < 10:
            self.TARGET = [randint(0, 2000) - 1000, 0, randint(0, 2000) - 1000]
            print "CHANGING TARGET "+str(self.TARGET[0])+" "+ str(self.TARGET[1])+" "+ str(self.TARGET[2])

        #id#x,y,z#x,y,z
        d = self.dir2t(  self.TARGET, self.POS)
        print "DIR : " + str(d[0])+" "+ str(d[1])+" "+ str(d[2]) + " dist " + str(self.dist(self.POS, self.TARGET))
        self.POS[0] += d[0]*10
        self.POS[2] += d[2]*10
        posStr = self.id+'#'+str(self.POS[0])+','+str(self.POS[1])+','+str(self.POS[2])+'#0,0,0'
        print posStr[-5:]+ " TARGT "+str(self.TARGET[0])+" "+ str(self.TARGET[1])+" "+ str(self.TARGET[2])
        self.conn.sendto(("04"+getLenStr(posStr)+posStr).encode('utf-8'), (self.HOST, 5556))

       

    def end(self):
        #self.alive = False
        print ("ending bot "+str(self.PORT)+' : '+self.id)
        msg = ("0202" + self.id)
        try:
            self.conn.sendto((msg).encode('utf-8'), (self.HOST, 5556))
        except socket.error as e:
            print(str(e))
        print("ok")
        

#client = Client(masterPort)
allClients=[]
nrOfCs = 1
i = 0 
alive = True
while nrOfCs >0:
    nrOfCs-=1
    allClients.append( Client(masterPort+i))
    i+=1

def run(k):
    while alive:
        time.sleep(1)
        for cli in allClients:
            cli.main()
    

def endit():
    #alive = False
    i =0;
    print(len(allClients))
    for cli in allClients:
        #time.sleep(10.0/60.0)
        print('ending client '+str(i) + '  '+cli.id+'   '+str(cli.PORT))
        i+=1
        cli.end()

    print('FINISHED '+str(i))
    #sys.exit(0)

#signal.signal(signal.SIGINT, catch_ctrl_C)
p = Process(target = run, args = (0,))
p.start()
#try:
while alive:
    print('listening ')
    inputstr = ''
    try:
        inputstr =  raw_input("input : ")
        print " you said "+inputstr
    except EOFError:
        inputstr = ''
    if inputstr != '':        
        print inputstr
    if inputstr == 'k':
        alive = False

p.terminate()
endit()
    #time.sleep(10.0/60.0)


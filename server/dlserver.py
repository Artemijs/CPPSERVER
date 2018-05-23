import socket
import sys
from thread import *
import os
import json
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

    def GetDirContents(self, srcDir):
        folders = []
        filesFull = []
        for root, dirs, files in os.walk(srcDir):
            if root != './':
                folders.append(root)
            if len(files) <= 0:
                continue
            for f in files:
                filesFull.append(root + "/" + f)
        return [folders, filesFull]

    def Run(self):
        #send size of index data
        dir_files = self.GetDirContents("../vf")
        print dir_files[0]
        for f in dir_files[1]:
            print "SENDING FUCKING FILE "+f
            self.SendFile(f)
            
        '''while True:
            data = self.conn.recv(1024)
            print data
            if not data:
                break
            #self.conn.sendall("k")
        '''
        self.conn.close()
        print"some one left"

    def GetOtherFile(self, original):
        print original
        return original[original.find("/build")+6:]

    def SendFile(self, fileName):
        print "sending "+fileName
        #send name 
        otherFile = self.GetOtherFile(fileName)
        self.Send(otherFile.encode("utf-8"))
        otherFile = "../build0/"+fileName[fileName.find("/vf")+3:]
        #send size of file
        #self.conn.sendall((str(len(data))+"_").encode('utf-8'))i
        #self.conn.recv(1)
        print str(os.stat(fileName).st_size)
        self.Send(str(os.stat(fileName).st_size).encode("utf-8"))
        allData = json.loads(self.SendFileData(fileName)) 
        with open(otherFile, "rb") as file:
            for i in allData:
                file.seek(i[2])
                data = file.read(i[3])                
                self.Send(data)
                print "GETTING DATA "+str(i[2]) + " " +str(i[3]) +": " + data
     

    def SendFileData(self, fileName):
        allData = ""
        print fileName
        with open(fileName, "rb") as file:
            while True:
                data = file.read(1024)  
                print "sending "+str(len(data))
                if not data:
                    break
                allData+= data.decode("utf-8")
                self.Send(data)
        return allData
       
    def Send(self, data):
        self.conn.sendall((str(len(data))+"_").encode('utf-8'))
        self.conn.recv(1)
      
        self.conn.sendall(data)       
        self.conn.recv(1)

while 1:
    conn,addr = sock.accept()
    print "Connected with "+str(addr[0])+":"+str(addr[1])
    start_new_thread(Client, (conn,))

sock.close()

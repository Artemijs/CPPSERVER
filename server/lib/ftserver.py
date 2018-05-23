from VFMaker import VersionMaker
import sys
import socket
import json
import os
import shutil
import subprocess

if len(sys.argv) < 2:
    print "missing arguments, expected : port"
    sys.exit(0)

class FTServer(object):
    def __init__(self):
        self.PORT = (int)(sys.argv[1]) 
        self.alive = True
        self.ROOT_DIR = '../../'
        self.CLIENT_DIR = self.ROOT_DIR+"client/"
        self.BUILD_DIR = self.ROOT_DIR+"build/"
        self.WRITE_DIR = self.ROOT_DIR
        self.sock= socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.vfm = VersionMaker(self.BUILD_DIR, self.BUILD_DIR[:-1]+"0/", self.ROOT_DIR+"vf/") 
        try:
            self.sock.bind(("", self.PORT))
        except socket.error as e:
            print("bind failed \n"+str(e))
            sys.exit(0)
        
        self.sock.listen(10)
        conn, addr = self.sock.accept()
        print "Connected " + addr[0] + " : " + (str)(addr[1])
        self.conn = conn
        self.state = 0
        self.callbacks = {'912' : self.CreateFolders,
                        '123' : self.SetName,
                        '234' : self.ReceiveFileData,
                        '789' : self.Disconnected,
                        '112' : self.GetDupe,
                        '111' : self.SetDupe,
                        '113' : self.SetWriteDir,
                        '114' : self.GitPush,
                        '115' : self.CompileVersion,
        }
        self.totalSize = 0
        self.size = 0
        self.fileName =''
        self.data=''
        self.file = 0
        self.dupe = '0'
    
    def GetDupe(self, data):
        self.conn.sendall(self.dupe)
        self.ResetState() 
    
    def SetWriteDir(self, data):
        d = data
        print "write dir : "+d
        if d == 'build':
            self.WRITE_DIR = self.BUILD_DIR[:-1]+"0/"
        else:
            self.WRITE_DIR = self.CLIENT_DIR
        self.ResetState()
        self.conn.sendall('k')

    def SetDupe(self, data):
        print "setting dupe "+data
        self.dupe = data 
        self.ResetState()
        self.conn.sendall('k')

    def RenameFolder(self):
        count=0
        dir = self.WRITE_DIR
        if os.path.isdir(dir):
            #os.rename(dir, self.WRITE_DIR[:-1]+str(count))
            while os.path.isdir(self.WRITE_DIR[:-1]+str(count)): 
                count+=1
        os.rename(dir, self.WRITE_DIR[:-1]+str(count))
            
    def CreateFolders(self, data):
        self.size += len(data)
        self.data += data
        if self.size == self.totalSize:
            if self.dupe == '1':
                self.RenameFolder()
            elif os.path.isdir(self.WRITE_DIR):
                shutil.rmtree(self.WRITE_DIR)
            self.MkDirs([''] + json.loads(self.data ))
            print "finished creating folders"
            self.conn.sendall('k')
            self.ResetState() 
    def SetName(self, data):
        self.size += len(data)
        self.fileName = data
        if self.size == self.totalSize:
            self.file = open(self.WRITE_DIR + self.fileName, "wb")
            print "FILE :"+self.fileName
            self.conn.sendall('k')
            self.ResetState() 
    def Disconnected(self, data): 
        self.conn.close()
        print "Disconnected "
        self.sock.listen(10)
        conn, addr = self.sock.accept()
        print "Connected " + addr[0] + " : " + (str)(addr[1])
        self.conn = conn
        self.ResetState()

    def ReceiveFileData(self, data):
        self.size += len(data)
        self.file.write(data)
        if self.size == self.totalSize:
            print "finished file  :"+self.fileName+" "+str(self.size)
            self.file.close()
            self.conn.sendall('k')
            self.ResetState() 

    def CompileVersion(self, data):
        print"Compiling Version data"  
        #comiple the version
        self.vfm.Run()
        #remove build
        #shutil.rmtree(self.BUILD_DIR)
        #os.rename(self.BUILD_DIR[:-1]+"0/", self.BUILD_DIR)
        #rename build0 to build

        self.conn.sendall('k')
        self.ResetState()

    def ChangeState(self, data):
        self.state = data[0:3]
        size = data[3:]
        print 'size : '+str(size)+" state "+str(self.state)
        self.totalSize = int(size);
        self.conn.sendall('k')
    
    def ResetState(self):
        self.state = 0
        self.size = 0
        self.data = ''

    def RunState(self, data):
        self.callbacks[self.state]( data )

    def Main(self):
        while self.alive:
            data = self.conn.recv(1024)
            if self.state == 0:
                self.ChangeState(data)
            else:
                self.RunState(data)

    def GitPush(self, msg):
        print subprocess.check_output('git add --all', shell = True)
        print subprocess.check_output('git commit -m"' + msg + '"', shell = True)
        print subprocess.check_output('git push', shell = True)
        self.conn.sendall('k')
        self.ResetState()

    def MkDirs(self, folders):
        
        for f in folders:
            if os.path.isdir(self.WRITE_DIR + f) == False:
                os.mkdir(self.WRITE_DIR + f)
        
    def End(self):
        self.alive = False
        self.sock.close()


server = FTServer()
def main():
    try:
        server.Main()
    except KeyboardInterrupt as e:
        print(str(e))
        server.End()
            
main()
print("Bye!")
'''TO DO:
the duplicate doesnt work well, i cant add different files without deleting
whats already there. The check for dupe should be in mkdir'''

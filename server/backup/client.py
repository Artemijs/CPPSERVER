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
    return
def main():
    print len(sys.argv)
    if len(sys.argv) < 3:
        sys.exit()

    HOST = sys.argv[1]
    PORT = (int)(sys.argv[2])
    try:
        conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        conn.bind(("localhost", PORT))
    except socket.error:
        print "failed to bind"
        sys.exit()
    name  = raw_input("Enter your name ")
    conn.sendto("-n-"+getLenStr(name)+name, (HOST, 5555))

    data, addr = conn.recvfrom(1054)
    print data   
    p = multiprocessing.Process(target = listener, args = (conn,)) 
    p.start()
    while 1:
        inputstr = raw_input("send :")
        conn.sendto("-m-"+getLenStr(inputstr)+inputstr, (HOST, 5555))
try:
    main()
except KeyboardInterrupt:
     

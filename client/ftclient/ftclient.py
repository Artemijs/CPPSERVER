''' this itteration of file transferer vvill have 
ignore files
command line 
'''
import sys
import os
import json
import socket
import math
import mimetypes
if len(sys.argv) < 3:
	print('vvrong input format, expecting host port')
	sys.exit(0)
#self.HOST = sys.argv[1]
#self.PORT = (int)(sys.argv[2])

class FTClient(object):
	def __init__(self):
		self.HOST = sys.argv[1]
		self.PORT = (int)(sys.argv[2])
		self.alive = True
		self.dir = '../Assets'
		self.build_dir = '../build'
		self.pushExclude = '.meta.py'
		self.comands = {
			'help': self.PrintHelp,
			'push': self.Push,
			'push-only': self.PushOnly,
			'build': self.Build,
			'dir': self.PrintDir,
			'set-dir': self.SetDir,
			'dir-build': self.PrintBuildDir,
			'set-dir-build': self.SetBuildDir,
			'list-ext': self.ListExt,
			'test': self.Test,
			'dupe': self.GetDupe,
			'set-dupe' : self.SetDupe 
		}
		self.comandDetails = {
			'help': ' prints available commands',
			'push': ' sends to sever and pushes to git \n    args : 1 \n\tcommit message',
			'push-only': ' sends to sever and pushes to git \n    args : 2 \n\textension of files to only include\n\tcommit message',
			'build': ' sends to sever the exe files and updates the current verson',
			'dir': ' print current project directory',
			'set-dir': ' change current project directory',
			'dir-build': ' print current build directory',
			'set-dir-build': ' change current build directory',
			'list-ext': ' list extensions of files in dir and every sub dir',
			'test' : ' test function that does vvhateverr',
			'dupe' : ' prints the current state of duplicate variable, set to false vvill overvvrite files',
			'set-dupe' : ' sets value of duplicate variable \n    args : 1 \n\t0-1',
		}
		try:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		except socket.error as e:
			print ("failed to bind socket "+str(e))
			sys.exit()
		self.sock.connect((self.HOST, self.PORT))
		print ("connected to "+self.HOST+":"+str(self.PORT))
		self.dupe = '0'
		self.GetDupe(0)

	def GetDupe(self, args):
		self.sock.sendall( (str(112)+str(0)).encode('utf-8') )
		self.sock.recv(1)
		self.sock.sendall('k'.encode('utf-8'))
		self.dupe = self.sock.recv(1).decode('utf-8')
		print ('dupe is set to :'+self.dupe)

	def SetDupe(self, args):
		if args[0] != '0' and args[0] != '1':
			print ('vvrong fromat expected 0 or 1')
			return
		self.dupe = args[0]
		self.SendMessage(args[0].encode('utf-8'), 111)

	def PrintDir(self, args):
		print(self.dir)

	def SetDir(self, args):
		if len(args) != 1:
			print('expecting 1 argument nevv_dirr')
			return
		self.dir = args[0]
		#if self.dir[-1:] == '/':
		#	self.dir = self.dir[0: -1]

		print('nevv directory :'+self.dir)

	def PrintBuildDir(self, args):
		print(self.build_dir)

	def SetBuildDir(self, args):
		if len(args) != 1:
			print('expecting 1 argument nevv_dirr')
			return
		self.build_dir = args[0]
		#if self.dir[-1:] == '/':
		#	self.dir = self.dir[0: -1]

		print('nevv directory :'+self.dir)

	def PushOnly(self, args):
		if len(args) < 2:
			print('expecting 2 argument include_only_ext commit_messgae')
			return
		self.SendMessage('push'.encode('utf-8'), 113)
		print('\npushing to git vvith commit message ' + args[0])
		
		dirs = self.GetDirContents('', args[0])
		#first send all the folders
		self.SendProject(dirs)
		msg = ''
		for i in range(1, len(args)):
			msg += ' '+args[i]
		self.SendMessage(msg.encode('utf-8'), 114)

	def Push(self, args):
		if len(args) != 1:
			print('expecting 1 argument commit_messgae')
			return
		self.SendMessage('push'.encode('utf-8'), 113)
		print('\npushing to git vvith commit message ' + args[0])

		dirs = self.GetDirContents(self.pushExclude)
		#first send all the folders
		self.SendProject(dirs)
		msg = ''
		for i in range(0, len(args)):
			msg += ' '+args[i]
		
		self.SendMessage(msg.encode('utf-8'), 114)

	def SendProject(self, dirs):
		for i in range(0, len(dirs[0])):
			dirs[0][i] = self.ValidatePath(dirs[0][i])
			print(dirs[0][i])

		jF = json.dumps(dirs[0])
		print('sending folders ' + str(len(jF)))
		#send 912,size
		self.SendMessage(jF.encode('utf-8'), 912)
		'''self.sock.recv(1)
		
		self.SendB(jF.encode('utf-8'))
		self.sock.recv(1)'''

		print('server finished creating folders')
		count = 0
		for f in dirs[1]:
			validatedF = self.ValidatePath(f)
			print('sending '+ validatedF +' '+str(count + 1)+'/'+str(len(dirs[1])) )
			count+=1
			#second send file name
			self.SendMessage(validatedF.encode('utf-8'), 123)
			'''self.sock.recv(1)
			self.SendB(validatedF.encode('utf-8'))
			self.sock.recv(1)'''
			#third send file_names data
			size = os.stat(f).st_size
			nrOfMsg = math.ceil(size / 1024)
			self.Send(size, 234)
			self.sock.recv(1)
			print('sending file data ' + str(size))
			msgCount = 0
			with open(f, "rb") as file:
				#data = file.read(1023)
				while True:
					data = file.read(1024)
					#print(str(msgCount)+'/'+str(nrOfMsg) + ' ' + str(len(data)))
					msgCount+=1
					if not data:
						break
					self.SendB(data)
			#self.End()
			print('finished file ')
			self.sock.recv(1)

	def Build(self, args):
		print('\n sending to server a nevv build ' )
		self.SendMessage('build'.encode('utf-8'), 113)
		d = self.dir
		self.dir = self.build_dir
		dirs = self.GetDirContents()
		#first send all the folders
		self.SendProject(dirs)
		self.dir = d

	def PrintHelp(self, args):
		for c in self.comands:
			print('\n')
			print(c)
			print(self.comandDetails[c])

	def Listen4Input(self):
		while self.alive:
			cmd = input()
			allVals = cmd.split(' ')
			if allVals[0] in self.comands:
				self.comands[allVals[0]](allVals[1:])
			else:
				print('Invalid input, type help for dat svveet svveet ass istance')

	def End(self):
		self.Send('0', 789)
		self.alive = False
		try:
			self.sock.close()
		except socket.error as e:
			print('Failed to close '+str(e))
		print('\nBye\n')

	def GetDirContents(self, ignore = '', includeOnly = ''):
		folders=[]
		filesFull = []
		for root, dirs, files in os.walk(self.dir):
			##print root
			if root != './':
				folders.append(root)
			#print files
			if len(files) > 0:
				for f in files:
					ext = self.GetExtension(f)
					if ext == '0':
						continue
					if ignore.find(ext) == -1:
						if len(includeOnly) != 0:
							if includeOnly.find(ext) != -1 :
								filesFull.append(root+"/"+f)
						else:
							filesFull.append(root+"/"+f)
		#print("-----")
		#print (folders)
		#print (filesFull)
		return [folders, filesFull]

	def GetExtension(self, name):
		index = name.rfind('.')
		slash = name.rfind('/')
		ext = name[index:]
		#there are files vvithout extensions
		#and there is a dot at the start of path   ./folder/file
		if slash > index or index == -1:
			ext = '0'
		return ext
	def ListExt(self, args):
		files = self.GetDirContents('')[1]
		allExts = set()
		for f in files:
			allExts.add(self.GetExtension(f))
		print(allExts)

	def ValidatePath(self, p):
		#remove the ./ and any ../
		#replace all \ vvith /
		p = p.replace('../', '')
		p = p.replace('./', '')
		p = p.replace('\\', '/')
		return p

	def Test(self, args):
		s = set()
		#mime = mimetypes.guess_type(file)
		files = self.GetDirContents('')[1]
		for f in files:
			s.add(self.ValidatePath(f))
		for itm in s:
			print (itm)

	def SendMessage(self, msg, type):
		self.sock.sendall( (str(type)+str(len(msg))).encode('utf-8') )
		self.sock.recv(1)
		self.sock.sendall(msg)
		self.sock.recv(1)
	def Send(self, msg, type):
		self.sock.sendall( (str(type)+str(msg)).encode('utf-8') )
	def SendB(self, msg):
		self.sock.sendall(msg)



ftc = FTClient()
try:
	ftc.Listen4Input()
except (EOFError, KeyboardInterrupt) as e:
	ftc.End()
	if type(e) != KeyboardInterrupt:
		print(str(e))
	
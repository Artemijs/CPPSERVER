from comparer import Comparer
import sys
import os
import json
import socket
import math
import mimetypes
import shutil
import json

class VersionMaker(object):
	def __init__(self, oDir, nDir, vDir):
		self.nSrc = nDir
		self.oSrc = oDir
		self.vfSrc = vDir

	def GetDirContents(self, dir):
		folders=[]
		filesFull = []
		for root, dirs, files in os.walk(dir):
			##print root
			folders.append(root)
			#print files
			for f in files:
				filesFull.append(root+'/'+f)
		return [filesFull, folders]

	def ValidatePath(self, p):
			#remove the ./ and any ../
			#replace all \ vvith /
			p = p.replace('../', '')
			p = p.replace('./', '')
			p = p.replace('\\', '/')
			return p

	def GetVfPaths(self, dirs, prepend):
		vfDirs = []
		for i in range(0, len(dirs)):
			vfDirs.append( prepend +"/"+dirs[i])
			print(vfDirs[i])
		return vfDirs

	def mkDirs(self, folders):
		for f in folders:
			print('making '+f)
			os.mkdir(f)

	def getOtherFile(self, f, otherDir):
		#f = ../folder_name/path
		print('changing file name '+f+' \nto \n'+
			(otherDir+f[f.find('/', 3)+1:]))
		return (otherDir+f[f.find('/', 3)+1:])


	def ProcessFile(self, fo, fn):
		print('\ngetting the differeance betveen\n'+fn+'\n'+fo)
		comp = Comparer(100)
		res = comp.Compare(fo, fn)
		if len(res) == 0:
			return
		#vf = self.vfSrc +"/"+ self.ValidatePath(fn)#self.getOtherFile(fn, self.vfSrc)#+'build/')
		temp = self.ValidatePath(fn)
		vf = self.vfSrc+"/"+temp[temp.find("/")+1:]
		print('vvriting '+str(len(res)) +' into \n'+vf )
		with open(vf, 'w') as file:
			file.write(json.dumps(res))

	def CheckExists(self, f):
		if os.path.isdir(f):
			print('removing ' +f)
			shutil.rmtree(f)

		#os.mkdir(f)
	def Add2Manifest(self, name, typeStr, actionStr):
		print("ADDING TO MANIFEST "+typeStr+" "+actionStr+" "+name)
		with open(self.vfSrc+"/manifest.txt", "w") as file:
			file.write(typeStr+actionStr+" "+name)	
	
	def CheckNewOrDel(self, odirs, ndirs):	
		print ndirs
		print odirs
		#check for new folders
		#folders that are in ndirs and not in odirs
		lenOfArr = len(ndirs[1])
		i = 0
		while i < lenOfArr:
			if self.NotIn(ndirs[1][i], odirs[1]):
				self.Add2Manifest(ndirs[1][i], "d", "n")
				ndirs[1].remove(ndirs[1][i])
				i-=1
				lenOfArr -=1
			i+=1

		#check for deleted folders
		#folders that are in odirs and not in ndirs
		lenOfArr = len(odirs[1])
		i = 0
		while i < lenOfArr:
			if self.NotIn(odirs[1][i], ndirs[1]):
				self.Add2Manifest(odirs[1][i], "d", "d")
				odirs[1].remove(odirs[1][i])
				i-=1
				lenOfArr -=1
			i+=1

		#check for new files
		lenOfArr = len(ndirs[0])
		i = 0
		while i < lenOfArr:
			if self.NotIn(ndirs[0][i], odirs[0]):
				self.Add2Manifest(ndirs[0][i], "f", "n")
				ndirs[0].remove(ndirs[0][i])
				i-=1
				lenOfArr -=1
			i+=1
		#check for delete files
		lenOfArr = len(odirs[0])
		i = 0
		while i < lenOfArr:
			if self.NotIn(odirs[0][i], ndirs[0]):
				self.Add2Manifest(odirs[0][i], "f", "d")
				odirs[0].remove(odirs[0][i])
				i-=1
				lenOfArr -=1
			i+=1


	def NotIn(self, obj, arr):
		for a in arr:
			if a == obj:
				return False;
		return True

	def StripPaths(self, dirs):
		for i in range(0, len(dirs)):
			dirs[i] = self.ValidatePath(dirs[i])
			dirs[i] = dirs[i][dirs[i].find("/")+1:]

	def Run(self):
		print("\nRedying dirs\n")
		ndirs = self.GetDirContents(self.nSrc)
		odirs = self.GetDirContents(self.oSrc)
		print ndirs
		print odirs
		self.StripPaths(ndirs[0])
		self.StripPaths(ndirs[1])
		self.StripPaths(odirs[0])
		self.StripPaths(odirs[1])
		vfDirs = self.GetVfPaths(ndirs[1], self.vfSrc)
		print("\nChecking if VF exists\n")
		self.CheckExists(self.vfSrc)
		self.mkDirs(vfDirs)
		#check if there are new folders
		print("\nChecking New Or Del\n")
		self.CheckNewOrDel(odirs, ndirs)	
		print("\nProcessing files\n")
		countOfFiles = 0
		for f in ndirs[0]:
			print(str(countOfFiles) + ' : '+str(len(ndirs[0])))
			self.ProcessFile(self.oSrc + f, self.nSrc + f)
			countOfFiles+=1


#vfm = VersionMaker("./otest/", "./ntest/", "./vf")
#vfm.Run()

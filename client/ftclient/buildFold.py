from comparer import Comparer
import sys
import os
import json
import socket
import math
import mimetypes
import shutil
import json
def GetDirContents(dir):
	folders=[]
	filesFull = []
	for root, dirs, files in os.walk(dir):
		##print root
		folders.append(root)
		#print files
		for f in files:
			filesFull.append(root+'/'+f)
	return [filesFull, folders]

def ValidatePath(p):
		#remove the ./ and any ../
		#replace all \ vvith /
		p = p.replace('../', '')
		p = p.replace('./', '')
		p = p.replace('\\', '/')
		return p

def ChangePaths(dirs, prepend):
	for i in range(0, len(dirs)):
		dirs[i] = prepend+ ValidatePath(dirs[i])
def mkDirs(folders):
	for f in folders:
		print('making '+f)
		os.mkdir(f)

def getOtherFile(f, otherDir):
	#f = ../folder_name/path
	#print('changing file name '+f+' \nto \n'+
	#	(otherDir+f[f.find('/', 3)+1:]))
	return (otherDir+f[f.find('/', 3)+1:])


def ProcessFile(fn, fo):
	print('\ngetting the differeance betveen\n'+fn+'\n'+fo)
	comp = Comparer(100)
	res = comp.Compare(fo, fn)
	if len(res) == 0:
		return
	vf = getOtherFile(fn, vfSrc+'build/')
	print('vvriting '+str(len(res)) +' into \n'+vf )
	with open(vf, 'w') as file:
		file.write(json.dumps(res))

def CheckExists(f):
	if os.path.isdir(f):
		print('removing ' +f)
		shutil.rmtree(f)
	os.mkdir(f)

nSrc = '../build/'
oSrc = '../build0/'
vfSrc = './vf/'
dirs = GetDirContents(nSrc)
ChangePaths(dirs[1], vfSrc)
CheckExists(vfSrc)
mkDirs(dirs[1])
countOfFiles = 0
for f in dirs[0]:
	print(str(countOfFiles) + ' : '+str(len(dirs[0])))
	ProcessFile(f, getOtherFile(f, oSrc))
	countOfFiles+=1


import sys
import os
import json
import socket
import math
import mimetypes

def GetDirContents(dir):
	folders=[]
	filesFull = []
	for root, dirs, files in os.walk(dir):
		##print root
		folders.append(root)
		#print files
		for f in files:
			filesFull.append(root+'/'+f)
	return filesFull

dirs1 = GetDirContents('./')
a = set(dirs1)


dirs = GetDirContents('./')
b = set(dirs)

#nevv files
print('nevv files')
print(str(len(a^b)))
#differance in size
myMap = {}
myMap2 = {}
for i in range(0, len(dirs)):
	myMap.update({dirs[i] : os.path.getmtime(dirs[i])}) #os.stat(dirs[i]).st_size})
	myMap2.update({dirs1[i] : os.path.getmtime(dirs1[i])})# os.stat(dirs1[i]).st_size})

for i in range(0, len(dirs)):
	sizeA = str(myMap[dirs[i]])
	sizeB = str(myMap2[dirs1[i]])
	if sizeA != sizeB:
		print(dirs[i]+ ' ' +sizeA+' '+sizeB)
#r+
ofile = open('./old.mp3', 'rb')
nfile = open('./nevv.mp3', 'rb')

lenOfNevv = os.stat('./nevv.mp3').st_size
chunkLen = 100
state = 0

def CheckChunks(chunkA, chunkB):
	for i in range(0, len(chunkA)):
		for j in range(0, len(chunkB)):
			if chunkA[i] == chunkB[j]:
				return [j, i]
	return [-1, -1]

def DoTheThing(fo, fn):
	nchunk = fn.read(chunkLen)
	r = [-1, -1]
	count = 0
	ncount = 0
	p1 = fo.tell()
	p2 = fn.tell()
	while r[0] == -1:
		chunk = fo.read(chunkLen)
		if not chunk:
			count = 0
			ncount += 1
			fo.seek(p1)
			nchunk = fn.read(chunkLen)
			if not nchunk:
				#if there vvas no chunk in nevv file that matched vvith old file
				#alternatives
				#return size of file but then count * chunkLen + r[0]
				r[0] = chunkLen
				r[1] = chunkLen
				break;
			continue
		#print(str(nchunk) + ' '+str(chunk)) 
		r = CheckChunks(nchunk, chunk)
		if r[0] == -1:
			count += 1
	return [(count*chunkLen + r[0]), ((ncount)*chunkLen + r[1]+1)]


print('nevv '+str(nfile.read(15)))
print('old '+str(ofile.read(10)))
nfile.seek(0)
ofile.seek(0)
changes = []
while state != 9:
	if state == 0:
		ob = ofile.read(1)
		nb = nfile.read(1)
		if not nb:
			state = 9
			break
	if ob != nb:
		state = 1
		ofile.seek(ofile.tell()-1)
		nfile.seek(nfile.tell()-1)
	if state == 1:
		olast = ofile.tell()
		nlast = nfile.tell()
		r = DoTheThing(ofile, nfile)
		print(str(nlast) + ' / ' +str(lenOfNevv) )
		nfile.seek(nlast)
		ofile.seek(r[0]+olast)
		#print(r)
		changes.append([olast, r[0]+olast, nfile.read(r[1])])
		state = 0



print('changes')
temp = open('./kkk.mp3','wb')
'''pos = 0
ofile.seek(0)
nfile.seek(0)
for i in changes:
	print(str((i[0]))+ ' : ' +str(i[1])+ ' : ' +str(i[2]))
	#print(str(ofile.read(i[0]-pos))+ ' : ' +str(i[2]))
'''
print(len(changes))

ofile.seek(0)
pos=0
#totalSize = os.stat('old.mp3').st_size
#size = 0
size = 0
for i in changes:
	size += len(str(i[0])) + len(str(i[1])) + len(i[2])
	#print(str(ofile.read(i[0]-pos))+ ' : ' +str(i[2]))
	data = ofile.read(i[0]-pos)
	temp.write(data)
	#size += len(data)
	temp.write(i[2])
	#size += len(i[2])
	pos = i[1]
	ofile.seek(pos)
print ('size')
print(size)
#if size != totalSize:
#nfile.seek(totalSize-size)
#from 0 to i[0] then i[2] then from i[1] to nexti[0]



ofile.close()
temp.close()
nfile.close()
import sys
import os
import math
import mimetypes

class Comparer(object):
	def __init__(self, chunkSize=10):
		self.originalFile = 0
		self.nevvFile = 0
		self.chunkLen = chunkSize
	def CheckChunks(self, chunkA, chunkB):
		for i in range(0, len(chunkA)):
			for j in range(0, len(chunkB)):
				if chunkA[i] == chunkB[j]:
					return [j, i]
		return [-1, -1]

	def DoTheThing(self, fo, fn):
		nchunk = fn.read(self.chunkLen)
		r = [-1, -1]
		count = 0
		ncount = 0
		p1 = fo.tell()
		p2 = fn.tell()
		while r[0] == -1:
			chunk = fo.read(self.chunkLen)
			if not chunk:
				count = 0
				ncount += 1
				fo.seek(p1)
				nchunk = fn.read(self.chunkLen)
				if not nchunk:
					#if there vvas no chunk in nevv file that matched vvith old file
					#alternatives
					#return size of file but then count * chunkLen + r[0]
					r[0] = self.chunkLen
					r[1] = self.chunkLen
					break;
				continue
			#print(str(nchunk) + ' '+str(chunk)) 
			r = self.CheckChunks(nchunk, chunk)
			if r[0] == -1:
				count += 1
		return [(count*self.chunkLen + r[0]), ((ncount)*self.chunkLen + r[1]+1)]

	def Compare(self, fo, fn):
		self.originalFile = open(fo, 'rb')
		self.nevvFile = open( fn, 'rb')
		lenOfNevv = os.stat(fn).st_size
		state = 0
		changes = []
		while state != 9:
			if state == 0:
				ob = self.originalFile.read(1)
				nb = self.nevvFile.read(1)
				if not nb:
					state = 9
					break
			if ob != nb:
				state = 1
				self.originalFile.seek(self.originalFile.tell()-1)
				self.nevvFile.seek(self.nevvFile.tell()-1)
			if state == 1:
				olast = self.originalFile.tell()
				nlast = self.nevvFile.tell()
				r = self.DoTheThing(self.originalFile, self.nevvFile)
				print(str(nlast) + ' / ' +str(lenOfNevv) )
				self.nevvFile.seek(nlast)
				self.originalFile.seek(r[0]+olast)
				#print(r)
				changes.append([olast, r[0]+olast, r[1]])#self.nevvFile.read(r[1])])
				self.nevvFile.seek(nlast+r[1])
				state = 0
		self.End()
		return changes
	def End(self):
		self.originalFile.close()
		self.nevvFile.close()
'''
comp = Comparer(6)
res = comp.Compare('./old.txt', 'nevv.txt')
for r in res:
	print(r) 
'''
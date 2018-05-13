
import sys
import os
import json
import socket
import math
import mimetypes

ofile = open('./old.txt', 'rb')
run = True
ofile.seek(2)
'''for i in range(0, 3):
	ob = ofile.read(1)
	print(str(i+2) +' : ' +str(ob))
'''
while run:
	ob = ofile.read(1)
	if not ob:
		run = False
	olast = ofile.tell()
	print(str(olast) +' : ' +str(ob))
	


ofile.close()

import numpy as np
import matplotlib.pyplot as plt

f = open("grids/decomposed_grid_pp.dat")
rawdata = f.read().splitlines()
ndomainline = rawdata[0].split(' ')
ndomain = int(ndomainline[1])
linecount=1
datacount = 0
rawcells = []
for i in range (0, ndomain):
	elemvec = rawdata[linecount].split(' ')
	ncell = int(elemvec[1])
	for j in range (0, ncell) :
		thisline = linecount + j + 1
		rawcells.append(rawdata[thisline])
		#datacount = datcount + 1
	nedge = int(elemvec[2])
	if (i == 0) :
		nbedge = int(elemvec[3])
	else :
		nbedge = 0
	linecount = linecount + ncell + nedge + nbedge + 1

cellx = []
for i in range (0,len(rawcells)): 
	#print(rawcells[i])
	thisrawcell = rawcells[i].split(' ')
	cellx.append([thisrawcell[13], thisrawcell[14]])
	#print([thisrawcell[13], thisrawcell[14]])

cellx = np.array(cellx, dtype=np.float64)

X = cellx[:,0]
Y = cellx[:,1]

plt.scatter(X,Y)
plt.show()


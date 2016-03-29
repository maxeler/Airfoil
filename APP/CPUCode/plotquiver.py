import numpy as np
import matplotlib.pyplot as plt
from matplotlib import patches

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


g = open("quiver.dat")
rawdata = g.read().splitlines()
for i in range (0,len(rawdata)): 
	rawdata[i] = rawdata[i].split(' ')
data = np.array(rawdata, dtype=np.float64)
#print(data)
#X = data[:,0]
#Y = data[:,1]
U = data[:,1]
V = data[:,2]
R = data[:,0]
E = data[:,3]

p = [X,Y]

nx = 200
ny = 200
xx = np.linspace(-1,2,nx)
yy = np.linspace(-1,1,ny)
#g_x,g_y = np.mgrid[0:1:100, 0:1:100]
g_x,g_y = np.meshgrid(xx,yy)

from scipy.interpolate import griddata
g_u = griddata(cellx, U, (g_x,g_y), method='cubic')
g_v = griddata(cellx, V, (g_x,g_y), method='cubic')

mask = np.zeros(g_u.shape, dtype=bool)
mask[90:110,95:135]=1;
g_u = np.ma.array(g_u, mask=mask)
speed = np.sqrt(g_u*g_u + g_v*g_v)

ell = patches.Ellipse((0.5,0), 0.8, 0.075, color='grey')
ell2 = patches.Ellipse((0.5,0), 0.8, 0.075, color='grey')

fig,axarr = plt.subplots(1,2)
q1=axarr[0].quiver(X,Y,U,V,R,pivot='mid',scale=12.0,width=0.005)
axarr[0].add_patch(ell)
axarr[0].set_title('Flow Field')
cbar0 = fig.colorbar(q1,ax=axarr[0])
cbar0.set_label('Density', rotation = 270, labelpad = 15)

axarr[0].axis([-1, 2, -0.75, 0.75])
q2=axarr[1].streamplot(g_x,g_y,g_u,g_v,color=speed)
axarr[1].add_patch(ell2)
axarr[1].set_title('Stream Function')
cbar1 = fig.colorbar(q2.lines,ax=axarr[1])
cbar1.set_label('Stream Function Magnitude', rotation =270, labelpad = 15)
axarr[1].axis([-1, 2, -0.75, 0.75])
#plt.scatter(X,Y)
plt.show()

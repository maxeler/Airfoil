/*
 * AirfoilDFEDomain.cpp
 *
 *  Created on: 22 Feb 2016
 *      Author: dpackwood
 */

#include "AirfoilDFEDomain.h"
#include <stdio.h>
#include <stdlib.h>
#include <deque>
#include <utility>

void AirfoilDFEDomain::read (std::string infile) {

	printf("reading in %s \n",infile.c_str());

	FILE *fp;
	if ( (fp = fopen(infile.c_str(),"r")) == NULL) {
		printf("can't open file %s\n",infile.c_str()); exit(-1);
	}

	if (fscanf(fp,"%d\n", &ndomain) != 1) {
		printf("error reading first line from %s\n",infile.c_str()); exit(-1);
	}

	ncell = (int*) malloc(ndomain*sizeof(int));
	nedge = (int*) malloc(ndomain*sizeof(int));

	cedgepart   = (int **) malloc(ndomain*sizeof(int*));
	cedgeind   = (int **) malloc(ndomain*sizeof(int*));
	cedgeid   = (int **) malloc(ndomain*sizeof(int*));
	ecellpart  = (int **) malloc(ndomain*sizeof(int*));
	ecellind  = (int **) malloc(ndomain*sizeof(int*));

	dx     = (double **) malloc(ndomain*sizeof(double*));
	q      = (double **) malloc(ndomain*sizeof(double*));
	qold   = (double **) malloc(ndomain*sizeof(double*));
	res    = (double **) malloc(ndomain*sizeof(double*));
	adt    = (double **) malloc(ndomain*sizeof(double*));
	adtold    = (double **) malloc(ndomain*sizeof(double*));

	if (fscanf(fp,"%d %d %d\n", &ncell[0],&nedge[0],&nbedge) != 3) {
		printf("error reading cpu domain description line from %s\n",infile.c_str()); exit(-1);
	}

	cedgepart[0] = (int *) malloc(4*ncell[0]*sizeof(int));
	cedgeind[0] = (int *) malloc(4*ncell[0]*sizeof(int));
	cedgeid[0] = (int *) malloc(4*ncell[0]*sizeof(int));
	ecellpart[0] = (int *) malloc(2*nedge[0]*sizeof(int));
	ecellind[0] = (int *) malloc(2*nedge[0]*sizeof(int));

	dx[0] = (double *) malloc(3*nedge[0]*sizeof(double));
	q[0] = (double *) malloc(4*ncell[0]*sizeof(double));
	qold[0] = (double *) malloc(4*ncell[0]*sizeof(double));
	res[0] = (double *) malloc(4*ncell[0]*sizeof(double));
	adt[0] = (double *) malloc(ncell[0]*sizeof(double));
	adtold[0] = (double *) malloc(ncell[0]*sizeof(double));

	becell = (int *) malloc(  nbedge*sizeof(int));
	bound  = (int *) malloc(  nbedge*sizeof(int));
	bdx      = (double *) malloc(3*nbedge*sizeof(double));


	for (int n=0; n<ncell[0]; n++) {
		for (int e = 0 ; e < 4; e++){
			if (fscanf(fp,"%d %d ",&cedgepart[0][4*n+e], &cedgeind[0][4*n+e]) != 2) {
				printf("error reading domain %d cell %d edge indices from %s\n",0,n,infile.c_str()); exit(-1);
			}
		}
		if (fscanf(fp,"%d %d %d %d\n", &cedgeid[0][4*n], &cedgeid[0][4*n+1], &cedgeid[0][4*n+2], &cedgeid[0][4*n+3]) != 4) {
			printf("error reading domain %d cell %d edge ids from %s\n",0,n,infile.c_str()); exit(-1);
		}
	}

	for (int n=0; n<nedge[0]; n++) {
		if (fscanf(fp,"%lf %lf %lf ",&dx[0][3*n], &dx[0][3*n+1], &dx[0][3*n+2]) != 3){
			printf("error reading domain %d edge %d data from %s\n",0,n,infile.c_str()); exit(-1);
		}
		for (int e = 0; e < 2; e++){
			if (fscanf(fp,"%d %d \n",&ecellpart[0][2*n+e],&ecellind[0][2*n+e]) != 2) {
				printf("error reading domain %d edge %d cell indices from %s\n",0,n,infile.c_str()); exit(-1);
			}
			fscanf(fp,"\n");
		}
	}

	for (int n=0; n<nbedge; n++) {
		if (fscanf(fp,"%lf %lf %lf %d %d\n",&bdx[3*n],&bdx[3*n+1],&bdx[3*n+2],
				&becell[n], &bound[n]) != 5) {
			printf("error reading bedge %d data from %s\n",n,infile.c_str()); exit(-1);
		}
	}

	for (int d = 1; d < ndomain; d++){
		printf("Reading subdomain %d\n",d);

		if (fscanf(fp,"%d %d\n", &ncell[d],&nedge[d]) != 2) {
			printf("error reading %d domain description line from %s\n",d,infile.c_str()); exit(-1);
		}
		cedgepart[d] = (int *) malloc(4*ncell[d]*sizeof(int));
		cedgeind[d] = (int *) malloc(4*ncell[d]*sizeof(int));
		cedgeid[d] = (int *) malloc(4*ncell[d]*sizeof(int));
		ecellpart[d] = (int *) malloc(2*nedge[d]*sizeof(int));
		ecellind[d] = (int *) malloc(2*nedge[d]*sizeof(int));

		dx[d] = (double *) malloc(3*nedge[d]*sizeof(double));
		q[d] = (double *) malloc(4*ncell[d]*sizeof(double));
		qold[d] = (double *) malloc(4*ncell[d]*sizeof(double));
		res[d] = (double *) malloc(4*ncell[d]*sizeof(double));
		adt[d] = (double *) malloc(ncell[d]*sizeof(double));
		adtold[d] = (double *) malloc(ncell[d]*sizeof(double));

		for (int n=0; n<ncell[d]; n++) {
			for (int e = 0 ; e < 4; e++){
				if (fscanf(fp,"%d %d ",&cedgepart[d][4*n+e], &cedgeind[d][4*n+e]) != 2) {
					printf("error reading domain %d cell %d edge indices  from %s\n",d,n,infile.c_str()); exit(-1);
				}
			}
			if (fscanf(fp,"%d %d %d %d\n", &cedgeid[d][4*n], &cedgeid[d][4*n+1], &cedgeid[d][4*n+2], &cedgeid[d][4*n+3]) != 4) {
				printf("error reading domain %d cell %d edge ids from %s\n",d,n,infile.c_str()); exit(-1);
			}
		}

		for (int n=0; n<nedge[d]; n++) {
			if (fscanf(fp,"%lf %lf %lf ",&dx[d][3*n], &dx[d][3*n+1], &dx[d][3*n+2]) != 3){
				printf("error reading domain %d edge %d data from %s\n",d,n,infile.c_str()); exit(-1);
			}
			for (int e = 0; e < 2; e++){
				if (fscanf(fp,"%d %d \n",&ecellpart[d][2*n+e],&ecellind[d][2*n+e]) != 2) {
					printf("error reading domain %d edge %d cell indices from %s\n",d,n,infile.c_str()); exit(-1);
				}
				fscanf(fp,"\n");

			}
		}
	}
	fclose(fp);

	nedgecomputedfe = 0;
	ncellcomputedfe = 0;
	for (int d = 1; d < ndomain; d++) {
		nedgecomputedfe += nedge[d];
		ncellcomputedfe += ncell[d];
	}

	initialized = true;
};

void AirfoilDFEDomain::writeQuiver() {

	bool ** placed = (bool**) malloc(sizeof(bool*)*ndomain);
	double ** pos = (double**) malloc(sizeof(double*)*ndomain);

	int totcell = 0;
	for (int d = 0; d < ndomain; d++){
		placed[d] = (bool*) calloc(sizeof(bool),ncell[d]);
		pos[d] = (double*) calloc(sizeof(double),2*ncell[d]);
		totcell += ncell[d];
	}

	std::deque<std::pair<int,int> > toPlace = std::deque<std::pair<int,int> >();
	toPlace.push_back(std::make_pair(0,0));
	pos[0][0] = 0.0;
	pos[0][1] = 0.0;
	placed[0][0] = true;

	int proccell = 0;
	while (toPlace.size() > 0) {
		std::pair<int,int> thisCell = toPlace.front();

		int thisCellPart = thisCell.first;
		int thisCellInd = thisCell.second;

		for (int i = 0; i < 4; i++) {

			int thisEdgePart = cedgepart[thisCellPart][thisCellInd*4+i];
			int thisEdgeInd = cedgeind[thisCellPart][thisCellInd*4+i];
			int thisEdgeId = cedgeid[thisCellPart][thisCellInd*4+i];

			if (thisEdgeId == 0) {

				int firstCellPart = ecellpart[thisEdgePart][thisEdgeInd*2];
				int firstCellInd = ecellind[thisEdgePart][thisEdgeInd*2];

				int secondCellPart = ecellpart[thisEdgePart][thisEdgeInd*2+1];
				int secondCellInd = ecellind[thisEdgePart][thisEdgeInd*2+1];

				bool thisFirst = (firstCellPart == thisCellPart) && (firstCellInd == thisCellInd);
				bool thisSecond = (secondCellPart == thisCellPart) && (secondCellInd == thisCellInd);

				if ((!thisFirst) && (!thisSecond)) printf("Problem printing quiver\n");

				if (thisFirst) {
					if (!placed[secondCellPart][secondCellInd]) {
						pos[secondCellPart][secondCellInd*2]   = pos[thisCellPart][thisCellInd*2]   + dx[thisEdgePart][thisEdgeInd*3];
						pos[secondCellPart][secondCellInd*2+1] = pos[thisCellPart][thisCellInd*2+1] + dx[thisEdgePart][thisEdgeInd*3+1];
						placed[secondCellPart][secondCellInd] = true;
						toPlace.push_back(std::make_pair(secondCellPart,secondCellInd));
					}
				} else {
					if (!placed[firstCellPart][firstCellInd]) {
						pos[firstCellPart][firstCellInd*2]   = pos[thisCellPart][thisCellInd*2]   - dx[thisEdgePart][thisEdgeInd*3];
						pos[firstCellPart][firstCellInd*2+1] = pos[thisCellPart][thisCellInd*2+1] - dx[thisEdgePart][thisEdgeInd*3+1];
						placed[firstCellPart][firstCellInd] = true;
						toPlace.push_back(std::make_pair(firstCellPart,firstCellInd));
					}
				}
			}
		}
		proccell++;
		toPlace.pop_front();
	}
	printf("Cells processed/total: %d/%d\n",proccell,totcell);

	FILE * fp;
	fp = fopen("quiver.dat","w");
	for (int d = 0; d < ndomain; d++) {
		for (int i = 0; i < ncell[d]; i++) {
			fprintf(fp,"%lf %lf %lf %lf\n", q[d][4*i], q[d][4*i+1], q[d][4*i+2], q[d][4*i+3]);
		}
	}
	fclose(fp);
};


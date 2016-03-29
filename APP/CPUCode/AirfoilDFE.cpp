/*
 * Open source copyright declaration based on BSD open source template:
 * http://www.opensource.org/licenses/bsd-license.php
 *
 * This file is part of the OP2 distribution.
 *
 * Copyright (c) 2011, Mike Giles and others. Please see the AUTHORS file in
 * the main source directory for a full list of copyright holders.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of Mike Giles may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Mike Giles ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Mike Giles BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
//     Nonlinear airfoil lift calculation
//
//     Written by Mike Giles, 2010-2011, based on FORTRAN code
//     by Devendra Ghate and Mike Giles, 2005
//

//
// standard headers
//
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "airfoildfe.h"

double get_wall_time(){
	struct timeval time;
	if (gettimeofday(&time,NULL)){
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
// global constants

double gam, gm1, cfl, eps, mach, alpha, qinf[4];

#include "save_soln.h"
#include "adt_calc.h"
#include "res_calc.h"
#include "bres_calc.h"
#include "update.h"
#include "AirfoilDFEDomain.h"
#include "AirfoilDFEInterface.h"
#include <string>


void calcCPUAdt(AirfoilDFEDomain airfoilDomain) {

	for (int adt_cell_iter = 0; adt_cell_iter < airfoilDomain.ncell[0]; adt_cell_iter++){

		int thispart = 0;
		int thisind = adt_cell_iter;

		int eid0 = airfoilDomain.cedgeid[thispart][thisind*4];
		int eid1 = airfoilDomain.cedgeid[thispart][thisind*4+1];
		int eid2 = airfoilDomain.cedgeid[thispart][thisind*4+2];
		int eid3 = airfoilDomain.cedgeid[thispart][thisind*4+3];

		double *dx0, *dx1, *dx2, *dx3;

		if (eid0 == 0) dx0 = airfoilDomain.dx[airfoilDomain.cedgepart[thispart][thisind*4]] + 3*airfoilDomain.cedgeind[thispart][thisind*4];
		else  dx0 = airfoilDomain.bdx + 3*airfoilDomain.cedgeind[thispart][thisind*4];

		if (eid1 == 0) dx1 = airfoilDomain.dx[airfoilDomain.cedgepart[thispart][thisind*4+1]] + 3*airfoilDomain.cedgeind[thispart][thisind*4+1];
		else  dx1 = airfoilDomain.bdx + 3*airfoilDomain.cedgeind[thispart][thisind*4+1];

		if (eid2 == 0) dx2 = airfoilDomain.dx[airfoilDomain.cedgepart[thispart][thisind*4+2]] + 3*airfoilDomain.cedgeind[thispart][thisind*4+2];
		else  dx2 = airfoilDomain.bdx + 3*airfoilDomain.cedgeind[thispart][thisind*4+2];

		if (eid3 == 0) dx3 = airfoilDomain.dx[airfoilDomain.cedgepart[thispart][thisind*4+3]] + 3*airfoilDomain.cedgeind[thispart][thisind*4+3];
		else  dx3 = airfoilDomain.bdx + 3*airfoilDomain.cedgeind[thispart][thisind*4+3];


		double *q0 = airfoilDomain.q[thispart] + 4*thisind;
		double *adt0 = airfoilDomain.adt[thispart] + thisind;

		adt_calc(dx0,dx1,dx2,dx3,q0,adt0);
	}
}

void calcCPURes(AirfoilDFEDomain airfoilDomain) {
	for (int res_edge_iter = 0; res_edge_iter < airfoilDomain.nedge[0]; res_edge_iter++){

		int thispart = 0;
		int thisind = res_edge_iter;

		double * dx0 = airfoilDomain.dx[thispart] + 3*thisind;
		double * q0 = airfoilDomain.q[airfoilDomain.ecellpart[thispart][thisind*2]]   + 4*airfoilDomain.ecellind[thispart][thisind*2];
		double * q1 = airfoilDomain.q[airfoilDomain.ecellpart[thispart][thisind*2+1]] + 4*airfoilDomain.ecellind[thispart][thisind*2+1];

		double * adt0 = airfoilDomain.adt[airfoilDomain.ecellpart[thispart][thisind*2]]   + airfoilDomain.ecellind[thispart][thisind*2];
		double * adt1 = airfoilDomain.adt[airfoilDomain.ecellpart[thispart][thisind*2+1]] + airfoilDomain.ecellind[thispart][thisind*2+1];

		double * res0 = airfoilDomain.res[airfoilDomain.ecellpart[thispart][thisind*2]]   + 4*airfoilDomain.ecellind[thispart][thisind*2];
		double * res1 = airfoilDomain.res[airfoilDomain.ecellpart[thispart][thisind*2+1]] + 4*airfoilDomain.ecellind[thispart][thisind*2+1];

		res_calc(dx0,q0,q1,adt0,adt1,res0,res1);
	}
}

void calcCPUBres(AirfoilDFEDomain airfoilDomain) {

	for (int bres_edge_iter = 0; bres_edge_iter < airfoilDomain.nbedge; bres_edge_iter++){

		double * dx0 = airfoilDomain.bdx + 3*bres_edge_iter;

		double * q0 = airfoilDomain.q[0] + 4*airfoilDomain.becell[bres_edge_iter];

		double * adt0 = airfoilDomain.adt[0] + airfoilDomain.becell[bres_edge_iter];

		double * res0 = airfoilDomain.res[0] + 4*airfoilDomain.becell[bres_edge_iter];

		int * bound0 = airfoilDomain.bound + bres_edge_iter;

		bres_calc(dx0,q0,adt0,res0,bound0);
	}
}

void calcCPUUpdate(AirfoilDFEDomain airfoilDomain, double *rms) {
	*rms = 0.0;
	for (int update_cell_iter = 0; update_cell_iter < airfoilDomain.ncell[0]; update_cell_iter ++){

		double *q0 = airfoilDomain.q[0] + 4*update_cell_iter;
		double *qold0 = airfoilDomain.qold[0] + 4*update_cell_iter;

		double *res0 = airfoilDomain.res[0] + 4*update_cell_iter;
		double *adt0 = airfoilDomain.adt[0] + update_cell_iter;

		update(qold0,q0,res0,adt0,rms);
	}
}

void calcCPUSave(AirfoilDFEDomain airfoilDomain) {

	for (int d = 0; d < airfoilDomain.ndomain; d++){
		for (int save_cell_iter = 0; save_cell_iter < airfoilDomain.ncell[d]; save_cell_iter++){

			double *q0 = airfoilDomain.q[d] + 4*save_cell_iter;
			double *qold0 = airfoilDomain.qold[d] + 4*save_cell_iter;

			save_soln(q0,qold0);
		}
	}
}

int main(int argc, char **argv)
{

	double  rms;

	//timer
	double wall_t1, wall_t2;

	std::string infile; 
	if (argc > 1){
		infile = std::string(argv[1]);
		printf(infile.c_str()); printf("\n");
	} else {
		infile = std::string("grids/decomposed_grid.dat");
	}
	// read in gri

	std::string planfile;
	if (argc > 2){
		planfile = std::string(argv[2]);
		printf(planfile.c_str()); printf("\n");
	} else {
		planfile = std::string("grids/res_plan.dat");
	}

	AirfoilDFEDomain airfoilDomain;
	airfoilDomain.read(infile);
	AirfoilDFEInterface airfoilInterface = AirfoilDFEInterface(planfile, &airfoilDomain);

	printf("Initialising flow field \n");

	gam = 1.4f;
	gm1 = gam - 1.0f;
	cfl = 0.9f;
	eps = 0.05f;

	double mach  = 0.4f;
	//double alpha = 3.0f*atan(1.0f)/45.0f;
	double p     = 1.0f;
	double r     = 1.0f;
	double u     = sqrt(gam*p/r)*mach;
	double e     = p/(r*gm1) + 0.5f*u*u;

	qinf[0] = r;
	qinf[1] = r*u;
	qinf[2] = 0.0f;
	qinf[3] = r*e;

	airfoilInterface.initLMEMArrays(qinf);
	airfoilInterface.runSetupAction();



	printf("Setup complete \n");
	wall_t1 =get_wall_time();

	int niter = 100;


	int ncelltot = 0;
	for (int i = 0; i < airfoilDomain.ndomain; i++) ncelltot+=airfoilDomain.ncell[i];
	for(int iter = 1; iter <= niter; iter++) {

		// save old flow solution
		calcCPUSave(airfoilDomain);

		// predictor/corrector update loop
		for(int k=0; k<2; k++) {
			calcCPUAdt(airfoilDomain);
			calcCPURes(airfoilDomain);

			double dferms = 0;
			airfoilInterface.runMainAction(k, cfl, gam, gm1, eps, &dferms);

			calcCPUBres(airfoilDomain);
			double cpurms = 0;
			calcCPUUpdate(airfoilDomain, &cpurms);

			rms = cpurms + dferms;

		}
		rms = sqrt(rms/(double) ncelltot);
		if (iter%1 == 0){

			printf("rms %d  %10.5e \n",iter,rms);
		}
	}
	airfoilInterface.runOutputAction();
	airfoilDomain.writeQuiver();
	wall_t2= get_wall_time();
	std::cout<<"Total walltime: " << wall_t2-wall_t1 <<" seconds"<<std::endl;
}


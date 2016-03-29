/*
 * AirfoilDFEInterface.cpp
 *
 *  Created on: 23 Feb 2016
 *      Author: dpackwood
 */

#include "AirfoilDFEInterface.h"

/**
 *	Constructor takes a string where a plan is contained and a pointer to a domain object, parses the plan and sets up unrolled arrays for streaming to DFE
 */
AirfoilDFEInterface::AirfoilDFEInterface(std::string planfile, AirfoilDFEDomain * domain_) {

	domain = domain_;

	nEdgeComputeDFE = (*domain).nedgecomputedfe;
	nCellComputeDFE = (*domain).ncellcomputedfe;

	maxfile = airfoildfe_init();
	engine = max_load(maxfile,"*");

	memAddresses = (int * ) malloc(sizeof(int)*(MEM_ADDRESS_ITEMS+1));

	parsePlan(planfile);

	int cumulativeAddress = 0;
	memAddresses[adtDx] = cumulativeAddress*burstSize;
	int floatBits = max_get_constant_uint64t(maxfile,"floatBits");
	int adtDxBursts = getNBursts(floatBits, nCellComputeDFE*12);
	cumulativeAddress += adtDxBursts;

	memAddresses[resReadOnly] = cumulativeAddress*burstSize;
	int resReadOnlyBursts = getNBursts(resReadOnlyBits, nEdgeComputeDFE);
	cumulativeAddress += resReadOnlyBursts;

	memAddresses[q] = cumulativeAddress*burstSize;
	int qBursts = getNBursts(floatBits, nCellComputeDFE*4);
	cumulativeAddress += qBursts;

	memAddresses[qold] = cumulativeAddress*burstSize;
	cumulativeAddress += qBursts;

	memAddresses[MEM_ADDRESS_ITEMS] = cumulativeAddress*burstSize;

	qpadtPortWidth = max_get_constant_uint64t(maxfile, "qpadtPortPCIeWidthInBytes");
			;
	printf("Generated lmem addresses, sizes:\n");
	for (int i = 0; i < MEM_ADDRESS_ITEMS; i++) printf("%d,%d\n",memAddresses[i], memAddresses[i+1]- memAddresses[i]);
	printf("\n");
}

/**
 * Simple parsing of the plan into memory
 */
void AirfoilDFEInterface::parsePlan(std::string planfile) {

	printf("reading in %s \n\n\n",planfile.c_str());

	FILE *fplan;
	if ( (fplan = fopen(planfile.c_str(),"r")) == NULL) {
		printf("can't open file %s\n",planfile.c_str()); exit(-1);
	}

	fscanf(fplan,"%d %d %d %d %d %d %d %d %d\n",&nPipes,&nReads,&nMainCommands,&nColours,&bufferDepth,&nFlushPipes,&nFlushCommands,&nFlushColours,&flushBufferDepth);
	printf("Read compute plan, with:\n");
	printf("--| %d Total Cell Reads/Writes\n",nReads);
	printf("--| %d Pipes with %d Commands\n",nPipes, nMainCommands);
	printf("--| %d Flushing Pipes with %d Commands\n",nFlushPipes,nFlushCommands);
	printf("--| Main Buffer has %d Colours and Depth of %d\n",nColours,bufferDepth);
	printf("--| Flushing Buffer has %d Colours and Depth of %d\n\n\n",nFlushColours,flushBufferDepth);

	schedDatSize = 7*nMainCommands;
	flushscheddatsize = 3*nFlushCommands;

	readlocs = (int*) malloc(nReads*sizeof(int)*2);
	reads = (int*) malloc(schedDatSize*sizeof(int));
	flushes = (int *) malloc(flushscheddatsize*sizeof(int));

	for (int i = 0; i < nReads; i++){
		fscanf(fplan, "%d %d\n", &readlocs[2*i], &readlocs[2*i+1]);
	}

	for (int i = 0; i < nMainCommands; i++){
		fscanf(fplan, "%d %d %d %d %d %d %d\n", &reads[7*i], &reads[7*i+1], &reads[7*i+2], &reads[7*i+3], &reads[7*i+4], &reads[7*i+5], &reads[7*i+6]);
	}
	for (int i = 0; i < nFlushCommands; i++){
		fscanf(fplan, "%d %d %d\n", &flushes[3*i], &flushes[3*i+1], &flushes[3*i+2]);
	}

	fclose(fplan);

	resComputeTicks = (nMainCommands/2 -1)/nPipes + 1;
	resFlushTicks = (nFlushCommands - 1)/nFlushPipes + 1;
	printf("Plan stipulates %d res compute ticks, %d total ticks(including flushing)\n",resComputeTicks,resFlushTicks);
}

/**
 * Initializes the read only arrays for DFE compute
 */
void AirfoilDFEInterface::initLMEMArrays(double * qinf) {


	adtDxDatSize = (memAddresses[adtDx+1] - memAddresses[adtDx]);
	resReadOnlyDatSize = (memAddresses[resReadOnly+1] - memAddresses[resReadOnly]);
	qDatSize = (memAddresses[q+1] - memAddresses[q]);

	int ncellcomputedfe = (*domain).ncellcomputedfe;

	int * ncell = (*domain).ncell;
	int * nedge = (*domain).nedge;

	int ** cedgepart = (*domain).cedgepart;
	int ** cedgeind = (*domain).cedgeind;
	int ** cedgeid = (*domain).cedgeid;

	double ** dx = (*domain).dx;
	double * bdx = (*domain).bdx;

	//printf("Res read only struct size:%d\n", sizeof(resReadOnlyStruct));

	dfeResReadOnly = (resReadOnlyStruct *) malloc(resReadOnlyDatSize);
	int thisPart = 1;
	int thisInd = 0;
	int schedInd = 0;
	int flushSchedInd = 0;
	passtorescount = 0;
	for (int i = 0; i < resComputeTicks; i++) {

		resReadOnlyStruct thisStruct;
		for (int j = 0; j < nPipes; j++) {
			for (int k = 0; k < 2; k++) {
				resMainStruct thisMainStruct;
				thisMainStruct.bufferId = reads[7*schedInd];
				thisMainStruct.bufferAddress = reads[7*schedInd+1];
				thisMainStruct.bufferRead = reads[7*schedInd+3] == 0 ? 1 : 0;
				thisMainStruct.bufferWrite = (reads[7*schedInd+3] == 0 && reads[7*schedInd+2] == 1) ? 1 : 0;
				thisMainStruct.inputSelect = reads[7*schedInd+3];
				thisMainStruct.flushBufferId = reads[7*schedInd+4];
				thisMainStruct.flushBufferAddress = reads[7*schedInd+5];
				thisMainStruct.flushBufferRead = 0;
				thisMainStruct.flushBufferWrite = reads[7*schedInd+6];
				thisMainStruct.padding = 0;
				thisStruct.mainStructs[2*j+k] = thisMainStruct;
				if (reads[7*schedInd+3] == 1){
					passtorescount++;
				}
				schedInd++;
			}
			for (int k = 0; k < 3; k++) {
				//printf("%d/%d %d\n", thisPart, (*domain).ndomain, thisInd);
				thisStruct.dxs[j*3+k] = dx[thisPart][thisInd*3+k];
			}
			thisInd ++;
			if (thisInd == nedge[thisPart] ){
				if (thisPart < ((*domain).ndomain-1)) thisPart++;
				thisInd = 0;
			}
		}
		for (int j = 0; j < nFlushPipes; j++) {
			resFlushStruct thisFlushStruct;
			thisFlushStruct.bufferID = flushes[3*flushSchedInd];
			thisFlushStruct.bufferAddress = flushes[3*flushSchedInd+1];
			thisFlushStruct.bufferRead = flushes[3*flushSchedInd+2];
			thisFlushStruct.bufferWrite = 0;
			thisFlushStruct.padding = 0;
			thisStruct.flushStructs[j] = thisFlushStruct;
			flushSchedInd++;
		}
		dfeResReadOnly[i] = thisStruct;
	}

	int paddedPasToResCount = qpadtPortWidth*((passtorescount*5 + qpadtPortWidth - 1)/qpadtPortWidth);
	cpuQpadtSize = paddedPasToResCount*sizeof(double);
	if (passtorescount%2 == 1) passtorescount+=1;
	cpu_res_qpadt = (double *) malloc(cpuQpadtSize);
	dfe_res_res = (double *) calloc(4*passtorescount,sizeof(double));

	adtdatsize = ncellcomputedfe;
	int adtrem = adtdatsize %4;
	if(adtrem != 0) adtdatsize += (4-adtrem);

	//dfeResReadOnly = (char *) malloc(resReadOnlyDatSize);
	dfeAdtDX = (double*) malloc(adtDxDatSize);
	dfeQ = (double*) malloc(qDatSize);
	dfe_update_q = (double*) malloc(4*sizeof(double)*adtdatsize);

	thisPart = 1;
	thisInd = 0;
	for (int i = 0; i < ncellcomputedfe; i++){

		int cellpart = readlocs[2*i];
		int cellind = readlocs[2*i+1];

		for (int j = 0; j < 4; j ++){

			int edgepart = cedgepart[cellpart][cellind*4+j];
			int edgeind = cedgeind[cellpart][cellind*4+j];
			int edgeid = cedgeid[cellpart][cellind*4+j];

			for(int k = 0; k < 3; k++){
				if (edgeid == 0){
					dfeAdtDX[i*12 + 3*j + k] = dx[edgepart][edgeind*3 + k];
				} else {
					dfeAdtDX[i*12 + 3*j + k] = bdx[edgeind*3 + k];
				}
			}
		}

		thisInd++;
		if (thisInd == ncell[thisPart]){
			thisPart++;
			thisInd = 0;
		}
	}


	int ncelltot = 0;
	for (int d = 0; d < (*domain).ndomain; d++){
		for (int n=0; n < (*domain).ncell[d]; n++) {
			for (int m=0; m<4; m++) {
				(*domain).q[d][4*n+m] = qinf[m];
				(*domain).res[d][4*n+m] = 0.0f;
			}
		}
		ncelltot += (*domain).ncell[d];
	}

	thisPart = 1;
	thisInd = 0;
	for (int i = 0; i < ncellcomputedfe; i++){

		int cellpart = readlocs[2*i];
		int cellind =  readlocs[2*i+1];

		for (int j = 0; j < 4; j ++) dfeQ[i*4+j] = (*domain).q[cellpart][cellind*4+j];

		thisInd++;
		if (thisInd == (*domain).ncell[thisPart]){
			thisPart++;
			thisInd = 0;
		}
	}

}

void AirfoilDFEInterface::runOutputAction () {

	max_actions_t * act;
	act =  max_actions_init(maxfile, NULL);

	max_queue_output(act, "qCPUOut", dfeQ, qDatSize);
	max_lmem_linear(act, "qRead", memAddresses[q], qDatSize);

	max_ignore_lmem(act, "setupWrite");
	max_ignore_lmem(act, "updateQ");
	max_ignore_lmem(act, "updateQold");
	max_ignore_lmem(act, "updateSaveQold");
	max_ignore_lmem(act, "adtQ");
	max_ignore_lmem(act, "adtDxRead");
	max_ignore_lmem(act, "resReadOnly");
	max_ignore_kernel(act, "AirfoilDFEResKernel");
	max_ignore_kernel(act, "AirfoilDFEAdtKernel");
	max_ignore_kernel(act, "AirfoilDFEUpdateKernel");
	max_run(engine, act);
	max_actions_free(act);

	int thispart = 1;
	int thisind = 0;
	for (int i = 0; i < (*domain).ncellcomputedfe; i++){

		int cellpart = readlocs[2*i];
		int cellind =  readlocs[2*i+1];

		for (int j = 0; j < 4; j ++) (*domain).q[cellpart][cellind*4+j] = dfeQ[i*4+j];

		thisind++;
		if (thisind == (*domain).ncell[thispart]){
			thispart++;
			thisind = 0;
		}
	}
}

/**
 * The initial action to set up arrays in lmem for main compute
 */
void AirfoilDFEInterface::runSetupAction () {

	max_actions_t * act;
	act = max_actions_init(maxfile, NULL);
	max_queue_input(act, "setupCPU", dfeAdtDX, adtDxDatSize);
	max_lmem_linear(act, "setupWrite", memAddresses[adtDx], adtDxDatSize);
	max_ignore_lmem(act, "adtDxRead");
	max_ignore_lmem(act, "resReadOnly");
	max_ignore_lmem(act, "qRead");
	max_ignore_lmem(act, "updateQ");
	max_ignore_lmem(act, "updateQold");
	max_ignore_lmem(act, "adtQ");
	max_ignore_lmem(act, "updateSaveQold");
	max_ignore_kernel(act, "AirfoilDFEResKernel");
	max_ignore_kernel(act, "AirfoilDFEAdtKernel");
	max_ignore_kernel(act, "AirfoilDFEUpdateKernel");
	max_run(engine, act);
	max_actions_free(act);

	act = max_actions_init(maxfile, NULL);
	max_queue_input(act, "setupCPU", dfeQ, qDatSize);
	max_lmem_linear(act, "setupWrite", memAddresses[q], qDatSize);
	max_ignore_lmem(act, "adtDxRead");
	max_ignore_lmem(act, "resReadOnly");
	max_ignore_lmem(act, "qRead");
	max_ignore_lmem(act, "updateQ");
	max_ignore_lmem(act, "updateQold");
	max_ignore_lmem(act, "updateSaveQold");
	max_ignore_lmem(act, "adtQ");
	max_ignore_kernel(act, "AirfoilDFEResKernel");
	max_ignore_kernel(act, "AirfoilDFEAdtKernel");
	max_ignore_kernel(act, "AirfoilDFEUpdateKernel");
	max_run(engine, act);
	max_actions_free(act);

	act = max_actions_init(maxfile, NULL);
	max_queue_input(act, "setupCPU", dfeQ, qDatSize);
	max_lmem_linear(act, "setupWrite", memAddresses[qold], qDatSize);
	max_ignore_lmem(act, "adtDxRead");
	max_ignore_lmem(act, "resReadOnly");
	max_ignore_lmem(act, "qRead");
	max_ignore_lmem(act, "updateQ");
	max_ignore_lmem(act, "updateQold");
	max_ignore_lmem(act, "adtQ");
	max_ignore_lmem(act, "updateSaveQold");
	max_ignore_kernel(act, "AirfoilDFEResKernel");
	max_ignore_kernel(act, "AirfoilDFEAdtKernel");
	max_ignore_kernel(act, "AirfoilDFEUpdateKernel");
	max_run(engine, act);
	max_actions_free(act);

	act = max_actions_init(maxfile, NULL);
	max_queue_input(act, "setupCPU", dfeResReadOnly, resReadOnlyDatSize);
	max_lmem_linear(act, "setupWrite", memAddresses[resReadOnly], resReadOnlyDatSize);
	max_ignore_lmem(act, "adtDxRead");
	max_ignore_lmem(act, "resReadOnly");
	max_ignore_lmem(act, "qRead");
	max_ignore_lmem(act, "updateQ");
	max_ignore_lmem(act, "updateQold");
	max_ignore_lmem(act, "adtQ");
	max_ignore_lmem(act, "updateSaveQold");
	max_ignore_kernel(act, "AirfoilDFEResKernel");
	max_ignore_kernel(act, "AirfoilDFEAdtKernel");
	max_ignore_kernel(act, "AirfoilDFEUpdateKernel");
	max_run(engine, act);
	max_actions_free(act);
}

/**
 * Runs the main action to compute a predictor or corrector step
 */
void AirfoilDFEInterface::runMainAction(int k, double cfl, double gam, double gm1, double eps, double *rms) {

	int cpuresind = 0;
	int schedind = 0;
	for (int d = 1; d < (*domain).ndomain; d++){

		for (int res_edge_iter = 0; res_edge_iter < (*domain).nedge[d]; res_edge_iter++){

			int thispart = d;
			int thisind = res_edge_iter;

			for (int i = 0; i < 2; i++){
				int thiscellpart = (*domain).ecellpart[thispart][thisind*2+i];
				int thiscellind = (*domain).ecellind[thispart][thisind*2+i];
				if (reads[7*schedind+3] == 1){
					for (int j = 0; j < 4; j++) {
						cpu_res_qpadt[cpuresind*5+j] = (*domain).q[thiscellpart][4*thiscellind+j];
					}
					cpu_res_qpadt[cpuresind*5+4] = (*domain).adt[thiscellpart][thiscellind];
					cpuresind ++;
				}
				schedind ++ ;
			}
		}
	}

	max_actions_t * act =  max_actions_init(maxfile, NULL);

	max_set_ticks(act, "AirfoilDFEAdtKernel", (*domain).ncellcomputedfe);
	max_set_uint64t(act, "AirfoilDFEAdtKernel", "numTicks", (*domain).ncellcomputedfe);
	max_set_double(act, "AirfoilDFEAdtKernel", "cfl", cfl);
	max_set_double(act, "AirfoilDFEAdtKernel", "gam", gam);
	max_set_double(act, "AirfoilDFEAdtKernel", "gm1", gm1);
	max_lmem_linear(act, "adtQ", memAddresses[q], memAddresses[q+1] - memAddresses[q]);
	max_lmem_linear(act, "adtDxRead", memAddresses[adtDx], adtDxDatSize);

	max_set_ticks(act, "AirfoilDFEResKernel", resFlushTicks);
	max_set_double(act, "AirfoilDFEResKernel", "gm1", gm1);
	max_set_double(act, "AirfoilDFEResKernel", "eps", eps);
	max_set_uint64t(act, "AirfoilDFEResKernel", "nTicks", resFlushTicks);
	max_queue_input(act, "cpu_qpadt_to_res", cpu_res_qpadt, cpuQpadtSize);
	max_lmem_linear(act, "resReadOnly", memAddresses[resReadOnly], resReadOnlyDatSize);
	max_queue_output(act,"cpu_res_from_res", dfe_res_res, passtorescount*sizeof(double)*4);

	double * rmsOut = (double *) malloc(16*sizeof(double));
	max_set_ticks(act,"AirfoilDFEUpdateKernel", (*domain).ncellcomputedfe);
	max_set_uint64t(act, "AirfoilDFEUpdateKernel", "numCells", (*domain).ncellcomputedfe);
	max_set_uint64t(act, "AirfoilDFEUpdateKernel", "doSaveQold", k==1);
	max_lmem_linear(act, "updateQ", memAddresses[q], memAddresses[q+1] - memAddresses[q]);
	max_lmem_linear(act, "updateQold", memAddresses[qold], memAddresses[qold+1] - memAddresses[qold]);
	max_queue_output(act,"rmsOut", rmsOut, 16*sizeof(double));
	if (k == 0) {
		max_ignore_lmem(act, "updateSaveQold");
	} else {
		max_lmem_linear(act, "updateSaveQold", memAddresses[qold], memAddresses[qold+1] - memAddresses[qold]);
	}

	max_ignore_lmem(act, "setupWrite");
	max_ignore_lmem(act, "qRead");

	max_run(engine, act);
	max_actions_free(act);


	for (int i = 0; i < 16; i++) (*rms) += rmsOut[i];

	cpuresind = 0;
	schedind = 0;
	for (int d = 1; d < (*domain).ndomain; d++){

		for (int res_edge_iter = 0; res_edge_iter < (*domain).nedge[d]; res_edge_iter++){

			int thispart = d;
			int thisind = res_edge_iter;

			for (int i = 0; i < 2; i++){
				int thiscellpart = (*domain).ecellpart[thispart][thisind*2+i];
				int thiscellind = (*domain).ecellind[thispart][thisind*2+i];
				if (reads[7*schedind+3] == 1){
					for (int j = 0; j < 4; j++) {
						(*domain).res[thiscellpart][4*thiscellind+j] += dfe_res_res[cpuresind*4+j];
					}
					cpuresind ++;
				}
				schedind ++ ;
			}
		}
	}
}

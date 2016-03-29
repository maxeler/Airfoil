/*
 * AirfoilDFEInterface.h
 *
 *  Created on: 23 Feb 2016
 *      Author: dpackwood
 */


#include "airfoildfe.h"
#include "AirfoilDFEDomain.h"
#include <string>


#ifndef AIRFOILDFEINTERFACE_H_
#define AIRFOILDFEINTERFACE_H_

class AirfoilDFEInterface {
private:

	int getNBursts(int bits, int commands) {
		int lmemBits = bits*commands;
		int lmemBytes = (lmemBits-1)/8 + 1;
		int lmemBursts = (lmemBytes-1)/burstSize + 1;
		return lmemBursts;
	}


public:

	AirfoilDFEDomain * domain;

	max_file_t * maxfile;
	max_engine_t * engine;

	int * memAddresses;
	const static int burstSize = 384;
	enum lmemArray {adtDx, resReadOnly, q, qold, MEM_ADDRESS_ITEMS};

	const static size_t bufferIDBits = airfoildfe_bufferIDBits;//max_get_constant_uint64t(maxfile, "bufferIDBits");
	const static size_t bufferAddressBits = airfoildfe_bufferAddressBits; // max_get_constant_uint64t(maxfile, "bufferAddressBits");
	const static size_t flushBufferIDBits = airfoildfe_flushBufferIDBits; //max_get_constant_uint64t(maxfile, "flushBufferIDBits");
	const static size_t flushBufferAddressBits = airfoildfe_flushBufferAddressBits; //max_get_constant_uint64t(maxfile, "flushBufferAddressBits");

	typedef struct __attribute__ ((__packed__)) {
		size_t bufferId : bufferIDBits;
		size_t bufferAddress : bufferAddressBits;
		size_t bufferRead : 1;
		size_t bufferWrite : 1;
		size_t inputSelect : 1;
		size_t flushBufferId : flushBufferIDBits;
		size_t flushBufferAddress: flushBufferAddressBits;
		size_t flushBufferRead : 1;
		size_t flushBufferWrite : 1;
		size_t padding : airfoildfe_mainSchedulePaddingBits;
	} resMainStruct;

	typedef struct __attribute__ ((__packed__)) {
		size_t bufferID : flushBufferIDBits;
		size_t bufferAddress : flushBufferAddressBits;
		size_t bufferRead : 1;
		size_t bufferWrite : 1;
		size_t padding : airfoildfe_flushSchedulePaddingBits;
	} resFlushStruct;

	typedef struct __attribute__ ((packed)) {
		resMainStruct mainStructs[airfoildfe_nPipes*2];
		resFlushStruct flushStructs[airfoildfe_nFlushPipes];
		double dxs[airfoildfe_nPipes*3];
	} resReadOnlyStruct;

	const static int resReadOnlyBits = airfoildfe_totalResReadOnlyBits;

	// Maxfile descriptors
	int nReads;
	int nMainCommands;
	int nPipes;
	int nColours;
	int bufferDepth;
	int nFlushPipes;
	int nFlushColours;
	int flushBufferDepth;
	int nFlushCommands;
	int qpadtPortWidth;

	// Some mesh data parameters
	int adtDxDatSize;
	int resReadOnlyDatSize;
	int cpuQpadtSize;
	int qDatSize;
	int edgedatsize;
	int adtdatsize;
	int passtorescount;
	int resComputeTicks;
	int resFlushTicks;
	int nEdgeComputeDFE;
	int nCellComputeDFE;

	// Schedule arrays
	int schedDatSize;
	int flushscheddatsize;
	int * readlocs;
	int * reads;
	int * flushes;

	// Data arrays to stream to DFE
	resReadOnlyStruct * dfeResReadOnly;
	double * dfeAdtDX;
	double * dfeQ;
	double * dfe_update_q;
	double * cpu_res_qpadt;
	double * dfe_res_res;

	AirfoilDFEInterface(std::string planfile, AirfoilDFEDomain * domain_);
	void parsePlan(std::string planfile);
	void initLMEMArrays(double * qinf);
	void runSetupAction();
	void runSaveAction();
	void runMainAction(int k, double cfl, double gam, double gm1, double eps, double * rms);
	void runOutputAction();
};

#endif /* AIRFOILDFEINTERFACE_H_ */

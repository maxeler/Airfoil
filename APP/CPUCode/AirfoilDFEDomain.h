/*
 * AirfoilDFEDomain.h
 *
 *  Created on: 22 Feb 2016
 *      Author: dpackwood
 */

#ifndef AIRFOILDFEDOMAIN_H_
#define AIRFOILDFEDOMAIN_H_

#include<string>

class AirfoilDFEDomain {

private:

	bool initialized;

public:

	// Mesh description
	int *ncell, *nedge;
	int nbedge, ndomain;
	int nedgecomputedfe, ncellcomputedfe;
	int **ecellpart, **ecellind,  **cedgepart, **cedgeind, **cedgeid;
	int *becell, *bound;

	// Problem data
	double **dx, **q, **qold, **adt, **res, **adtold;
	double  *bdx;

	AirfoilDFEDomain() {initialized = false;}

	void read(std:: string infile);

	void writeQuiver();
};

#endif /* AIRFOILDFEDOMAIN_H_ */

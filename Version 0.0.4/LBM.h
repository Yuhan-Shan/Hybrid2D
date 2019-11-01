#ifndef LBM_H
#define LBM_H

//Standard Library
#include <memory>
#include <string>
#include <fstream>

//Hybrid Library
#include "Lattice.h"
#include "Math.h"

class LBM {
public:

	//Getters:
	int getCell(int i, int j);

	//Boundary conditions:
	void setVelBC(int i, int j, Vec2d _vel);
	void setDenBC(int i, int j, double _rho);
	void setZouBC();
	void setBounceBack();

	//LBM Engine:
	void initializeCells();
	void setInitCond(double _rhoInit, Vec2d _vel);
	void resetSolidFraction();
	void applyBodyForce();
	void collision();
	void collisionNT();
	void stream();

	bool setVelNorth = false;
	bool setVelEast  = false;
	bool setVelWest  = false;
	bool setVelSouth = false;

	bool setDenNorth   = false;
	bool setDenEast = false;
	bool setDenWest  = false;
	bool setDenSouth  = false;

	std::vector<std::shared_ptr<Lattice>> cells;

	Vec2d  gravity       = { 0.0, -9.81 };
	Vec2d  domainSize    = Vec2d::Zero();
	double dx            = 1.0;
	double dtLBM         = 1.0;
	double tau           = 1.0;
	double latticeSpeed  = 1.0;
	double kinViscosity  = 1.0;
	int    vtkCounter    = 0;
};
#endif // !LBM_H


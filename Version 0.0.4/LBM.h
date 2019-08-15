#ifndef LBM_H
#define LBM_H

#include "Lattice.h"
#include <memory>
#include <string>
#include <fstream>

class LBM {
public:
	//Constructor:
	LBM(Vec2d _dim, double _dx, double _dt, double _tau) {
		dim          = _dim;
		Ncells       = _dim[0] * _dim[1];
		dx           = _dx;
		dt           = _dt;
		tau          = _tau;
		tauInv       = 1.0 / _tau;
		latticeSpeed = _dx / _dt;

		for (int j = 0; j < _dim[1]; j++)
		for (int i = 0; i < _dim[0]; i++) {
			Vec2d cellPos = { i,j };
			int id = cells.size();
			cells.emplace_back(std::make_shared<Lattice>(id, latticeSpeed, dim, cellPos));
		}
	}

	//Getters:
	int getCell(int i, int j);

	//Setters:
	void setBoundary(bool _top, bool _bot, bool _left, bool _right);
	void setObstacle(int _obsX, int _obsY, int _radius);
	void setSquare(Vec2d _coordInit, Vec2d _coordFinal);
	void setinitCond(double _rhoInit, Vec2d _vel);
	void setvelBC(int i, int j, Vec2d _vel);
	void setdenBC(int i, int j, double _rho);
	void setzouBC();

	//Engine;
	void updateMacro();
	void collision();
	void bounceback();
	void stream();
	void outputFVTK(std::string _fileName);
	void solver(int _nIter, std::string _fileName);

	std::vector<std::shared_ptr<Lattice>> cells;

	Vec2d dim;
	double dx;
	double dt;
	double tau;
	double Ncells;
	double latticeSpeed;
	double tauInv;

	bool isFluid    = false;
	bool isSolid    = true;
	int  vtkCounter = 0;

};
#endif // !LBM_H


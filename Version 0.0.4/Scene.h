#ifndef SCENE_H
#define SCENE_H

#include "LBM.h"
#include "DEM.h"

class Scene {
public:
	//Constructor:
	Scene(Vec2d _dim, double _dx, double _dt, double _tau) : fluid(_dim, _dx, _dt, _tau), particle(_dim), dx(_dx), tau(_tau) {};
	
	//Prepara��o de cen�rio;
	void addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel);
	void setTimeStep(double _FoS, double _maxStiffness);
	void setSolidFraction();
	void calculateLBMForce();
	void solve(std::string _fileName, int _nIter);


	double dx;
	double tau;
	LBM fluid;
	DEM particle;

};

#endif // !SCENE_H


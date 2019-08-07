#ifndef SCENE_H
#define SCENE_H

#include "LBM.h"
#include "DEM.h"

class Scene {
public:
	//Constructor:
	Scene(Vec2d _dim, double _dx, double _dt, double _tau) : L(_dim, _dx, _dt, _tau), D() {};
	
	//Prepara��o de cen�rio;
	void addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel);
	void setTimeStep(double _FoS, double _maxStiffness);
	LBM L;
	DEM D;

	void print();
};

#endif // !SCENE_H


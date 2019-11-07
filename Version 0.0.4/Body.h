#ifndef BODY_H
#define BODY_H

//Standard Library
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>
#include <vector>

//Hybrid Library
#include "Math.h"

class Interaction;

class Body {
public:
	
	Body(double _mass, double _radius, Vec2d _pos, Vec2d _vel, int _id) {
		mass          = _mass;
		radius        = _radius;
		inertiaMoment = _mass * _radius * _radius * 0.5;
		pos           = _pos;
		vel           = _vel;
		id            = _id;
	}

	bool checkInteraction(int _bodyId);
	bool fluidInteraction(Vec2d _cellPos, double _dx);

	//Body variables:
	double density;
	double radius;
	double mass;
	double inertiaMoment;
	int    id;

	//Smart pointers:
	std::vector<std::weak_ptr<Interaction>> inter;
	std::vector<int> fluidSolidInteraction;

	//Vectors:
	Vec2d pos         = Vec2d::Zero();
	Vec2d vel         = Vec2d::Zero();
	Vec2d force       = Vec2d::Zero();
	Vec2d forceLBM    = Vec2d::Zero();
	Vec2d torqueLBM   = Vec2d::Zero();
	Vec2d blockedDOFs = { 1,1 };

	//Body motion variables:
	double rot          = 0.0;
	double rotVel       = 0.0;
	double moment       = 0.0;
	double blockedMDOFs = 1;

	~Body() {};
};

#endif //BODY_H
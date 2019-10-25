#include "Lattice.h"

double Lattice::setEqFun(double _rho, Vec2d _vel, int k) {
	double cdotv = discreteVelocity[k].dot(_vel);
	double vdotv = _vel.dot(_vel);
	return weight[k] * _rho * (1 + (3 * cdotv / latticeSpeed) + (4.5 * cdotv * cdotv / (latticeSpeed * latticeSpeed)) - (1.5 * vdotv / (latticeSpeed * latticeSpeed)));
}

double Lattice::setSourceTerm(double& _tau, double _dt, int k) {
	Vec2d c = discreteVelocity[k] * latticeSpeed;
	Vec2d cminusv = c - vel;
	double cdotv = discreteVelocity[k].dot(vel);
	return (1 - _dt / (2 * _tau)) * weight[k] * (3 / (latticeSpeed * latticeSpeed) * cminusv + 9 / (latticeSpeed * latticeSpeed) * cdotv * discreteVelocity[k]).dot(sourceForce);
}

void Lattice::updateMacro() {
	if (node == isSolid) {
		vel = Vec2d::Zero();
		rho = 0.0;
	}
	else {
		rho    = f[0] + f[1] + f[2] + f[3] + f[4] + f[5] + f[6] + f[7] + f[8];
		vel[0] = latticeSpeed * ((f[1] + f[5] + f[8] - f[3] - f[6] - f[7]) / rho);
		vel[1] = latticeSpeed * ((f[2] + f[5] + f[6] - f[4] - f[7] - f[8]) / rho);
	}
	ASSERT(!(std::isnan(rho) || std::isnan(vel.norm())));
}

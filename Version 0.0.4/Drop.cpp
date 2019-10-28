//#include "Scene.h"
//
//int main() {
//	Scene S;
//	
//	//Geometry
//	S.domainSize = { 10, 10 };
//	S.addCircle(1.0, 1.0, { 5, 5 }, { 0.0, 0.0 });
//	//S.addCircle(1.0, 1.0, { 3, 5 }, { 0.0, 0.0 });
//
//	//Solid Properties:
//	S.frictionAngle = 30;
//	S.localDamping = 0;
//	S.factorOfSafety = 0.1;
//	S.normalStiffness = 1.0e6;
//	S.shearStiffness = 0.5e6;
//	S.simDuration = 30000;
//	
//	//Solver
//	S.prepareScenario();
//	S.DEMSolver();
//
//	return 0;
//}
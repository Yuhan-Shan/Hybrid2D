#include "Fluid.hpp"
#include <iostream>

double Fluid::EqFun(double _rho, double _ux, double _uy, int k) {
    double cdotu = D->cx[k]*_ux + D->cy[k]*_uy;
    double udotu = _ux*_ux + _uy*_uy;
    
    return D->w[k]*_rho*(1 + 3*cdotu + 4.5*cdotu*cdotu - 1.5*udotu);
}

void Fluid::InitCond() {
    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            int id = D->MapGrid(i,j);
            rho[id] = rhoInit;
            ux[id]  = uxInit;
            uy[id]  = uyInit;
            
            for (int k = 0; k < D->Q; k++){
                int idf = D->MapFunction(i,j,k);
                f[idf] = EqFun(rho[id], ux[id], uy[id], k);
                //std::cout << rho[id] << " " << ux[id] << " " << uy[id] << " " << f[idf] << std::endl;
            }
        }
    }
}

void Fluid::Collision() { 
    double tauInv = D->dt/tau;
    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            for (int k = 0; k < D->Q; k++){
                int id  = D->MapGrid(i,j);
                int idf = D->MapFunction(i,j,k);
                if (!D->Boundary[id]){
                    double EDF = EqFun(rho[id], ux[id], uy[id], k);     //Equi. Dist. Func.
                    f[idf] = (1 - tauInv)*f[idf] + tauInv*EDF;          
                    //fPos[idf] = f[idf] - (f[idf]-EDF)/tau;
                    //std::cout << fPos[idf] << " " << f[idf] << std::endl;
                }
            }
        }
    }
}

void Fluid::Stream() {

    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            for (int k = 0; k < D->Q; k++){
                int idf = D->MapFunction(i,j,k);
                fTemp[idf] =  f[idf];
                //f[D->MapFunction(i,j,k)] = fPos[D->MapFunction(id,jd,k)];
            }
        }
    }

//Swap distribuition function
    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            for (int k = 0; k < D->Q; k++){
                int idf = D->MapFunction(i,j,k);
                f[idf] =  fTemp[idf];
                //f[D->MapFunction(i,j,k)] = fPos[D->MapFunction(id,jd,k)];
            }
        }
    }
}

void Fluid::setVelBC(int i, int j, double _ux, double _uy) {
    
    int id = D->MapGrid(i,j);
    if (!D->Boundary[D->MapGrid(i,j)]){
        ux[id] = _ux;
        uy[id] = _uy;
    }
}

void Fluid::setDensBC(int i, int j, double _rho) {
    int id = D->MapGrid(i,j);
    if (!D->Boundary[id]){
        rho[id] = _rho;
    }
}

void Fluid::setObstacle(int _obsX, int _obsY, int _radius) {

    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            if (((i-_obsX)*(i-_obsX) + (j - _obsY)*(j - _obsY)) <= _radius*_radius){
                D->Boundary[D->MapGrid(i,j)] = 1;
            }
        }
    }

}
void Fluid::BounceBack(){

    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            for (int k = 0; k < D->Q; k++){
                int id = D->MapGrid(i,j);
                int idf = D->MapFunction(i,j,k);
                if (D->Boundary[id]){
                    fTemp[idf] = f[idf];
                }   
            }
        }
    }

   for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            for (int k = 0; k < D->Q; k++){
                int id = D->MapGrid(i,j);
                int idf = D->MapFunction(i,j,k);
                if (D->Boundary[id]){
                    f[idf] = fTemp[D->MapFunction(i,j,op[k])];
                }   
            }
        }
    } 
}

void Fluid::ZouHeBC() 
{
    //Prescribed velocity:
    //Left side
    for (int j = 0; j < D->ny; j++){
        int i = 0;
        if (!D->Boundary[D->MapGrid(i,j)]){

            double rho = (f[D->MapFunction(i,j,0)] + f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)] + 
            2.0*(f[D->MapFunction(i,j,3)]+f[D->MapFunction(i,j,6)]+f[D->MapFunction(i,j,7)])) / (1.0 - ux[D->MapGrid(i,j)]);

            f[D->MapFunction(i,j,1)] = f[D->MapFunction(i,j,3)] + (2.0/3.0)*rho*ux[D->MapGrid(i,j)];

            f[D->MapFunction(i,j,5)] = f[D->MapFunction(i,j,7)] + (1.0/6.0)*rho*ux[D->MapGrid(i,j)] +
            0.5*rho*uy[D->MapGrid(i,j)] - 0.5*(f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,4)]);

            f[D->MapFunction(i,j,8)] = f[D->MapFunction(i,j,6)] + (1.0/6.0)*rho*ux[D->MapGrid(i,j)] -
            0.5*rho*uy[D->MapGrid(i,j)] + 0.5*(f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,4)]);
        }
    }

    //Right Side:
    for (int j = 0; j < D->ny; j++){
        int i = D->nx-1;
        if (!D->Boundary[D->MapGrid(i,j)]){

            double rho = (f[D->MapFunction(i,j,0)] + f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)] + 
            2.0*(f[D->MapFunction(i,j,1)]+f[D->MapFunction(i,j,5)]+f[D->MapFunction(i,j,8)])) / (1.0 + ux[D->MapGrid(i,j)]);

            f[D->MapFunction(i,j,3)] = f[D->MapFunction(i,j,1)] - (2.0/3.0)*rho*ux[D->MapGrid(i,j)];

            f[D->MapFunction(i,j,7)] = f[D->MapFunction(i,j,5)] - (1.0/6.0)*rho*ux[D->MapGrid(i,j)] + 
            0.5*(f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,4)]);

            f[D->MapFunction(i,j,6)] = f[D->MapFunction(i,j,8)] - (1.0/6.0)*rho*ux[D->MapGrid(i,j)] - 
            0.5*(f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,4)]);
        }
    }

    //Prescribed density:
    //Right side
    for (int j = 0; j < D->ny; j++){
        int i = D->nx-1;
        if (!D->Boundary[D->MapGrid(i,j)]){

            double vx = -1.0 + (f[D->MapFunction(i,j,0)] + f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)] + 
            2.0*(f[D->MapFunction(i,j,1)] + f[D->MapFunction(i,j,5)] + f[D->MapFunction(i,j,8)])) / rho[D->MapGrid(i,j)];

            f[D->MapFunction(i,j,3)] = f[D->MapFunction(i,j,1)] - (2.0/3.0)*rho[D->MapGrid(i,j)]*vx;

            f[D->MapFunction(i,j,7)] = f[D->MapFunction(i,j,5)] - (1.0/6.0)*rho[D->MapGrid(i,j)]*vx +
            0.5*(f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)]);

            f[D->MapFunction(i,j,6)] = f[D->MapFunction(i,j,8)] - (1.0/6.0)*rho[D->MapGrid(i,j)]*vx -
            0.5*(f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)]);
        }
    }
}

void Fluid::MacroUpdate() {
    
    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            if (!D->Boundary[D->MapGrid(i,j)]) {
                rho[D->MapGrid(i,j)] = f[D->MapFunction(i,j,0)]+f[D->MapFunction(i,j,1)]+f[D->MapFunction(i,j,2)]+f[D->MapFunction(i,j,3)]+f[D->MapFunction(i,j,4)]+f[D->MapFunction(i,j,5)]+f[D->MapFunction(i,j,6)]+f[D->MapFunction(i,j,7)]+f[D->MapFunction(i,j,8)];
                
                ux[D->MapGrid(i,j)] = (f[D->MapFunction(i,j,1)] + f[D->MapFunction(i,j,5)] + f[D->MapFunction(i,j,8)] - f[D->MapFunction(i,j,3)] - f[D->MapFunction(i,j,6)] - f[D->MapFunction(i,j,7)])/rho[D->MapGrid(i,j)];
                
                ux[D->MapGrid(i,j)] = (f[D->MapFunction(i,j,5)] + f[D->MapFunction(i,j,6)] + f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,7)] - f[D->MapFunction(i,j,8)] - f[D->MapFunction(i,j,4)])/rho[D->MapGrid(i,j)];
                
                //std::cout << rho[D->MapGrid(i,j)] << " " << ux[D->MapGrid(i,j)] << " " << uy[D->MapGrid(i,j)] << std::endl;
                }
            }
        }
    }

void Fluid::writeFluidVTK(std::string _Filename)
{
    int size = D->nx * D->ny;
	std::ofstream Output;
	Output.open(_Filename + std::to_string(vtkCounter) + ".vtk");

	Output << "# vtk DataFile Version 3.0\n";
	Output << "Fluid_state\n";
	Output << "ASCII\n";
	Output << "DATASET STRUCTURED_POINTS\n";
	Output << "DIMENSIONS " << D->nx << " " << D->ny << " " << 1 << "\n";
	Output << "ORIGIN "     << 0     << " " << 0     << " " << 0 << "\n";
    Output << "SPACING "    << 1     << " " << 1     << " " << 1 << "\n";
	Output << "POINT_DATA " << size << "\n";
    
    Output << "SCALARS Geom float 1\n";
    Output << "LOOKUP_TABLE default\n";
    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            int id = D->MapGrid(i,j);
            Output << D->Boundary[id] << "\n";
        }
    }
    
	Output << "SCALARS Density float 1\n";
	Output << "LOOKUP_TABLE default\n";
	for (int j = 0; j < D->ny; j++) {
		for (int i = 0; i < D->nx; i++) {
			Output << rho[D->MapGrid(i, j)] << "\n";
		}
	}
	
	Output << "VECTORS Velocity float\n" << std::endl;
	for (int j = 0; j < D->ny; j++) {
		for (int i = 0; i < D->nx; i++) {
			Output << ux[D->MapGrid(i,j)] << " " << uy[D->MapGrid(i,j)] << " " << 0 << "\n";
		}
	}
	Output.close();
	vtkCounter++;
}

void Fluid::solve(int nIter, std::string _Filename)
{
    D->setBoundary(/*Top*/false,/*Bot*/false,/*Left*/ true,/*Right*/ true);
    InitCond();
    for(int i = 0; i != nIter; i++){
        std::cout << i << std::endl;
        Collision();
        BounceBack();
        Stream();
        //ZouHeBC();
        MacroUpdate();
        if(i % 100 == 0)   writeFluidVTK(_Filename);
    }
}



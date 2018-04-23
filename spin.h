//
//  SpinSite.h
//  MC-2D Ising model
//
//  Created by Jiang Yi on 3/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifndef MC_2D_Ising_model_SpinSite_h
#define MC_2D_Ising_model_SpinSite_h
#include <math.h>
#include <complex>
#include <fstream>
using namespace std; 
class Spin{
 public:
  int nspin; 
  int nx, ny;
  double J; 
  int *spin;
  int inquire(int x, int y) {
    return spin[((x+nx)%nx)*ny + (y+ny)%ny]; 
  }
  //  Cluster cluster, neighbors, waitinglist; 
  double E;
  Spin(int nx, int ny, double J) {
    spin = new int [nx*ny]; 
    this->J = J; 
    this->nx = nx; 
    this->ny = ny; 
    this->nspin = nx*ny;//total number of spin in a bcc lattice
    Initialize();
  }
  double Mag; //------------------
  void Initialize();
  void Print();
  // return the spin of the neigbhors; 
  double GetEnergy();
  double LocalEnergy(int i, int j); 
  double GetMag(); // --------------------
  double GetMSquare();
  void PackWrite(char *); 
  //  void BuildCluster(double beta); 
  //  void ExpandCluster(int row, int col, double beta); 
  void Flip(int i, int j); 
  //  void FlipCluster(); 
  int PutInLattice(int i); 
  ~Spin() { delete [] spin; }; 
};
#endif

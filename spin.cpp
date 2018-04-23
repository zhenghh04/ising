#include "spin.h"
#include "randgen.h"
#include "math.h"
#include "ctime"
#include <iostream>
#include "stdio.h"
#include <fstream>
using namespace std;
void Spin::Initialize(){
  Rng rng; 
  for (int i=0; i<nx; i++) {
    for (int j=0; j<ny; j++) {
      spin[i*ny + j] =2*rng.Rint(2)-1; 
    }
  }
  Mag = GetMag(); //-----------------------------------------
  E = GetEnergy(); 
}

int Spin::PutInLattice(int i) {
  if ( i<=nspin && i>=0 ) 
    return i; 
  else if ( i==-1 ) 
    return nspin; 
  else if ( i==nspin+1) 
    return 0;
  else {
      std::cout << "Error" << std::endl; 
      return i; 
  }
}
void Spin::PackWrite(char *ss) {
  for (int i=0; i<nspin; i++) 
    if (spin[i]>0)
      ss[i] = '1'; 
    else
      ss[i] ='0'; 
}

void Spin::Print() {
  // print out the spin configuration
  std::cout << "Spin configuration" << std::endl; 
  std::cout << "=============================" << std::endl;
  int up = 0; 
  int down = 0; 
  for (int i=0; i<nx; i++) {
    for (int j=0; j<ny; j++) {
      if (inquire(i, j) > 0) {
	std::cout << "+ " << " ";
	up++; 
      }
      else {
	std::cout << "- " << " ";
	down++; 
      }
    }
    std::cout << "\n";
    std::cout << "\n";
  }
  std::cout << "=============================" << std::endl;
  std::cout << "up: " <<up << " down: " << down << "\n";
}

double Spin::GetEnergy() {
  E=0.;
  for (int i=0; i<nx; i++) {
    for (int j=0; j<ny; j++) {
	E=E + LocalEnergy(i, j);
    }
  }
  return E;
}

double Spin::LocalEnergy(int i, int j){
  //  return -0.5*spin[i][j]*(UpNeighbor(i,j)+DownNeighbor(i,j)
  //			  +LeftNeighbor(i,j)+RightNeighbor(i,j));
  return -0.5*(inquire(i+1, j) + inquire(i-1, j) + inquire(i, j-1)
	+ inquire(i, j+1))*J*inquire(i, j); 
}

double Spin::GetMSquare(){
  return Mag*Mag; 
}

double Spin::GetMag()
{
  double sum=0.;
  for (int i=0; i<nx; i++) {
    for (int j=0; j<ny; j++) {
      sum = sum + inquire(i, j); 
    }
  }
  return sum/nspin; 
}

void Spin::Flip(int i, int j){
  //update the spin and renew the magnitisation and total energy; 
  E += -4.0*LocalEnergy(i, j); 
  Mag += -2.0*inquire(i, j)/nspin; 
  spin[i*ny + j] = -spin[i*ny + j]; 
}

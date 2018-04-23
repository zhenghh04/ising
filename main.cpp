//----------------------------------------------//
//  * main.cpp *                                //
//  MC-2D Ising model                           //
//                                              //
// Huihuo Zheng @ Argonne National Laboratory---//
//----------------------------------------------//
#include <iostream>
#include <ctime>
#include <fstream>
#include "spin.h"
#include "math.h"
#include "randgen.h"
#include "stat.h"
#include <cstring>
#include "mpi.h"
#include <gsl/gsl_rng.h>
#include <cstdlib>
using namespace std;
// Reading parameter from external file fin, 
// This includes:
//   nx, ny -- lattice grid
//        J -- coupling constant, H = -J*s_i*s_j
//     Tmax -- range of temperature 0 - Tmax
//       Dt -- temperature spacing for scan. 
void readpara(string fin, int &nx, int &ny, double &J, int &nsweep, int &nwarmup, 
	      double &Tmax, double &Dt, bool &ifoutp); 
double pAccept(double beta, double deltaE, char* TYPE="H") 
{
  /*
    Different update, chooose M for metropolis update
   */
  if (TYPE=="H") return 1.0/(exp(beta*deltaE) + 1); 
  else if (TYPE=="M") {
    if (deltaE < 0) 
      return 1.0001; 
    else
      return exp(-beta*deltaE); 
  }
  else {
    cout << "Error" << endl; 
    return 0; 
  }
}
int main (int argc, char * argv[])
{
  MPI_Init(&argc, &argv); 
  int nproc, mype; 
  MPI_Comm_size(MPI_COMM_WORLD, &nproc); 
  MPI_Comm_rank(MPI_COMM_WORLD, &mype); 
  double beta = 40;
  int nspin = 20; 
  int nsweep, nwarmup; 
  double Tmax, Dt, J; 
  bool ifoutp=false; 
  int nx, ny; 
  if (argc>1) {
    if (mype==0)
      readpara(argv[1], nx, ny, J, nsweep, nwarmup, Tmax, Dt, ifoutp); 
    MPI_Bcast(&nx, 1, MPI_INT, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&ny, 1, MPI_INT, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&nsweep, 1, MPI_INT, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&nwarmup, 1, MPI_INT, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&J, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&Tmax, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&Dt, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&ifoutp, 1, MPI_INT, 0, MPI_COMM_WORLD); 
  }
  else {
    cout << "No input file, ..." << endl; 
    return 1; 
  }
  Spin s(nx, ny, J);
  nspin = s.nspin; 
  s.Initialize();
  int N = (nwarmup + nsweep)*nspin, ifequiv = nwarmup*nspin;
  Stat stat_m2(N, ifequiv, N); 
  double Energy;
  ofstream myfile, magfile, energyfile, fm2, fauto;
  int row, col;
  double x=0.;
  if (mype==0) {
    s.Print(); 
  }
  Energy = s.GetEnergy(); 
  myfile.open ("msquare.dat");
  magfile.open("mag.dat"); 
  energyfile.open("energy.dat");
  // fauto.open("autocorr.dat"); 
  // s.Print(); 
  Rng rng(mype);
  if (mype == 0) {
    printf("T  M M2 E autocorr #auto correlation\n"); 
  }
  double Output[4]; 
  double Output_A[4]; 
  fstream instm; 
  //  instm.open("instm.dat"); 
  for (double T=Tmax; T>Dt; T=T-Dt) {
    string fstr = "T_"+to_string(T); 
    string cmd = "[ -e " + fstr + " ] || mkdir " + fstr;  
    string str = fstr+"/instrm_" + to_string(mype)+".dat"; 
    system(cmd.c_str()); 
    instm.open(str, ios::out ); 
    Output[0] = 0.0;
    Output[1] = 0.0;
    Output[2] = 0.0; 
    Output[3] = 0.0; 
    double M = 0.0;
    double M2 = 0.0;
    double E = 0.0;
    double E2 = 0.0;
    int icount = 0; 
    beta = 1.0/T; 
    for (int i=0; i<nwarmup*nspin; i++) {
      int x = rng.Rint(s.nx);
      int y = rng.Rint(s.ny);
      double deltaE = -4.0*s.LocalEnergy(x, y); 
      if ( rng.Uniform() < pAccept(beta, deltaE, "M") ) 
      	s.Flip(x, y);
    }
    for (int is = 0; is < nsweep; is++ ) {
      for (int i=0; i<nspin; i++) {
	//****** matropolis **************************
	int x = rng.Rint(s.nx);
	int y = rng.Rint(s.ny);
	double deltaE = -4.0*s.LocalEnergy(x, y); 
	if ( rng.Uniform() < pAccept(beta, deltaE, "M") ) 
	  s.Flip(x, y);
	icount++;
	Output[0] += s.Mag; 
	Output[1] += s.GetMSquare();
	Output[2] += s.E/s.nspin; 
	Output[3] += (s.E/s.nspin)*(s.E/s.nspin);
	stat_m2.data[i]=s.Mag*s.Mag; 
      }
      char ss[s.nspin]; 
      if (ifoutp) {
	s.PackWrite(&ss[0]); 
	instm.write(&ss[0], s.nspin); 
      }
    }
    instm.close(); 
    Output[0] = Output[0]/icount;
    Output[1] = Output[1]/icount;
    Output[2] = Output[2]/icount;
    Output[3] = Output[3]/icount;
    MPI_Reduce(&Output[0], &Output_A[0], 4, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
    for(int j=0; j<4; j++) {
      Output_A[j] = Output_A[j]/nproc; 
    }
    //double autocorr = 0.0; 
    //autocorr = stat_m2.GetAutoCorrelation(); 
    //fauto << T << " " << autocorr << endl; 
    if (mype == 0) {
      printf("%10lf %10lf %10lf %10lf %10lf #auto correlation\n", T, Output_A[0], Output_A[1], Output_A[2], Output_A[3]);
      magfile << T << " "  <<Output_A[0] <<"\n"; //--------------
      energyfile << T << " " << Output_A[2] << endl; //----------
      myfile << T << " " << Output_A[1]<< endl;
    }

  }
  myfile.close();
  energyfile.close();
  magfile.close();
  fm2.close(); 
  if (mype==0)
    s.Print(); 
  MPI_Finalize(); 
  return 0;
}

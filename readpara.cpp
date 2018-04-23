#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
void readpara(string infile, int &nx, int &ny, double &J, int &nsweep, int &nwarmup, 
	      double &Tmax, double &Dt, bool &ifoutp)
{
  string line, strt;
  char *skip = "#";
  ifstream ifs(infile.c_str());
  cout << "Trying to read parameters from " << infile << endl; 
  while (!ifs.eof()) {
    getline(ifs, line);
    if (line[0] != skip[0]) {
      istringstream iss(line);
      iss >> strt;
      if (strt == "nx") {   
	iss >> strt; sscanf(&strt[0], "%d", &nx); 
      }
      else if (strt == "ny") {   
	iss >> strt; sscanf(&strt[0], "%d", &ny); 
      }
      else if (strt == "nsweep") {   
	iss >> strt; sscanf(&strt[0], "%d", &nsweep); 
      }
      else if (strt == "nwarmup") {   
	iss >> strt; sscanf(&strt[0], "%d", &nwarmup); 
      }
      else if (strt == "J") {   
	iss >> strt; sscanf(&strt[0], "%lf", &J); 
      }
      else if (strt == "Tmax") { 
	iss >> strt; sscanf(&strt[0], "%lf", &Tmax);
      }
      else if (strt == "Dt") { 
	iss >> strt; sscanf(&strt[0], "%lf", &Dt);
      }
      else if (strt == "ifoutp") {
	iss >> strt; sscanf(&strt[0], "%d", &ifoutp);
      }
    }
  }
  printf("\nParameters of the simulations\n========\n");
  printf("Total number of spins:       %d\n", nx*ny); 
  printf("                    J:       %lf\n",  J); 
  printf("               nsweep:       %d\n",  nsweep); 
  printf("              nwarmup:       %d\n",  nwarmup); 
  printf("                 Tmax:       %lf\n", Tmax);
  printf("                   Dt:       %lf\n", Dt); 
  printf("               ifoutp:       %d\n", ifoutp); 
}

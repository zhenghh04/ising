#Makefile
#!/bin/bash
##fortran makefile
#include $(HOME)/mylibs/make.inc
GSL         =  -L$(HOME)/opt/lib -lgsl -lgslcblas
CC=/usr/local/bin/mpicxx -qopenmp 
OBJ = main.o randgen.o stat.o spin.o readpara.o
OPTFLAG = -O3 
.SUFFIXES : .o .f90 .f
.cpp.o:
	$(CC) $(OPTFLAG) -c $(@D)/$(<F) -o $(@D)/$(@F)
.f.o:
	$(CC) $(OPTFLAG) -c $(@D)/$(<F) -o $(@D)/$(@F)

#####
main:$(OBJ)
	$(CC) $(OPTFLAG) -o ising.x $(OBJ) $(GSL)

clean:
	rm -f *.x *~ *.mod *# *.o

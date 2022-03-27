#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/*
This is a lib with different applications for matrix multiplication
based in different strategies below sequential and parallel application,
sequential/parallel function nomeclature:
    S/P_methodMM/VMM
*/
void S_orgMM();

void P_rowMM();
void P_colMM();
void P_blockMM();

void S_triangularVMM();
void P_triangularVMM();
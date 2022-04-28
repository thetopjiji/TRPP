/*
 * trpp_lib.h
 *
 *  Created on: 10Sept2020
 *      Author: ren
 */


#ifndef TRPP_LIB_H_
#define TRPP_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <ctime>
#include <vector>
#include <cmath>
#include <math.h>
#include <complex>
#include <assert.h>
#include <iomanip>
//#include <sys/times.h>
#include <unistd.h>

#include "struct.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define CONST_E 2.718
using namespace std;

//Variable from input
extern int alreadybest;
extern char *rep;
extern char *name_instance;		//-i
extern int seed;					//-seed
extern int pop;					//population
extern char resultsFile[100];
extern char name_final_result[256];
extern char benchmark[100];
extern char inputsol[100];
extern char pure_name[100];
extern int L1;
extern int L2;
extern int L3;
extern double alpha;
extern double cuttime;

//global variable
extern int *cordx;
extern int *cordy;
extern int *profitA;
extern int **matrix_dis;
extern int num_city;
extern int **Matrix_chosenT;

//another part

extern Struc_Sol *CurrentS, *BestS, *TempS, *LBS, *RecordS, *T_BS, *I_BS, *ReS1, *ReS2;
extern double **Profit_dis;			// The ratio of profit-distance for i to j
extern int **Candidate_set;			// Max_candidate length vector for each city to store the best ratio-profit_distance candidate city
extern int *Num_candidate_per_city;	// To store the number of candidate city for each city
extern bool *FSel;
//extern double **weight;


extern double **Profit_dis_pre;
extern int **Candidate_pre_set;
extern int *Num_candidate_pre_per_city;

extern int Total_sim;
extern int rep_result;

extern int Max_candidate;
extern int Max_smallset;
extern int dep_lk;
extern int diameter_gd;
extern int perturb1;
extern int perturb2;
//extern int perturb3;
//extern int perturb4;

extern double Param_H;
//extern double Alpha;
extern double Para_M;
//extern double Beta;

extern double t_2opt;
extern double t_swap;
extern double t_add;
extern double t_drop;
extern double t_multiinsert;
extern double t_insert;
extern double t_interchange;
extern double t_lk;
extern double t_swapadj;
extern double t_base;

extern clock_t glo_start;
extern clock_t glo_end;
#endif /* TRPP_LIB_H_ */

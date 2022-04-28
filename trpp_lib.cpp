/*
 * trpp_lib.cpp
 *
 *  Created on: 23Step2020
 *      Author: JT
 *      To declare the global variables
 */


#include "struct.h"
#include <time.h>
#include <ctime>
//variable

//Variable from input
int alreadybest=999999999;
char *rep;
char *name_instance;		//-i
int seed;					//-seed
int pop;					//population
char resultsFile[100];
char name_final_result[256];
char benchmark[100];
char inputsol[100];
char pure_name[100];
//int L1;
//int L2;
//int L3;
//double alpha;
double cuttime;

//global variable
int *cordx;
int *cordy;
int *profitA;
int **matrix_dis;
int num_city;
int **Matrix_chosenT;

//another part

Struc_Sol *CurrentS, *BestS, *TempS, *LBS, *RecordS, *T_BS, *I_BS, *ReS1, *ReS2;
double **Profit_dis;			// The ratio of profit-distance for i to j
int **Candidate_set;			// Max_candidate length vector for each city to store the best ratio-profit_distance candidate city
int *Num_candidate_per_city;	// To store the number of candidate city for each city
bool *FSel;
//double **weight;


double **Profit_dis_pre;
int **Candidate_pre_set;
int *Num_candidate_pre_per_city;


int Total_sim=0;
int rep_result=0;

//int Max_candidate=50;
//int Max_smallset=10;
//int dep_lk=10;
//int diameter_gd=5;
//int perturb1=5;
//int perturb2=5;
//int perturb3=5;
//
//
//double Alpha=1.0;
//double Param_H=1.0;
//double Para_M=2.0;
//double Beta=10.0;

int Max_candidate;
int Max_smallset;
int dep_lk;
int diameter_gd;
int perturb1;
int perturb2;
//int perturb3;
//int perturb4;

double Param_H;
//double Alpha;
double Para_M;
//double Beta;



double t_2opt=0.0;
double t_swap=0.0;
double t_add=0.0;
double t_drop=0.0;
double t_multiinsert=0.0;
double t_insert=0.0;
double t_interchange=0.0;
double t_lk=0.0;
double t_swapadj=0.0;

double t_base=0.0;

clock_t glo_start;
clock_t glo_end;

/****************/


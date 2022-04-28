/*
 * Detail_TRPP.h
 *
 *  Created on: 11Step2020
 *      Author: DELL
 */

#ifndef DETAIL_TRPP_H_
#define DETAIL_TRPP_H_

#include "struct.h"

//export function
int *get_vector(int size);
int **get_matrix(int num_row,int num_col);
void free_vec(int *arr);
void free_mat(int **mat, int l);
double **setdoubleM(int l, int c);
void freedoubleM(double **poi, int l);
Struc_Sol *allcoStruct(int size);
void freeStruct(Struc_Sol *temp, int size);
void copy_vec(int *fromv, int *tov, int length);

//void getfitness(Struc_Sol *S);
void getfitness2(Struc_Sol *S);
int getfitness_vec(int *arr_v);
void getcdis(Struc_Sol *S);
void getflag(Struc_Sol *S);

void ll2perm(Struc_Sol *S);
void perm2ll(Struc_Sol *S);
void perm2position(Struc_Sol *S);
void perm2SbSe(Struc_Sol *S);
void perm2sd(Struc_Sol *S);

void CopySolution(Struc_Sol *fromS, Struc_Sol *toS);
void CopyVec(int *fromv, int *tov, int length);
void UpdateBestS(Struc_Sol *S, int method);

void RanIniSol(Struc_Sol *S);
void GreedyIniSol(Struc_Sol *S);
void Ini_Profit_dis(void);
void Ini_Candidate_set(void);
void Ini_fromRecordS(Struc_Sol *S);
void Ini_Profit_dis_pre(void);
void Ini_Candidate_pre_set(void);
void Ini_fromRecordS_greedy(Struc_Sol *S);

void ExcGene(int *lvec, int l);
void layoutSol(Struc_Sol *S);
void check_positive(Struc_Sol *S);

int get_e2e_cdis_general(int *arr_v, int length, int startpos, int Nsel);

void ini_Matrix_chosenT(void);
void ini_weight(void);
void update_chosenT(int node1, int node2);
void update_weight(int node1,int node2, double rate);

double get_time(void);
bool check_time(void);
void verify_ll(Struc_Sol *S);
void verify_SbSe(Struc_Sol *S);
void verify_Vsd(Struc_Sol *S);
void verify_cdis(Struc_Sol *S);


int get_cdis_vec(int *arr);
void cc_positive_vec(int *arr);
void verify_duplicate_perm(int *arr);
void verify_duplicate_ll(int *arr);
void sort_detail_sol(Struc_Sol *S);

int FwI(int i, int j, int k, int *Vsd, int N, int *perm);
int BwI(int i, int j, int k, int *Vsd, int N, int *perm);
int fast2opt(int i, int j, int *Vsd, int N, int *perm, int *Sb, int *Se, int cdis);
void updateVsd(int *Vsd, int startpos, int endpos, int *perm);
void updateSbSe(int i, int j, int N, int *perm, int *Vsd, int *Sb, int *Se);
void updateCandidateSet(void);
int locate_nextcity(Struc_Sol *S, int curcity);
int locate_prevcity(Struc_Sol *S, int curcity);

#endif /* DETAIL_TRPP_H_ */

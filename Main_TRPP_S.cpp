 /*
 * Main_TRPP_S.cpp
 *
 *  Created on: 2020.9.11
 *      Author: DELL
 */

#include "struct.h"
#include "trpp_lib.h"
#include "Detail_TRPP.h"
#include "Ope_swap.h"
#include "Ope_2opt.h"
#include "Ope_insert.h"
#include "Ope_drop.h"
#include "Ope_add.h"
#include "Ope_interchange.h"
#include "Ope_multiinsert.h"
#include "Ope_LK.h"
#include "Shake.h"
#include "Ope_swapadj.h"

//export function
int algo_search(void);

//local function
void setstruc(int size);
void freestruc(int size);
void newTH(Struc_Sol *S);
void distance_guided2(Struc_Sol *S);
//function

int seq_exe[8];

void setstruc(int size){
	CurrentS=allcoStruct(1);
	BestS=allcoStruct(1);
	TempS=allcoStruct(1);
	LBS=allcoStruct(1);
	RecordS=allcoStruct(1);
	T_BS=allcoStruct(1);
	I_BS=allcoStruct(1);
	ReS1=allcoStruct(1);
	ReS2=allcoStruct(1);


	Candidate_set=get_matrix(num_city,Max_candidate);
	Num_candidate_per_city=get_vector(num_city);

	Candidate_pre_set=get_matrix(num_city,Max_smallset);
	Num_candidate_pre_per_city=get_vector(num_city);


	/*allocate space for Profit_dis*/
//	Profit_dis=setdoubleM(num_city,num_city);

	/*allocate space for weight*/
//	weight=(double**)malloc(num_city*sizeof(double*));
//	if(!weight){
//		cout<<"Memory error in Profit_dis"<<endl;
//		exit(-1);
//	}
//	for(int i=0;i<num_city;i++){
//		weight[i]=(double*)malloc(num_city*sizeof(double));
//		if(!weight[i]){
//			cout<<"Memory error in Profit_dis[i]"<<endl;
//			exit(-1);
//		}
//		memset(weight[i],1.0,num_city*sizeof(double));
//	}
	/*allocate space for FSel*/
	FSel=(bool*)malloc(num_city*sizeof(bool));
	if(!FSel){
		cout<<"Memory error in FSel"<<endl;
		exit(-1);
	}
	/*****************************/
}

void freestruc(int size){
	freeStruct(CurrentS,1);
	freeStruct(BestS,1);
	freeStruct(TempS,1);
	freeStruct(LBS,1);
	freeStruct(RecordS,1);
	freeStruct(T_BS,1);
	freeStruct(I_BS,1);
	freeStruct(ReS1,1);
	freeStruct(ReS2,1);

	free_mat(Candidate_set,num_city);
	free_vec(Num_candidate_per_city);

	free_mat(Candidate_pre_set,num_city);
	free_vec(Num_candidate_pre_per_city);

	/*free space for Profit_dis*/
	freedoubleM(Profit_dis,num_city);

	/*free space for weight*/
//	freedoubleM(weight,num_city);

	/*free space for FSel*/
	free(FSel);
	FSel=NULL;
	/******************************/
}

void newTH(Struc_Sol *S){
	int ite=0;
	double st_time;
	ExcGene(seq_exe,8);


	st_time=get_time();
	Ope_add(S, ite);
	t_add+=get_time()-st_time;

	for(int i=0;i<8;i++){
		switch (seq_exe[i]){
		case 0:

			st_time=get_time();
			Ope_swap(S, ite);
			t_swap+=get_time()-st_time;

			st_time=get_time();
			Ope_drop(S, ite);
			t_drop+=get_time()-st_time;


			if(check_time()) return;
			break;
		case 1:

			break;
		case 2:

			st_time=get_time();
			Ope_2opt(S, ite);
			t_2opt+=get_time()-st_time;


			st_time=get_time();
			Ope_drop(S, ite);
			t_drop+=get_time()-st_time;

			if(check_time()) return;
			break;
		case 3:

			st_time=get_time();
			Ope_insert(S, ite);
			t_insert+=get_time()-st_time;

			st_time=get_time();
			Ope_drop(S, ite);
			t_drop+=get_time()-st_time;

			if(check_time()) return;
			break;
		case 4:
//			st_time=get_time();
//			Ope_drop(S, ite);
//			t_drop+=get_time()-st_time;
			break;
		case 5:

//			st_time=get_time();
//			Ope_interchange(S, ite);
//			t_interchange+=get_time()-st_time;

//			st_time=get_time();
//			Ope_drop(S, ite);
//			t_drop+=get_time()-st_time;

			if(check_time()) return;
			break;
		case 6:

			st_time=get_time();
			Ope_multiinsert(S, ite);
			t_multiinsert+=get_time()-st_time;

			st_time=get_time();
			Ope_drop(S, ite);
			t_drop+=get_time()-st_time;

			if(check_time()) return;
			break;
		case 7:
//			st_time=get_time();
//			Ope_swapadj(S, ite);
//			t_swapadj+=get_time()-st_time;


//			st_time=get_time();
//			Ope_drop(S, ite);
//			t_drop+=get_time()-st_time;

			if(check_time()) return;
			break;
		default:
			break;
		}
	}

	st_time=get_time();
	Ope_LK(S,ite);
	t_lk+=get_time()-st_time;

	st_time=get_time();
	Ope_drop(S, ite);
	t_drop+=get_time()-st_time;
}


void distance_guided2(Struc_Sol *S){
	int count=0;
	int cc_turn=0;
//	int max_return=10;

	/***************/
//	int ite=0;
//
//	Ope_add(RecordS,ite);
//
//	exit(0);

	/**************/
	CopySolution(S,T_BS);
	CopySolution(S,I_BS);

	while(1){

		do{
			CopySolution(S,LBS);
//			VNS(S);
			newTH(S);
//			NewVNS(S);
			if(check_time()) return;
		}while(S->profit>LBS->profit);

		if(LBS->profit>I_BS->profit) CopySolution(LBS,I_BS);

		if(I_BS->profit>T_BS->profit) {
			CopySolution(I_BS,T_BS);
			cc_turn=0;
		}
		else cc_turn++;

		if(cc_turn>diameter_gd){
			CopySolution(T_BS,S);
			cc_turn=0;
		}
//		cout<<"turn="<<count<<"\t N="<<S->Nsel<<"\t t="<<get_time()<<endl;
		shake(S);
//		nosoul_shake(S);
		// updateCandidateSet();
		count++;

		if(check_time()) return;
	}
}

int algo_search(void){

	int ite __attribute__((unused));
//	int last_profit;

	setstruc(1);
	BestS->profit=-1;
	T_BS->profit=-1;

	Profit_dis=(double**)setdoubleM(num_city,num_city);
    Ini_Profit_dis();
    Ini_Candidate_set();

    Ini_Candidate_pre_set();
//  freedoubleM(Profit_dis,num_city);

	/*initial the file*/
    ofstream caout(name_final_result,ios::out|ios::trunc);
	if (caout.is_open()){
		caout<<"0 ";
		caout<<BestS->profit<<" ";
		caout<<get_time()<<endl;
		caout.close();
	}

//	/*****************************/
	/*The main while of VNS*/
	for(int i=0;i<8;i++) seq_exe[i]=i;

	while(!check_time()){

	    GreedyIniSol(CurrentS);
//
//	    cout<<"CurrentS "<<CurrentS->profit<<endl;
//	    exit(0);
		/*********************************/
		ini_Matrix_chosenT();
		ini_weight();
		Total_sim=1;
		distance_guided2(CurrentS);
		if(BestS->profit>=alreadybest) break;
	}

	cout<<"Best "<<BestS->profit<<endl;

	freestruc(1);
//	return last_profit;
	return 0;
}

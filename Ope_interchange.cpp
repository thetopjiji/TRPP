/*
 * Ope_interchange.cpp
 *
 *  Created on: 28Step2020
 *      Author: JT
 *      exchange a city in the set with a city out of the set
 *		Nsel is not changed
 */

#include "trpp_lib.h"
#include "Detail_TRPP.h"

//export function
int Ope_interchange(Struc_Sol *S, int &ite);		//one vs one exchange

//local function
int interchange_old(Struc_Sol *S, int &ite);
int interchange_all(Struc_Sol *S, int &ite);
int interchange_correct_old(Struc_Sol *S, int &ite);

int get_delta_interchange(Struc_Sol *S, int nodeins, int nodeout);
int ope_delta_interchange(Struc_Sol *S, int nodeins, int nodeout);

int fast_get_delta(Struc_Sol *S, int nodeins, int nodeout);
int fast_evaluation_interx(Struc_Sol *S, int i, int N, int nodeout, int *perm);

//add city into the candidate set

int Ope_interchange(Struc_Sol *S, int &ite){



	interchange_correct_old(S,ite);

//	interchange_old(S,ite);
//	interchange_all(S,ite);
	return 5;
}

int interchange_old(Struc_Sol *S, int &ite){

	bool Fimp=false;
	int nodeins;
	int nodeout;
	int nodepin;
	int i,j;


	do{
		Fimp=false;
		for(i=1;i<num_city;i++){
			nodeout=i;
			if(S->f_city[nodeout]==1) continue;			//To find the node out of the set
			for(j=0;j<Num_candidate_per_city[nodeout];j++){
				nodepin=Candidate_set[nodeout][j];
				if(S->f_city[nodepin]==0) continue;		//not in the set
//				nodeins=S->nextC[nodepin];				//Last node, no node to exchange
				nodeins=locate_nextcity(S,nodepin);
				if(nodeins==-1) continue;
				if(check_time()) return 1;
				if(fast_get_delta(S,nodeins,nodeout)>0){	//get impoved
//				if(get_delta_interchange(S,nodeins,nodeout)>0){	//get impoved
					ope_delta_interchange(S, nodeins, nodeout);
					UpdateBestS(S,6);
					break;
				}
			}
		}


	}while(Fimp);

	return 0;
}


int interchange_all(Struc_Sol *S, int &ite){

	bool Fimp=false;
	int nodeins;
	int nodeout;
	int i,j;

	do{
		Fimp=false;
		for(i=1;i<num_city;i++){
			nodeout=i;
			if(S->f_city[nodeout]==1) continue;			//To find the node out of the set
			for(j=1;j<S->Nsel;j++){
				nodeins=S->perm[j];
				if(check_time()) return 1;
//				if(fast_get_delta(S,nodeins,nodeout)>0){
				if(get_delta_interchange(S,nodeins,nodeout)>0){	//get impoved
					ope_delta_interchange(S, nodeins, nodeout);
					UpdateBestS(S,6);
					break;
				}
			}
		}
	}while(Fimp);

	return 0;
}

int interchange_correct_old(Struc_Sol *S, int &ite){
	bool Fimp=false;
	int nodeins;
	int nodeout;			//node which is not chosen in the set
	int nodepin;			//node which is chosen in the set
	int i,j;


	do{
		Fimp=false;
		for(i=1;i<num_city;i++){
			nodepin=i;
			if(S->f_city[nodepin]==0) continue;			//To find in the set
//			nodeins=S->nextC[nodepin];
			nodeins=locate_nextcity(S,nodepin);

			if(nodeins==-1) continue;					//nodepin is the bottom of the solution

			for(j=0;j<Num_candidate_per_city[nodepin];j++){
				nodeout=Candidate_set[nodepin][j];
				if(S->f_city[nodeout]==1) continue;		//To find out of the set

				if(check_time()) return 1;
				if(get_delta_interchange(S,nodeins,nodeout)>0){	//get impoved
					ope_delta_interchange(S, nodeins, nodeout);
					UpdateBestS(S,6);
					break;
				}
			}
		}


	}while(Fimp);

	return 0;
}


int get_delta_interchange(Struc_Sol *S, int nodeins, int nodeout){

	int arr_v[num_city+1];
	int i;
	int postarget=S->position[nodeins];

	memset(arr_v,-1,(num_city+1)*sizeof(int));

	for(i=0;i<S->Nsel;i++) arr_v[i]=S->perm[i];
	arr_v[postarget]=nodeout;

	/*new way**/


	/****************/
	return getfitness_vec(arr_v)-S->profit;
}

int ope_delta_interchange(Struc_Sol *S, int nodeins, int nodeout){

	int targetpos=S->position[nodeins];
	int preC=locate_prevcity(S,nodeins);
	int nexC=locate_nextcity(S,nodeins);

	if(nodeins==0) {
		cout<<"error to exchange depot city"<<endl;
		exit(-2);
	}

	//update the f_city
	S->f_city[nodeins]=0;
	S->f_city[nodeout]=1;

	//update the perm
	S->perm[targetpos]=nodeout;

	//update the ll: nextC, prevC
//	int preC=S->prevC[nodeins];

//	int nexC=S->nextC[nodeins];

//
//	S->nextC[preC]=nodeout;
//	S->prevC[nodeout]=preC;
//	S->nextC[nodeout]=nexC;
//	if(nexC!=-1) S->prevC[nexC]=nodeout;
//
//	S->prevC[nodeins]=-1;
//	S->nextC[nodeins]=-1;

	//update the position
	S->position[nodeins]=-1;
	S->position[nodeout]=targetpos;

	//update the profit
	getfitness2(S);

	//update the cdis
	S->cdis=S->cdis-(S->Nsel-targetpos)*matrix_dis[preC][nodeins]+(S->Nsel-targetpos)*matrix_dis[preC][nodeout];
	if(nexC!=-1) S->cdis=S->cdis-(S->Nsel-targetpos-1)*matrix_dis[nodeins][nexC]+(S->Nsel-targetpos-1)*matrix_dis[nodeout][nexC];

	//Nsel is not changed

	return 0;
}


int fast_get_delta(Struc_Sol *S, int nodeins, int nodeout){

	int postarget=S->position[nodeins];

	return fast_evaluation_interx(S,postarget,S->Nsel,nodeout,S->perm);

}

int fast_evaluation_interx(Struc_Sol *S, int i, int N, int nodeout, int *perm){
	int deltap=0;

	deltap=profitA[nodeout]-profitA[perm[i]];
	deltap-=(N-i)*(matrix_dis[perm[i-1]][nodeout]-matrix_dis[perm[i-1]][perm[i]]);
	deltap-=(N-i-1)*(matrix_dis[nodeout][perm[i+1]]-matrix_dis[perm[i]][perm[i+1]]);


	return deltap;

}

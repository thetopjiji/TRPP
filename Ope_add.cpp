/*
 * Ope_drop.cpp
 *
 *  Created on: 24Step2020
 *      Author: JT
 *      add some city to the city set
 */

#include "trpp_lib.h"
#include "Detail_TRPP.h"

//export function
int Ope_add(Struc_Sol *S, int &ite);		//One node add

//local function
int _add_before_canc(Struc_Sol *S, int &ite);
int _add_before_allc(Struc_Sol *S, int &ite);


int get_real_delta(Struc_Sol *S, int nodeadd, int posadd);
int get_delta_add(Struc_Sol *S, int nodeadd, int posadd);
int ope_delta_add(Struc_Sol *S, int nodeadd, int posadd);

int fast_profit_delta(int i, int nodeadd, int N, int *Vsd, int *perm);
int fast_update_cdis(int i, int nodeadd, int N, int *Vsd, int *perm);

//add city into the candidate set




int Ope_add(Struc_Sol *S, int &ite){

	perm2sd(S);
	verify_cdis(S);
	verify_Vsd(S);

//	int i,j;
//	for(i=0;i<num_city;i++){
//		if(S->f_city[i]) continue;
//		for(j=1;j<S->Nsel;j++){
//			get_delta_add(S,i,j);
//
//		}
//	}
//
//
//	return 4;


	/**********************************************/
//	_add_before_canc(S,ite);
	_add_before_allc(S,ite);
	return 4;
}

int _add_before_canc(Struc_Sol *S, int &ite){

	int i,j;
	bool Fimp=false;
	int nodeadd;
	int nodepis;
	int posadd;

	do{
		Fimp=false;
		for(i=1;i<num_city;i++){
			nodeadd=i;
			if(S->f_city[nodeadd]) continue;
			for(j=0;j<Num_candidate_per_city[nodeadd];j++){
				nodepis=Candidate_set[nodeadd][j];
				if(S->f_city[nodepis]==0) continue;
				posadd=S->position[nodepis];
				if(check_time()) return 1;
//				if(get_real_delta(S,nodeadd,posadd)>0){
				if(get_delta_add(S,nodeadd,posadd)>0){			//if profit is augmented
					ope_delta_add(S,nodeadd,posadd);

					/******************************/
//					verify_Vsd(S);
//					verify_cdis(S);
					/******************************/
					UpdateBestS(S,2);
					break;
				}
			}
		}

	}while(Fimp);

	return 0;
}
int _add_before_allc(Struc_Sol *S, int &ite){

	int i,j;
	bool Fimp=false;
	int nodeadd;


	do{
		Fimp=false;
		for(i=1;i<num_city;i++){
			nodeadd=i;
			if(S->f_city[nodeadd]) continue;
			for(j=1;j<S->Nsel;j++){
				//when it is full charged, it will not be taken into consideration
				if(check_time()) return 1;
//				if(get_real_delta(S,nodeadd,posadd)>0){
				if(get_delta_add(S,nodeadd,j)>0){			//if profit is augmented
					ope_delta_add(S,nodeadd,j);
					UpdateBestS(S,2);
					break;
				}
			}
		}

	}while(Fimp);

	return 0;
}

int get_real_delta(Struc_Sol *S, int nodeadd, int posadd){
	//nodeadd is put in front of the nodepis
//
	int i;
	int arr_r[num_city+1];

	memset(arr_r,-1,(num_city+1)*sizeof(int));

	for(i=0;i<posadd;i++) arr_r[i]=S->perm[i];
	arr_r[posadd]=nodeadd;
	for(i=posadd;i<S->Nsel;i++) arr_r[i+1]=S->perm[i];


	return getfitness_vec(arr_r)-S->profit;
}

int get_delta_add(Struc_Sol *S, int nodeadd, int posadd){

	//nodeadd is put in front of the nodepis
//
//	int i;
//	int arr_r[num_city+1];
//
//	memset(arr_r,-1,(num_city+1)*sizeof(int));
//
//	for(i=0;i<posadd;i++) arr_r[i]=S->perm[i];
//	arr_r[posadd]=nodeadd;
//	for(i=posadd;i<S->Nsel;i++) arr_r[i+1]=S->perm[i];
//

	/***********************************************************************/
	int newp=0;

	if(posadd==0) {
		cout<<"error in posadd =0"<<endl;
		exit(-2);
	}

	newp=fast_profit_delta(posadd, nodeadd, S->Nsel, S->vec_sd, S->perm);
	return newp;
	/***********************************************************************/
//	if(newp<0 && getfitness_vec(arr_r)-S->profit>0){
//		cout<<"old="<<getfitness_vec(arr_r)-S->profit<<"\t new="<<newp<<endl;
//	}
//	return getfitness_vec(arr_r)-S->profit;
}

int ope_delta_add(Struc_Sol *S, int nodeadd, int posadd){

	//update the f_city
	S->f_city[nodeadd]=1;

	//update the ll: nextC, prevC
	int oripreC=S->perm[posadd-1];
	int orinexC=S->perm[posadd];
//	S->nextC[oripreC]=nodeadd;
//	S->prevC[nodeadd]=oripreC;
//	S->nextC[nodeadd]=orinexC;
//	if(orinexC!=-1) S->prevC[orinexC]=nodeadd;

	//update cdis
	S->cdis+= fast_update_cdis(posadd, nodeadd, S->Nsel, S->vec_sd, S->perm);


	//update the perm
	for(int i=S->Nsel-1;i>=posadd;i--)	S->perm[i+1]=S->perm[i];

	S->perm[posadd]=nodeadd;

	//update the Nsel
	S->Nsel++;

	//update the position
	for(int i=posadd;i<S->Nsel;i++) S->position[S->perm[i]]=i;

//	//update the cdis, a little complex
//	if(orinexC!=-1) S->cdis=S->cdis-(S->Nsel-1-posadd)*matrix_dis[oripreC][orinexC];
//	if(orinexC!=-1) S->cdis=S->cdis+(S->Nsel-1-posadd)*matrix_dis[nodeadd][orinexC];
//	S->cdis=S->cdis+(S->Nsel-posadd)*matrix_dis[oripreC][nodeadd];
//	for(int i=0;i<posadd-1;i++) S->cdis=S->cdis+matrix_dis[S->perm[i]][S->perm[i+1]];

 	//profit
	getfitness2(S);

	//update the vsd

	updateVsd(S->vec_sd, posadd, 1, S->perm);

	/*update the chosen Times*/
	update_chosenT(S->perm[posadd-1],nodeadd);
	update_chosenT(S->perm[posadd+1],nodeadd);

	return 0;
}

int fast_profit_delta(int i, int nodeadd, int N, int *Vsd, int *perm){
	int deltap=0;

	deltap+=profitA[nodeadd];
	deltap-=Vsd[i-1]-Vsd[0];
	deltap-=(N+1-i)*matrix_dis[perm[i-1]][nodeadd];
	if(N!=i) deltap-=(N-i)*matrix_dis[nodeadd][perm[i]];
	if(N!=i) deltap-=-(N-i)*matrix_dis[perm[i-1]][perm[i]];


	return deltap;
}

int fast_update_cdis(int i, int nodeadd, int N, int *Vsd, int *perm){
	int deltacdis=0;
	if(perm[i]==-1 && N!=i) cout<<"exceed to perm in fast_update_cdis"<<endl;
	deltacdis+=Vsd[i-1]-Vsd[0];
	deltacdis+=(N+1-i)*matrix_dis[perm[i-1]][nodeadd];
	if(N!=i) deltacdis+=(N-i)*matrix_dis[nodeadd][perm[i]];
	if(N!=i) deltacdis+=-(N-i)*matrix_dis[perm[i-1]][perm[i]];

	return deltacdis;
}

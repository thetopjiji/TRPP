/*
 * Ope_drop.cpp
 *
 *  Created on: 24Step2020
 *      Author: JT
 *      remove some city from the city set
 */

#include "trpp_lib.h"
#include "Detail_TRPP.h"

//export function
int Ope_drop(Struc_Sol *S, int &ite);		//One node drop
int drop_gab_bot(Struc_Sol *S);
int drop_gab_all(Struc_Sol *S);

//local function

int get_delta_drop(Struc_Sol *S, int ind);
int ope_delta_drop(Struc_Sol *S, int ind);

//insert domain

int Ope_drop(Struc_Sol *S, int &ite){


	int i=2;
//
//	printf("estimated=%d\n",get_delta_drop(S,i));
//	ope_delta_drop(S,i);
//	getfitness2(S);
//	printf("real_getp=%d\n",S->profit);
//	exit(0);

//
//	CopySolution(S,BestS);
//
//	ope_delta_drop(S,i);
//
//	CopySolution(S,TempS);
//	perm2ll(S);
//	perm2position(S);
//	getcdis(S);
//
//	for(int i=0;i<num_city+1;i++) cout<<BestS->perm[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city+1;i++) cout<<TempS->perm[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city+1;i++) cout<<S->perm[i]<<" ";
//	cout<<endl;
//	cout<<endl;
//
//	for(int i=0;i<num_city;i++) cout<<BestS->nextC[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city;i++) cout<<TempS->nextC[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city;i++) cout<<S->nextC[i]<<" ";
//	cout<<endl;
//	cout<<endl;
//
//	for(int i=0;i<num_city;i++) cout<<BestS->prevC[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city;i++) cout<<TempS->prevC[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city;i++) cout<<S->prevC[i]<<" ";
//	cout<<endl;
//	cout<<endl;
//
//	for(int i=0;i<num_city;i++) cout<<BestS->position[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city;i++) cout<<TempS->position[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city;i++) cout<<S->position[i]<<" ";
//	cout<<endl;
//	cout<<endl;
//
//	for(int i=0;i<num_city;i++) cout<<BestS->f_city[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city;i++) cout<<TempS->f_city[i]<<" ";
//	cout<<endl;
//	for(int i=0;i<num_city;i++) cout<<S->f_city[i]<<" ";
//	cout<<endl;
//	cout<<endl;
//
//
//	for(int k=0;k<num_city;k++)
//		if(S->nextC[k]!=TempS->nextC[k]) cout<<"error in delta nextC"<<endl;
//	for(int k=0;k<num_city;k++)
//		if(S->prevC[k]!=TempS->prevC[k]) cout<<"error in delta prevC"<<endl;
//	for(int k=0;k<num_city;k++)
//		if(S->position[k]!=TempS->position[k]) cout<<"error in delta position"<<endl;
//	cout<<TempS->cdis<<" "<<S->cdis<<" "<<TempS->cdis-S->cdis<<endl;
//	exit(0);

//	CopySolution(S,BestS);
//
//	for(i=1;i<num_city;i++){
//		CopySolution(BestS,S);
//		if(S->f_city[i]==0) continue;
//		ope_delta_drop(S,i);
//		CopySolution(S,TempS);
//		perm2ll(S);
//		perm2position(S);
//		getcdis(S);
//		for(int k=0;k<num_city;k++)
//			if(S->nextC[k]!=TempS->nextC[k]) cout<<"error in delta nextC"<<endl;
//		for(int k=0;k<num_city;k++)
//			if(S->prevC[k]!=TempS->prevC[k]) cout<<"error in delta prevC"<<endl;
//		for(int k=0;k<num_city;k++)
//			if(S->position[k]!=TempS->position[k]) cout<<"error in delta position"<<endl;
//		cout<<TempS->cdis<<" "<<S->cdis<<" "<<TempS->cdis-S->cdis<<endl;
//	}
//	exit(0);


//	int i=1;					//index of the perm
	bool Fimp=false;

	int bestind=-1;
	int bestprofit=S->profit;
	int tempprofit;

	do{
		bestind=-1;
		bestprofit=S->profit;
		Fimp=false;

		for(i=1;i<S->Nsel;i++){			//best improve strategy
			tempprofit=get_delta_drop(S,i);
			if(check_time()) return 1;
			if(tempprofit>bestprofit){
				bestprofit=tempprofit;
				bestind=i;
			}
		}
		if(bestind!=-1) {
			ope_delta_drop(S,bestind);
			S->profit=bestprofit;
			UpdateBestS(S,3);
		}
	}while(Fimp);

	drop_gab_bot(S);
	drop_gab_bot(BestS);

	return 3;
}

int get_delta_drop(Struc_Sol *S, int ind){

	int arr_v[num_city+1];

	memset(arr_v,-1,(num_city+1)*sizeof(int));


	for(int i=0;i<ind;i++) arr_v[i]=S->perm[i];
	for(int i=ind;i<S->Nsel;i++) arr_v[i]=S->perm[i+1];

//	for(int i=0;i<num_city+1;i++) {
//		if(count==ind) continue;
//		arr_v[count]=S->perm[i];
//		count++;
//		cout<<arr_v[count]<<" ";
//	}
//	cout<<endl;
	return getfitness_vec(arr_v);
}

int ope_delta_drop(Struc_Sol *S, int ind){

	int nodedrop=S->perm[ind];
	if(ind==0){
		cout<<"error in drop depot"<<endl;
		exit(-3);
	}
	/*udpate the f_city*/
	S->f_city[nodedrop]=0;
	/*udpate the ll: nextC, prevC*/
//	int preN=S->prevC[nodedrop];
	int preN=locate_prevcity(S,nodedrop);
//	int nexN=S->nextC[nodedrop];
	int nexN=locate_nextcity(S,nodedrop);
//	S->nextC[preN]=nexN;
//	if(nexN!=-1) S->prevC[nexN]=preN;
//	S->prevC[nodedrop]=-1;
//	S->nextC[nodedrop]=-1;
	/*update the cdis*/

	S->cdis=S->cdis-matrix_dis[preN][nodedrop]*(S->Nsel-ind);
	if(nexN!=-1) {
		S->cdis=S->cdis-matrix_dis[nodedrop][nexN]*(S->Nsel-1-ind);
		S->cdis=S->cdis+matrix_dis[preN][nexN]*(S->Nsel-1-ind);
	}
	for(int i=0;i<ind-1;i++) S->cdis=S->cdis-matrix_dis[S->perm[i]][S->perm[i+1]];

	/*udpate the perm*/
	for(int i=ind;i<S->Nsel;i++) S->perm[i]=S->perm[i+1];

	/*update the position*/
	for(int i=ind;i<S->Nsel;i++) {
		if(S->perm[i]==-1) break;
		S->position[S->perm[i]]=i;
	}
	S->position[nodedrop]=-1;
	/*update the Nsel*/
	S->Nsel=S->Nsel-1;

	return 0;
}

int drop_gab_bot(Struc_Sol *S){
	/*drop useless node from the bottom*/
	int Nsel=S->Nsel;
	int i;
	int delta_profit[S->Nsel];
	int accdis=0;
	int nowc,nexc;

	delta_profit[0]=0;
	for(i=0;i<Nsel;i++){
		nowc=S->perm[i];
		nexc=S->perm[i+1];
		if(nexc==-1) break;
		accdis=accdis+matrix_dis[nowc][nexc];
		delta_profit[i+1]=profitA[nexc]-accdis;
	}

//	for(i=0;i<Nsel;i++) printf("%d ",delta_profit[i]);
//	cout<<endl;

	for(i=Nsel-1;i>0;i--)
		if(delta_profit[i]<0)
			ope_delta_drop(S,i);
		else break;

	return 0;
}

int drop_gab_all(Struc_Sol *S){
	/*drop useless node from the bottom*/
	int Nsel=S->Nsel;
	int i;
	int delta_profit[S->Nsel];
	int accdis=0;
	int nowc,nexc;

	delta_profit[0]=0;
	for(i=0;i<Nsel;i++){
		nowc=S->perm[i];
		nexc=S->perm[i+1];
		if(nexc==-1) break;
		accdis=accdis+matrix_dis[nowc][nexc];
		delta_profit[i+1]=profitA[nexc]-accdis;
	}

//	for(i=0;i<Nsel;i++) printf("%d ",delta_profit[i]);
//	cout<<endl;

	for(i=Nsel-1;i>0;i--)
		if(delta_profit[i]<0)
			ope_delta_drop(S,i);

	return 0;
}

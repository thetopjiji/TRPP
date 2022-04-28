/*
 * Ope_2opt.cpp
 *
 *  Created on: 23Step2020
 *      Author: JT
 *      Only change the order of the cities, not change the selected city set
 */


#include "trpp_lib.h"
#include "Detail_TRPP.h"

//export function
int Ope_2opt(Struc_Sol *S, int &ite);

//local funtion

bool D2opt_old(Struc_Sol *S);
bool D2opt_allsearch(Struc_Sol *S);

int get_delta_2opt(Struc_Sol *S, int node1, int node2);
int ope_delta_2opt(Struc_Sol *S, int node1, int node2);
int real_get_2opt(Struc_Sol *S, int smaposnode, int bigposnode);
int real_ope_2opt(Struc_Sol *S, int smaposnode, int bigposnode);

//int get_e2e_cdis_general(int *arr_v, int length, int startpos, int Nsel);
//2opt domain

int Ope_2opt(Struc_Sol *S, int &ite){

	perm2sd(S);
	perm2SbSe(S);
	verify_cdis(S);
	verify_Vsd(S);

	D2opt_old(S);
//	D2opt_allsearch(S);
	return 1;
}

bool D2opt_old(Struc_Sol *S){		//exchange with the candidate set and follow an order of the city number
	bool Fimp=false;
	int i,j;
	int fixnode, swapnode;
	int num_f=0;
	do{
		Fimp=false;
		for(i=0;i<num_city;i++){		//start from the city 0, because we exchange the edge with a previous node (which is city 0)
			fixnode=i;
			if(S->f_city[fixnode]==0) continue;
			for(j=0;j<Num_candidate_per_city[fixnode];j++){
				swapnode=Candidate_set[fixnode][j];
				if(S->f_city[swapnode]==0) continue;
				if(fixnode==swapnode) continue;
//				if(abs(S->position[fixnode]-S->position[swapnode])==0) continue;	//adjacent nodes ？？？？？？？？？？
				if(abs(S->position[fixnode]-S->position[swapnode])<=1) continue;	//adjacent nodes
				if(check_time()) return true;
				if(get_delta_2opt(S,fixnode,swapnode)<0){
					Fimp=true;
					num_f=ope_delta_2opt(S,fixnode,swapnode);

					/***************/
//					verify_SbSe(S);
//					verify_Vsd(S);
//					verify_cdis(S);
					/****************/

					getfitness2(S);
					UpdateBestS(S, num_f);
					break;							// first improve strategy
				}
			}
		}
	}while(Fimp);

	return Fimp;
}

bool D2opt_allsearch(Struc_Sol *S){
	bool Fimp=false;
	int i,j;
	int fixnode, swapnode;
	int num_f=0;
	do{
		Fimp=false;
		for(i=0;i<S->Nsel;i++){		//start from the city 0, because we exchange the edge with a previous node (which is city 0)
			fixnode=S->perm[i];
			for(j=i+2;j<S->Nsel;j++){
				swapnode=S->perm[j];
				if(abs(S->position[fixnode]-S->position[swapnode])<=1) continue;	//adjacent nodes

				if(check_time()) return true;
				if(get_delta_2opt(S,fixnode,swapnode)<0){
					Fimp=true;
					num_f=ope_delta_2opt(S,fixnode,swapnode);

					/***************/
//					verify_SbSe(S);
//					verify_Vsd(S);
//					verify_cdis(S);
					/****************/

					getfitness2(S);
					UpdateBestS(S, num_f);
					break;							// first improve strategy
				}
			}
		}
	}while(Fimp);

	return Fimp;
}

int get_delta_2opt(Struc_Sol *S, int node1, int node2){


 	if(S->position[node1]<S->position[node2]) return real_get_2opt(S,node1,node2);
	else if(S->position[node1]>S->position[node2]) return real_get_2opt(S,node2,node1);
	else {
		cout<<"error in equal city number"<<endl;
		exit(-2);
	}
	return 0;
}

int ope_delta_2opt(Struc_Sol *S, int node1, int node2){

	if(S->position[node1]<S->position[node2]) {
		S->cdis=S->cdis+ real_ope_2opt(S,node1,node2);
		return 11;
	}
	else if(S->position[node1]>S->position[node2]) {
		S->cdis=S->cdis+ real_ope_2opt(S,node2,node1);
		return 12;
	}
	else {
		cout<<"error in equal city number"<<endl;
		exit(-2);
	}

	return 3;
}

int real_get_2opt(Struc_Sol *S, int smaposnode, int bigposnode){

	int E1pre=smaposnode;
	int pos1=S->position[smaposnode];
//	int E2nex=S->nextC[bigposnode];
	int E2nex=locate_nextcity(S,bigposnode);
	int pos2=S->position[bigposnode];
	int Before_swap=0, After_swap=0;
	int length=pos2-pos1+2;
	int arr_v[length];
	int count=0;


//	count=0;
//	for(int i=pos1;i<=pos2+1;i++) {
//		arr_v[count]=S->perm[i];
//		count++;
//	}
//	Before_swap=get_e2e_cdis_general(arr_v,length,pos1,S->Nsel);
//
//	arr_v[0]=E1pre;
//	count=1;
//	for(int i=pos2;i>pos1;i--) {
//		arr_v[count]=S->perm[i];
//		count++;
//	}
//	arr_v[length-1]=E2nex;
//	After_swap=get_e2e_cdis_general(arr_v,length,pos1,S->Nsel);

	/***********************/
	int newcdis=0;
	newcdis= fast2opt(pos1, pos2, S->vec_sd, S->Nsel, S->perm, S->Sb, S->Se, S->cdis);
//
//	if(newcdis!=After_swap-Before_swap){
//		cout<<"i="<<pos1<<", j="<<pos2<<"\t";
//		cout<<"old="<<After_swap-Before_swap<<"\t new="<<newcdis<<endl;
//	}
	return newcdis;
	/*****************************/
//	return  After_swap-Before_swap;
}

int real_ope_2opt(Struc_Sol *S, int smaposnode, int bigposnode){
	int E1pre=smaposnode;
//	int E1nex=S->nextC[smaposnode];
	int pos1=S->position[smaposnode];
//	int E2pre=bigposnode;
//	int E2nex=S->nextC[bigposnode];
	int E2nex=locate_nextcity(S,bigposnode);
	int pos2=S->position[bigposnode];
//	int Before_swap=0, After_swap=0;
	int length=pos2-pos1+2;
	int arr_v[length];
	int count=0;
	int i;
	/*get the delta_cdis*/
//	count=0;
//	for( i=pos1;i<=pos2+1;i++) {
//		arr_v[count]=S->perm[i];
//		count++;
//	}
//	Before_swap=get_e2e_cdis_general(arr_v,length,pos1,S->Nsel);

//	double st_ime=get_time();

	arr_v[0]=E1pre;
	count=1;
	for( i=pos2;i>pos1;i--) {
		arr_v[count]=S->perm[i];
		count++;
	}
	arr_v[length-1]=E2nex;
//	After_swap=get_e2e_cdis_general(arr_v,length,pos1,S->Nsel);

	/*get the new cdis*/
	int newcdis=0;
	newcdis= fast2opt(pos1, pos2, S->vec_sd, S->Nsel, S->perm, S->Sb, S->Se, S->cdis);

//	double rate = Beta * (pow(CONST_E, 1.0*abs(newcdis)/(1.0*S->cdis)) - 1);

	/*update the delta perm*/
	count=0;
	for(i=pos1; i<=pos2+1;i++){
		S->perm[i]=arr_v[count];
		count++;
	}
	/*update the delta pos*/
	for(i=pos1; i<=pos2+1;i++) {
		if(S->perm[i]==-1) break;
		S->position[S->perm[i]]=i;
	}

	/*update the ll: NextC, prevN*/
//	int pren=-1, nown=-1;
//	for(i=pos1+1;i<=pos2+1;i++){
//		pren=S->perm[i-1];
//		nown=S->perm[i];
//		S->nextC[pren]=nown;
//		if(nown!=-1) S->prevC[nown]=pren;
//	}

//	t_base+=get_time()-st_ime;

	/*update SbSe and Vsd*/
	updateSbSe(pos1, pos2, S->Nsel, S->perm, S->vec_sd, S->Sb, S->Se);

	/*update the chosenT and weight*/
	update_chosenT(arr_v[0],arr_v[1]);
	update_chosenT(arr_v[length-2],arr_v[length-1]);

//	update_weight(arr_v[0],arr_v[1],rate);
//	update_weight(arr_v[length-2],arr_v[length-1],rate);

	/*the f_city and Nsel are not changed and the profit is updated outside*/
	return newcdis;
}

/*
 * OPE_SWAPADJ.cpp
 *
 *  Created on: 11Step2020
 *      Author: DELL
 */


#include "trpp_lib.h"
#include "Detail_TRPP.h"

//export function
int Ope_swapadj(Struc_Sol *S, int &ite);

//local funtion

int get_delta_swapad_copie(Struc_Sol *S, int node1, int node2);
int ope_delta_swapad_copie(Struc_Sol *S, int node1, int node2);
int get_pre_nex_cdis_copie(Struc_Sol *S, int pos, int node, int prenode, int nexnode);

//Swap domain
int Ope_swapadj(Struc_Sol *S, int &ite){	//only change the order, not the containing set of city

	bool flag_improve=false;
	int i;
	int fixnode,swapnode;
	int delta_dis;

	do{
		flag_improve=false;
		for(i=1;i<S->Nsel-1;i++){							//depot is out of consideration, so start from 1
			fixnode=S->perm[i];
			swapnode=S->perm[i+1];
			delta_dis=get_delta_swapad_copie(S,fixnode,swapnode);
			if(delta_dis<0) {
				ope_delta_swapad_copie(S,fixnode,swapnode);

				/****************/
//				verify_ll(S);
				/****************/


				if(check_time()) return 1;
				flag_improve=true;						//local best get improved
				getfitness2(S);
				UpdateBestS(S, 8);
				break;
			}
		}
	}while(flag_improve);

	return 0;
}

int get_delta_swapad_copie(Struc_Sol *S, int node1, int node2){
	int pos1=S->position[node1];
	int pos2=S->position[node2];
//	int npre1=S->prevC[node1];
//	int nnex1=S->nextC[node1];
//	int npre2=S->prevC[node2];
//	int nnex2=S->nextC[node2];
	int npre1=locate_prevcity(S,node1);
	int nnex1=locate_nextcity(S,node1);
	int npre2=locate_prevcity(S,node2);
	int nnex2=locate_nextcity(S,node2);
	int Before_swap=0;
	int After_swap=0;

	if(pos1==S->Nsel-1){
		if(nnex1!=-1) {						//verify if pos1 is the bottom position;
			cout<<"error in pos1"<<endl;
			exit(-3);
		}
		Before_swap= matrix_dis[npre2][node2]*2;
		After_swap = matrix_dis[npre2][node1]*2;

		/*to check the cdis and the profit*/
//		int all_v[num_city+1];
//		int temp_profit;
//		memset(all_v,-1,(num_city+1)*sizeof(int));
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		/***********************************/
//		return S->profit-temp_profit;
		/***********************************/

		return After_swap-Before_swap;
	}
	else if(pos2==S->Nsel-1){
		if(nnex2!=-1) {						//verify if pos2 is the bottom position;
			cout<<"error in pos2"<<endl;
			exit(-3);
		}
		Before_swap=matrix_dis[npre1][node1]*2;
		After_swap =matrix_dis[npre1][node2]*2;

		/*to check the cdis and the profit*/
//		int all_v[num_city+1];
//		int temp_profit;
//		memset(all_v,-1,(num_city+1)*sizeof(int));
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		/***********************************/
//		return S->profit-temp_profit;
		/***********************************/

		return After_swap-Before_swap;
	}
	else if(pos1<pos2){						// node 1 is in front of node2
		Before_swap=matrix_dis[npre1][node1]*(S->Nsel-pos1) + matrix_dis[node2][nnex2]*(S->Nsel-pos2-1);
		After_swap =matrix_dis[npre1][node2]*(S->Nsel-pos1) + matrix_dis[node1][nnex2]*(S->Nsel-pos2-1);

		/*to check the cdis and the profit*/
//		int all_v[num_city+1];
//		int temp_profit;
//		memset(all_v,-1,(num_city+1)*sizeof(int));
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		/***********************************/
//		return S->profit-temp_profit;
		/***********************************/

		return After_swap-Before_swap;
	}
	else if(pos1>pos2){
		Before_swap=matrix_dis[node1][nnex1]*(S->Nsel-pos1-1) + matrix_dis[npre2][node2]*(S->Nsel-pos2);
		After_swap =matrix_dis[node2][nnex1]*(S->Nsel-pos1-1) + matrix_dis[npre2][node1]*(S->Nsel-pos2);

		/*to check the cdis and the profit*/
//		int all_v[num_city+1];
//		int temp_profit;
//		memset(all_v,-1,(num_city+1)*sizeof(int));
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		/***********************************/
//		return S->profit-temp_profit;
		/***********************************/

		return After_swap-Before_swap;
	}

	return 0;
}

int ope_delta_swapad_copie(Struc_Sol *S, int node1, int node2){
	int pos1=S->position[node1];
	int pos2=S->position[node2];
//	int npre1=S->prevC[node1];
//	int nnex1=S->nextC[node1];
//	int npre2=S->prevC[node2];
//	int nnex2=S->nextC[node2];
	int npre1=locate_prevcity(S,node1);
	int nnex1=locate_nextcity(S,node1);
	int npre2=locate_prevcity(S,node2);
	int nnex2=locate_nextcity(S,node2);
	int Before_swap=0;
	int After_swap=0;

	if(pos1==S->Nsel-1){
		if(nnex1!=-1) {						//verify if pos1 is the bottom position;
			cout<<"error in pos1"<<endl;
			exit(-3);
		}
		Before_swap= matrix_dis[npre2][node2]*2;
		After_swap = matrix_dis[npre2][node1]*2;

	}
	else if(pos2==S->Nsel-1){
		if(nnex2!=-1) {						//verify if pos2 is the bottom position;
			cout<<"error in pos2"<<endl;
			exit(-3);
		}
		Before_swap=matrix_dis[npre1][node1]*2;
		After_swap =matrix_dis[npre1][node2]*2;

	}
	else if(pos1<pos2){						// node 1 is in front of node2
		Before_swap=matrix_dis[npre1][node1]*(S->Nsel-pos1) + matrix_dis[node2][nnex2]*(S->Nsel-pos2-1);
		After_swap =matrix_dis[npre1][node2]*(S->Nsel-pos1) + matrix_dis[node1][nnex2]*(S->Nsel-pos2-1);

	}
	else if(pos1>pos2){
		Before_swap=matrix_dis[node1][nnex1]*(S->Nsel-pos1-1) + matrix_dis[npre2][node2]*(S->Nsel-pos2);
		After_swap =matrix_dis[node2][nnex1]*(S->Nsel-pos1-1) + matrix_dis[npre2][node1]*(S->Nsel-pos2);

	}

	/*update the cdis*/
//	double rate = Beta * (pow(CONST_E, 1.0*abs(After_swap-Before_swap)/(1.0*S->cdis)) - 1);
	S->cdis=S->cdis-Before_swap+After_swap;
	/*update the pos*/
	S->position[node1]=pos2;
	S->position[node2]=pos1;
	/*update the perm*/
	S->perm[pos1]=node2;
	S->perm[pos2]=node1;
	/*update the ll*/

//	if(pos1==S->Nsel-1){			//nnex1=-1
//		S->prevC[node1]=npre2;
//		S->nextC[node1]=node2;
//		S->prevC[node2]=node1;
//		S->nextC[node2]=nnex1;
//		S->nextC[npre2]=node1;
////		S->prevC[nnex1]=node2;
//	}
//	else if(pos2==S->Nsel-1){		//nnex2=-1
//		S->prevC[node1]=node2;
//		S->nextC[node1]=nnex2;
//		S->prevC[node2]=npre1;
//		S->nextC[node2]=node1;
//		S->nextC[npre1]=node2;
////		S->prevC[nnex2]=node1;
//	}
//	else{
//		if(pos1<pos2){
//			S->prevC[node1]=node2;
//			S->nextC[node1]=nnex2;
//			S->prevC[node2]=npre1;
//			S->nextC[node2]=node1;
//			S->nextC[npre1]=node2;
//			S->prevC[nnex2]=node1;
//		}
//		else{
//			S->prevC[node1]=npre2;
//			S->nextC[node1]=node2;
//			S->prevC[node2]=node1;
//			S->nextC[node2]=nnex1;
//			S->nextC[npre2]=node1;
//			S->prevC[nnex1]=node2;
//		}
//	}

	/*update the matrix_chosentimes*/
	update_chosenT(node1,npre2);
	update_chosenT(node1,nnex2);
	update_chosenT(node2,npre1);
	update_chosenT(node2,nnex1);

//	update_weight(node1,npre2,rate);
//	update_weight(node1,nnex2,rate);
//	update_weight(node2,npre1,rate);
//	update_weight(node2,nnex1,rate);

	/*Nsel, f_city are not changed and profit is updated in the end*/
	return 0;
}

int get_pre_nex_cdis_copie(Struc_Sol *S, int pos, int node, int prenode, int nexnode){
	return (S->Nsel-pos)*matrix_dis[prenode][node] + (S->Nsel-pos-1)*matrix_dis[node][nexnode];
}
/***********************************/

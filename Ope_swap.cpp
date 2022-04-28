/*
 * OPE_SWAP.cpp
 *
 *  Created on: 11Step2020
 *      Author: DELL
 */


#include "trpp_lib.h"
#include "Detail_TRPP.h"

//export function
int Ope_swap(Struc_Sol *S, int &ite);

//local funtion

int Ope_oldswap(Struc_Sol *S, int &ite);
int Ope_allswap(Struc_Sol *S, int &ite);
int Ope_correct_old(Struc_Sol *S, int &ite);
int Ope_correct_new(Struc_Sol *S, int &ite);

int get_delta_swap(Struc_Sol *S, int node1, int node2);
int ope_delta_swap(Struc_Sol *S, int node1, int node2);
int get_delta_swapad(Struc_Sol *S, int node1, int node2);
int ope_delta_swapad(Struc_Sol *S, int node1, int node2);
int get_pre_nex_cdis(Struc_Sol *S, int pos, int node, int prenode, int nexnode);

//Swap domain
int Ope_swap(Struc_Sol *S, int &ite){	//only change the order, not the containing set of city

//	Ope_correct_new(S,ite);
	Ope_correct_old(S,ite);
//	Ope_oldswap(S,ite);
//	Ope_allswap(S,ite);

	return 0;
}

int Ope_oldswap(Struc_Sol *S, int &ite){
	bool flag_improve=false;
	int i,j;
	int fixnode,swapnode;
	int delta_dis;

////	/*checking variable*/
//	int temp_prevN[num_city];
//	int temp_nextN[num_city];
//	int temp_posiN[num_city];
////	Struc_Sol TempS;
//	int tempdis;
//
//	CopySolution(S,BestS);
//	for(int p=1;p<num_city;p++){
////		CopySolution(&TempS,S);
//		fixnode=p;
//		if(S->f_city[fixnode]==0) continue;
//		for(int q=1;q<num_city;q++){
//			CopySolution(BestS,S);
//			swapnode=q;
//			if(S->f_city[swapnode]==0) continue;
//			if(swapnode==fixnode) continue;
//
//			if(abs(S->position[swapnode]-S->position[fixnode])<=1){
//				tempdis=S->cdis+get_delta_swapad(S,fixnode,swapnode);
//				ope_delta_swapad(S,fixnode,swapnode);
//			}
//			else{
//				tempdis=S->cdis+get_delta_swap(S,fixnode,swapnode);
//				ope_delta_swap(S,fixnode,swapnode);
//			}
//			if(S->cdis-tempdis!=0) {
//				cout<<"error in delta dis"<<endl;
//				cout<<fixnode<<swapnode<<" "<<endl;
//			}
//			for(i=0;i<num_city;i++) temp_prevN[i]=S->prevC[i];
//			for(i=0;i<num_city;i++) temp_nextN[i]=S->nextC[i];
//			for(i=0;i<num_city;i++) temp_posiN[i]=S->position[i];
//			/*check the ll*/
//			perm2ll(S);
//			perm2position(S);
//			for(i=0;i<num_city;i++)
//				if(S->prevC[i]-temp_prevN[i]!=0) {
//					cout<<"error in deltaPreN"<<endl;
//					break;
//				}
//			for(i=0;i<num_city;i++)
//				if(S->nextC[i]-temp_nextN[i]!=0) {
//					cout<<"error in deltaNextC"<<endl;
//					break;
//				}
//			for(i=0;i<num_city;i++)
//				if(S->position[i]-temp_posiN[i]!=0) {
//					cout<<"error in DeltaposiN"<<endl;
//					break;
//				}
//		}
//	}
//	exit(0);

	do{
		flag_improve=false;
		for(i=1;i<num_city;i++){							//depot is out of consideration, so start from 1
			fixnode=i;
			if(S->f_city[fixnode]==0) continue;				//consider only the selected cites
			for(j=0;j<Num_candidate_per_city[fixnode];j++){
				swapnode=Candidate_set[fixnode][j];			// evaluate only the candidate_set nodes
				if(S->f_city[swapnode]==0) continue;		//consider only the selected cites

				if(fixnode==swapnode) continue;				// filter fixnode==swapnode
				/************focus on the non-adjacent cities*************/
				if(abs(S->position[fixnode]-S->position[swapnode])>=2){
					delta_dis=get_delta_swap(S,fixnode,swapnode);
					if(check_time()) return 1;
					if(delta_dis<0) {
						ope_delta_swap(S,fixnode,swapnode);
						flag_improve=true;						//local best get improved
//						/***************************/
//						printf("exchange %d and %d\n", fixnode, swapnode);
//						layoutSol(S);
//						getcdis(S);
//						cout<<S->cdis<<endl;
//						getfitness2(S);
//						cout<<S->profit<<endl;
//						/***************************/
//						UpdateBestS(S, 1);							//verify if the BestS is improved
						break;
					}
				}
				/*************focus on the adjacent cities************/
				else if(abs(S->position[fixnode]-S->position[swapnode])==1){
					delta_dis=get_delta_swapad(S,fixnode,swapnode);
					if(delta_dis<0) {
						ope_delta_swapad(S,fixnode,swapnode);
						if(check_time()) return 1;
						flag_improve=true;						//local best get improved
//						/***************************/
//						printf("exchange %d and %d\n", fixnode, swapnode);
//						layoutSol(S);
//						getcdis(S);
//						cout<<S->cdis<<endl;
//						getfitness2(S);
//						cout<<S->profit<<endl;
//						/***************************/
//						UpdateBestS(S, 1);							//verify if the BestS is improved
						break;
					}
				}
				else{
					cout<<"error in the swap in swap.cpp 2"<<endl;
					exit(-2);
				}
				/*************************/

			}
			getfitness2(S);
			UpdateBestS(S, 1);
		}
	}while(flag_improve);



	return 0;

}

int Ope_allswap(Struc_Sol *S, int &ite){
	bool flag_improve=false;
	int i,j;
	int fixnode,swapnode;
	int delta_dis;

////	/*checking variable*/
//	int temp_prevN[num_city];
//	int temp_nextN[num_city];
//	int temp_posiN[num_city];
////	Struc_Sol TempS;
//	int tempdis;
//
//	CopySolution(S,BestS);
//	for(int p=1;p<num_city;p++){
////		CopySolution(&TempS,S);
//		fixnode=p;
//		if(S->f_city[fixnode]==0) continue;
//		for(int q=1;q<num_city;q++){
//			CopySolution(BestS,S);
//			swapnode=q;
//			if(S->f_city[swapnode]==0) continue;
//			if(swapnode==fixnode) continue;
//
//			if(abs(S->position[swapnode]-S->position[fixnode])<=1){
//				tempdis=S->cdis+get_delta_swapad(S,fixnode,swapnode);
//				ope_delta_swapad(S,fixnode,swapnode);
//			}
//			else{
//				tempdis=S->cdis+get_delta_swap(S,fixnode,swapnode);
//				ope_delta_swap(S,fixnode,swapnode);
//			}
//			if(S->cdis-tempdis!=0) {
//				cout<<"error in delta dis"<<endl;
//				cout<<fixnode<<swapnode<<" "<<endl;
//			}
//			for(i=0;i<num_city;i++) temp_prevN[i]=S->prevC[i];
//			for(i=0;i<num_city;i++) temp_nextN[i]=S->nextC[i];
//			for(i=0;i<num_city;i++) temp_posiN[i]=S->position[i];
//			/*check the ll*/
//			perm2ll(S);
//			perm2position(S);
//			for(i=0;i<num_city;i++)
//				if(S->prevC[i]-temp_prevN[i]!=0) {
//					cout<<"error in deltaPreN"<<endl;
//					break;
//				}
//			for(i=0;i<num_city;i++)
//				if(S->nextC[i]-temp_nextN[i]!=0) {
//					cout<<"error in deltaNextC"<<endl;
//					break;
//				}
//			for(i=0;i<num_city;i++)
//				if(S->position[i]-temp_posiN[i]!=0) {
//					cout<<"error in DeltaposiN"<<endl;
//					break;
//				}
//		}
//	}
//	exit(0);

	do{
		flag_improve=false;
		for(i=1;i<num_city;i++){							//depot is out of consideration, so start from 1
			fixnode=i;
			if(S->f_city[fixnode]==0) continue;				//consider only the selected cites
			for(j=1;j<S->Nsel;j++){
				swapnode=j;			// evaluate all node
				if(S->f_city[swapnode]==0) continue;		//consider only the selected cites except the depot

				if(fixnode==swapnode) continue;				// filter fixnode==swapnode
				/************focus on the non-adjacent cities*************/
				if(abs(S->position[fixnode]-S->position[swapnode])>=2){
					delta_dis=get_delta_swap(S,fixnode,swapnode);
					if(check_time()) return 1;
					if(delta_dis<0) {
						ope_delta_swap(S,fixnode,swapnode);
						flag_improve=true;						//local best get improved
//						/***************************/
//						printf("exchange %d and %d\n", fixnode, swapnode);
//						layoutSol(S);
//						getcdis(S);
//						cout<<S->cdis<<endl;
//						getfitness2(S);
//						cout<<S->profit<<endl;
//						/***************************/
//						UpdateBestS(S, 1);							//verify if the BestS is improved
						break;
					}
				}
				/*************focus on the adjacent cities************/
				else if(abs(S->position[fixnode]-S->position[swapnode])==1){
					delta_dis=get_delta_swapad(S,fixnode,swapnode);
					if(delta_dis<0) {
						ope_delta_swapad(S,fixnode,swapnode);
						if(check_time()) return 1;
						flag_improve=true;						//local best get improved
//						/***************************/
//						printf("exchange %d and %d\n", fixnode, swapnode);
//						layoutSol(S);
//						getcdis(S);
//						cout<<S->cdis<<endl;
//						getfitness2(S);
//						cout<<S->profit<<endl;
//						/***************************/
//						UpdateBestS(S, 1);							//verify if the BestS is improved
						break;
					}
				}
				else{

					for(int m=0;m<num_city;m++) cout<<S->position[m]<<" ";
					cout<<endl;
					for(int m=0;m<num_city;m++) cout<<S->perm[m]<<" ";
					cout<<endl;
					cout<<"error in the swap in swap.cpp"<<endl;
					exit(-2);
				}
				/*************************/

			}
			getfitness2(S);
			UpdateBestS(S, 1);
		}
	}while(flag_improve);



	return 0;

}


int Ope_correct_old(Struc_Sol *S, int &ite){
	bool flag_improve=false;
	int i,j;
	int fixnode,swapnode,fixnext;
	int delta_dis;

	do{
		flag_improve=false;
		for(i=1;i<num_city;i++){							//depot is out of consideration, so start from 1

			fixnode=i;
  			if(S->f_city[fixnode]==0) continue;				//consider only the selected cites

//			fixnext=S->nextC[fixnode];
			fixnext=locate_nextcity(S,fixnode);

			if(fixnext==-1) continue; 						// fixnode is the bottom node and fixnext is an invalid node

			for(j=0;j<Num_candidate_per_city[fixnode];j++){
				swapnode=Candidate_set[fixnode][j];			// evaluate only the candidate_set nodes
				if(S->f_city[swapnode]==0) continue;		//consider only the selected cites

				if(fixnode==swapnode){
					cout<<"error in swap, fixnode=swapnode, candidate set is filled by itself."<<endl;
					exit(-2);
				}
				if(fixnext==swapnode) continue;				// filter fixnext==swapnode
				/************focus on the non-adjacent cities*************/
				if(abs(S->position[fixnext]-S->position[swapnode])>=2){
					delta_dis=get_delta_swap(S,fixnext,swapnode);
					if(check_time()) return 1;

					/****************/
//					verify_ll(S);
					/****************/

					if(delta_dis<0) {
						ope_delta_swap(S,fixnext,swapnode);
						flag_improve=true;						//local best get improved
						getfitness2(S);
						UpdateBestS(S, 1);
						break;
					}
				}
				/*************focus on the adjacent cities************/
				else if(abs(S->position[fixnext]-S->position[swapnode])==1){
					delta_dis=get_delta_swapad(S,fixnext,swapnode);
					if(delta_dis<0) {
						ope_delta_swapad(S,fixnext,swapnode);

						/****************/
//						verify_ll(S);
						/****************/
						if(check_time()) return 1;
						flag_improve=true;						//local best get improved
						getfitness2(S);
						UpdateBestS(S, 1);
						break;
					}
				}
				else{
					cout<<"error in the swap in swap.cpp 2"<<endl;
					exit(-2);
				}
				/*************************/
			}
		}
	}while(flag_improve);

	return 0;
}


int Ope_correct_new(Struc_Sol *S, int &ite){
	bool flag_improve=false;
	int i,j;
	int fixnode,swapnode,fixnext,fixprev;
	int delta_dis;

	do{
		flag_improve=false;
		for(i=1;i<num_city;i++){							//depot is out of consideration, so start from 1

			fixnode=i;
			if(S->f_city[fixnode]==0) continue;				//consider only the selected cites

//			fixnext=S->nextC[fixnode];
			fixnext=locate_nextcity(S,fixnode);
//			fixprev=S->prevC[fixnode];
			fixprev=locate_prevcity(S,fixnode);

			if(fixnext==-1) continue; 						// fixnode is the bottom node and fixnext is an invalid node

			for(j=0;j<Num_candidate_per_city[fixnode];j++){
				swapnode=Candidate_set[fixnode][j];			// evaluate only the candidate_set nodes

				if(swapnode==0){
					cout<<"error in swap operator,  0 is taken as the swapnode"<<endl;
					exit(-2);
				}

				if(S->f_city[swapnode]==0) continue;		//consider only the selected cites

				if(fixnode==swapnode){
					cout<<"error in swap, fixnode=swapnode, candidate set is filled by itself."<<endl;
					exit(-2);
				}
				if(fixnext==swapnode) continue;				// filter fixnext==swapnode
				/************focus on the non-adjacent cities*************/
				if(abs(S->position[fixnext]-S->position[swapnode])>=2){
					delta_dis=get_delta_swap(S,fixnext,swapnode);
					if(check_time()) return 1;
					if(delta_dis<0) {
						ope_delta_swap(S,fixnext,swapnode);
						flag_improve=true;						//local best get improved
						break;
					}
				}
				/*************focus on the adjacent cities************/
				else if(abs(S->position[fixnext]-S->position[swapnode])==1){
					delta_dis=get_delta_swapad(S,fixnext,swapnode);
					if(delta_dis<0) {
						ope_delta_swapad(S,fixnext,swapnode);
						if(check_time()) return 1;
						flag_improve=true;						//local best get improved
						break;
					}
				}
				else{
					cout<<"error in the swap in swap.cpp 2"<<endl;
					exit(-2);
				}
				/***********about the fixprev**************/
				if(fixprev==swapnode) continue;				// filter fixnext==swapnode
				if(fixprev==0) continue;					// 0 is not taken as swap

				/************focus on the non-adjacent cities*************/
				if(abs(S->position[fixprev]-S->position[swapnode])>=2){
					delta_dis=get_delta_swap(S,fixprev,swapnode);
					if(check_time()) return 1;
					if(delta_dis<0) {
						ope_delta_swap(S,fixprev,swapnode);
						flag_improve=true;						//local best get improved
						break;
					}
				}
				/*************focus on the adjacent cities************/
				else if(abs(S->position[fixprev]-S->position[swapnode])==1){
					delta_dis=get_delta_swapad(S,fixprev,swapnode);
					if(delta_dis<0) {
						ope_delta_swapad(S,fixprev,swapnode);
						if(check_time()) return 1;
						flag_improve=true;						//local best get improved
						break;
					}
				}
				else{
					cout<<"error in the swap in swap.cpp 2"<<endl;
					exit(-2);
				}



			}
			getfitness2(S);
			UpdateBestS(S, 1);
		}
	}while(flag_improve);

	return 0;
}


int get_delta_swap(Struc_Sol *S, int node1, int node2){

	int pos1=S->position[node1];
	int pos2=S->position[node2];
//	int npre1=S->prevC[node1];
	int npre1=locate_prevcity(S,node1);

//	int nnex1=S->nextC[node1];
	int nnex1=locate_nextcity(S,node1);

//	int npre2=S->prevC[node2];
	int npre2=locate_prevcity(S,node2);
//	int nnex2=S->nextC[node2];
	int nnex2=locate_nextcity(S,node2);

	int Before_swap=0;
	int After_swap=0;


//	int temp_next[num_city];
//	for(int i=0;i<num_city;i++) temp_next[i]=S->nextC[i];


	if(pos1==S->Nsel-1){
		if(nnex1!=-1) {						//verify if pos1 is the bottom position;
			cout<<"error in pos1"<<endl;
			exit(-3);
		}
		Before_swap=matrix_dis[npre1][node1] + get_pre_nex_cdis(S, pos2,node2, npre2,nnex2);
		After_swap =matrix_dis[npre1][node2] + get_pre_nex_cdis(S, pos2,node1, npre2,nnex2);


		/*to check the cdis and the profit*/
//		int all_v[num_city+1];
//		int temp_profit;
//		memset(all_v,-1,(num_city+1)*sizeof(int));
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		return S->profit-temp_profit;
		/***********************/
		return After_swap-Before_swap;
	}
	else if(pos2==S->Nsel-1){
		if(nnex2!=-1) {						//verify if pos2 is the bottom position;
			cout<<"error in pos2 in get_delta_swap 1 "<<endl;
			exit(-3);
		}
		Before_swap=get_pre_nex_cdis(S,pos1,node1, npre1,nnex1) + matrix_dis[npre2][node2];
		After_swap =get_pre_nex_cdis(S,pos1,node2, npre1,nnex1) + matrix_dis[npre2][node1];


		/*to check the cdis and the profit*/
//		int all_v[num_city+1];
//		int temp_profit;
//		memset(all_v,-1,(num_city+1)*sizeof(int));
//
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		return S->profit-temp_profit;
		/*************************/
		return After_swap-Before_swap;
	}
	else{
		Before_swap=get_pre_nex_cdis(S,pos1,node1, npre1,nnex1) + get_pre_nex_cdis(S, pos2,node2, npre2,nnex2);
		After_swap= get_pre_nex_cdis(S,pos1,node2, npre1,nnex1) + get_pre_nex_cdis(S, pos2,node1, npre2,nnex2);

//		/*to check the cdis and the profit*/
//		int all_v[num_city+1];
//		int temp_profit;
//		memset(all_v,-1,(num_city+1)*sizeof(int));
//
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		return S->profit-temp_profit;
//		/***********************/
		return After_swap-Before_swap;
	}

	return 0;
}

int ope_delta_swap(Struc_Sol *S, int node1, int node2){

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
//	int temp_node;

	if(pos1==S->Nsel-1){
		if(nnex1!=-1) {						//verify if pos1 is the bottom position;
			cout<<"error in pos1 in ope"<<endl;
			exit(-3);
		}
		Before_swap=matrix_dis[npre1][node1] + get_pre_nex_cdis(S, pos2,node2, npre2,nnex2);
		After_swap=matrix_dis[npre1][node2] + get_pre_nex_cdis(S, pos2, node1, npre2,nnex2);
	}

	else if(pos2==S->Nsel-1){
		if(nnex2!=-1) {						//verify if pos2 is the bottom position;
			cout<<"error in pos2 in ope"<<endl;
			exit(-3);
		}
		Before_swap=get_pre_nex_cdis(S,pos1,node1, npre1,nnex1) + matrix_dis[npre2][node2];
		After_swap =get_pre_nex_cdis(S,pos1,node2, npre1,nnex1) + matrix_dis[npre2][node1];
	}

	else{
		Before_swap=get_pre_nex_cdis(S,pos1,node1, npre1,nnex1) + get_pre_nex_cdis(S, pos2,node2, npre2,nnex2);
		After_swap= get_pre_nex_cdis(S,pos1,node2, npre1,nnex1) + get_pre_nex_cdis(S, pos2,node1, npre2,nnex2);
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
//	S->prevC[node1]=npre2;
//	S->nextC[node1]=nnex2;
//	S->prevC[node2]=npre1;
//	S->nextC[node2]=nnex1;
//	if(pos1==S->Nsel-1){			//nnex1=-1
//		S->nextC[npre1]=node2;
////		S->prevC[nnex1]=node2;
//		S->nextC[npre2]=node1;
//		S->prevC[nnex2]=node1;
//	}
//	else if(pos2==S->Nsel-1){		//nnex2=-1
//		S->nextC[npre1]=node2;
//		S->prevC[nnex1]=node2;
//		S->nextC[npre2]=node1;
////		S->prevC[nnex2]=node1;
//	}
//	else{
//		S->nextC[npre1]=node2;
//		S->prevC[nnex1]=node2;
//		S->nextC[npre2]=node1;
//		S->prevC[nnex2]=node1;
//	}

	/*update the matrix_chosentimes*/
	update_chosenT(node1,npre2);
	update_chosenT(node1,nnex2);
	update_chosenT(node2,npre1);
	update_chosenT(node2,nnex1);

//	update_weight(node1,npre2, rate);
//	update_weight(node1,nnex2, rate);
//	update_weight(node2,npre1, rate);
//	update_weight(node2,nnex1, rate);

	/*Nsel, f_city are not changed and profit is updated in the end*/
	return 0;
}

int get_delta_swapad(Struc_Sol *S, int node1, int node2){
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
//
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		return S->profit-temp_profit;
		/***********************/
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
//
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		return S->profit-temp_profit;
		/***********************/

		return After_swap-Before_swap;
	}
	else if(pos1<pos2){						// node 1 is in front of node2
		Before_swap=matrix_dis[npre1][node1]*(S->Nsel-pos1) + matrix_dis[node2][nnex2]*(S->Nsel-pos2-1);
		After_swap =matrix_dis[npre1][node2]*(S->Nsel-pos1) + matrix_dis[node1][nnex2]*(S->Nsel-pos2-1);


		/*to check the cdis and the profit*/
//		int all_v[num_city+1];
//		int temp_profit;
//		memset(all_v,-1,(num_city+1)*sizeof(int));
//
//		for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//		all_v[pos1]=node2;
//		all_v[pos2]=node1;
//		temp_profit=getfitness_vec(all_v);
//		return S->profit-temp_profit;
		/***********************/
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
//		return S->profit-temp_profit;
		/***********************/
		return After_swap-Before_swap;
	}

	return 0;
}

int ope_delta_swapad(Struc_Sol *S, int node1, int node2){
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

int get_pre_nex_cdis(Struc_Sol *S, int pos, int node, int prenode, int nexnode){
	return (S->Nsel-pos)*matrix_dis[prenode][node] + (S->Nsel-pos-1)*matrix_dis[node][nexnode];
}
/***********************************/

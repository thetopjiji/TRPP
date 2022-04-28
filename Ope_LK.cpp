/*
 * Ope_LK.cpp
 *
 *  Created on: 30Step2020
 *      Author: JT
 *      the candidate set is not changed, just change the order
 *      famous LK method like ejection chain
 */

#include "trpp_lib.h"
#include "Detail_TRPP.h"
#include "Ope_2opt.h"

//export function
int Ope_LK(Struc_Sol *S, int &ite);

//local function

int simulate_LK(int &bc_best,int &stp_best,int maxi_simtime,Struc_Sol *S, int *seq_best_candi, int *seq_best_disco);
int simulate_one(int &stp_temp, int begincity, Struc_Sol *S, int *seq_temp_candi, int *seq_temp_disco);

int LK_choosecity(Struc_Sol *S, int Cur_city, int Beg_city);
int LK_randomchoose(Struc_Sol *S, int Cur_city, int Beg_city);
double get_avg_weight(Struc_Sol *S, int Cur_city);
void remplir_promising(int *prom, int Cur_city, Struc_Sol *S, int &num_pro,  double avg_weight);
double Get_Potential(int Cur_city, int temp_city, Struc_Sol *S, double avg_weight);

int ope_lk2op(Struc_Sol *S, int node1, int node2);
int ope_real_lk2opt(Struc_Sol *S, int smaposnode, int bigposnode, int &delta_profit);
int ope_LK_final(int bc_best, int stp_best, Struc_Sol *S, int *seq_best_candi, int *seq_best_disco);


int Ope_LK(Struc_Sol *S, int &ite){


	perm2SbSe(S);
	perm2sd(S);


	int max_simtime=Param_H*S->Nsel;
	int delta_cdis=0;

	int seq_best_candi[num_city];
	int seq_best_disco[num_city];
	int stp_best;
	int bc_best=-1;

	while(true){

		memset(seq_best_candi,-1,num_city*sizeof(int));
		memset(seq_best_disco,-1,num_city*sizeof(int));

		delta_cdis=simulate_LK(bc_best, stp_best, max_simtime,S,seq_best_candi,seq_best_disco);
		if(check_time()) return true;

		if(delta_cdis<0){	//if improve, get updated and continue the LK
			ope_LK_final(bc_best, stp_best, S,seq_best_candi,seq_best_disco);
			getfitness2(S);
			UpdateBestS(S,7);
		}
		else break;			//if there is no improve, break; successive  max_simtime exploration
	}

	return 7;
}

int simulate_LK(int &bc_best, int &stp_best, int maxi_simtime,Struc_Sol *S,  int *seq_best_candi, int *seq_best_disco){
	int seq_temp_candi[num_city];
	int seq_temp_disco[num_city];
	int stp_temp=0;

	int best_delta_cdis=999999999;
	int temp_delta_cdis=0;

	int begincity;
	int acc_city[S->Nsel];
	int count=0;


	for(int i=0;i<num_city;i++)
		if(S->f_city[i]==1) {
			acc_city[count]=i;
			count++;
		}
	CopySolution(S, TempS);
	for(int i=0;i<maxi_simtime;i++){
		CopySolution(TempS, S);
		begincity=acc_city[rand()%count];
		if(S->position[begincity]==S->Nsel-1) continue;
		temp_delta_cdis=simulate_one(stp_temp, begincity, S,seq_temp_candi,seq_temp_disco);
		Total_sim++;
		if(check_time()) return true;
		if(temp_delta_cdis<best_delta_cdis){
			best_delta_cdis=temp_delta_cdis;
			CopyVec(seq_temp_candi,seq_best_candi,num_city);
			CopyVec(seq_temp_disco,seq_best_disco,num_city);
			stp_best=stp_temp;
			bc_best=begincity;
		}
		if(temp_delta_cdis<0) break; //Once get improved, return
	}
	CopySolution(TempS, S);
	return best_delta_cdis;
}

int ope_LK_final(int bc_best, int stp_best, Struc_Sol *S, int *seq_best_candi, int *seq_best_discos){

	for(int i=0;i<stp_best;i++) ope_delta_2opt(S,bc_best,seq_best_discos[i]);

	return 0;
}

int simulate_one(int &stp_temp, int begincity, Struc_Sol *S, int *seq_temp_candi, int *seq_temp_disco){

//	if(S->nextC[begincity]==-1) return 999999999;		//the last node should be taken into consideration
//	int Cur_city=S->nextC[begincity];
	int Cur_city=locate_nextcity(S,begincity);
	int Con_city=-1;
	int Pro_city=-1;
	int maxdepth=min(num_city-1,dep_lk);

	int sumdelta=0;
	stp_temp=0;

	double st_time;



	while(true){

		if(check_time()) return true;

		st_time=get_time();

		Con_city=LK_randomchoose(S,Cur_city, begincity);
//		Con_city=LK_choosecity(S,Cur_city, begincity);
		if(Con_city==-1) {
//			cout<<"find a -1 ciy in LK"<<endl;
			break;
		}

		t_base+=get_time()-st_time;


		seq_temp_candi[stp_temp]=Con_city;

		update_chosenT(Con_city, Cur_city);

		if(Con_city==0) Pro_city=S->perm[S->Nsel-1];
//		else Pro_city=S->prevC[Con_city];
		else Pro_city=locate_prevcity(S,Con_city);

		seq_temp_disco[stp_temp]=Pro_city;
		sumdelta+=ope_lk2op(S,begincity,Pro_city);
		stp_temp++;
		if(stp_temp>=maxdepth || sumdelta<0) break;		//Once get improved or reache the limit, finished
		Cur_city=Pro_city;
	}
	return sumdelta;
}

/*Choosing the promising vertex*/

int LK_choosecity(Struc_Sol *S, int Cur_city, int Beg_city){
	int num_pro=0;
	int Promising_city[num_city];
	double avg_weight=get_avg_weight(S,Cur_city);
	double Total_Potential = 0.0;
	double d_Prob[num_city];
	int rand_ind=rand()%1000;

	memset(Promising_city,-1,num_city*sizeof(int));
	remplir_promising(Promising_city,Cur_city, S, num_pro, avg_weight);

	if(num_pro==0) return -1;

	for (int i = 0; i < num_pro; i++)
		Total_Potential += Get_Potential(Cur_city, Promising_city[i],S ,avg_weight);

	d_Prob[0] = (int)(1000 * Get_Potential(Cur_city, Promising_city[0],S, avg_weight) / Total_Potential);
	for (int i = 1; i < num_pro - 1; i++)
		d_Prob[i] = d_Prob[i - 1] + (int)(1000 * Get_Potential(Cur_city, Promising_city[i],S, avg_weight) / Total_Potential);
	d_Prob[num_pro - 1] = 1000;

	for (int i = 0; i < num_pro; i++)
		if (rand_ind < d_Prob[i])
			return Promising_city[i];

	return -1;
}

int LK_randomchoose(Struc_Sol *S, int Cur_city, int Beg_city){
	int Promising_city[num_city];
	int citychoose=-1;
	int num_pro=0;
	int temp_city;

	memset(Promising_city,-1,num_city*sizeof(int));

	for(int i=0;i<Num_candidate_pre_per_city[Cur_city];i++){					// Num_candidate_per_city-> Num_candidate_pre_per_city
		temp_city=Candidate_pre_set[Cur_city][i];								// Candidate_set-> Candidate_pre_set
		if(S->f_city[temp_city]==0) continue;									//vertices should be in the chosen set
		if(abs(S->position[Cur_city]-S->position[temp_city])*1.0<2.1) continue; //adjacent vertices are excluded.
		Promising_city[num_pro]=temp_city;
		num_pro++;
	}


	if (num_pro==0)	return citychoose;
	return Promising_city[rand()%num_pro];
}

double get_avg_weight(Struc_Sol *S, int Cur_city){
	double sumweight=0.0;
	for(int i=0;i<num_city;i++){
		if(S->f_city[i]==0 || i==Cur_city) continue;
//		sumweight+=weight[Cur_city][i];
	}
	return sumweight*1.0/(S->Nsel-2);
}


void remplir_promising(int *prom, int Cur_city, Struc_Sol *S, int &num_pro, double avg_weight){
	int temp_city;
	num_pro=0;
	for(int i=0;i<Num_candidate_pre_per_city[Cur_city];i++){				// Num_candidate_per_city-> Num_candidate_pre_per_city
		temp_city=Candidate_pre_set[Cur_city][i];							// Candidate_set-> Candidate_pre_set
		if(S->f_city[temp_city]==0) continue;								//vertices should be in the chosen set
		if(abs(S->position[Cur_city]-S->position[temp_city])*1.0<2.1) continue; 	//adjacent vertices are excluded.
		if (Get_Potential(Cur_city, temp_city, S, avg_weight) < 1) continue;
		prom[num_pro]=temp_city;
		num_pro++;
	}
}

double Get_Potential(int Cur_city, int temp_city, Struc_Sol *S, double avg_weight){
//	double d_pro=weight[Cur_city][temp_city] / avg_weight + Alpha * sqrt(log(Total_sim + 1) / (log (CONST_E) * ((double)Matrix_chosenT[Cur_city][temp_city] + 1)));;

//	return d_pro;
	return 0;
}

/*******************************/


int ope_lk2op(Struc_Sol *S, int node1, int node2){

	int delta_dis;
	int delta_profit=0;

//	if(S->position[node2]==S->Nsel-1) Pro_city=S->nextC[node1];
//	else Pro_city=node2;

//	Pro_city=node2;

//	if(S->nextC[node2]==-1) Pro_city=S->nextC[node1];
//	else Pro_city=S->nextC[node2];

	if(node1==-1 || node2==-1 || node1==node2){
		cout<<"error in node1=-1 || node2=-1"<<endl;
		for(int i=0;i<S->Nsel;i++) cout<<S->perm[i]<<" ";
		cout<<endl;
		exit(-2);
	}

	if(S->position[node1]<S->position[node2]) {
		delta_dis=ope_real_lk2opt(S,node1,node2, delta_profit);
		S->cdis=S->cdis+delta_dis;
	}
	else if(S->position[node1]>S->position[node2]) {
		delta_dis= ope_real_lk2opt(S,node2,node1, delta_profit);
		S->cdis=S->cdis+delta_dis;
	}
	else {
		cout<<"error in equal city number in LK"<<endl;
		exit(-2);
	}

//	return delta_profit;		//key to switch

	return delta_dis;

}

int ope_real_lk2opt(Struc_Sol *S, int smaposnode, int bigposnode, int &delta_profit){

	int E1pre=smaposnode;
//	int E1nex=S->nextC[smaposnode];
	int pos1=S->position[smaposnode];
//	int E2pre=bigposnode;
//	int E2nex=S->nextC[bigposnode];
	int E2nex=locate_nextcity(S,bigposnode);
	int pos2=S->position[bigposnode];
	int Before_swap=0, After_swap=0;
	int length=pos2-pos1+2;
	int arr_v[length];
	int count=0;
	int i;

//	double st_ime=get_time();
	/*get the delta_cdis*/
	count=0;
	for( i=pos1;i<=pos2+1;i++) {
		arr_v[count]=S->perm[i];
		count++;
	}
	Before_swap=get_e2e_cdis_general(arr_v,length,pos1,S->Nsel);

	arr_v[0]=E1pre;
	count=1;
	for( i=pos2;i>pos1;i--) {
		arr_v[count]=S->perm[i];
		count++;
	}
	arr_v[length-1]=E2nex;
	After_swap=get_e2e_cdis_general(arr_v,length,pos1,S->Nsel);

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

	/*delta_profit*/
	/*to check the cdis and the profit*/
//	int all_v[num_city+1];
//	int temp_profit;
//	memset(all_v,-1,(num_city+1)*sizeof(int));
//
//	for(int i=0;i<num_city+1;i++) all_v[i]=S->perm[i];
//	for(int i=0;i<length;i++) all_v[i+pos1]=arr_v[i];
//	temp_profit=getfitness_vec(all_v);
//
//	delta_profit= S->profit-temp_profit;

	/*the f_city and Nsel are not changed and the profit is updated outside*/

	return After_swap-Before_swap;
}



/*
 * Shake.cpp
 *
 *  Created on: 28Step2020
 *      Author: JT
 *      Provide a perturbation to shake the solution
 */

#include "trpp_lib.h"
#include "Detail_TRPP.h"
#include "Ope_interchange.h"
#include "Ope_insert.h"
#include "Ope_swap.h"
#include "Ope_drop.h"
#include "Ope_add.h"

//export function
int shake(Struc_Sol *S);
int nosoul_shake(Struc_Sol *S);

//local function

int shake_interx(Struc_Sol *S);
int shake_insert(Struc_Sol *S);
int shake_adjswp(Struc_Sol *S);
int shake_ranswp(Struc_Sol *S);
int shake_delicate_insert(Struc_Sol *S);
int shake_focus_insert(Struc_Sol *S);
int shake_more_focus_insert(Struc_Sol *S);
int shake_random_drop(Struc_Sol *S);
int shake_random_add(Struc_Sol *S);

int special_adding(Struc_Sol *S, int unv);

bool step_shake(Struc_Sol *S, int unv);
int choose_victim(Struc_Sol *S, int *selcity, int unv);
int choose_objnode(Struc_Sol *S, int *sel_c, int asknode, int count_c);

int ope_delta_insert_copie(Struc_Sol *S, int asknode, int objnode);
int ope_delta_swapad_sj(Struc_Sol *S, int node1, int node2);
int ope_delta_swap_sj(Struc_Sol *S, int node1, int node2);

/*random get city from the selected city+ candidate set*/

int random_get_city_CS(Struc_Sol *S, int *sel_city, int count, int objcity);

/*main function*/

int shake(Struc_Sol *S){

//	int num_i=rand()%3;
//	if(num_i==0) shake_interx(S);
//	else if(num_i==1) shake_more_focus_insert(S);
//	else shake_random_add(S);


//	for(int i=0;i<5;i++)
//	shake_interx(S);

//	shake_ranswp(S);
//	shake_adjswp(S);
//	shake_insert(S);
//	shake_delicate_insert(S);
//	shake_focus_insert(S);

	shake_more_focus_insert(S);

//	shake_random_drop(S);
	shake_random_add(S);


	return 9;

}

int nosoul_shake(Struc_Sol *S){
	int maxstep=6;
	int nfrom_out=-1;
	int nfrom_ins=-1;
	int ind_in;
	int ind_out;
	int count_in=0;
	int count_out=0;


	while(maxstep>0){

		if(rand()%2==0){
			shake_insert(S);
		}
		else{
			if(S->Nsel==num_city) continue;	//full of nodes in the solution

			ind_in=rand()%(S->Nsel-1)+1;
			ind_out=rand()%(num_city-S->Nsel);
			count_in=0;
			count_out=0;
			nfrom_out=-1;
			nfrom_ins=-1;

			for(int i=0;i<num_city;i++){
				if(nfrom_ins!=-1 && nfrom_out!=-1) break;
				if(S->f_city[i]==1){
					if(count_in==ind_in) nfrom_ins=i;
					else count_in++;
				}
				else{
					if(count_out==ind_out) nfrom_out=i;
					else count_out++;
				}
			}

			if(nfrom_ins==-1 || nfrom_out==-1) {
				cout<<"error in nosoul_shake"<<endl;
				exit(-2);
			}

			if(rand()%2==0)	{
//				perm2sd(S);
				ope_delta_add(S, nfrom_out, S->position[nfrom_ins]);
			}
			else ope_delta_interchange(S,nfrom_ins,nfrom_out);
		}


		maxstep--;
	}


	return 9;
}

int shake_interx(Struc_Sol *S){
	int unvcity[num_city-S->Nsel];
	int count=0;
	int max_step=min(perturb1,num_city-S->Nsel);
//	int max_step=min(2,num_city-S->Nsel);

	for(int i=0;i<num_city;i++)
		if(S->f_city[i]==0) {
			unvcity[count]=i;
			count++;
		}
	ExcGene(unvcity,num_city-S->Nsel);

	for(int i=0;i<max_step;i++) step_shake(S,unvcity[i]);

	return 91;
}

int shake_insert(Struc_Sol *S){
	int sel_city[S->Nsel-1];
	int length=S->Nsel-1;
	int max_step=1;
	int count=0;
	int mov_city;
	int obj_city;

//	for(int i=0;i<num_city;i++) cout<<S->f_city[i]<<" ";
//	cout<<endl;

	for(int i=0;i<num_city;i++)
		if(S->f_city[i]==1 && i!=0) {
			sel_city[count]=i;
			count++;
		}
	ExcGene(sel_city,S->Nsel-1);

	for(int i=0;i<max_step;i++){
		mov_city=sel_city[rand()%length];

		do{
			obj_city=sel_city[rand()%length];
		}while(obj_city==mov_city);
		ope_delta_insert_copie(S,mov_city,obj_city);

	}

	return 92;
}

int shake_adjswp(Struc_Sol *S){

	int i;
	int node1,node2;
	double threshold=20*1.0/(S->Nsel-1);
//	int cct=0;

	for(i=1;i<S->Nsel-1;i++){
		node1=S->perm[i];
		node2=S->perm[i+1];
		if(rand()*1.0/RAND_MAX<threshold) {
			ope_delta_swapad_sj(S,  node1,  node2);
//			cct++;
		}

	}
//	cout<<cct<<" "<<endl;

	return 93;
}

int shake_ranswp(Struc_Sol *S){
	int max_step=1000;
	double cdis_excee=0.01*S->cdis;
	int length=S->Nsel-1;
	int node1,node2;
	int count=0;


	for(int i=0;i<max_step;i++){
		node1=S->perm[rand()%length+1];
		node2=S->perm[rand()%length+1];
		if(node1==node2) continue;
		if(abs(S->position[node1]-S->position[node2])==1){
			if(get_delta_swapad(S,node1,node2)<cdis_excee) {
				ope_delta_swapad_sj(S, node1, node2);
				count++;
			}
		}
		else {
			if(get_delta_swap(S,node1,node2)<cdis_excee) {
				ope_delta_swap_sj(S, node1, node2);
				count++;
			}
		}

	}
	getfitness2(S);
//	cout<<"random threshold swap is operated "<<count<<" times"<<endl;
	return 94;
}

int shake_delicate_insert(Struc_Sol *S){

	int max_step=0.2*num_city;
	int i;
	int sel_c[num_city];
	int count=0;
	int asknode, objnode;
	int real_c=0;

	for(i=0;i<num_city;i++)
		if(S->f_city[i]==1 && i!=0){
			sel_c[count]=i;
			count++;
		}
	for(i=0;i<num_city;i++){
		asknode=sel_c[rand()%count];
		objnode=choose_objnode(S,sel_c,asknode, count);
		if(objnode==-1 || abs(S->position[asknode]-S->position[objnode])<=1) continue;
		ope_delta_insert_copie(S,asknode,objnode);
		real_c++;
		if(real_c>max_step) break;
	}

//	cout<<real_c<<endl;
	return 95;
}

int shake_focus_insert(Struc_Sol *S){
	int sel_city[S->Nsel-1];
	int length=S->Nsel-1;
	int max_step=2;
	int count=0;
	int mov_city;
	int obj_city;

	int city_god[num_city];
	int count_g=0;


	int accdis=0;
	int nowc,nexc;

	for(int i=0;i<S->Nsel;i++){
		nowc=S->perm[i];
		nexc=S->perm[i+1];
		if(nexc==-1) break;
		accdis=accdis+matrix_dis[nowc][nexc];
		if(profitA[nexc]-accdis<=0) {
			city_god[count_g]=nexc;
			count_g++;
		}
	}

	/*********************************************/

	for(int i=0;i<num_city;i++)
		if(S->f_city[i]==1 && i!=0) {
			sel_city[count]=i;
			count++;
		}
	ExcGene(sel_city,S->Nsel-1);

	/*********************************************/
	if(count_g==0){

		for(int i=0;i<max_step;i++){
			mov_city=sel_city[rand()%length];

			do{
				obj_city=sel_city[rand()%length];
			}while(obj_city==mov_city);
			ope_delta_insert_copie(S,mov_city,obj_city);

		}
	}
	/*********************************************/
	else{
		for(int i=0;i<count_g;i++){
			mov_city=city_god[i];

			do{
				obj_city=sel_city[rand()%length];
			}while(obj_city==mov_city);
			ope_delta_insert_copie(S,mov_city,obj_city);

		}
	}

	return 96;
}


int shake_more_focus_insert(Struc_Sol *S){
	int sel_city[S->Nsel-1];
	int length=S->Nsel-1;
	int max_step=perturb1;
	int count=0;
	int mov_city;
	int obj_city;

	int city_god[num_city];
	int count_g=0;


	int accdis=0;
	int nowc,nexc;



	/*New form*/
	for(int i=0;i<num_city;i++)
		if(S->f_city[i]==1 && i!=0) {
			sel_city[count]=i;
			count++;
		}
	ExcGene(sel_city,S->Nsel-1);
	/*random insert*/
	for(int i=0;i<max_step;i++){
		mov_city=sel_city[rand()%length];

		do{
			obj_city=sel_city[rand()%length];
		}while(obj_city==mov_city);
		ope_delta_insert_copie(S,mov_city,obj_city);
	}
	return 97;
	/*focus insert+ random choose from the candidate set*/

	for(int i=0;i<count_g;i++){
		mov_city=sel_city[rand()%length];

		obj_city=random_get_city_CS(S,sel_city,count,mov_city);

//		do{
//			obj_city=sel_city[rand()%length];
//		}while(obj_city==mov_city);
		ope_delta_insert_copie(S,mov_city,obj_city);
	}
	return 97;
	/*previous form*/
	/*********************************************/

	for(int i=0;i<S->Nsel;i++){
		nowc=S->perm[i];
		nexc=S->perm[i+1];
		if(nexc==-1) break;
		accdis=accdis+matrix_dis[nowc][nexc];
		if(profitA[nexc]-accdis<=0) {
			city_god[count_g]=nexc;
			count_g++;
		}
	}

	for(int i=0;i<num_city;i++)
		if(S->f_city[i]==1 && i!=0) {
			sel_city[count]=i;
			count++;
		}
	ExcGene(sel_city,S->Nsel-1);

	/*********************************************/
	if(count_g==0){

		for(int i=0;i<max_step;i++){
			mov_city=sel_city[rand()%length];

			do{
				obj_city=sel_city[rand()%length];
			}while(obj_city==mov_city);
			ope_delta_insert_copie(S,mov_city,obj_city);

		}
	}
	/*********************************************/
	else{
		for(int i=0;i<count_g;i++){
			mov_city=city_god[i];

			do{
				obj_city=sel_city[rand()%length];
				obj_city=choose_objnode(S,sel_city,mov_city, count);
			}while(obj_city==mov_city);
			ope_delta_insert_copie(S,mov_city,obj_city);

		}
	}

	return 97;
}

int shake_random_drop(Struc_Sol *S){
	int max_time;
	int set_num=50;
	int i;
	int ind_drop=0;

	max_time=min(set_num,S->Nsel-1);

	for(i=0;i<max_time;i++){
		ind_drop=rand()%(S->Nsel-1)+1;
		ope_delta_drop(S,ind_drop);
	}

	return 98;
}


int shake_random_add(Struc_Sol *S){
	int max_time;
	int set_num;
//	int set_num=perturb3;
//	int set_num=15;
	int i;
	int nodeadd=0;
	int posadd;
	int count=0;
	int vec_free[num_city-S->Nsel];

	perm2sd(S);

	/********************adding to special position*****************/

//	cout<<"Before shake:\t"<<S->cdis<<endl;
	count=0;
	for(i=0;i<num_city;i++){
		if(S->f_city[i]==0) {
			vec_free[count]=i;
			count++;
		}
	}
	if(count!=num_city-S->Nsel){
		cout<<"error in count of unsel"<<endl;
		exit(-1);
	}

	ExcGene(vec_free,num_city-S->Nsel);

	max_time=min(num_city-S->Nsel,perturb2);

	for(i=0;i<max_time;i++){
		nodeadd=vec_free[i];
		special_adding(S,nodeadd);
	}

	return 99;
//	/********************adding to random position*****************/

//	cout<<"Before shake:\t"<<S->cdis<<endl;
//	count=0;
//	for(i=0;i<num_city;i++){
//		if(S->f_city[i]==0) {
//			vec_free[count]=i;
//			count++;
//		}
//	}
//	if(count!=num_city-S->Nsel) {
//		cout<<"error in count of unsel"<<endl;
//		exit(-1);
//	}
//
//	ExcGene(vec_free,num_city-S->Nsel);
//
//	max_time=min(num_city-S->Nsel,perturb4);
//
//	for(i=0;i<2;i++){
//		nodeadd=vec_free[i];
//		posadd=rand()%(S->Nsel-1)+1;
//		ope_delta_add(S, nodeadd, posadd);
//	}

	/********************adding to end****************************/
//	count=0;
//	for(i=0;i<num_city;i++){
//		if(S->f_city[i]==0) {
//			vec_free[count]=i;
//			count++;
//		}
//	}
//	if(count!=num_city-S->Nsel) {
//		cout<<"error in count of unsel"<<endl;
//		exit(-1);
//	}
//
//	ExcGene(vec_free,num_city-S->Nsel);
//
//	max_time=min(num_city-S->Nsel,perturb3);
//
//	for(i=0;i<max_time;i++){
//		nodeadd=vec_free[i];
//		ope_delta_add(S, nodeadd, S->Nsel);
//	}
//
//	return 99;
//	/********************adding to random position*****************/

//	cout<<"Before shake:\t"<<S->cdis<<endl;

//	for(i=0;i<num_city;i++){
//		if(S->f_city[i]==0) {
//			vec_free[count]=i;
//			count++;
//		}
//	}
//	if(count!=num_city-S->Nsel) {
//		cout<<"error in count of unsel"<<endl;
//		exit(-1);
//	}
//
//	ExcGene(vec_free,num_city-S->Nsel);
//
//	max_time=min(num_city-S->Nsel,set_num);
//
//	for(i=0;i<max_time;i++){
//		nodeadd=vec_free[i];
//		posadd=rand()%(S->Nsel-1)+1;
//		ope_delta_add(S, nodeadd, posadd);
//	}

	/********************adding to special position*****************/

//	cout<<"Before shake:\t"<<S->cdis<<endl;
	count=0;
	for(i=0;i<num_city;i++){
		if(S->f_city[i]==0) {
			vec_free[count]=i;
			count++;
		}
	}
	if(count!=num_city-S->Nsel) {
		cout<<"error in count of unsel"<<endl;
		exit(-1);
	}

	ExcGene(vec_free,num_city-S->Nsel);

	max_time=min(num_city-S->Nsel,set_num);

	for(i=0;i<max_time;i++){
		nodeadd=vec_free[i];
		special_adding(S,nodeadd);
	}

//	cout<<"After shake:\t"<<S->cdis<<endl;
	return 99;
}

int special_adding(Struc_Sol *S, int unv){
	int selcity[num_city];
	int count=0;
	int temp_city;
	int vic_city=-1;

	for(int i=0;i<Num_candidate_per_city[unv];i++){
		temp_city=Candidate_set[unv][i];
		if(S->f_city[temp_city]==0) continue;
		selcity[count]=temp_city;
		count++;
	}
	if(count==0) return 0;
	vic_city=selcity[rand()%count];
//	vic_city=choose_victim(S,selcity,unv);
	if(vic_city!=-1) {
		ope_delta_add(S,unv,S->position[vic_city]);
//		ope_delta_interchange(S,vic_city,unv);
		return 1;
	}
	return 0;
}

/***************************************************************************************/

bool step_shake(Struc_Sol *S, int unv){
	int selcity[S->Nsel-1];
	int count=0;
	int vic_city=-1;

	for(int i=0;i<num_city;i++)
		if(S->f_city[i]==1 && i!=0) {		//exclude the depot city
			selcity[count]=i;
			count++;
		}
	vic_city=choose_victim(S,selcity,unv);
	if(vic_city!=-1) {
		ope_delta_interchange(S,vic_city,unv);
		return true;
	}
	return false;
}

int choose_victim(Struc_Sol *S, int *selcity, int unv){



//	int length=S->Nsel-1;
//	int Pro_int[length];
//	int total_times=0;
//	int rand_I=rand()%1000;
//
//	for(int i=0;i<length;i++) total_times=total_times+Matrix_chosenT[unv][selcity[i]];
//
//	if(total_times==0) return -1;
//	Pro_int[0]=(int)(1000*(total_times-Matrix_chosenT[unv][selcity[0]])/(total_times*(length-1)));
//	for(int i=1;i<length-1;i++)
//		Pro_int[i]=Pro_int[i-1]+(int)(1000*(total_times-Matrix_chosenT[unv][selcity[i]])/(total_times*(length-1)));
//	Pro_int[length-1]=1000;
//
//	for(int i=0;i<length;i++)
//		if(rand_I<Pro_int[i]) return selcity[i];

	return -1;
}

int choose_objnode(Struc_Sol *S, int *sel_c, int asknode, int count_c){

	int Pro_int[count_c];
	int total_times=0;
	int rand_I=rand()%1000;

	for(int i=0;i<count_c;i++) total_times=total_times+Matrix_chosenT[asknode][sel_c[i]];

	if(total_times==0) return -1;
	Pro_int[0]=(int)(1000*(total_times-Matrix_chosenT[asknode][sel_c[0]])/(total_times*(count_c-1)));
	for(int i=1;i<count_c-1;i++)
		Pro_int[i]=Pro_int[i-1]+(int)(1000*(total_times-Matrix_chosenT[asknode][sel_c[i]])/(total_times*(count_c-1)));
	Pro_int[count_c-1]=1000;

	for(int i=0;i<count_c;i++)
		if(rand_I<Pro_int[i]) return sel_c[i];

	return -1;
	/********************************choose from the candidate set***************************************************/

//	int cc_can_sel=0;
//	for(int i=0;i<Num_candidate_per_city[asknode];i++)
//		if(S->f_city[Candidate_set[asknode][i]]==1) cc_can_sel++;
//	if(cc_can_sel==0) return -1;
//
//	int ssel_c[cc_can_sel];
//
//	cc_can_sel=0;
//	for(int i=0;i<Num_candidate_per_city[asknode];i++)
//		if(S->f_city[Candidate_set[asknode][i]]==1) {
//			ssel_c[cc_can_sel]=Candidate_set[asknode][i];
//			cc_can_sel++;
//		}
//
//	int Pro_int[cc_can_sel];
//	int total_times=0;
//	int rand_I=rand()%1000;
//
//	for(int i=0;i<cc_can_sel;i++) total_times=total_times+Matrix_chosenT[asknode][ssel_c[i]];
//
//	if(total_times==0) return -1;
//	Pro_int[0]=(int)(1000*(total_times-Matrix_chosenT[asknode][ssel_c[0]])/(total_times*(cc_can_sel-1)));
//	for(int i=1;i<cc_can_sel-1;i++)
//		Pro_int[i]=Pro_int[i-1]+(int)(1000*(total_times-Matrix_chosenT[asknode][ssel_c[i]])/(total_times*(cc_can_sel-1)));
//	Pro_int[cc_can_sel-1]=1000;
//
//	for(int i=0;i<cc_can_sel;i++)
//		if(rand_I<Pro_int[i]) return ssel_c[i];
//
//	return -1;


}

int ope_delta_insert_copie(Struc_Sol *S, int asknode, int objnode){
	int Before_insert=0;
	int After_insert=0;
	int askpos= S->position[asknode];
	int objpos= S->position[objnode];
	int length=abs(askpos-objpos)+3;
	int arr_v[length];
	int count=0;


	if(objpos==0) {
		cout<<"error in the depot in insert operator"<<endl;
		exit(-2);
		//depot position is not taken into consideration
	}

	if(objpos<askpos){
		count=0;
		for(int i=0;i<length;i++){
			arr_v[count]=S->perm[i+objpos-1];
			count++;
		}
		Before_insert=get_e2e_cdis_general(arr_v,length,objpos-1,S->Nsel);

		arr_v[0]=S->perm[objpos-1];
		arr_v[1]=asknode;
		count=2;
		for(int i=0;i<askpos-objpos;i++){
			arr_v[count]=S->perm[objpos+i];
			count++;
		}
		After_insert=get_e2e_cdis_general(arr_v,length,objpos-1,S->Nsel);
	}

	else if(objpos>askpos){
		count=0;
		for(int i=0;i<length;i++){
			arr_v[count]=S->perm[askpos-1+i];
			count++;
		}
		Before_insert=get_e2e_cdis_general(arr_v, length, askpos-1, S->Nsel);

		arr_v[0]=S->perm[askpos-1];
		count=1;
		for(int i=0;i<objpos-askpos;i++) {
			arr_v[count]=S->perm[askpos+1+i];
			count++;
		}
		arr_v[length-2]=asknode;
		arr_v[length-1]=S->perm[objpos+1];
		After_insert=get_e2e_cdis_general(arr_v, length, askpos-1, S->Nsel);
	}


	/*update the weight*/
//	double rate = Beta * (pow(CONST_E, 1.0*abs(After_insert-Before_insert)/(1.0*S->cdis)) - 1);

	/*update the cdis*/
	S->cdis=S->cdis+After_insert-Before_insert;
	/*update the perm*/
	int begpos=-1, endpos=-1;
	if(objpos<askpos) {
		begpos=objpos-1;
		endpos=askpos+1;
	}
	else{
		begpos=askpos-1;
		endpos=objpos+1;
	}
	for(int i=0;i<length;i++) S->perm[begpos+i]=arr_v[i];
	/*update the pos*/
	for(int i=begpos;i<=endpos;i++){
		if(S->perm[i]==-1) break;
		S->position[S->perm[i]]=i;
	}
	/*update the ll: NextC, prevN*/
//	int pren=-1, nown=-1;
//	for(int i=begpos+1;i<=endpos;i++){
//		pren=S->perm[i-1];
//		nown=S->perm[i];
//		S->nextC[pren]=nown;
//		if(nown!=-1) S->prevC[nown]=pren;
//	}

	/*f_city, Nsel are not changed and the profit is updated outside*/
//	/*update the chosen Times and weight*/
//
//
//	Total_sim++;
//	update_chosenT(asknode,S->perm[objpos-1]);
//	update_chosenT(asknode,S->perm[objpos+1]);
//	update_weight(asknode,S->perm[objpos-1], rate);
//	update_weight(asknode,S->perm[objpos+1], rate);
//
//	if(askpos<objpos) {
//		update_chosenT(S->perm[askpos-1],S->perm[askpos]);
//		update_weight (S->perm[askpos-1],S->perm[askpos],rate);
//	}
//	else {
//		update_chosenT(S->perm[askpos],S->perm[askpos+1]);
//		update_weight(S->perm[askpos],S->perm[askpos+1], rate);
//	}
	/*******************************/
	return 0;
}

int ope_delta_swapad_sj(Struc_Sol *S, int node1, int node2){
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

//	/*update the matrix_chosentimes*/
//	update_chosenT(node1,npre2);
//	update_chosenT(node1,nnex2);
//	update_chosenT(node2,npre1);
//	update_chosenT(node2,nnex1);
//
//	update_weight(node1,npre2,rate);
//	update_weight(node1,nnex2,rate);
//	update_weight(node2,npre1,rate);
//	update_weight(node2,nnex1,rate);

	/*Nsel, f_city are not changed and profit is updated in the end*/
	return 0;
}

int ope_delta_swap_sj(Struc_Sol *S, int node1, int node2){

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
//	update_chosenT(node1,npre2);
//	update_chosenT(node1,nnex2);
//	update_chosenT(node2,npre1);
//	update_chosenT(node2,nnex1);
//
//	update_weight(node1,npre2, rate);
//	update_weight(node1,nnex2, rate);
//	update_weight(node2,npre1, rate);
//	update_weight(node2,nnex1, rate);

	/*Nsel, f_city are not changed and profit is updated in the end*/
	return 0;
}

int random_get_city_CS(Struc_Sol *S,int *sel_city, int count, int objcity){
	int selcity=-1;
	int canC[num_city];
	int cnt=0;
	int tempc=-1;

	for(int i=0;i<Num_candidate_per_city[objcity];i++){
		tempc=Candidate_set[objcity][i];
		if(S->f_city[tempc]){
			canC[cnt]=tempc;
			cnt++;
		}
	}

	if(cnt!=0){
		selcity=canC[rand()%cnt];
	}
	else{
		do{
			selcity=sel_city[rand()%count];
		}while(selcity==objcity);
	}
	return selcity;
}

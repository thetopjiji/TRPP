/*
 * Ope_multiinsert.cpp
 *
 *  Created on: 28Step2020
 *      Author: JT
 *      the candidate set is not changed, just change the order
 *      multiple insert, take some sequence nodes as a cluster
 */

#include "trpp_lib.h"
#include "Detail_TRPP.h"

//export function
int Ope_multiinsert(Struc_Sol *S, int &ite);
int ins_BwI(Struc_Sol *S);
int ins_FwI(Struc_Sol *S);
//local function

int multiinsert_compatible(Struc_Sol *S);

int multiinsert_fix(Struc_Sol *S);
int multiinsert_inner(Struc_Sol *S);
int multiinsert_allsearch(Struc_Sol *S);

int get_delta_multiinsert(Struc_Sol *S, int sizepat, int mov_beg, int ins_pos);
int ope_delta_multiinsert(Struc_Sol *S, int sizepat, int mov_beg, int ins_pos);

//add city into the candidate set

int Ope_multiinsert(Struc_Sol *S, int &ite){


	perm2sd(S);
	verify_cdis(S);
	verify_Vsd(S);
//	int i,j;
//	for(int i=1;i<S->Nsel;i++)
//		for(int j=1;j<S->Nsel;j++)
//			get_delta_multiinsert(S,3,i,j);

//	multiinsert_old(S);
//	multiinsert_all(S);
//	multiinsert_compatible(S);
	multiinsert_fix(S);
//	multiinsert_inner(S);
//	multiinsert_allsearch(S);
	return 6;
}

int ins_BwI(Struc_Sol *S){

	perm2sd(S);
//	verify_cdis(S);
//	verify_Vsd(S);

	bool Fimp=false;
	bool Fbreak=false;
	int i,j;
	int cdis;
	int nodefix, nodemov;
	int mov_beg, ins_pos;



	do{
		Fimp=false;
		for(i=S->Nsel-1;i>0;i--){
			/*patsize=1*/
			Fbreak=false;

			nodemov=S->perm[i];
			for(j=i-2;j>0;j--){
				nodefix=S->perm[j];
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis=get_delta_multiinsert(S,1,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis<0) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,1,mov_beg,ins_pos);
					getfitness2(S);
					UpdateBestS(S,61);
					Fimp=true;
					Fbreak=true;
					break;
				}
			}
			if(Fbreak) continue;
			/*patsize=2*/
			for(j=i-2;j>0;j--){
				nodefix=S->perm[j];
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis=get_delta_multiinsert(S,2,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis<0) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,2,mov_beg,ins_pos);
					getfitness2(S);
					UpdateBestS(S,61);
					Fimp=true;
					Fbreak=true;
					break;
				}
			}
			if(Fbreak) continue;
			/*patsize=3*/
			for(j=i-2;j>0;j--){
				nodefix=S->perm[j];
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis=get_delta_multiinsert(S,3,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis<0) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,3,mov_beg,ins_pos);
					getfitness2(S);
					UpdateBestS(S,61);
					Fimp=true;
					Fbreak=true;
					break;
				}
			}
		}

	}while(Fimp);

	return 61;
}


int ins_FwI(Struc_Sol *S){

	perm2sd(S);
//	verify_cdis(S);
//	verify_Vsd(S);


	bool Fimp=false;
	bool Fbreak=false;
	int i,j;
	int cdis;
	int nodefix, nodemov;
	int mov_beg, ins_pos;



	do{
		Fimp=false;
		for(i=1;i<S->Nsel;i++){
			/*patsize=1*/
			Fbreak=false;

			nodemov=S->perm[i];
			for(j=i+2;j<S->Nsel;j++){
				nodefix=S->perm[j];
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis=get_delta_multiinsert(S,1,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis<0) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,1,mov_beg,ins_pos);
					getfitness2(S);
					UpdateBestS(S,62);
					Fimp=true;
					Fbreak=true;
					break;
				}
			}
			if(Fbreak) continue;
			/*patsize=2*/
			for(j=i+2;j<S->Nsel;j++){
				nodefix=S->perm[j];
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis=get_delta_multiinsert(S,2,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis<0) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,2,mov_beg,ins_pos);
					getfitness2(S);
					UpdateBestS(S,62);
					Fimp=true;
					Fbreak=true;
					break;
				}
			}
			if(Fbreak) continue;
			/*patsize=3*/
			for(j=i+2;j<S->Nsel;j++){
				nodefix=S->perm[j];
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis=get_delta_multiinsert(S,3,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis<0) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,3,mov_beg,ins_pos);
					getfitness2(S);
					UpdateBestS(S,62);
					Fimp=true;
					Fbreak=true;
					break;
				}
			}
		}

	}while(Fimp);


	return 62;
}

int multiinsert_compatible(Struc_Sol *S){
	int i,j;

	int mov_beg, ins_pos;		// mov_beg is counted from 1
	int nodemov, nodefix;
	int cdis2=-1,cdis3=-1;

	for(i=1;i<num_city;i++){

		if(S->f_city[i]==0) continue;
		nodefix=i;
		for(j=0;j<Num_candidate_per_city[nodefix];j++){
			nodemov=Candidate_set[nodefix][j];
			if(S->f_city[nodemov]==0) continue;
			mov_beg=S->position[nodemov];
			ins_pos=S->position[nodefix];
			cdis2=get_delta_multiinsert(S,2,mov_beg,ins_pos);
			cdis3=get_delta_multiinsert(S,3,mov_beg,ins_pos);
			if(check_time()) return 1;
			if(cdis2<0 && cdis2<=cdis3) {						// if cdis<0, which means it is get "improved"
				ope_delta_multiinsert(S,2,mov_beg,ins_pos);
				/***************/
				verify_ll(S);
				/****************/
				getfitness2(S);
				UpdateBestS(S,5);
				break;
			}
			else if(cdis3<0 && cdis2<cdis3){
				ope_delta_multiinsert(S,3,mov_beg,ins_pos);
				/***************/
//				verify_SbSe(S);
//				verify_Vsd(S);
//				verify_cdis(S);
				/****************/
				getfitness2(S);
				UpdateBestS(S,5);
				break;
			}
		}
	}

	return 0;
}

int multiinsert_fix(Struc_Sol *S){
	int i,j;

	int mov_beg, ins_pos;		// mov_beg is counted from 1
	int nodemov, nodefix;
	int cdis2=-1,cdis3=-1;
	bool Fimp=false;

	do{
		Fimp=false;
		for(i=1;i<num_city;i++){

			if(S->f_city[i]==0) continue;
			nodefix=i;
			for(j=0;j<Num_candidate_per_city[nodefix];j++){
				nodemov=Candidate_set[nodefix][j];
				if(S->f_city[nodemov]==0) continue;
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis2=get_delta_multiinsert(S,2,mov_beg,ins_pos);
				cdis3=get_delta_multiinsert(S,3,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis2<0 && cdis2<=cdis3) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,2,mov_beg,ins_pos);
//					/***************/
//					verify_ll(S);
//					/****************/
					getfitness2(S);
					UpdateBestS(S,5);
					Fimp=true;
					break;
				}
				else if(cdis3<0 && cdis2<cdis3){
					ope_delta_multiinsert(S,3,mov_beg,ins_pos);

					getfitness2(S);
					UpdateBestS(S,5);
					Fimp=true;
					break;
				}
			}
		}
	}while(Fimp);
	return 0;
}

int multiinsert_inner(Struc_Sol *S){
	int i,j;

	int mov_beg, ins_pos;		// mov_beg is counted from 1
	int nodemov, nodefix;
	int cdis2=-1,cdis3=-1;
	bool Fimp=false;

	do{
		Fimp=false;
		for(i=1;i<num_city;i++){
			if(S->f_city[i]==0) continue;
			nodefix=i;
			for(j=0;j<Num_candidate_per_city[nodefix];j++){
				nodemov=Candidate_set[nodefix][j];
				if(S->f_city[nodemov]==0) continue;
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis2=get_delta_multiinsert(S,2,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis2<0) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,2,mov_beg,ins_pos);
					/***************/
					verify_ll(S);
					/****************/
					getfitness2(S);
					UpdateBestS(S,5);
					Fimp=true;
					break;
				}
			}
		}

		for(i=1;i<num_city;i++){
			if(S->f_city[i]==0) continue;
			nodefix=i;
			for(j=0;j<Num_candidate_per_city[nodefix];j++){
				nodemov=Candidate_set[nodefix][j];
				if(S->f_city[nodemov]==0) continue;
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis3=get_delta_multiinsert(S,3,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis3<0){
					ope_delta_multiinsert(S,3,mov_beg,ins_pos);
					getfitness2(S);
					UpdateBestS(S,5);
					Fimp=true;
					break;
				}
			}
		}
	}while(Fimp);
	return 0;
}

int multiinsert_allsearch(Struc_Sol *S){
	int i,j;

	int mov_beg, ins_pos;		// mov_beg is counted from 1
	int nodemov, nodefix;
	int cdis2=-1,cdis3=-1;
	bool Fimp=false;

	do{
		Fimp=false;
		for(i=1;i<S->Nsel;i++){
			nodemov=S->perm[i];
//			nodefix=i;
			for(j=1;j<S->Nsel;j++){
				nodefix=S->perm[i];
//				nodemov=Candidate_set[nodefix][j];
//				if(S->f_city[nodemov]==0) continue;
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis2=get_delta_multiinsert(S,2,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis2<0) {						// if cdis<0, which means it is get "improved"
					ope_delta_multiinsert(S,2,mov_beg,ins_pos);
					/***************/
					verify_ll(S);
					/****************/
					getfitness2(S);
					UpdateBestS(S,5);
					Fimp=true;
					break;
				}
			}
		}

		for(i=1;i<S->Nsel;i++){
			nodemov=S->perm[i];
//			if(S->f_city[i]==0) continue;
//			nodefix=i;
			for(j=1;j<S->Nsel;j++){
				nodefix=S->perm[j];
//				nodemov=Candidate_set[nodefix][j];
//				if(S->f_city[nodemov]==0) continue;
				mov_beg=S->position[nodemov];
				ins_pos=S->position[nodefix];
				cdis3=get_delta_multiinsert(S,3,mov_beg,ins_pos);
				if(check_time()) return 1;
				if(cdis3<0){
					ope_delta_multiinsert(S,3,mov_beg,ins_pos);
					getfitness2(S);
					UpdateBestS(S,5);
					Fimp=true;
					break;
				}
			}
		}
	}while(Fimp);
	return 0;
}



int get_delta_multiinsert(Struc_Sol *S, int sizepat, int mov_beg, int ins_pos){

	if(ins_pos<1){
		cout<<"error in multiinsert"<<endl;
		exit(-6);
	}

	if(ins_pos>=mov_beg && ins_pos<mov_beg+sizepat) return 100000; 	//ins_pos should not be in the range of moving pattern
	if(mov_beg+sizepat-1>S->Nsel-1) return 100000; 					//not enough nodes to clustered in a pattern
	if(abs(mov_beg-ins_pos)<=1) return 100000;						//ins_pos is adjacent to mov_beg
	if(abs(ins_pos-mov_beg-sizepat+1)<=1) return 100000;			//ins_pos is adjacent to mov_beg+sizepat-1


//	/************************************************/
//	int Bef_p=-1;
//	int Aft_p=-1;
//	int startpos;
//	int alendpos;
//
//	if(mov_beg<ins_pos){
//		startpos=mov_beg-1;
//		alendpos=ins_pos+1;
//	}
//	else {
//		startpos=ins_pos-1;
//		alendpos=mov_beg+sizepat;
//	}
//	/***************************************************/
//	int length=alendpos-startpos+1;
//	int arr_v[length];
//	int count=0;
//
//	for(int i=0;i<length;i++) arr_v[i]=S->perm[i+startpos];
//	Bef_p=get_e2e_cdis_general(arr_v, length, startpos, S->Nsel);
//
//	arr_v[0]=S->perm[startpos];
//	count=1;
//	if(mov_beg<ins_pos){								// move to right, afterward
//														// ins_pos--
//		for(int i=mov_beg+sizepat;i<alendpos;i++) {
//			arr_v[count]=S->perm[i];
//			count++;
//		}
//		for(int i=mov_beg;i<mov_beg+sizepat;i++) {
//			arr_v[count]=S->perm[i];
//			count++;
//		}
//	}												// backward
//	else{												// startp-ins_node-asknode-askpat
//		arr_v[1]=S->perm[ins_pos];
//		count++;
//		for(int i=mov_beg;i<mov_beg+sizepat;i++) {
//			arr_v[count]=S->perm[i];
//			count++;
//		}
//		for(int i=ins_pos+1;i<mov_beg;i++) {
//			arr_v[count]=S->perm[i];
//			count++;
//		}
//	}
//	arr_v[length-1]=S->perm[alendpos];
//
//	Aft_p=get_e2e_cdis_general(arr_v, length, startpos, S->Nsel);
//	return Aft_p-Bef_p;
//	/***************************************************/

	int newcdis=0;

	if(mov_beg<ins_pos)	newcdis=FwI(mov_beg,ins_pos,sizepat,S->vec_sd,S->Nsel,S->perm);		//
	else newcdis=BwI(mov_beg,ins_pos,sizepat,S->vec_sd,S->Nsel,S->perm);

//	if(Aft_p-Bef_p!=newcdis){
//		if(mov_beg>ins_pos) cout<<"old_cdis="<<Aft_p-Bef_p<<"， new_cdis="<<newcdis<<" BwI"<<endl;
//		else cout<<"old_cdis="<<Aft_p-Bef_p<<"， new_cdis="<<newcdis<<" FwI"<<endl;
//	}
	return newcdis;
}

int ope_delta_multiinsert(Struc_Sol *S, int sizepat, int mov_beg, int ins_pos){

	//update the cdis
	if(ins_pos<1){
		cout<<"error in multiinsert"<<endl;
		exit(-6);
	}

	if(ins_pos>=mov_beg && ins_pos<mov_beg+sizepat) return 100000; 	//ins_pos should not be in the range of moving pattern
	if(mov_beg+sizepat-1>S->Nsel-1) return 100000; 					//not enough nodes to clustered in a pattern
	if(abs(mov_beg-ins_pos)<=1) return 100000;						//ins_pos is adjacent to mov_beg
	if(abs(ins_pos-mov_beg-sizepat+1)<=1) return 100000;				//ins_pos is adjacent to mov_beg+sizepat-1

//	int Bef_p=-1;
//	int Aft_p=-1;
	int startpos;
	int alendpos;

	if(mov_beg<ins_pos){
		startpos=mov_beg-1;
		alendpos=ins_pos+1;
	}
	else {
		startpos=ins_pos-1;
		alendpos=mov_beg+sizepat;
	}
	/***************************************************/
	int length=alendpos-startpos+1;
	int arr_v[length];
	int count=0;

	for(int i=0;i<length;i++) arr_v[i]=S->perm[i+startpos];
//	Bef_p=get_e2e_cdis_general(arr_v, length, startpos, S->Nsel);

	arr_v[0]=S->perm[startpos];
	count=1;
	if(mov_beg<ins_pos){								// move to right, afterward
														// ins_pos--
		for(int i=mov_beg+sizepat;i<alendpos;i++) {
			arr_v[count]=S->perm[i];
			count++;
		}
		for(int i=mov_beg;i<mov_beg+sizepat;i++) {
			arr_v[count]=S->perm[i];
			count++;
		}
	}												// backward
	else{												// startp-ins_node-asknode-askpat
		arr_v[1]=S->perm[ins_pos];
		count++;
		for(int i=mov_beg;i<mov_beg+sizepat;i++) {
			arr_v[count]=S->perm[i];
			count++;
		}
		for(int i=ins_pos+1;i<mov_beg;i++) {
			arr_v[count]=S->perm[i];
			count++;
		}
	}
	arr_v[length-1]=S->perm[alendpos];

//	Aft_p=get_e2e_cdis_general(arr_v, length, startpos, S->Nsel);

	/********************************/
	int newcdis=0;
	if(mov_beg<ins_pos)	newcdis=FwI(mov_beg,ins_pos,sizepat,S->vec_sd,S->Nsel,S->perm);		//
	else newcdis=BwI(mov_beg,ins_pos,sizepat,S->vec_sd,S->Nsel,S->perm);
	/********************************/

//	double rate = Beta * (pow(CONST_E, 1.0*abs(newcdis)/(1.0*S->cdis)) - 1);

	S->cdis=S->cdis+newcdis;

	//update the ll; nextC, prevC

	int pat_pre=S->perm[mov_beg-1];
	int pat_nex=S->perm[mov_beg+sizepat];
//	int tar_pre=S->perm[ins_pos-1];
	int tar_nex=S->perm[ins_pos+1];

	int beg_node=S->perm[mov_beg];
	int end_node=S->perm[mov_beg+sizepat-1];
	int tar_node=S->perm[ins_pos];

	if(mov_beg<ins_pos){	// from small to big		//forward
//		S->nextC[pat_pre]=pat_nex;
//		S->prevC[pat_nex]=pat_pre;
//		S->nextC[tar_node]=beg_node;
//		S->prevC[beg_node]=tar_node;
//		S->nextC[end_node]=tar_nex;
//		if(tar_nex!=-1) S->prevC[tar_nex]=end_node;

		/***************/
		update_chosenT(pat_pre,pat_nex);
		update_chosenT(tar_node,beg_node);
		update_chosenT(tar_nex,end_node);

//		update_weight(pat_pre,pat_nex,rate);
//		update_weight(tar_node,beg_node,rate);
//		update_weight(tar_nex,end_node,rate);
		/***************/
	}
	else{					// from big to small		//backward
//		S->nextC[pat_pre]=pat_nex;
//		if(pat_nex!=-1) S->prevC[pat_nex]=pat_pre;
////		S->nextC[tar_pre]=beg_node;
////		S->prevC[beg_node]=tar_pre;
//		S->nextC[tar_node]=beg_node;
//		S->prevC[beg_node]=tar_node;
////		S->nextC[end_node]=tar_node;
////		S->prevC[tar_node]=end_node;
//		S->nextC[end_node]=tar_nex;
//		S->prevC[tar_nex]=end_node;

		/***************/
		update_chosenT(pat_pre,pat_nex);
		update_chosenT(tar_node,beg_node);
		update_chosenT(end_node,tar_nex);

//		update_weight(pat_pre,pat_nex,rate);
//		update_weight(tar_node,beg_node,rate);
//		update_weight(end_node,tar_nex,rate);
		/***************/
	}
	//update the perm

	for(int i=0;i<length;i++) S->perm[i+startpos]=arr_v[i];

	/*update Vsd*/

	if(mov_beg<ins_pos) updateVsd(S->vec_sd,mov_beg,ins_pos, S->perm);
	else updateVsd(S->vec_sd,ins_pos,mov_beg+sizepat-1,S->perm);

	//update the position

	for(int i=startpos+1;i<=alendpos-1;i++) S->position[S->perm[i]]=i;

	//f_city, Nsel donnot need to change

	// profit is changed after this function
	return 0;
}

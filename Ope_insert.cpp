/*
 * Ope_insert.cpp
 *
 *  Created on: 23Step2020
 *      Author: JT
 *      Only change the order of the cities, not change the selected city set
 *      The adjacent positions are not taken into consideration
 */

#include "trpp_lib.h"
#include "Detail_TRPP.h"

//export function
int Ope_insert(Struc_Sol *S, int &ite);		//One node insert

//local function
int ope_compatible_insert(Struc_Sol *S, int &ite);
int ope_allsearch_insert(Struc_Sol *S);

int get_delta_compatible_insert(Struc_Sol *S, int fixnode, int movenode);
int ope_delta_compatible_insert(Struc_Sol *S, int fixnode, int movenode);
//insert domain

int Ope_insert(Struc_Sol *S, int &ite){

	perm2sd(S);
	verify_cdis(S);
	verify_Vsd(S);

	/****************************************/
	ope_compatible_insert(S,ite);
//	ope_allsearch_insert(S);
	return 2;
}

int ope_compatible_insert(Struc_Sol *S, int &ite){
	int i,j;
	int fixnode;
	int movenode;
	bool Fimp=false;

	do{
		Fimp=false;
		for(i=1;i<num_city;i++){
			fixnode=i;
			if(S->f_city[fixnode]==0) continue;
			for(j=0;j<Num_candidate_per_city[fixnode];j++){		//insert in only the positions of the candidate set cities
				movenode=Candidate_set[fixnode][j];
				if(S->f_city[movenode]==0) continue;

				if(abs(S->position[fixnode]-S->position[movenode])<=1) continue;		//adjacent positions

				if(fixnode==movenode) {
					cout<<"error in insert, duplicate nodes"<<endl;	// To verify the Candidate_set
					exit(-2);
				}
				if(check_time()) return 1;
				if(get_delta_compatible_insert(S,fixnode,movenode)<0){
					ope_delta_compatible_insert(S,fixnode,movenode);
					Fimp=true;

					/***************/
//					verify_SbSe(S);
//					verify_Vsd(S);
//					verify_cdis(S);
					/****************/

					getfitness2(S);
					UpdateBestS(S,4);
					break;
				}
			}
		}
	}while(Fimp);

	return 0;
}

int ope_allsearch_insert(Struc_Sol *S){
	int i,j;
	int fixnode;
	int movenode;
	bool Fimp=false;

	do{
		Fimp=false;
		for(i=1;i<S->Nsel;i++){
			fixnode=S->perm[i];
			for(j=i+2;j<S->Nsel;j++){		//insert in only the positions of the candidate set cities
				movenode=S->perm[j];

				if(abs(S->position[fixnode]-S->position[movenode])<=1) continue;		//adjacent positions

				if(check_time()) return 1;
				if(get_delta_compatible_insert(S,fixnode,movenode)<0){
					ope_delta_compatible_insert(S,fixnode,movenode);
					Fimp=true;

					getfitness2(S);
					UpdateBestS(S,4);
					break;
				}
			}
		}
	}while(Fimp);

	return 0;
}

int get_delta_compatible_insert(Struc_Sol *S, int fixnode, int movenode){
//	int Before_insert=0;
//	int After_insert=0;
	int fixpos= S->position[fixnode];
	int movpos= S->position[movenode];
//	int length=abs(fixpos-movpos)+3;
//	int arr_v[length];
//	int count=0;


	if(movpos==0) return 999999999; //depot position is not taken into consideration
									//always fixnode-movnode
	if(fixpos==movpos) return 999999999;


	/****************************************************/
//
//	if(fixpos<movpos){					// move to left, backward
//		count=0;
//		for(int i=0;i<length;i++){
//			arr_v[count]=S->perm[i+fixpos-1];
//			count++;
//		}
//		Before_insert=get_e2e_cdis_general(arr_v,length,fixpos-1,S->Nsel);
//
//		arr_v[0]=S->perm[fixpos-1];
//		arr_v[1]=fixnode;
//		arr_v[2]=movenode;
//		count=3;
//		for(int i=1;i<movpos-fixpos;i++){
//			arr_v[count]=S->perm[fixpos+i];
//			count++;
//		}
//		arr_v[length-1]=S->perm[movpos+1];
//		After_insert=get_e2e_cdis_general(arr_v,length,fixpos-1,S->Nsel);
//	}
//
//	else if(fixpos>movpos){			// move to right, afterward
//		count=0;
//		for(int i=0;i<length;i++){
//			arr_v[count]=S->perm[movpos-1+i];
//			count++;
//		}
//		Before_insert=get_e2e_cdis_general(arr_v, length, movpos-1, S->Nsel);
//
//		arr_v[0]=S->perm[movpos-1];
//		count=1;
//		for(int i=1;i<fixpos-movpos;i++) {
//			arr_v[count]=S->perm[movpos+i];
//			count++;
//		}
//		arr_v[length-3]=fixnode;
//		arr_v[length-2]=movenode;
//
//		arr_v[length-1]=S->perm[fixpos+1];
//
//		After_insert=get_e2e_cdis_general(arr_v, length, movpos-1, S->Nsel);
//	}
//	return After_insert-Before_insert;
	/*******************/
	int newcdis=0;
	if(movpos<fixpos) newcdis=FwI(movpos,fixpos,1,S->vec_sd,S->Nsel,S->perm);
	else newcdis=BwI(movpos,fixpos,1,S->vec_sd,S->Nsel,S->perm);

//	if(newcdis!=After_insert-Before_insert){
//		if(movpos>fixpos) cout<<fixpos<<" "<<movpos<<" "<<"old_cdis="<<After_insert-Before_insert<<"，new_cdis="<<newcdis<<" BwI"<<endl;
//		else cout<<fixpos<<" "<<movpos<<" "<<"old_cdis="<<After_insert-Before_insert<<"，new_cdis="<<newcdis<<" FwI"<<endl;
//	}
	return newcdis;

}

int ope_delta_compatible_insert(Struc_Sol *S, int fixnode, int movenode){

	int fixpos= S->position[fixnode];
	int movpos= S->position[movenode];
	int length=abs(fixpos-movpos)+3;
	int arr_v[length];
	int count=0;


	if(movpos==0) return 999999999; //depot position is not taken into consideration
									//always fixnode-movnode
	if(fixpos==movpos) return 999999999;

	if(fixpos<movpos){					// move to left, backward

		arr_v[0]=S->perm[fixpos-1];
		arr_v[1]=fixnode;
		arr_v[2]=movenode;
		count=3;
		for(int i=1;i<movpos-fixpos;i++){
			arr_v[count]=S->perm[fixpos+i];
			count++;
		}
		arr_v[length-1]=S->perm[movpos+1];

	}
	else if(fixpos>movpos){			// move to right, afterward
		arr_v[0]=S->perm[movpos-1];
		count=1;
		for(int i=1;i<fixpos-movpos;i++) {
			arr_v[count]=S->perm[movpos+i];
			count++;
		}
		arr_v[length-3]=fixnode;
		arr_v[length-2]=movenode;
		arr_v[length-1]=S->perm[fixpos+1];
	}


		int newcdis=0;
		if(movpos<fixpos) newcdis=FwI(movpos,fixpos,1,S->vec_sd,S->Nsel,S->perm);
		else newcdis=BwI(movpos,fixpos,1,S->vec_sd,S->Nsel,S->perm);

		/*update the weight*/
//		double rate = Beta * (pow(CONST_E, 1.0*abs(newcdis)/(1.0*S->cdis)) - 1);

		/*update the cdis*/
		S->cdis=S->cdis+newcdis;

		/*update the ll: NextC, prevN*/
//		int fix_nex=S->perm[fixpos+1];
//		int	mov_pre=S->perm[movpos-1];
//		int mov_nex=S->perm[movpos+1];
//
//		if(movpos<fixpos){
//			S->nextC[mov_pre]=mov_nex;
//			S->prevC[mov_nex]=mov_pre;
//			S->nextC[fixnode]=movenode;
//			S->prevC[movenode]=fixnode;
//			S->nextC[movenode]=fix_nex;
//			if(fix_nex!=-1) S->prevC[fix_nex]=movenode;
//		}
//		else{
//			S->nextC[mov_pre]=mov_nex;
//			if(mov_nex!=-1) S->prevC[mov_nex]=mov_pre;
//			S->nextC[fixnode]=movenode;
//			S->prevC[movenode]=fixnode;
//			S->nextC[movenode]=fix_nex;
//			S->prevC[fix_nex]=movenode;
//		}

		/*update the perm*/
		int begpos=-1, endpos=-1;
		if(fixpos<movpos) {
			begpos=fixpos-1;
			endpos=movpos+1;
		}
		else{
			begpos=movpos-1;
			endpos=fixpos+1;
		}
		for(int i=0;i<length;i++) S->perm[begpos+i]=arr_v[i];

		/*update the pos*/
		for(int i=begpos;i<=endpos;i++){
			if(S->perm[i]==-1) break;
			S->position[S->perm[i]]=i;
		}

		/*f_city, Nsel are not changed and the profit is updated outside*/
		/*update Vsd*/
		if(movpos<fixpos) updateVsd(S->vec_sd, movpos, fixpos,S->perm);
		else updateVsd(S->vec_sd, fixpos, movpos, S->perm);
		/*update the chosen Times and weight*/
		Total_sim++;
		update_chosenT(movenode, fixnode);
//		update_weight(movenode, fixnode, rate);

		if(movpos<fixpos){
			update_chosenT(S->perm[fixpos], S->perm[fixpos+1]);
//			update_weight(S->perm[fixpos], S->perm[fixpos+1], rate);
			update_chosenT(S->perm[movpos-1], S->perm[movpos]);
//			update_weight(S->perm[movpos-1], S->perm[movpos], rate);
		}
		else{
			update_chosenT(S->perm[fixpos+1], S->perm[fixpos+2]);
//			update_weight(S->perm[fixpos+1], S->perm[fixpos+2], rate);
			update_chosenT(S->perm[movpos], S->perm[movpos+1]);
//			update_weight(S->perm[movpos], S->perm[movpos+1], rate);
		}
		/*******************************/
		return 0;
}



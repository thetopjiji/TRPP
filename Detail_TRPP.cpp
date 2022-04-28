/*
 * Detail_TRPP.cpp
 *
 *  Created on: 11Step2020
 *      Author: DELL
 */

#include "struct.h"
#include "trpp_lib.h"


//export function
int *get_vector(int size);
int **get_matrix(int num_row,int num_col);
void free_vec(int *arr);
void free_mat(int **mat, int l);
double **setdoubleM(int l, int c);
void freedoubleM(double **poi, int l);
Struc_Sol *allcoStruct(int size);
void freeStruct(Struc_Sol *temp, int size);
void copy_vec(int *fromv, int *tov, int length);

//void getfitness(Struc_Sol *S);
void getfitness2(Struc_Sol *S);
int getfitness_vec(int *arr_v);
void getcdis(Struc_Sol *S);
void getflag(Struc_Sol *S);

void ll2perm(Struc_Sol *S);
void perm2ll(Struc_Sol *S);
void perm2position(Struc_Sol *S);
void perm2SbSe(Struc_Sol *S);
void perm2sd(Struc_Sol *S);

void CopySolution(Struc_Sol *fromS, Struc_Sol *toS);
void CopyVec(int *fromv, int *tov, int length);
void UpdateBestS(Struc_Sol *S, int method);

void RanIniSol(Struc_Sol *S);
void GreedyIniSol(Struc_Sol *S);
void Ini_Profit_dis(void);
void Ini_Candidate_set(void);
void Ini_fromRecordS(Struc_Sol *S);
void Ini_Profit_dis_pre(void);
void Ini_Candidate_pre_set(void);
void Ini_fromRecordS_greedy(Struc_Sol *S);

void ExcGene(int *lvec, int l);
void layoutSol(Struc_Sol *S);
void check_positive(Struc_Sol *S);

int get_e2e_cdis_general(int *arr_v, int length, int startpos, int Nsel);

void ini_Matrix_chosenT(void);
void ini_weight(void);
void update_chosenT(int node1, int node2);
void update_weight(int node1,int node2, double rate);

double get_time(void);
bool check_time(void);
void verify_ll(Struc_Sol *S);
void verify_SbSe(Struc_Sol *S);
void verify_Vsd(Struc_Sol *S);
void verify_cdis(Struc_Sol *S);


int get_cdis_vec(int *arr);
void cc_positive_vec(int *arr);
void verify_duplicate_perm(int *arr);
void verify_duplicate_ll(int *arr);
void sort_detail_sol(Struc_Sol *S);

int FwI(int i, int j, int k, int *Vsd, int N, int *perm);
int BwI(int i, int j, int k, int *Vsd, int N, int *perm);
int fast2opt(int i, int j, int *Vsd, int N, int *perm, int *Sb, int *Se, int cdis);
void updateVsd(int *Vsd, int startpos, int endpos, int *perm);
void updateSbSe(int i, int j, int N, int *perm, int *Vsd, int *Sb, int *Se);
void updateCandidateSet(void);
int locate_nextcity(Struc_Sol *S, int curcity);
int locate_prevcity(Struc_Sol *S, int curcity);

//local functions

int FindGoodCity(int nowc);
int FindPrevCity(int nowc);
int Sel_greedy_random_city(int deg_good, int deg_bad, int nowc, Struc_Sol *S);
int Sel_fk_random_city(int *arr_c, int nowc, Struc_Sol *S);

int updateCandidate_foreach(int nowc);


/*standard initial_ choose the candidate city for the partial solution*/

int standard_ratio_select(Struc_Sol *S, int poscity, int num_current);
int standard_nearest_select(Struc_Sol *S, int poscity, int num_current);
//int ini_candidateSet_nearness_choose(int nowc);

//functions
int *get_vector(int size){
	int *Poi;
	Poi= (int*)malloc(size*sizeof(int));
	if (Poi==NULL){
		cout<<"Memory error in get_vector"<<endl;
		exit(-1);
	}
//	memset(Poi,-1,size*sizeof(int));
	return Poi;
}

int **get_matrix(int num_row,int num_col){
	int **Poi,i;
	Poi=(int**)malloc(num_row*sizeof(int*));
	if (!Poi){
		cout<<"Memory error in get_matrix"<<endl;
		exit(-1);
	}
	for (i=0;i<num_row;i++){
		Poi[i]=(int*)malloc(num_col*sizeof(int));
		if (!Poi[i]){
			cout<<"Memory error in get_matrix"<<endl;
			exit(-1);
		}
		memset(Poi[i],-1,num_col*sizeof(int));
	}
	return Poi;
}


void free_vec(int *arr){
	free(arr);
	arr=NULL;
}
void free_mat(int **mat, int l){
	for(int i=0;i<l;i++){
		free(mat[i]);
		mat[i]=NULL;
	}
	free(mat);
	mat=NULL;
}


double **setdoubleM(int l, int c){

	double **poi;

	poi=(double**)malloc(l*sizeof(double*));
	if(!poi){
		cout<<"Memory error in double set in Detail.cpp"<<endl;
		exit(-1);
	}
	for(int i=0;i<l;i++){
		poi[i]=(double*)malloc(c*sizeof(double));
		if(!poi[i]){
			cout<<"Memory error in poi[i] in double set in Detail.cpp"<<endl;
			exit(-1);
		}
	}
	return poi;
}


void freedoubleM(double **poi, int l){
	for(int i=0;i<l;i++){
		free(poi[i]);
		poi[i]=NULL;
	}
	free(poi);
	poi=NULL;
}


Struc_Sol *allcoStruct(int size){
	Struc_Sol *temp;

	if(size==1){
		temp= (Struc_Sol*)malloc(sizeof(Struc_Sol));
		if (temp==NULL){
			cout<<"Memory error in allcoStruct"<<endl;
			exit(-1);
		}
//		temp->prevC=get_vector(num_city);
//		temp->nextC=get_vector(num_city);
		temp->f_city=get_vector(num_city);
		temp->position=get_vector(num_city);
		temp->perm=get_vector(num_city+1);
		temp->vec_sd=get_vector(num_city);
		temp->Sb=get_vector(num_city);
		temp->Se=get_vector(num_city);

		for(int i=0;i<num_city;i++){
//			temp->prevC[i]=-1;
//			temp->nextC[i]=-1;
			temp->f_city[i]=0;
			temp->position[i]=-1;
			temp->perm[i]=-1;
			temp->vec_sd[i]=-1;
			temp->Sb[i]=-1;
			temp->Se[i]=-1;
		}
		temp->perm[num_city]=-1;
		temp->Nsel=0;
		temp->cdis=0;
		temp->profit=0;
	}
	else{
		temp= (Struc_Sol*)malloc(size*sizeof(Struc_Sol));
		if (temp==NULL){
			cout<<"Memory error in allcoStruct"<<endl;
			exit(-1);
		}

		for(int j=0;j<size;j++){
//			temp[j].prevC=get_vector(num_city);
//			temp[j].nextC=get_vector(num_city);
			temp[j].f_city=get_vector(num_city);
			temp[j].position=get_vector(num_city);
			temp[j].perm=get_vector(num_city+1);
			temp[j].vec_sd=get_vector(num_city);
			temp[j].Sb=get_vector(num_city);
			temp[j].Se=get_vector(num_city);

			for(int i=0;i<num_city;i++){
//				temp[j].prevC[i]=-1;
//				temp[j].nextC[i]=-1;
				temp[j].f_city[i]=0;
				temp[j].position[i]=-1;
				temp[j].perm[i]=-1;
				temp[j].vec_sd[i]=-1;
				temp[j].Sb[i]=-1;
				temp[j].Se[i]=-1;
			}
			temp[j].perm[num_city]=-1;
			temp[j].Nsel=0;
			temp[j].cdis=0;
			temp[j].profit=0;
		}

	}


	return temp;
}
void freeStruct(Struc_Sol *temp, int size){
	int i;

	if(size==1){
//		free(temp->prevC); 		temp->prevC=NULL;
//		free(temp->nextC); 		temp->nextC=NULL;
		free(temp->perm); 		temp->perm=NULL;
		free(temp->f_city);		temp->f_city=NULL;
		free(temp->position); 	temp->position=NULL;
		free(temp->vec_sd); 	temp->vec_sd=NULL;
		free(temp->Sb); 		temp->Sb=NULL;
		free(temp->Se); 		temp->Se=NULL;
		free(temp); 			temp=NULL;
	}
	else{
		for(i=0;i<size;i++){
//			free(temp[i].prevC); 	temp[i].prevC=NULL;
//			free(temp[i].nextC); 	temp[i].nextC=NULL;
			free(temp[i].perm); 	temp[i].perm=NULL;
			free(temp[i].f_city);	temp[i].f_city=NULL;
			free(temp[i].position);	temp[i].position=NULL;
			free(temp[i].vec_sd);	temp[i].vec_sd=NULL;
			free(temp[i].Sb);		temp[i].Sb=NULL;
			free(temp[i].Se);		temp[i].Se=NULL;
		}
		free(temp); 		temp=NULL;
	}
}

void copy_vec(int *fromv, int *tov, int length){
	for(int i=0;i<length;i++) tov[i]=fromv[i];
}

void getfitness(Struc_Sol *S){
	// city 0 is always chosen.
	// get the fitness value from the S.perm
	int i;
	int temp_p=0;

	S->profit=0;
	for(i=0;i<num_city;i++)
		if(S->f_city[i]) temp_p=temp_p+profitA[i];		//get all the profit positive
	cout<<temp_p<<endl;
	for(i=0;i<S->Nsel-1;i++) {
		temp_p=temp_p-matrix_dis[S->perm[i]][S->perm[i+1]]*(S->Nsel-1-i);

	}
	S->profit=temp_p;

}

void ll2perm(Struc_Sol *S){
//	int i;
//	S->Nsel=0;
//	for(i=0;i<num_city+1;i++) S->perm[i]=-1;
//	for(i=0;i<num_city;i++)	S->f_city[i]=0;
//
//	int nexc;
//	S->perm[0]=0;
//	S->f_city[0]=1;
//	nexc=S->nextC[0];
//	S->Nsel=1;
//
//	while(nexc!=-1){
//		S->perm[S->Nsel]=nexc;
//		S->f_city[nexc]=1;
//		nexc=S->nextC[nexc];
//		S->Nsel++;
//	}

}

void perm2ll(Struc_Sol *S){
//	int i;
//	for(i=0;i<num_city;i++){
//		S->nextC[i]=-1;
////		S->prevC[i]=-1;
//		S->f_city[i]=0;
//	}
//	S->Nsel=0;
//
//	int nexc=0;
//	int nowc=-1;
//
//	S->Nsel++;
//	nowc=nexc;
//	S->f_city[nowc]=1;
//	nexc=S->perm[S->Nsel];
//	while(nexc!=-1){
//
//		S->nextC[nowc]=nexc;
//		S->prevC[nexc]=nowc;
//
//		S->Nsel++;
//		nowc=nexc;
//		S->f_city[nowc]=1;
//		nexc=S->perm[S->Nsel];
//	}
}

void perm2position(Struc_Sol *S){
	int i;
	int count=0;

	for(i=0;i<num_city;i++) S->position[i]=-1;

	while(S->perm[count]!=-1){
		S->position[S->perm[count]]=count;
		count++;
	}
}

void perm2sd(Struc_Sol *S){

	int nowc;
	int count=0;

//	for(int i=0;i<num_city;i++) S->vec_sd[i]=-1;	//No need to initialize

	S->vec_sd[count]=0;
	count++;
	nowc=S->perm[count];
	while(nowc!=-1){
		S->vec_sd[count]=S->vec_sd[count-1]+matrix_dis[S->perm[count-1]][nowc];
		count++;
		nowc=S->perm[count];
		if(count>num_city+2) {
			cout<<"error in per2sd, no -1 at the end"<<endl;
			exit(-5);
		}
	}
}

void updateVsd(int *Vsd, int startpos, int endpos, int *perm){
//	if(startpos<1){
//		cout<<"error in updateVsd"<<endl;
//		exit(-3);
//	}

	int nowc;
	int count;

	if(startpos==0) {
		Vsd[0]=0;
		startpos=1;
	}


	count=startpos;
	nowc=perm[count];
//	while(count<=endpos){
	while(nowc!=-1){
		Vsd[count]=Vsd[count-1]+matrix_dis[perm[count-1]][nowc];
		count++;
		nowc=perm[count];
		if(count>num_city+2) {
			cout<<"error in update Vsd, no -1 at the end"<<endl;
			exit(-5);
		}
	}
}

void updateSbSe(int i, int j, int N, int *perm, int *Vsd, int *Sb, int *Se){
	int p;

	if(j==N-1){
		for(p=i+1;p<N;p++) 	Sb[p]=Sb[p-1]+(N-p)*matrix_dis[perm[p-1]][perm[p]];
		Se[j]=0;
		for(p=j-1;p>=0;p--)	Se[p]=Se[p+1]+(N-1-p)*matrix_dis[perm[p+1]][perm[p]];
	}
	else{
		for(p=i+1;p<N;p++) 	Sb[p]=Sb[p-1]+(N-p)*matrix_dis[perm[p-1]][perm[p]];
		for(p=j;p>=0;p--)	Se[p]=Se[p+1]+(N-1-p)*matrix_dis[perm[p+1]][perm[p]];
	}

	updateVsd(Vsd,i,j,perm);
}

void updateCandidateSet(void){
	int chosenCity=-1;
	/*********************Version of most visited************************************/
	/*Update the Candidate_set*/
	for(int i=0;i<num_city;i++){
		Num_candidate_per_city[i]=0;				//find the candidate_city for city i;
		for(int j=0;j<num_city;j++) FSel[j]=false;
		FSel[0]=true;
		for(int j=0;j<Max_candidate;j++){
			chosenCity=updateCandidate_foreach(i);
			if(chosenCity==-1) break;
			Candidate_set[i][j]=chosenCity;
			Num_candidate_per_city[i]++;
			FSel[chosenCity]=true;
		}
	}
	/*Update the Candidate_pre_set*/

	for(int i=0;i<num_city;i++){
		Num_candidate_pre_per_city[i]=0;		//find the candidate_city for city i;
		for(int j=0;j<num_city;j++) FSel[j]=false;
		FSel[0]=true;
		for(int j=0;j<Max_smallset;j++){
			chosenCity=updateCandidate_foreach(i);
			if(chosenCity==-1) break;
			Candidate_pre_set[i][j]=chosenCity;
			Num_candidate_pre_per_city[i]++;
			FSel[chosenCity]=true;
		}
	}

	return;
	/*********************Version of less visited************************************/
	for(int i=0;i<num_city;i++){
		Num_candidate_per_city[i]=0;				//find the candidate_city for city i;
		for(int j=0;j<num_city;j++) FSel[j]=false;
		FSel[0]=true;
		for(int j=0;j<Max_candidate;j++){
			chosenCity=updateCandidate_foreach(i);
			if(chosenCity==-1) break;
			Candidate_set[i][j]=chosenCity;
			Num_candidate_per_city[i]++;
			FSel[chosenCity]=true;
		}
	}
	/*Update the Candidate_pre_set*/

	for(int i=0;i<num_city;i++){
		Num_candidate_pre_per_city[i]=0;		//find the candidate_city for city i;
		for(int j=0;j<num_city;j++) FSel[j]=false;
		FSel[0]=true;
		for(int j=0;j<Max_smallset;j++){
			chosenCity=updateCandidate_foreach(i);
			if(chosenCity==-1) break;
			Candidate_pre_set[i][j]=chosenCity;
			Num_candidate_pre_per_city[i]++;
			FSel[chosenCity]=true;
		}
	}

	return;
	/*********************Version of half "most" and half "less""************************/

}

void perm2SbSe(Struc_Sol *S){
	int nowc;
	int count=0;

	/*get the number of the selected nodes*/
	do{
		nowc=S->perm[count];
		if(nowc==-1) break;
		count++;
	}while(1);
	S->Nsel=count;
	/*calculate the SbSe*/
	S->Sb[0]=0;
	for(int i=1;i<S->Nsel;i++) S->Sb[i]=S->Sb[i-1]+(S->Nsel-i)*matrix_dis[S->perm[i-1]][S->perm[i]];
	S->Se[S->Nsel-1]=0;
	for(int i=S->Nsel-2;i>=0;i--) S->Se[i]=S->Se[i+1]+(S->Nsel-i-1)*matrix_dis[S->perm[i]][S->perm[i+1]];

}


void getfitness2(Struc_Sol *S){
// from the orignial formulation
	int sumdis=0;
	int sumprofit=0;
	int delta_profit;
	int indx=0;
	int nexc=0;
	int nowc=0;

	S->profit=0;

	while(1){
		indx++;
		nowc=nexc;
		nexc=S->perm[indx];
		if(nexc==-1) break;
		sumdis=sumdis+matrix_dis[nowc][nexc];
		delta_profit=profitA[nexc]-sumdis;
		if(delta_profit>=0)	sumprofit=sumprofit+delta_profit;
	}
	S->profit=sumprofit;
}

int getfitness_vec(int *arr_v){

	int sumdis=0;
	int sumprofit=0;
	int delta_profit;
	int indx=0;
	int nexc=0;
	int nowc=0;


	while(1){
		indx++;
		nowc=nexc;
		nexc=arr_v[indx];
		if(nexc==-1) break;
		sumdis=sumdis+matrix_dis[nowc][nexc];
		delta_profit=profitA[nexc]-sumdis;
		if(delta_profit>=0)	sumprofit=sumprofit+delta_profit;
	}

	return sumprofit;
}

void getcdis(Struc_Sol *S){
	S->cdis=0;

	int sumdis=0;
	int indx=0;
	int nexc=0;
	int nowc=0;
	while(1){
		indx++;
		nowc=nexc;
		nexc=S->perm[indx];
		if(nexc==-1) break;
		sumdis=sumdis+matrix_dis[nowc][nexc];
		S->cdis=S->cdis+sumdis;
//		printf("\nc1=%d c2=%d dis=%d accd=%d profitnew=%d delta=%d\n",nowc,nexc,matrix_dis[nowc][nexc],sumdis,profitA[nexc], profitA[nexc]-sumdis);
	}

}

void getflag(Struc_Sol *S){

	for(int i=0;i<num_city;i++) S->f_city[i]=0;

	for(int i=0;i<num_city+1;i++){
		if(S->perm[i]==-1) break;
		S->f_city[S->perm[i]]=1;
	}
}

void CopySolution(Struc_Sol *fromS, Struc_Sol *toS){
	for(int i=0;i<num_city;i++){
//		toS->prevC[i]=fromS->prevC[i];
//		toS->nextC[i]=fromS->nextC[i];
		toS->f_city[i]=fromS->f_city[i];
		toS->perm[i]=fromS->perm[i];
		toS->position[i]=fromS->position[i];
	}
	toS->perm[num_city]=fromS->perm[num_city];
	toS->Nsel=fromS->Nsel;
	toS->profit=fromS->profit;
	toS->cdis=fromS->cdis;
}

void CopyVec(int *fromv, int *tov, int length){
	for(int i=0;i<length;i++) tov[i]=fromv[i];
}

void UpdateBestS(Struc_Sol *S, int method){
	char meth[100];

	switch(method){
	case 0:
		strcpy(meth,"met_2opt");
		break;
	case 1:
		strcpy(meth,"met_swap");
		break;
	case 2:
		strcpy(meth,"met_add");
		break;
	case 3:
		strcpy(meth,"met_drop");
		break;
	case 4:
		strcpy(meth,"met_insert");
		break;
	case 5:
		strcpy(meth,"met_multi");
		break;
	case 6:
		strcpy(meth,"met_interx");
		break;
	case 7:
		strcpy(meth,"met_lk");
		break;
	case 8:
		strcpy(meth, "met_swapadj");
		break;
	case 10:
		strcpy(meth, "met_2optCor");
		break;
	case 11:
		strcpy(meth, "met_2optNor");
		break;
	case 12:
		strcpy(meth, "met_2optRev");
		break;
	case 13:
		strcpy(meth, "met_2optNor");
		break;
	case 14:
		strcpy(meth, "met_2optAno");
		break;
	case 61:
		strcpy(meth, "met_BwI");
		break;
	case 62:
		strcpy(meth, "met_FwI");
		break;
	default:
		strcpy(meth,"others");
		break;
	}

//	sort_detail_sol(S);

	if(S->profit>BestS->profit) {
		CopySolution(S,BestS);
		printf("%d %d %f %s\n", BestS->profit , BestS->Nsel, get_time(), meth);
//		printf("%d %d %f %d %f %s\n",RecordS->profit, BestS->profit,(RecordS->profit-BestS->profit)*1.0/RecordS->profit , BestS->Nsel, get_time(), meth);
//		for(int i=0;i<BestS->Nsel;i++) cout<<BestS->perm[i]<<'\t';	cout<<endl;
		ofstream caout(name_final_result,ios::out|ios::app);
		if (caout.is_open()){
			caout<<"1 ";
			caout<<BestS->profit<<" ";
			caout<<get_time()<<endl;
			caout.close();
		}
//		check_positive(BestS);

//		ofstream fkout("All_result.txt",ios::out|ios::app);
//		if (fkout.is_open()){
//			fkout<<"Best_Res: "<<BestS->profit<<endl;
//			fkout<<"Per_cent: "<<(RecordS->profit-BestS->profit)*1.0/RecordS->profit<<endl;
//			fkout<<"Perm_Bst: ";
//			for(int i=0;i<RecordS->Nsel;i++) fkout<<BestS->perm[i]<<" ";
//			fkout<<endl;
//			fkout<<endl;
//			fkout<<endl;
//			fkout.close();
//		}

	}


}

void RanIniSol(Struc_Sol *S){
	int temp_poi[num_city-1];

	/*Reset solution S*/
	for(int i=0;i<num_city;i++){
//		S->prevC[i]=-1;
//		S->nextC[i]=-1;
		S->f_city[i]=0;
		S->perm[i]=-1;
		S->position[i]=-1;
	}
	S->perm[num_city]=-1;
	S->Nsel=0;
	S->profit=0;
	/****************************/

	for(int i=0;i<num_city-1;i++) temp_poi[i]=i;
	ExcGene(temp_poi,num_city-1);

	for(int i=0;i<num_city-1;i++) S->perm[i+1]=temp_poi[i]+1;	//0 must be at the start position
	S->perm[0]=0;

	perm2position(S);
	perm2ll(S);
	getfitness2(S);
}

void GreedyIniSol(Struc_Sol *S){

	/*Reset solution S*/
	for(int i=0;i<num_city;i++){
//		S->prevC[i]=-1;
//		S->nextC[i]=-1;
		S->f_city[i]=0;
		S->perm[i]=-1;
		S->position[i]=-1;
	}
	S->perm[num_city]=-1;
	S->Nsel=0;
	S->profit=0;
	/****************************/

	int nowc;
	int citysel=-1;
	int deg_good=3;
	int	deg_bad=1;
	int count=0;

	nowc=0;
	S->perm[count]=nowc;
	S->f_city[nowc]=1;
	count++;

	do{

//		citysel=standard_ratio_select(S,nowc,count);
		citysel=standard_nearest_select(S,nowc,count);
//		citysel=Sel_greedy_random_city(deg_good,deg_bad,nowc,S);	//if there is no greedy city, choose random one
		if(citysel!=-1) {										//if the city is selected is identified in Sel_greedy_random_city, if citysel==-1 which means there is no free city.
			S->perm[count]=citysel;
			S->f_city[citysel]=1;
			nowc=citysel;
			count++;
		}
	}while(citysel!=-1);

	perm2ll(S);
	perm2position(S);
	getfitness2(S);
	getcdis(S);
	perm2sd(S);
	perm2SbSe(S);
}

void Ini_Profit_dis(void){
	for(int i=0;i<num_city;i++)
		for(int j=0;j<num_city;j++)
			if(i==j) Profit_dis[i][j]=0;
			else{
				if(Para_M!=0) Profit_dis[i][j]=profitA[j]*1.0/(Para_M*matrix_dis[i][j]);
				else Profit_dis[i][j]=profitA[j]*1.0/matrix_dis[i][j];
			}
}

void Ini_Candidate_set(void){
	int chosenCity=-1;
	//if we use sort, the complexity is nlg(n), but if we select one at one time, the complexity is Max_candidate*n (where Max_candidate is set to 22)
	for(int i=0;i<num_city;i++){
		Num_candidate_per_city[i]=0;		//find the candidate_city for city i;
		for(int j=0;j<num_city;j++) FSel[j]=false;
		FSel[0]=true;
		for(int j=0;j<Max_candidate;j++){
			chosenCity=FindGoodCity(i);
			if(chosenCity==-1) break;
			Candidate_set[i][j]=chosenCity;
			Num_candidate_per_city[i]++;
			FSel[chosenCity]=true;
		}
	}
}

void Ini_fromRecordS(Struc_Sol *S){
	int lenth=RecordS->Nsel-1;
	int arr_[lenth];

	for(int i=0;i<lenth;i++) arr_[i]=RecordS->perm[i+1];

	ExcGene(arr_,lenth);

	memset(S->perm,-1, (num_city+1)*sizeof(int));

	for(int i=0;i<lenth;i++) S->perm[i+1]=arr_[i];
	S->perm[0]=0;
	perm2position(S);
	perm2ll(S);
	getfitness2(S);
	getcdis(S);
	getflag(S);
}

void Ini_fromRecordS_greedy(Struc_Sol *S){

	int arr_c[num_city];
	int count=0;
	int citysel=-1;
	int nowc=-1;

	for(int i=0;i<num_city;i++) arr_c[i]=RecordS->f_city[i];
	for(int i=0;i<num_city;i++) FSel[i]=0;


	FSel[0]=1;
	S->perm[count]=0;
	nowc=S->perm[count];
	count++;
	do{
		citysel=Sel_fk_random_city(arr_c,  nowc, S);
		if(citysel!=-1){
			S->perm[count]=citysel;
			count++;
			FSel[citysel]=1;
			nowc=citysel;
		}

	}while(citysel!=-1);

//	cout<<"num_all="<<count<<endl;
	perm2ll(S);
	perm2position(S);
	getfitness2(S);
	getcdis(S);
}

void Ini_Profit_dis_pre(void){
//	for(int i=0;i<num_city;i++)
//		for(int j=0;j<num_city;j++)
//			if(i==j) Profit_dis_pre[i][j]=0;
//			else{
//				if(Para_M!=0) Profit_dis_pre[i][j]=profitA[i]*1.0/(Para_M*matrix_dis[i][j]);		//from j to i, the profit-distance
//				else Profit_dis_pre[i][j]=profitA[i]*1.0/matrix_dis[i][j];
//			}
	for(int i=0;i<num_city;i++)
		for(int j=0;j<num_city;j++)
			if(i==j) Profit_dis_pre[i][j]=0;
			else{
				if(Para_M!=0) Profit_dis_pre[i][j]=profitA[j]*1.0/(Para_M*matrix_dis[i][j]);
				else Profit_dis_pre[i][j]=profitA[j]*1.0/matrix_dis[i][j];
			}
}

void Ini_Candidate_pre_set(void){
//	int chosenCity=-1;
//	/*To set up the Candidate_pre_set*/
//	for(int i=1;i<num_city;i++){				//skip the depot city
//		Num_candidate_pre_per_city[i]=0;		//find the candidate_city for city i;
//		for(int j=0;j<num_city;j++) FSel[j]=false;
////		FSel[0]=true;							//depot city could be chosen because it is in front the i
//		for(int j=0;j<Max_candidate;j++){
////			chosenCity=FindGoodCity(i);
//			chosenCity=FindPrevCity(i);
//			if(chosenCity==-1) break;
//			Candidate_pre_set[i][j]=chosenCity;
//			Num_candidate_pre_per_city[i]++;
//			FSel[chosenCity]=true;
//		}
//	}

	int chosenCity=-1;
	//if we use sort, the complexity is nlg(n), but if we select one at one time, the complexity is Max_candidate*n (where Max_candidate is set to 22)
	for(int i=0;i<num_city;i++){
		Num_candidate_pre_per_city[i]=0;		//find the candidate_city for city i;
		for(int j=0;j<num_city;j++) FSel[j]=false;
		FSel[0]=true;
		for(int j=0;j<Max_smallset;j++){
			chosenCity=FindGoodCity(i);
			if(chosenCity==-1) break;
			Candidate_pre_set[i][j]=chosenCity;
			Num_candidate_pre_per_city[i]++;
			FSel[chosenCity]=true;
		}
	}

}


int FindGoodCity(int nowc){
	int cityFind=-1;
	int i;
	for(i=0;i<num_city;i++){
		if(i==nowc) continue;
		if(cityFind==-1 && FSel[i]==false) cityFind=i;
		else if(cityFind!=-1 && FSel[i]==false) {
//			if(Profit_dis[nowc][i]>Profit_dis[nowc][cityFind]) cityFind=i;	//ratio biggest
			if(matrix_dis[nowc][i]<matrix_dis[nowc][cityFind]) cityFind=i;	//nearest
		}
	}

	return cityFind;
}

int FindPrevCity(int nowc){
	int cityFind=-1;
	int i;
	for(i=0;i<num_city;i++){
		if(i==nowc) continue;
		if(cityFind==-1 && FSel[i]==false) cityFind=i;
		else if(cityFind!=-1 && FSel[i]==false) {
			if(Profit_dis_pre[nowc][i]>Profit_dis_pre[nowc][cityFind]) cityFind=i;
		}
	}
	return cityFind;
}

int Sel_greedy_random_city(int deg_good, int deg_bad, int nowc, Struc_Sol *S){
	int startp=0;
	int deg_choose=0;
	int i=0;
	int candicity=0;
	//choose a greedy city;
	deg_choose=min(min(deg_good,num_city+1-deg_bad),Num_candidate_per_city[nowc]);	//well, deg_bad is useless
	startp=rand()%deg_choose;

	for(i=0;i<Num_candidate_per_city[nowc];i++){
		candicity=Candidate_set[nowc][(i+startp)%Num_candidate_per_city[nowc]];
		if(candicity==-1) {
			cout<<"candicity==-1"<<endl;
			exit(-3);
		}
		if(S->f_city[candicity]==0) return candicity;
	}
	//if there is no greedy city, choose a random one;

	startp=rand()%num_city;
	for(i=0;i<num_city;i++){
		candicity=(i+startp)%num_city;							//random choose is not from the candidate set
		if(S->f_city[candicity]==0) return candicity;
	}

	return -1;
}

int Sel_fk_random_city(int *arr_c, int nowc, Struc_Sol *S){

	int citysel=-1;
	int startpos=0;
	int i;
	int tempsel=-1;

	startpos=rand()%Num_candidate_pre_per_city[nowc];
	for(i=0;i<Num_candidate_pre_per_city[nowc];i++){
		tempsel=Candidate_set[nowc][(i+startpos)%Num_candidate_pre_per_city[nowc]];
		if(FSel[tempsel]==0 && arr_c[tempsel]==1) {			// free node and in the set
			citysel=tempsel;
			break;
		}
	}

	if(citysel!=-1) return citysel;

	startpos=rand()%num_city;
	for(i=0;i<num_city;i++){
		tempsel=(i+startpos)%num_city;
		if(FSel[tempsel]==0 && arr_c[tempsel]==1) {			// free node and in the set
			citysel=tempsel;
			break;
		}
	}

	return citysel;
}

int updateCandidate_foreach(int nowc){
	int cityFind=-1;
	int i;
	for(i=0;i<num_city;i++){
		if(i==nowc) continue;
		if(cityFind==-1 && FSel[i]==false) cityFind=i;
		else if(cityFind!=-1 && FSel[i]==false) {
			if(Matrix_chosenT[nowc][i]>Matrix_chosenT[nowc][cityFind]) cityFind=i;
		}
	}

	return cityFind;
}

void ExcGene(int *lvec, int l){
	int exc[l];
	int i;
	int temp;

	for(i=0;i<l;i++) exc[i]=rand()%l;

	for(i=0;i<l;i++) {
		temp=lvec[i];
		lvec[i]=lvec[exc[i]];
		lvec[exc[i]]=temp;
	}
}

void layoutSol(Struc_Sol *S){
//	printf("\n");
//	for(int i=0;i<S->Nsel;i++) printf("%d ",S->perm[i]);
	printf("\n");
	printf("%d %d\n",S->profit,S->Nsel);
	check_positive(S);
}

void check_positive(Struc_Sol *S){
	// if the delta profit is less than 0, there must be space to improve

	int accdis=0;
	int nowc,nexc;

	cout<<"Perm_Pro: 0\t";
	for(int i=0;i<S->Nsel;i++){
		nowc=S->perm[i];
		nexc=S->perm[i+1];
		if(nexc==-1) break;
		accdis=accdis+matrix_dis[nowc][nexc];
		printf("%d\t",profitA[nexc]-accdis);
	}
	printf("\n");
}

int get_e2e_cdis_general(int *arr_v, int length, int startpos, int Nsel){
	int sumdis=0;

	for(int i=0;i<length-1;i++) {
		if(arr_v[i+1]==-1) break;
		sumdis=sumdis+matrix_dis[arr_v[i]][arr_v[i+1]]*(Nsel-1-i-startpos);
//		printf("%d %d, from %d to %d\n",Nsel-1-i-startpos,matrix_dis[arr_v[i]][arr_v[i+1]],arr_v[i],arr_v[i+1]);
//		cout<<Nsel-1-i-startpos<<" "<<matrix_dis[arr_v[i]][arr_v[i+1]]<<endl;
	}

	return sumdis;
}

void ini_Matrix_chosenT(void){

	for(int i=0;i<num_city;i++) for(int j=0;j<num_city;j++) Matrix_chosenT[i][j]=1;

}

void ini_weight(void){

//	for(int i=0;i<num_city;i++) for(int j=0;j<num_city;j++) weight[i][j]=1.0;

}

void update_chosenT(int node1, int node2){
	if(node1==-1 || node2==-1) return;

	Matrix_chosenT[node1][node2]++;
	Matrix_chosenT[node2][node1]++;
}

void update_weight(int node1,int node2, double rate){

	if(node1==-1 || node2==-1) return;

//	weight[node1][node2]+=rate;
//	weight[node2][node1]+=rate;
}


double get_time(void){
	double endTimeSeconds=0.0;
	glo_end=clock();
	endTimeSeconds = (glo_end-glo_start)*1.0/CLOCKS_PER_SEC;
	return (endTimeSeconds);
}

bool check_time(void){
//	return false;
	double timenow=get_time();
	if(timenow>cuttime || BestS->profit>=alreadybest) return true;
	else return false;
	return true;
}

void verify_ll(Struc_Sol *S){
//	int tempnex[num_city];
//	int temppre[num_city];
//
//	for(int i=0;i<num_city;i++){
//		tempnex[i]=S->nextC[i];
//		temppre[i]=S->prevC[i];
//	}
//	perm2ll(S);
//
//	for(int i=0;i<num_city;i++){
//		if(tempnex[i]-S->nextC[i]!=0){
//			cout<<"error in nextC "<<i<<endl;
//			cout<<"error in nextC "<<i<<endl;
//		}
//	}
//
//	for(int i=0;i<num_city;i++){
//		if(temppre[i]-S->prevC[i]!=0){
//			cout<<"error in prevC "<<i<<endl;
//			cout<<"error in prevC "<<i<<endl;
//		}
//	}

}

void verify_SbSe(Struc_Sol *S){
	int temp_Sb[num_city];
	int temp_Se[num_city];


	for(int i=0;i<num_city;i++){
		temp_Sb[i]=S->Sb[i];
		temp_Se[i]=S->Se[i];
	}

	perm2SbSe(S);

	for(int i=0;i<num_city;i++){
		if(temp_Sb[i]-S->Sb[i]!=0){
			cout<<"error in Sb "<<i<<endl;
			cout<<"error in Sb "<<i<<endl;
		}
	}

	for(int i=0;i<num_city;i++){
		if(temp_Se[i]-S->Se[i]!=0){
			cout<<"error in Se "<<i<<endl;
			cout<<"error in Se "<<i<<endl;
		}
	}
}

void verify_Vsd(Struc_Sol *S){
	int temp_Vsd[num_city];

	for(int i=0;i<num_city;i++) temp_Vsd[i]=S->vec_sd[i];

	perm2sd(S);
	for(int i=0;i<num_city;i++){
		if(temp_Vsd[i]-S->vec_sd[i]!=0){
			cout<<"error in vec_sd "<<i<<endl;
			cout<<"error in vec_sd "<<i<<endl;
		}
	}
}

void verify_cdis(Struc_Sol *S){

	int temp_cdis;

	temp_cdis=S->cdis;

	getcdis(S);

	if(temp_cdis-S->cdis!=0){
		cout<<"error in cdis "<<endl;
		cout<<"error in cdis "<<endl;
	}

}




int get_cdis_vec(int *arr){
	int sumcdis=0;
	int tempsum=0;
	int nowcc,nexcc;
	int count=0;

	do{
		nowcc=arr[count];
		nexcc=arr[count+1];
		if(nexcc==-1) break;
		tempsum=tempsum+matrix_dis[nowcc][nexcc];
		sumcdis=sumcdis+tempsum;
		count++;
	}while(1);

	return sumcdis;
}

void cc_positive_vec(int *arr){
	int accdis=0;
	int nowc,nexc;
	int count=0;
	cout<<"0\t";

	do{
		nowc=arr[count];
		nexc=arr[count+1];
		if(nexc==-1) break;
		accdis=accdis+matrix_dis[nowc][nexc];
//		printf("%d\t",profitA[nexc]-accdis);
		printf("%d\t",accdis);
		count++;
	}while(1);

	printf("\n");
}

void verify_duplicate_perm(int *arr){
	int nexcc;
	int i;

	for(i=0;i<num_city;i++) FSel[i]=false;
	i=0;
	do{
		nexcc=arr[i];
		if(nexcc==-1) break;
		if(FSel[nexcc]==false) FSel[nexcc]=true;
		else cout<<"duplicate in perm "<<i<<" "<<nexcc<<endl;
		i++;
	}while(1);
}


void verify_duplicate_ll(int *arr){
	int nowcc,nexcc;
	int i;
//	int count=0;

	for(i=0;i<num_city;i++) FSel[i]=false;
	nowcc=0;
	do{
		nexcc=arr[nowcc];
		if(nexcc==-1) break;
		if(FSel[nexcc]==false) FSel[nexcc]=true;
		else{
			cout<<"duplicate in ll "<<i<<" "<<nexcc<<endl;
		}
		nowcc=nexcc;
	}while(1);
	/*layout the ll*/
}

void sort_detail_sol(Struc_Sol *S){

	int flag_i=0;

	int cc_dupli=0;

	for(int i=0;i<num_city;i++)
		if(S->f_city[i]==1 && BestS->f_city[i]==1) cc_dupli++;


	if(S->profit>BestS->profit) flag_i=1;
	else flag_i=0;

	printf("BestS=%d\t profit=%d\t Percent=%f\t cdis=%d\t flag_i=%d \t dupli=%f\n",BestS->profit,S->profit,(S->profit-BestS->profit)*1.0/BestS->profit,S->cdis,flag_i, cc_dupli*1.0/BestS->Nsel);

	ofstream fkout("All_result.txt",ios::out|ios::app);
	if (fkout.is_open()){
		fkout<<"BestS="<<BestS->profit<<"\t profit="<<S->profit<<"\t Percent="<<(S->profit-BestS->profit)*1.0/BestS->profit;
		fkout<<"\t cdis="<<S->cdis<<"\t flag_i="<<flag_i<<"\t dupli="<<cc_dupli*1.0/BestS->Nsel<<endl;
		fkout.close();
	}
}

int FwI(int i, int j, int k, int *Vsd, int N, int *perm){
	int delta_cdis=0;

	delta_cdis+=(N-i)*(matrix_dis[perm[i-1]][perm[i+k]]-matrix_dis[perm[i-1]][perm[i]]);
	if((N-j-1)!=0) delta_cdis+=(N-j-1)*(matrix_dis[perm[i+k-1]][perm[j+1]]-matrix_dis[perm[j]][perm[j+1]]);
	delta_cdis+=(N-j+k-1)*matrix_dis[perm[j]][perm[i]];
	delta_cdis-=(N-i-k)*matrix_dis[perm[i+k-1]][perm[i+k]];
	delta_cdis+=(k+i-j-1)*(Vsd[i+k-1]-Vsd[i]);
	delta_cdis+=k*(Vsd[j]-Vsd[i+k]);

	return delta_cdis;
}

int BwI(int i, int j, int k, int *Vsd, int N, int *perm){
	int delta_cdis=0;

	if(k==1 && j+1==i) return 0;

	delta_cdis+=(N-j-1)*(matrix_dis[perm[j]][perm[i]]-matrix_dis[perm[j]][perm[j+1]]);
	if((N-i-k)!=0) delta_cdis+=(N-i-k)*(matrix_dis[perm[i-1]][perm[i+k]]-matrix_dis[perm[i+k-1]][perm[i+k]]);
	delta_cdis+=(N-k-j-1)*matrix_dis[perm[i+k-1]][perm[j+1]];
	delta_cdis-=(N-i)*matrix_dis[perm[i-1]][perm[i]];
	delta_cdis+=(i-j-1)*(Vsd[i+k-1]-Vsd[i]);
	delta_cdis+=(-k)*(Vsd[i-1]-Vsd[j+1]);
	return delta_cdis;
}

int fast2opt(int i, int j, int *Vsd, int N, int *perm, int *Sb, int *Se, int cdis){

	int delta_dis=0;

	if(j==N-1) {
		delta_dis+=2*Sb[i];
		delta_dis+=(N-i-1)*(matrix_dis[perm[i]][perm[i+1]]+matrix_dis[perm[i]][perm[j]]);
		delta_dis+=(N-i-1)*(Vsd[j]-Vsd[i+1]);
	}
	else{
		delta_dis+=2*(Sb[i]+Se[j+1]);
		delta_dis+=(N-i-1)*(matrix_dis[perm[i]][perm[i+1]]+matrix_dis[perm[i]][perm[j]]);
		delta_dis+=(N-j-1)*(matrix_dis[perm[j]][perm[j+1]]+matrix_dis[perm[i+1]][perm[j+1]]);
		delta_dis+=(2*N-i-j-2)*(Vsd[j]-Vsd[i+1]);
	}


	return delta_dis-2*cdis;
}

int locate_nextcity(Struc_Sol *S, int curcity){
	int nexcity=-1;
	if(curcity==-1) {
		cout<<"error in lcoate nexcity"<<endl;
		exit(-2);
	}
	int poscur=S->position[curcity];
	nexcity=S->perm[poscur+1];

	return nexcity;
}


int locate_prevcity(Struc_Sol *S, int curcity){
	int precity=-1;

	if(curcity==0) return precity;
	if(curcity==-1) {
		cout<<"error in lcoate precity"<<endl;
		exit(-2);
	}

	int poscur=S->position[curcity];
	precity=S->perm[poscur-1];


	return precity;
}

int standard_ratio_select(Struc_Sol *S, int poscity, int num_current){
	int selcity=-1;
	int k=3;
	int maxisize=min(k,num_city-num_current);
	int cs_city[maxisize];
	int cnt=0;

	int city_find;

	/*remplir the cs_city*/
	for(int i=0;i<num_city;i++) FSel[i]=false;

	while(cnt<maxisize){
		city_find=-1;
		for(int i=0;i<num_city;i++){
			if(S->f_city[i]==1) continue;
			if(FSel[i]==true) continue;
			if(city_find==-1) city_find=i;
			else if(Profit_dis[poscity][i]>Profit_dis[poscity][city_find]) city_find=i;
		}
		FSel[city_find]=true;
		cs_city[cnt]=city_find;
		cnt++;
	}
	/*select from the cs*/
	if(cnt!=0) selcity=cs_city[rand()%cnt];
	else if(num_current!=num_city) {
		cout<<"error in standard ratio select"<<endl;
		exit(-1);
	}
	return selcity;
}


int standard_nearest_select(Struc_Sol *S, int poscity, int num_current){
	int selcity=-1;
	int k=3;
	int maxisize=min(k,num_city-num_current);
	int cs_city[maxisize];
	int cnt=0;

	int city_find;

	/*remplir the cs_city*/

	for(int i=0;i<num_city;i++) FSel[i]=false;

	while(cnt<maxisize){
		city_find=-1;
		for(int i=0;i<num_city;i++){
			if(S->f_city[i]==1) continue;
			if(FSel[i]==true) continue;
			if(city_find==-1) city_find=i;
			else if(matrix_dis[poscity][i]<matrix_dis[poscity][city_find]) city_find=i;
		}
		FSel[city_find]=true;
		cs_city[cnt]=city_find;
		cnt++;
	}
	/*select from the cs*/
	if(cnt!=0) selcity=cs_city[rand()%cnt];
	else if(num_current!=num_city) {
		cout<<"error in standard ratio select"<<endl;
		exit(-1);
	}

	return selcity;
}


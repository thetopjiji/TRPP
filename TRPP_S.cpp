//============================================================================
// Name        : TRPP_S.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "trpp_lib.h"
#include "Detail_TRPP.h"
#include "Main_TRPP_S.h"

//Declaration of functions
int parameters(int count, char *arguments[]);
void read_fiche(void);
void setdataStrucGraph(void);
void freedataStrucGraph(void);
void make_dis(void);
void normale_exec(void);
//functions

void make_dis(void){
	int i,j;
	for(i=0;i<num_city;i++)
		for(j=0;j<num_city;j++)
			if(i==j) matrix_dis[i][j]=10000000;
			else {
				matrix_dis[i][j]=round(sqrt((cordx[i]-cordx[j])*(cordx[i]-cordx[j])+(cordy[i]-cordy[j])*(cordy[i]-cordy[j])));
				matrix_dis[j][i]=matrix_dis[i][j];
			}
}

void setdataStrucGraph(void){
	cordx=(int *)get_vector(num_city);
	cordy=(int *)get_vector(num_city);
	profitA=(int *)get_vector(num_city);
	matrix_dis=(int **)get_matrix(num_city,num_city);
	Matrix_chosenT=(int **)get_matrix(num_city,num_city);
}

void freedataStrucGraph(void){
	free_vec(cordx);
	free_vec(cordy);
	free_vec(profitA);
	free_mat(matrix_dis,num_city);
	free_mat(Matrix_chosenT,num_city);
}

void read_fiche(void){

	ifstream FIC;
	int i, count_city=-1;
	int x,y,prof;

	FIC.open(benchmark);
	if ( FIC.fail() ){
	 cout << "### No way,check your fiche " << benchmark << endl;
	 exit(-1);
	}
	FIC>>count_city;

	num_city=count_city+1;
//	cout<<"num_city="<<num_city<<endl;

	setdataStrucGraph();

	for(i=0;i<num_city;i++){
		FIC>>x>>y>>prof;
		cordx[i]=x;
		cordy[i]=y;
		profitA[i]=prof;
	}
	make_dis();
}



int parameters(int count, char *arguments[])  {
	char *temp, filename[80] = "no";
	char *nf=filename;
//	char *wewant=filename;
//	char *token;

//	alreadybest=-1;

	strcpy(resultsFile, filename);
	strcpy(benchmark, filename);
	strcpy(nf, filename);

	while (count != 1) {
		temp = arguments[count - 2];
		if (strcmp(temp,"-i") == 0) strcpy(benchmark, arguments[count - 1]);
		else
			if (strcmp(temp,"--seed") == 0) seed= atoi(arguments[count - 1]);
		else
			if (strcmp(temp,"-rep") == 0) rep=arguments[count - 1];
		// else
		// 	if (strcmp(temp,"-iMs") == 0) Max_candidate=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-iSs") == 0) Max_smallset=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-idlk") == 0) dep_lk=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-idgd") == 0) diameter_gd=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-ipb1") == 0) perturb1=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-ipb2") == 0) perturb2=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-ipb3") == 0) perturb3=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-ipb4") == 0) perturb4=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-alpha") == 0) Alpha=atof(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-fph") == 0) Param_H=atof(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-fpm") == 0) Para_M=atof(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-fpb") == 0) Beta=atof(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-s") == 0) strcpy(inputsol, arguments[count - 1]);
//		else
//			if (strcmp(temp,"-p") == 0) strcpy(pure_name, arguments[count - 1]);
		else {  // unknow parameter
			return 0;
		}
		count = count - 2;
	}
	if (strcmp(benchmark, "no") == 0 || (alreadybest==-1)) {
		printf("enter error\n");
		exit(-1);
	}
	strcpy(nf, benchmark);

	Max_candidate=25;
	Max_smallset=5;
	dep_lk=10;
	diameter_gd=2;
	perturb1=2;
	perturb2=1;
	Param_H=5.9;
	Para_M=8.2;		// this parameter is not important and will not influence the final resutls.

	return 1;
}

void normale_exec(void){
	char final[50]="F";

	read_fiche();						//read graph and fill the link_list and seq_node.
//	cuttime=10.0;
	cuttime=(num_city-1)*1.0;

//	sprintf(name_final_result,"result_rep/%s.txtF%d",pure_name,rep);
//	sprintf(inputsol,"report/%dR",pure_name);

    strcpy(name_final_result,benchmark);
    strcat(name_final_result,final);
    strcat(name_final_result,rep);
    printf("%s\n",name_final_result);
/*******************************************************/
//  printf("%s\n",benchmark);

    algo_search();
    freedataStrucGraph();

}

int main(int argc, char **argv) {
	if(parameters(argc, argv)==0) exit(-1);
	srand(seed);
//	srand(time(NULL));
//	srand((unsigned int)time(NULL));

	normale_exec();


	return 0;
}

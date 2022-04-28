/*
 * struct.h
 *
 *  Created on: 10Step2020
 *      Author: ren
 */

#ifndef STRUCT_H_
#define STRUCT_H_

typedef struct struct_individual {
//	int *prevC;	//ll
//	int *nextC;	//ll
	int *perm;
	int *f_city;
	int *position;
	int profit;
	int Nsel;
	int cdis;

	/**New Struct**/
	int *vec_sd;
	int *Sb;
	int *Se;


} Struc_Sol;

#endif /* STRUCT_H_ */

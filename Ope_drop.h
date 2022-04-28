/*
 * OPERATOR.h
 *
 *  Created on: 11Step2020
 *      Author: DELL
 */

#ifndef OPE_DROP_H_
#define OPE_DROP_H_

#include "struct.h"

int Ope_drop(Struc_Sol *S, int &ite);		//One node drop
int drop_gab_bot(Struc_Sol *S);
int drop_gab_all(Struc_Sol *S);

int ope_delta_drop(Struc_Sol *S, int ind);

#endif /* OPE_INSERT_H_ */

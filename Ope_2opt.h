/*
 * OPERATOR.h
 *
 *  Created on: 11Step2020
 *      Author: DELL
 */

#ifndef OPE_2OPT_H_
#define OPE_2OPT_H_

#include "struct.h"

int Ope_2opt(Struc_Sol *S, int &ite);
int ope_delta_2opt(Struc_Sol *S, int node1, int node2);
#endif /* OPE_SWAP_H_ */

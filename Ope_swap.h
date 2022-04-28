/*
 * OPERATOR.h
 *
 *  Created on: 11Step2020
 *      Author: DELL
 */

#ifndef OPE_SWAP_H_
#define OPE_SWAP_H_

#include "struct.h"

int Ope_swap(Struc_Sol *S, int &ite);
int get_delta_swap(Struc_Sol *S, int node1, int node2);
int get_delta_swapad(Struc_Sol *S, int node1, int node2);
int get_pre_nex_cdis(Struc_Sol *S, int pos, int node, int prenode, int nexnode);

#endif /* OPE_SWAP_H_ */

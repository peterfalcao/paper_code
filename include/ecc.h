/*
 * ecc.h
 *
 *  Created on: 11 de set de 2019
 *      Author: LESC
 */

#ifndef INCLUDE_ECC_H_
#define INCLUDE_ECC_H_
#include "mem_map.h"

#define HAMMING


#ifdef CLC
#include "clc.h"
#endif

#ifdef MRSC
#include "MRSC.h"
#endif

#ifdef HAMMING
#include "hammingex.h"
#endif




#endif /* INCLUDE_ECC_H_ */

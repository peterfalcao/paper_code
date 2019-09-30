/*
 * clc.h
 *
 *  Created on: 12 de jul de 2019
 *      Author: Michelly Karen e Rodrigo Lima
 */

#ifndef INCLUDE_CLC_H_
#define INCLUDE_CLC_H_

#define L 5

#include <inttypes.h>
#include "mem_map.h"

answer CLC_encode(uint16_t mensagemOriginal, ECCRam* RAM_Mem_ECC);
uint16_t CLC_decode(ECCRam* RAM_Mem_ECC);

#endif /* INCLUDE_CLC_H_ */

/*
 * hammingex.h
 *
 *  Created on: 26 de ago de 2019
 *      Author: Pedro
 *      Modificacoes: Michelly e Gaspar
 */

#ifndef INCLUDE_HAMMINGEX_H_
#define INCLUDE_HAMMINGEX_H_

#include <stdint.h>
#include <inttypes.h>
#include "mem_map.h"

answer HAMM_encode(uint16_t mensagemOriginal, ECCRam* RAM_Mem_ECC);
uint16_t HAMM_decode(ECCRam* RAM_Mem_ECC);

#endif /* INCLUDE_HAMMINGEX_H_ */

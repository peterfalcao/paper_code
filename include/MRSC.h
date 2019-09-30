/*
 * MRSC.h
 *
 *  Created on: 26 de ago de 2019
 *      Author: Michelly Karen
 */

#ifndef INCLUDE_MRSC_H_
#define INCLUDE_MRSC_H_

#include <stdint.h>
#include <inttypes.h>
#include "mem_map.h"

answer MRSC_encode(uint16_t mensagemOriginal, ECCRam* RAM_Mem_ECC);
uint16_t MRSC_decode(ECCRam* RAM_Mem_ECC);

#endif /* INCLUDE_MRSC_H_ */

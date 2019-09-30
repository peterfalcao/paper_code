#ifndef _func_EI_H_
#define _func_EI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>


int vetunique(int vet[8],int numerros);
void error_injector(uint8_t vet_error_hex[5],int num_erros,int Lin, int Col,uint8_t **matrix_error);
#endif

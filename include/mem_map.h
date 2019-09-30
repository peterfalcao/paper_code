/*
 * Criado em: 21 de 05 de 2019
 * Autor: Equipe de desenvolvimento do SACODE (www.sacode.lesc.ufc.br)
 */
#ifndef INCLUDE_MEM_MAP_H_
#define INCLUDE_MEM_MAP_H_
#include <stdint.h>

//-----------------------------------------------DEFINES---------------------------------------------------------------------------

//defines que permitem termos um acesso direto a memoria RAM
//o endereço deve ser o de onde a memória RAM começa
#define RAM_mem_ECC ((ECCRam *)(0x08000000)) // devemos colocar o endereco correto de inicio da RAM

//-----------------------------------------------STRUCTS----------------------------------------------------------------------------
    /*
     * essa struct começara no primeiro endereço onde a RAM e monitorada pelo ECC
     */
typedef volatile struct ECCRam
{
    uint8_t RAM_cel0;   //  como as celulas de memoria devem ser colocadas, tem que ser o numero exato de endereços que o ECC engloba
    uint8_t RAM_cel1;
    uint8_t RAM_cel2;   //
    uint8_t RAM_cel3;
    uint8_t RAM_cel4;
} ECCRam;

typedef enum
{
    OP_OK = 1,
    OP_ERROR = 0
}answer;


//-----------------------------------------------FUNCOES-------------------------------------------------------------------------------

uint8_t read_ram_cel(uint8_t cel_data); //retorna o valor de um espaco de memoria da RAM
uint8_t modify_ram_cel(uint8_t *cel_data, uint8_t matrix_line); //faz um XOR com o valor lido na memoria RAM
answer write_ram_cel(volatile uint8_t *cel_data, uint8_t word);  // ler o valor em determinado endereco da memoria RAM

#endif

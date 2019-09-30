
/*
 * Criado em: 21 de 05 de 2019
 * Autor: Equipe de desenvolvimento do SACODE (www.sacode.lesc.ufc.br)
 */

#include "mem_map.h"

/*! \fn uint32_t read_ram_cel(uint32_t cel_data)
    \brief funcao criada por precaucao, apenas retorna o valor de um espaco de memoria da RAM
    \param cel_data: endereco de memoria (definido na struct ECCRam no arquivo mem_map.h) a ser lido
*/
uint8_t read_ram_cel(uint8_t cel_data){

    return  cel_data;
}


/*! \fn uint32_t modify_ram_cel(uint32_t *cel_data, uint32_t matrix_line)
    \brief essa funcao deve fazer um XOR com o valor lido na memoria RAM
           a insercao de erro pode ser selecionada por uma estrutura matricial
           a ser definida e colocada no lugar da variavel "matrix_line"
    \param cel_data: ponteiro que aponta para o ender. de memoria (definido na struct ECCRam no arquivo mem_map.h) a ser mosificado
    \param matrix_line: linha da matriz de erro para inserir em determinada posicao
*/
uint8_t modify_ram_cel(uint8_t *cel_data, uint8_t matrix_line){

    *cel_data  = (*cel_data)^(matrix_line);
    return *cel_data;
}

/*! \fn answer write_ram_cel(uint32_t *cel_data, uint32_t word)
    \brief funcao criada para ler o valor em determinado endereco da memoria RAM
    \param *cel_data: ponteiro que aponta para o ender. de memoria (definido na struct ECCRam no arquivo mem_map.h) a ser mosificado
    \param word: palavra a ser armazenada no endereco onde o ponteiro aponta
*/
answer write_ram_cel(volatile uint8_t *cel_data, uint8_t word){

    *cel_data  = word;
    if(*cel_data == word){
        return OP_OK;
    }
    else{
        return OP_ERROR;
    }
}

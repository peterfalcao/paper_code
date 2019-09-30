/*
 * Criado em: 21 de 05 de 2019
 * Autor: Equipe de desenvolvimento do SACODE (www.sacode.lesc.ufc.br)
 */

#include "ecc.h"
#include <stdio.h>


void main(void){
   volatile struct ECCRam* RAM_mem_ECC_Address = RAM_mem_ECC;
   answer res;
   uint16_t dadoOriginal=0xCAFE;
   uint16_t dadoLido;
   int i=0,cnt_sucess=0,cnt_fail=0;
   int j = 0;
   uint8_t **matrix_error;
   uint8_t *matrix_lines;
   uint8_t cols;

   matrix_lines = &cols;
   matrix_error = &matrix_lines;
   /*
    * 0x05d6 -> 0x0C58DE6B
    * 0x5134 -> 0x58193745
    */
      matrix_error=(int **) malloc(4 * sizeof(int *));
      for( i=0;i<4;i++)
      {
          matrix_error[i]=(int *) malloc(8 * sizeof(int));
          for(j=0;j<8;j++)
              *(*(matrix_error + i)+ j) = 0;
      }


   for(i=0;i<100;i++)
   {
       printf("%d\n",i);
#ifdef CLC
       res = CLC_encode(dadoOriginal, RAM_mem_ECC_Address);
       dadoLido = CLC_decode(RAM_mem_ECC_Address);
#endif

 //MRSC
#ifdef MRSC
       res = MRSC_encode(dadoOriginal,RAM_mem_ECC_Address);
       dadoLido = MRSC_decode(RAM_mem_ECC_Address);
#endif

 //Hamming
#ifdef HAMMING
          res = HAMM_encode(dadoOriginal,RAM_mem_ECC_Address,matrix_error);
          dadoLido = HAMM_decode(RAM_mem_ECC_Address);
#endif
         // printf("\nPalavra lida: %x\n",dadoLido);
          if(dadoLido==dadoOriginal){
              cnt_sucess++;
         // printf("erro corrigido");
          }
          else
              cnt_fail++;

    }
     // printf("\nPalavras Corrigidas: %d",cnt_sucess);
     //   printf("\nPalavras Erradas: %d",cnt_fail);
      if (res){
             while(1);
                 }

}

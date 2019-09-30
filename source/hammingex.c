/*
 * hammingex.c
 *
 *  Created on: 26 de ago de 2019
 *      Author: Pedro
 *      Modificacoes: Michelly e Gaspar
 */


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "hammingex.h"
 uint8_t **matnew;
 uint8_t   matriz[4][8];

 uint8_t vet_error_hex[5]={0,0,0,0,0};
answer HAMM_encode(uint16_t mensagemOriginal, ECCRam* RAM_Mem_ECC){
    answer status;
    /* int par[4][4],i,j;
    int cod[4][8];
    int intervcod[4][8];
     */
    uint8_t input[4][4];
    uint8_t par[4][4],cod[4][8],intervcod[4][8];
    uint8_t mensagemCodificada[4];
    uint32_t mensagemCodificadaFinal;
    int i,j;

    input[0][0]=((mensagemOriginal) & (0x8000))>>15;
    input[0][1]=((mensagemOriginal) & (0x4000))>>14;
    input[0][2]=((mensagemOriginal) & (0x2000))>>13;
    input[0][3]=((mensagemOriginal) & (0x1000))>>12;

    input[1][0]=((mensagemOriginal) & (0x0800))>>11;
    input[1][1]=((mensagemOriginal) & (0x0400))>>10;
    input[1][2]=((mensagemOriginal) & (0x0200))>>9;
    input[1][3]=((mensagemOriginal) & (0x0100))>>8;

    input[2][0]=((mensagemOriginal) & (0x0080))>>7;
    input[2][1]=((mensagemOriginal) & (0x0040))>>6;
    input[2][2]=((mensagemOriginal) & (0x0020))>>5;
    input[2][3]=((mensagemOriginal) & (0x0010))>>4;

    input[3][0]=((mensagemOriginal) & (0x0008))>>3;
    input[3][1]=((mensagemOriginal) & (0x0004))>>2;
    input[3][2]=((mensagemOriginal) & (0x0002))>>1;
    input[3][3]=((mensagemOriginal) & (0x0001));

    for(i=0;i<4;i++){
        par[i][0]=input[i][0]^input[i][2]^input[i][3];
        par[i][1]=input[i][0]^input[i][1]^input[i][3];
        par[i][2]=input[i][1]^input[i][2]^input[i][3];
        par[i][3]=input[i][0]^input[i][1]^input[i][2]^input[i][3]^par[i][0]^par[i][1]^par[i][2];
    }
    //Matriz codificada
        for(i=0;i<4;i++)
            for(j=0;j<8;j++){
                if(j<4)
                    cod[i][j]=input[i][j];
                else if(j>=4)
                    cod[i][j]=par[i][j-4];
            }
        //Matriz com interleaving
        for(j=0;j<8;j++)
            for(i=0;i<4;i++){
                if((2+j)%2==0)
                intervcod[j/2][i]=cod[i][j];
                else if((2+j)%2==1)
                intervcod[j/2][i+4]=cod[i][j];
            }
    /*    printf(" \nMatriz inicial:\n");
           for(i=0;i<4;i++)
             {
                 for(j=0;j<8;j++)
                     printf("%d",intervcod[i][j]);
                     printf("\n");
             }
*/
        matnew=error_injector(vet_error_hex,5,4,8);
       printf("Matriz de IE:\n");
        for(i=0;i<4;i++)
          {
              for(j=0;j<8;j++)
                  printf("%d",matnew[i][j]);
                  printf("\n");

          }

        for(i=0;i<4;i++)
            {
                for(j=0;j<8;j++)
                    if(matnew[i][j]==1)
                        intervcod[i][j]=(!(intervcod[i][j]));
            }

        free(matnew);
  /*     printf("Matriz final: \n");
          for(i=0;i<4;i++)
            {
                for(j=0;j<8;j++)
                    printf("%d",intervcod[i][j]);
                    printf("\n");
            }
*/
        for(i=0;i<4;i++)
            mensagemCodificada[i]=128*intervcod[i][0] + 64*intervcod[i][1] + 32*intervcod[i][2] + 16*intervcod[i][3] + 8*intervcod[i][4] + 4*intervcod[i][5] + 2*intervcod[i][6] + 1*intervcod[i][7];
        // DADO QUE SERÁ SALVO NA MEMÓRIA
        mensagemCodificadaFinal=mensagemCodificada[0]<<24 | mensagemCodificada[1]<<16 | mensagemCodificada[2]<<8 | mensagemCodificada[3];
        status = write_ram_cel(&RAM_Mem_ECC->RAM_cel0, mensagemCodificada[0]);
        status = write_ram_cel(&RAM_Mem_ECC->RAM_cel1, mensagemCodificada[1]);
        status = write_ram_cel(&RAM_Mem_ECC->RAM_cel2, mensagemCodificada[2]);
        status = write_ram_cel(&RAM_Mem_ECC->RAM_cel3, mensagemCodificada[3]);

    return status;
}
uint16_t HAMM_decode(ECCRam* RAM_Mem_ECC){
        int  npar[4][4],c1[4],c2[4],c3[4],c4[4],i,j;
        uint8_t data_in[4][8],data_org[4][8];
        uint8_t  matrizCodificada[4];
        uint16_t  decodificacaoFinal;
        uint32_t data_in_orl;

        matrizCodificada[0] = read_ram_cel(RAM_Mem_ECC->RAM_cel0);
        matrizCodificada[1] = read_ram_cel(RAM_Mem_ECC->RAM_cel1);
        matrizCodificada[2] = read_ram_cel(RAM_Mem_ECC->RAM_cel2);
        matrizCodificada[3] = read_ram_cel(RAM_Mem_ECC->RAM_cel3);

        data_in_orl = matrizCodificada[0]<<24 | matrizCodificada[1]<<16 | matrizCodificada[2]<<8 | matrizCodificada[3];

        //-----REPASSANDO O DADO PARA BIT-------//
        data_in[0][0]=((data_in_orl) & (0x80000000))>>31;
        data_in[0][1]=((data_in_orl) & (0x40000000))>>30;
        data_in[0][2]=((data_in_orl) & (0x20000000))>>29;
        data_in[0][3]=((data_in_orl) & (0x10000000))>>28;
        data_in[0][4]=((data_in_orl) & (0x08000000))>>27;
        data_in[0][5]=((data_in_orl) & (0x04000000))>>26;
        data_in[0][6]=((data_in_orl) & (0x02000000))>>25;
        data_in[0][7]=((data_in_orl) & (0x01000000))>>24;

        data_in[1][0]=((data_in_orl) & (0x00800000))>>23;
        data_in[1][1]=((data_in_orl) & (0x00400000))>>22;
        data_in[1][2]=((data_in_orl) & (0x00200000))>>21;
        data_in[1][3]=((data_in_orl) & (0x00100000))>>20;
        data_in[1][4]=((data_in_orl) & (0x00080000))>>19;
        data_in[1][5]=((data_in_orl) & (0x00040000))>>18;
        data_in[1][6]=((data_in_orl) & (0x00020000))>>17;
        data_in[1][7]=((data_in_orl) & (0x00010000))>>16;

        data_in[2][0]=((data_in_orl) & (0x00008000))>>15;
        data_in[2][1]=((data_in_orl) & (0x00004000))>>14;
        data_in[2][2]=((data_in_orl) & (0x00002000))>>13;
        data_in[2][3]=((data_in_orl) & (0x00001000))>>12;
        data_in[2][4]=((data_in_orl) & (0x00000800))>>11;
        data_in[2][5]=((data_in_orl) & (0x00000400))>>10;
        data_in[2][6]=((data_in_orl) & (0x00000200))>>9;
        data_in[2][7]=((data_in_orl) & (0x00000100))>>8;

        data_in[3][0]=((data_in_orl) & (0x00000080))>>7;
        data_in[3][1]=((data_in_orl) & (0x00000040))>>6;
        data_in[3][2]=((data_in_orl) & (0x00000020))>>5;
        data_in[3][3]=((data_in_orl) & (0x00000010))>>4;
        data_in[3][4]=((data_in_orl) & (0x00000008))>>3;
        data_in[3][5]=((data_in_orl) & (0x00000004))>>2;
        data_in[3][6]=((data_in_orl) & (0x00000002))>>1;
        data_in[3][7]=((data_in_orl) & (0x00000001));
        //-----REPASSANDO O DADO PARA BIT-------//
        //-----ORGANIZANDO OS BITS DE DADOS-----//
        data_org[0][0]=data_in[0][0];
        data_org[0][1]=data_in[0][4];
        data_org[0][2]=data_in[1][0];
        data_org[0][3]=data_in[1][4];
        data_org[0][4]=data_in[2][0];
        data_org[0][5]=data_in[2][4];
        data_org[0][6]=data_in[3][0];
        data_org[0][7]=data_in[3][4];

        data_org[1][0]=data_in[0][1];
        data_org[1][1]=data_in[0][5];
        data_org[1][2]=data_in[1][1];
        data_org[1][3]=data_in[1][5];
        data_org[1][4]=data_in[2][1];
        data_org[1][5]=data_in[2][5];
        data_org[1][6]=data_in[3][1];
        data_org[1][7]=data_in[3][5];

        data_org[2][0]=data_in[0][2];
        data_org[2][1]=data_in[0][6];
        data_org[2][2]=data_in[1][2];
        data_org[2][3]=data_in[1][6];
        data_org[2][4]=data_in[2][2];
        data_org[2][5]=data_in[2][6];
        data_org[2][6]=data_in[3][2];
        data_org[2][7]=data_in[3][6];

        data_org[3][0]=data_in[0][3];
        data_org[3][1]=data_in[0][7];
        data_org[3][2]=data_in[1][3];
        data_org[3][3]=data_in[1][7];
        data_org[3][4]=data_in[2][3];
        data_org[3][5]=data_in[2][7];
        data_org[3][6]=data_in[3][3];
        data_org[3][7]=data_in[3][7];

        //-----ORGANIZANDO OS BITS DE DADOS-----//
        /*printf("Matriz codificada normal\n");
        for(i=0;i<4;i++){
            for(j=0;j<8;j++)
                printf("%d ",data_org[i][j]);
            printf("\n");
        }*/
        for(i=0;i<4;i++){
            npar[i][0]=data_org[i][0]^data_org[i][2]^data_org[i][3];
            npar[i][1]=data_org[i][0]^data_org[i][1]^data_org[i][3];
            npar[i][2]=data_org[i][1]^data_org[i][2]^data_org[i][3];
            npar[i][3]=data_org[i][0]^data_org[i][1]^data_org[i][2]^data_org[i][3]^data_org[i][4]^data_org[i][5]^data_org[i][6];
            c1[i]= data_org[i][4]^npar[i][0];
            c2[i]= data_org[i][5]^npar[i][1];
            c3[i]= data_org[i][6]^npar[i][2];
            c4[i]= data_org[i][7]^npar[i][3];
        }
        for(i=0;i<4;i++){

            if (c4[i] == 1){
                if (c1[i]==1 && c2[i]==1 && c3[i]==0){
                    data_org[i][0]=!data_org[i][0];
                    //printf("\n Erro simples no bit 1 \n");
                }
                else if (c1[i]==0 && c2[i]==1 && c3[i]==1){
                    data_org[i][1]=!data_org[i][1];
                    //printf("\n Erro simples no bit 2 \n");
                }
                else if (c1[i]==1 && c2[i]==0 && c3[i]==1){
                    data_org[i][2]=!data_org[i][2];
                    //printf("\n Erro simples no bit 3 \n");
                }
                else if (c1[i]==1 && c2[i]==1 && c3[i]==1){
                    data_org[i][3]=!data_org[i][3];
                    //printf("\n Erro simples no bit 4 \n");
                }
                else if (c1[i]==0 && c2[i]==0 && c3[i]==0){
                    //printf("\n Erro na paridade \n");
                }
            }
        }
        for(i=0;i<4;i++)
            matrizCodificada[i]=8*data_org[i][0] + 4*data_org[i][1] + 2*data_org[i][2] + 1*data_org[i][3];
        // DADO QUE SER SALVO NA MEMRIA
        decodificacaoFinal=matrizCodificada[0]<<12 | matrizCodificada[1]<<8 | matrizCodificada[2]<<4 | matrizCodificada[3];
        //printf("\nDado final decodificado: %x\n",decodificacaoFinal);
        return decodificacaoFinal;
    }

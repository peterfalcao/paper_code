/*
 * MRSC.c
 *
 *  Created on: 26 de ago de 2019
 *      Author: Gaspar
 *      Modificacoes: Michelly
 */


#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "MRSC.h"


answer MRSC_encode(uint16_t mensagemOriginal, ECCRam* RAM_Mem_ECC){

    answer status;
    uint8_t P[4]={0,0,0,0},Di[4]={0,0,0,0};
    uint8_t data_in[4][4];
    uint8_t encoded_data[4][8];
    uint8_t dadoCodificado8[4];
    //uint32_t dadoCodificado32;

    //int somaSCB=0, Reg1=0, Reg2=0;
    //uint8_t enc_data[4][8];
    int i,j;
    //Convertendo para valores binários
    data_in[0][0]=((mensagemOriginal) & (0x8000))>>15;
    data_in[0][1]=((mensagemOriginal) & (0x4000))>>14;
    data_in[0][2]=((mensagemOriginal) & (0x2000))>>13;
    data_in[0][3]=((mensagemOriginal) & (0x1000))>>12;

    data_in[1][0]=((mensagemOriginal) & (0x0800))>>11;
    data_in[1][1]=((mensagemOriginal) & (0x0400))>>10;
    data_in[1][2]=((mensagemOriginal) & (0x0200))>>9;
    data_in[1][3]=((mensagemOriginal) & (0x0100))>>8;

    data_in[2][0]=((mensagemOriginal) & (0x0080))>>7;
    data_in[2][1]=((mensagemOriginal) & (0x0040))>>6;
    data_in[2][2]=((mensagemOriginal) & (0x0020))>>5;
    data_in[2][3]=((mensagemOriginal) & (0x0010))>>4;

    data_in[3][0]=((mensagemOriginal) & (0x0008))>>3;
    data_in[3][1]=((mensagemOriginal) & (0x0004))>>2;
    data_in[3][2]=((mensagemOriginal) & (0x0002))>>1;
    data_in[3][3]=((mensagemOriginal) & (0x0001));

    //printf("\n%d %d %d %d",data_in[i][0],data_in[i][1],data_in[i][2],data_in[i][3]);
    //Repassar valores da dados para o dado codificado
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            encoded_data[i][j]=data_in[i][j];
    //Codificando os bits de Checagem
    encoded_data[0][6]= data_in[0][0]^data_in[0][2];
    encoded_data[0][7]= data_in[0][1]^data_in[0][3];

    encoded_data[1][6]= data_in[1][0]^data_in[1][2];
    encoded_data[1][7]= data_in[1][1]^data_in[1][3];

    encoded_data[2][6]= data_in[2][0]^data_in[2][2];
    encoded_data[2][7]= data_in[2][1]^data_in[2][3];

    encoded_data[3][6]= data_in[3][0]^data_in[3][2];
    encoded_data[3][7]= data_in[3][1]^data_in[3][3];

    //Codificando os bits de paridade
    for(i=0;i<4;i++){
        P[i]= data_in[0][i]^data_in[1][i]^data_in[2][i]^data_in[3][i];
        //printf("P(%d): %d\n",i,P[i]);
    }
    encoded_data[2][4]=P[0];
    encoded_data[3][4]=P[1];
    encoded_data[2][5]=P[2];
    encoded_data[3][5]=P[3];

    //Codificando os bits de Diagonal
    Di[0]=data_in[0][0]^data_in[1][1]^data_in[2][0]^data_in[3][1];
    Di[1]=data_in[0][1]^data_in[1][0]^data_in[2][1]^data_in[3][0];
    Di[2]=data_in[0][2]^data_in[1][3]^data_in[2][2]^data_in[3][3];
    Di[3]=data_in[0][3]^data_in[1][2]^data_in[2][3]^data_in[3][2];


    encoded_data[0][4]=Di[0];
    encoded_data[1][4]=Di[1];
    encoded_data[0][5]=Di[2];
    encoded_data[1][5]=Di[3];

    //enc_data print

    /*for(i=0;i<4;i++)
        for(j=0;j<8;j++){
            printf("%d ",encoded_data[i][j]);
            if(j==7)
                printf("\n");
        }
    */
    //converter para o formato pedido
    for(i=0;i<4;i++)
        dadoCodificado8[i]=128*encoded_data[i][0] + 64*encoded_data[i][1] + 32*encoded_data[i][2] + 16*encoded_data[i][3] + 8*encoded_data[i][4] + 4*encoded_data[i][5] + 2*encoded_data[i][6] + 1*encoded_data[i][7];
    // DADO QUE SERÁ SALVO NA MEMÓRIA

    //dadoCodificado32=dadoCodificado8[0]<<24 | dadoCodificado8[1]<<16 | dadoCodificado8[2]<<8 | dadoCodificado8[3];
    //printf("\nDado final codificado: %x\n",encoded_data_final);

    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel0, dadoCodificado8[0]);
    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel1, dadoCodificado8[1]);
    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel2, dadoCodificado8[2]);
    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel3, dadoCodificado8[3]);
    return status;
}

uint16_t MRSC_decode(ECCRam* RAM_Mem_ECC){

    uint8_t SCB[4][2];
    uint8_t SP[4]={0,0,0,0},SDi[4]={0,0,0,0};
    uint8_t data_in[4][8];
    uint8_t decoded_data[4][4];
    uint8_t matrizCodificada[4];
    uint16_t decodificacaoFinal;
    uint32_t dadoCodificadoFinal;
    int somaSCB=0, Reg1=0, Reg2=0;
    //uint8_t enc_data[4][8];
    int i,j;
    //Conversão do dado
    matrizCodificada[0] = read_ram_cel(RAM_Mem_ECC->RAM_cel0);
    matrizCodificada[1] = read_ram_cel(RAM_Mem_ECC->RAM_cel1);
    matrizCodificada[2] = read_ram_cel(RAM_Mem_ECC->RAM_cel2);
    matrizCodificada[3] = read_ram_cel(RAM_Mem_ECC->RAM_cel3);

    dadoCodificadoFinal = matrizCodificada[0]<<24 | matrizCodificada[1]<<16 | matrizCodificada[2]<<8 | matrizCodificada[3];

    data_in[0][0]=((dadoCodificadoFinal) & (0x80000000))>>31;
    data_in[0][1]=((dadoCodificadoFinal) & (0x40000000))>>30;
    data_in[0][2]=((dadoCodificadoFinal) & (0x20000000))>>29;
    data_in[0][3]=((dadoCodificadoFinal) & (0x10000000))>>28;
    data_in[0][4]=((dadoCodificadoFinal) & (0x08000000))>>27;
    data_in[0][5]=((dadoCodificadoFinal) & (0x04000000))>>26;
    data_in[0][6]=((dadoCodificadoFinal) & (0x02000000))>>25;
    data_in[0][7]=((dadoCodificadoFinal) & (0x01000000))>>24;

    data_in[1][0]=((dadoCodificadoFinal) & (0x00800000))>>23;
    data_in[1][1]=((dadoCodificadoFinal) & (0x00400000))>>22;
    data_in[1][2]=((dadoCodificadoFinal) & (0x00200000))>>21;
    data_in[1][3]=((dadoCodificadoFinal) & (0x00100000))>>20;
    data_in[1][4]=((dadoCodificadoFinal) & (0x00080000))>>19;
    data_in[1][5]=((dadoCodificadoFinal) & (0x00040000))>>18;
    data_in[1][6]=((dadoCodificadoFinal) & (0x00020000))>>17;
    data_in[1][7]=((dadoCodificadoFinal) & (0x00010000))>>16;

    data_in[2][0]=((dadoCodificadoFinal) & (0x00008000))>>15;
    data_in[2][1]=((dadoCodificadoFinal) & (0x00004000))>>14;
    data_in[2][2]=((dadoCodificadoFinal) & (0x00002000))>>13;
    data_in[2][3]=((dadoCodificadoFinal) & (0x00001000))>>12;
    data_in[2][4]=((dadoCodificadoFinal) & (0x00000800))>>11;
    data_in[2][5]=((dadoCodificadoFinal) & (0x00000400))>>10;
    data_in[2][6]=((dadoCodificadoFinal) & (0x00000200))>>9;
    data_in[2][7]=((dadoCodificadoFinal) & (0x00000100))>>8;

    data_in[3][0]=((dadoCodificadoFinal) & (0x00000080))>>7;
    data_in[3][1]=((dadoCodificadoFinal) & (0x00000040))>>6;
    data_in[3][2]=((dadoCodificadoFinal) & (0x00000020))>>5;
    data_in[3][3]=((dadoCodificadoFinal) & (0x00000010))>>4;
    data_in[3][4]=((dadoCodificadoFinal) & (0x00000008))>>3;
    data_in[3][5]=((dadoCodificadoFinal) & (0x00000004))>>2;
    data_in[3][6]=((dadoCodificadoFinal) & (0x00000002))>>1;
    data_in[3][7]=((dadoCodificadoFinal) & (0x00000001));

    /*for(i=0;i<4;i++)
        for(j=0;j<8;j++)
            decoded_data[i][j]=data_in[i][j];*/


    //Sindrome dos bits de Checagem
    SCB[0][0]= data_in[0][0]^data_in[0][2]^data_in[0][6];
    SCB[0][1]= data_in[0][1]^data_in[0][3]^data_in[0][7];

    SCB[1][0]= data_in[1][0]^data_in[1][2]^data_in[1][6];
    SCB[1][1]= data_in[1][1]^data_in[1][3]^data_in[1][7];

    SCB[2][0]= data_in[2][0]^data_in[2][2]^data_in[2][6];
    SCB[2][1]= data_in[2][1]^data_in[2][3]^data_in[2][7];

    SCB[3][0]= data_in[3][0]^data_in[3][2]^data_in[3][6];
    SCB[3][1]= data_in[3][1]^data_in[3][3]^data_in[3][7];
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            somaSCB=somaSCB + SCB[i][j];
    //Sindrome da paridade
    SP[0]= data_in[0][0]^data_in[1][0]^data_in[2][0]^data_in[3][0]^data_in[2][4];
    SP[1]= data_in[0][1]^data_in[1][1]^data_in[2][1]^data_in[3][1]^data_in[3][4];
    SP[2]= data_in[0][2]^data_in[1][2]^data_in[2][2]^data_in[3][2]^data_in[2][5];
    SP[3]= data_in[0][3]^data_in[1][3]^data_in[2][3]^data_in[3][3]^data_in[3][5];
    //Sindrome da Diagonal
    SDi[0]=data_in[0][0]^data_in[1][1]^data_in[2][0]^data_in[3][1]^data_in[0][4];
    SDi[1]=data_in[0][1]^data_in[1][0]^data_in[2][1]^data_in[3][0]^data_in[1][4];
    SDi[2]=data_in[0][2]^data_in[1][3]^data_in[2][2]^data_in[3][3]^data_in[0][5];
    SDi[3]=data_in[0][3]^data_in[1][2]^data_in[2][3]^data_in[3][2]^data_in[1][5];

    if(((SP[0]|SP[1]|SP[2]|SP[3] != 0) && (SDi[0]|SDi[1]|SDi[2]|SDi[3] != 0)) | somaSCB > 1)
    {
        //printf("\nCorrecao iniciada\n");
        Reg1 = SDi[0] + SDi[1] + SP[0] + SP[1];
        Reg2 = SDi[2] + SDi[3] + SP[2] + SP[3];
        //printf("\nReg1: %d \nReg2: %d \n",Reg1,Reg2);
        if(Reg1 > Reg2)
        {
            for(i=0;i<4;i++)
                for(j=0;j<2;j++)
                    data_in[i][j] = data_in[i][j] ^ SCB[i][j];

         //   printf("\nReg 1 corrigida\n");
        }
        else if(Reg1 > Reg2)
        {
            for(i=0;i<4;i++)
                for(j=2;j<4;j++)
                    data_in[i][j] = data_in[i][j] ^ SCB[i][j-2];

            //printf("\nReg 2 corrigida\n");
        }
        else if(Reg1 == Reg2)
        {
                data_in[0][1]= data_in[0][1] ^ SCB[0][1];
                data_in[0][2]= data_in[0][2] ^ SCB[0][0];
                data_in[1][1]= data_in[1][1] ^ SCB[1][1];
                data_in[1][2]= data_in[1][2] ^ SCB[1][0];
                data_in[2][1]= data_in[2][1] ^ SCB[2][1];
                data_in[2][2]= data_in[2][2] ^ SCB[2][0];
                data_in[3][1]= data_in[3][1] ^ SCB[3][1];
                data_in[3][2]= data_in[3][2] ^ SCB[3][0];
                //printf("\nEmpate corrigido\n");
        }
    }
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            decoded_data[i][j]=data_in[i][j];

    for(i=0;i<4;i++)
        matrizCodificada[i]=8*decoded_data[i][0] + 4*decoded_data[i][1] + 2*decoded_data[i][2] + 1*decoded_data[i][3];
    // DADO QUE SERÁ SALVO NA MEMÓRIA
    decodificacaoFinal=matrizCodificada[0]<<12 | matrizCodificada[1]<<8 | matrizCodificada[2]<<4 | matrizCodificada[3];
    //printf("\nDado final decodificado: %x\n",decoded_data_final);

    return decodificacaoFinal;

}

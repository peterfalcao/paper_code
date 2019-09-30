/*
 * clc.c
 *
 *  Created on: 12 de jul de 2019
 *      Author: Michelly Karen e Rodrigo Lima
 */

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "clc.h"

#define L 5
#define C 8

answer CLC_encode(uint16_t mensagemOriginal, ECCRam* RAM_Mem_ECC){
    answer status;
    uint8_t mensagemCodificada[L];
    short i=0,j=0;

    for(i=0;i<L-1;i++){
        mensagemCodificada[i]=((mensagemOriginal>>(12-i*4))&0b1111)<<4;
    }
    for(i=0;i<L-1;i++){
        for(j=C-1;j>=0;j--){
            if(((mensagemCodificada[i]>>6)^(mensagemCodificada[i]>>5)^(mensagemCodificada[i]>>4))&0x01){
                mensagemCodificada[i]|=(1<<1);
            }
            if(((mensagemCodificada[i]>>7)^(mensagemCodificada[i]>>5)^(mensagemCodificada[i]>>4))&0x01){
                mensagemCodificada[i]|=(1<<2);
            }
            if(((mensagemCodificada[i]>>7)^(mensagemCodificada[i]>>6)^(mensagemCodificada[i]>>4))&0x01){
                mensagemCodificada[i]|=(1<<3);
            }
            if(((mensagemCodificada[i]>>7)^(mensagemCodificada[i]>>6)^(mensagemCodificada[i]>>5)^(mensagemCodificada[i]>>4)^(mensagemCodificada[i]>>3)^(mensagemCodificada[i]>>2)^(mensagemCodificada[i]>>1))&0x01){
                mensagemCodificada[i]|=(1<<0);
            }
        }
    }
    for(j=C-1;j>=0;j--){
        if(((mensagemCodificada[0]>>j)^(mensagemCodificada[1]>>j)^(mensagemCodificada[2]>>j)^(mensagemCodificada[3]>>j))&0x01){
            mensagemCodificada[4]|=(1<<j);
        }
    }
    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel0, mensagemCodificada[0]);
    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel1, mensagemCodificada[1]);
    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel2, mensagemCodificada[2]);
    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel3, mensagemCodificada[3]);
    status = write_ram_cel(&RAM_Mem_ECC->RAM_cel4, mensagemCodificada[4]);

    return status;
}

uint16_t CLC_decode(ECCRam* RAM_Mem_ECC){

    uint8_t matrizCodificada[L];
    uint8_t sindromeColuna=0;
    uint8_t sindromeLinhas[L];
    uint8_t sindromeParLinhas=0;
    uint8_t numLinSindrome=0;
    uint16_t decodificacaoFinal=0;

    short i=0,j=0,c=0;

    matrizCodificada[0] = read_ram_cel(RAM_Mem_ECC->RAM_cel0);
    matrizCodificada[1] = read_ram_cel(RAM_Mem_ECC->RAM_cel1);
    matrizCodificada[2] = read_ram_cel(RAM_Mem_ECC->RAM_cel2);
    matrizCodificada[3] = read_ram_cel(RAM_Mem_ECC->RAM_cel3);
    matrizCodificada[4] = read_ram_cel(RAM_Mem_ECC->RAM_cel4);

    for(c=0;c<2;c++){
        sindromeColuna=0;

        sindromeParLinhas=0;
        numLinSindrome=0;
        memset(sindromeLinhas, 0, sizeof(sindromeLinhas)); //reinicia as variáveis

        for(i=0;i<C;i++){
            if(((matrizCodificada[0]>>i)^(matrizCodificada[1]>>i)^(matrizCodificada[2]>>i)^(matrizCodificada[3]>>i)^(matrizCodificada[4]>>i))&0x01){
                sindromeColuna|=(1<<i);
            }
        }
        for(i=0;i<L;i++){
            if(((matrizCodificada[i]>>7)^(matrizCodificada[i]>>6)^(matrizCodificada[i]>>4)^(matrizCodificada[i]>>3))&0x01){
                sindromeLinhas[i]|=(1<<0);
            }
            if(((matrizCodificada[i]>>7)^(matrizCodificada[i]>>5)^(matrizCodificada[i]>>4)^(matrizCodificada[i]>>2))&0x01){
                sindromeLinhas[i]|=(1<<1);
            }
            if(((matrizCodificada[i]>>6)^(matrizCodificada[i]>>5)^(matrizCodificada[i]>>4)^(matrizCodificada[i]>>1))&0x01){
                sindromeLinhas[i]|=(1<<2);
            }
            if(((matrizCodificada[i]>>7)^(matrizCodificada[i]>>6)^(matrizCodificada[i]>>5)^(matrizCodificada[i]>>4)^(matrizCodificada[i]>>3)^(matrizCodificada[i]>>2)^(matrizCodificada[i]>>1)^(matrizCodificada[i]>>0))&0x01){
                sindromeParLinhas|=(1<<i);
            }
        }
        numLinSindrome=((sindromeLinhas[0])||(sindromeParLinhas&(0b01<<0)))+((sindromeLinhas[1])||(sindromeParLinhas&(0b01<<1)))+((sindromeLinhas[2])||(sindromeParLinhas&(0b01<<2)))+((sindromeLinhas[3])||(sindromeParLinhas&(0b01<<3)))+((sindromeLinhas[4])||(sindromeParLinhas&(0b01<<4)));
        for(i=0;i<4;i++){
            if((numLinSindrome==1)&&(sindromeLinhas[i]!=0)){
                for(j=0;j<C;j++){
                    if(((sindromeColuna>>j)&0b01)!=0b0){
                        matrizCodificada[i]^=(1<<j);
                    }
                }
            } else if(((sindromeParLinhas>>i)&0b01)&&(sindromeLinhas[i]==0b000)){
                for(j=0;j<C;j++){
                    if(((sindromeColuna>>j)&0b01)!=0b0){
                        matrizCodificada[i]^=(1<<j);
                    }
                }
            } else if(sindromeLinhas[i]!=0b000){
                if(((sindromeParLinhas>>i)&0b01)==0b0){
                    for(j=0;j<C;j++){
                        if(((sindromeColuna>>j)&0b01)!=0b0){
                            matrizCodificada[i]^=(1<<j);
                        }
                    }
                } else if(((sindromeParLinhas>>i)&0x01)==0b01){
                    switch (sindromeLinhas[i]){
                        case 0b001:
                            matrizCodificada[i]^=(1<<1);
                        break;
                        case 0b010:
                            matrizCodificada[i]^=(1<<2);
                        break;
                        case 0b100:
                            matrizCodificada[i]^=(1<<3);
                        break;
                        case 0b111:
                            matrizCodificada[i]^=(1<<4);
                        break;
                        case 0b110:
                            matrizCodificada[i]^=(1<<5);
                        break;
                        case 0b101:
                            matrizCodificada[i]^=(1<<6);
                        break;
                        case 0b011:
                            matrizCodificada[i]^=(1<<7);
                        break;
                        case 0b000:

                        break;
                    }
                }
            }
        }
    }

    for(i=0;i<L-1;i++){
        decodificacaoFinal = (decodificacaoFinal<<4);
        decodificacaoFinal = ((decodificacaoFinal)|(matrizCodificada[i]>>4));
    }
    return decodificacaoFinal;
}

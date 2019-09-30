//
// Created by LESC on 26/09/2019.
//

#include "func_EI.h"
int vetunique(int vet[8], int numerros)
{
    int i=0,j=0,count_distintos=0,num_distintos=0;

    for(i=0;i<numerros;i++)
    {
        for(j=0;j<numerros;j++)
            if(vet[i]!=vet[j])
            {
                //printf("count_distintos %d",count_distintos);
                count_distintos++;
            }
        if(count_distintos==numerros-1)
            num_distintos++;
        count_distintos=0;
    }
    //printf("\nDistintos de todos %d, Numerros %d",num_distintos,numerros);
    if(num_distintos<numerros)
    {
        return 1;//se retorna 0, vetor não contem quantidade de distintos correta
    }
    else
        return 0;// caso contrário é um vetor válido
}
uint8_t **error_injector(uint8_t vet_error_hex[5],int num_erros,int Lin, int Col)
{
    int num[8] = {99,99,99,99,99,99,99,99};
    int tentativa=0;
    int L=Lin,C=Col;
    uint8_t **matrix_error;
    uint8_t posError = 0;
    uint8_t posError2 = 0;
    uint8_t posError3 = 0;
    uint8_t posError4 = 0;
    uint8_t posError5 = 0;
    uint8_t posError6 = 0;
    uint8_t posError7 = 0;
    uint8_t posError8 = 0;
    uint8_t errorSum = 0;
    short testCount = 0;
    short errorNumber;
    int vectorLenght;
    int posErrorVector[8]={99,99,99,99,99,99,99,99};
    short validPosition = 0;
    int vprincipal[8]={-(C+1),-(C),-(C-1),-1,1,(C-1),C,(C+1)};
    int vp1[5]={-1,1,(C-1),C,(C+1)};
    int vp2[5]={-1,1,-(C-1),-C,-(C+1)};
    int vp3[3]={1,C,(C+1)};
    int vp4[3]={1,-C,-(C-1)};
    int vp5[5]={1,-C,C,(C+1),-(C-1)};
    int vp6[3]={-1,(C-1),C};
    int vp7[3]={-1,-(C+1),-C};
    int vp8[5]={-1,-C,-(C+1),(C-1),C};
    int i,j;

    matrix_error=(int **) malloc(L * sizeof(int *));
    for(i=0;i<L;i++)
    {
        matrix_error[i]=(int *) malloc(C * sizeof(int));
        for(j=0;j<C;j++)
            matrix_error[i][j]=0;
    }
    /*for(i=0;i<L;i++)
    {
        for(j=0;j<C;j++)
            printf("%d ",matrix_error[i][j]);
        printf("\n");
    }*/
    //printf("Digite o numero de erros a ser inserido: ");
    //scanf("%d",&errorNumber);
    //printf("Digite o numero de rodadas: ");
    //scanf("%d",&testNumber);
    //00 01 02 03 04 05 06 07
    //08 09 10 11 12 13 14 15
    //16 17 18 19 20 21 22 23
    //24 25 26 27 28 29 30 31
    //32 33 34 35 36 37 38 39
    errorNumber=num_erros;
    while(testCount<1)
    {
        srand(time(0)+testCount);
        for(i=0;i<8;i++)
            num[i]=99;
        switch(errorNumber)
        {
            case 1:
                posError = rand()%(L*C-1);
                vectorLenght = 1;
                posErrorVector[0] = posError;
                testCount++;
                break;

            case 2:
                //srand(time(0));
                posError = rand()%(L*C) + 1;
                num[0] = posError;
                num[1] = posError + vprincipal[rand()%8];
                //printf("\n%d %d %d\n",num[0]-1,num[1]-1,num[2]-1);
                for(i=2;i<=(C-1);i++)
                {
                    if(posError==i)
                    {
                        num[1] = posError +vp1[rand()%5];
                    }
                    else if(posError==((L-1)*C)+i)
                    {
                        //printf("\n AQUI 1 %d  %d",num,vp2[rand()%5]);
                        num[1] = posError + vp2[rand()%5];
                    }
                    //num=rand()%5;

                }
                for(i=1;i<=L;i++)
                {
                    if(posError==(i-1)*C +1)
                    {
                        if(posError==1)
                        {
                            num[1] =posError + vp3[rand()%3];
                        }
                        else if(posError==((L-1)*C)+1)
                        {
                            num[1] =posError + vp4[rand()%3];
                        }
                        else
                        {
                            num[1] =posError + vp5[rand()%5];
                        }//printf("\n AQUI 2");
                    }
                    else if(posError==i*C)
                    {
                        if(posError==C)
                        {
                            num[1] =posError + vp6[rand()%3];
                        }
                        else if(posError==(C*L))
                        {
                            num[1] =posError + vp7[rand()%3];
                        }
                        else
                        {
                            num[1] =posError + vp8[rand()%5];
                        }
                        //printf("\n AQUI 2");
                    }
                }
                for(i=0;i<errorNumber;i++)
                    num[i]=num[i]-1;
                vectorLenght = 2;
                tentativa++;

                /*for(i=0;i<errorNumber;i++)
                    printf("%d ",num[i]);
                    printf("tentativa:%d\n",tentativa);*/
                break;

            case 3:
                while(vetunique(num,3))
                {
                    //srand(time(0));
                    posError = rand()%(L*C) + 1;
                    num[0] = posError;
                    num[1] = posError + vprincipal[rand()%8];
                    num[2] = posError + vprincipal[rand()%8];
                    //printf("\n%d %d %d\n",num[0]-1,num[1]-1,num[2]-1);
                    for(i=2;i<=(C-1);i++)
                    {
                        if(posError==i)
                        {
                            num[1] = posError +vp1[rand()%5];
                            num[2] = posError +vp1[rand()%5];
                        }
                        else if(posError==((L-1)*C)+i)
                        {
                            //printf("\n AQUI 1 %d  %d",num,vp2[rand()%5]);
                            num[1] = posError + vp2[rand()%5];
                            num[2] = posError + vp2[rand()%5];
                        }
                        //num=rand()%5;

                    }
                    for(i=1;i<=L;i++)
                    {
                        if(posError==(i-1)*C +1)
                        {
                            if(posError==1)
                            {
                                num[1] =posError + vp3[rand()%3];
                                num[2] =posError + vp3[rand()%3];
                            }
                            else if(posError==((L-1)*C)+1)
                            {
                                num[1] =posError + vp4[rand()%3];
                                num[2] =posError + vp4[rand()%3];
                            }
                            else
                            {
                                num[1] =posError + vp5[rand()%5];
                                num[2] =posError + vp5[rand()%5];
                            }//printf("\n AQUI 2");
                        }
                        else if(posError==i*C)
                        {
                            if(posError==C)
                            {
                                num[1] =posError + vp6[rand()%3];
                                num[2] =posError + vp6[rand()%3];
                            }
                            else if(posError==(C*L))
                            {
                                num[1] =posError + vp7[rand()%3];
                                num[2] =posError + vp7[rand()%3];
                            }
                            else
                            {
                                num[1] =posError + vp8[rand()%5];
                                num[2] =posError + vp8[rand()%5];
                            }
                            //printf("\n AQUI 2");
                        }
                    }
                    for(i=0;i<errorNumber;i++)
                        num[i]=num[i]-1;
                    vectorLenght = 2;
                    tentativa++;
                    //posErrorVector[0] = posError;
                    //posErrorVector[1] = posError2 + num[0];

                }
                /*for(i=0;i<errorNumber;i++)
                    printf("%d ",num[i]);
                    printf("tentativa:%d\n",tentativa);*/
                break;

            case 4:
                while(vetunique(num,4))
                {
                    //srand(time(0));
                    posError = rand()%(L*C) + 1;
                    num[0] = posError;
                    num[1] = posError + vprincipal[rand()%8];
                    num[2] = posError + vprincipal[rand()%8];
                    num[3] = posError + vprincipal[rand()%8];
                    //printf("\n%d %d %d\n",num[0]-1,num[1]-1,num[2]-1);
                    for(i=2;i<=(C-1);i++)
                    {
                        if(posError==i)
                        {
                            num[1] = posError +vp1[rand()%5];
                            num[2] = posError +vp1[rand()%5];
                            num[3] = posError +vp1[rand()%5];
                        }
                        else if(posError==((L-1)*C)+i)
                        {
                            //printf("\n AQUI 1 %d  %d",num,vp2[rand()%5]);
                            num[1] = posError + vp2[rand()%5];
                            num[2] = posError + vp2[rand()%5];
                            num[3] = posError + vp2[rand()%5];
                        }
                        //num=rand()%5;

                    }
                    for(i=1;i<=L;i++)
                    {
                        if(posError==(i-1)*C +1)
                        {
                            if(posError==1)
                            {
                                num[1] =posError + vp3[rand()%3];
                                num[2] =posError + vp3[rand()%3];
                                num[3] =posError + vp3[rand()%3];
                            }
                            else if(posError==((L-1)*C)+1)
                            {
                                num[1] =posError + vp4[rand()%3];
                                num[2] =posError + vp4[rand()%3];
                                num[3] =posError + vp4[rand()%3];
                            }
                            else
                            {
                                num[1] =posError + vp5[rand()%5];
                                num[2] =posError + vp5[rand()%5];
                                num[3] =posError + vp5[rand()%5];
                            }//printf("\n AQUI 2");
                        }
                        else if(posError==i*C)
                        {
                            if(posError==C)
                            {
                                num[1] =posError + vp6[rand()%3];
                                num[2] =posError + vp6[rand()%3];
                                num[3] =posError + vp6[rand()%3];
                            }
                            else if(posError==(C*L))
                            {
                                num[1] =posError + vp7[rand()%3];
                                num[2] =posError + vp7[rand()%3];
                                num[3] =posError + vp7[rand()%3];
                            }
                            else
                            {
                                num[1] =posError + vp8[rand()%5];
                                num[2] =posError + vp8[rand()%5];
                                num[3] =posError + vp8[rand()%5];
                            }
                            //printf("\n AQUI 2");
                        }
                    }
                    for(i=0;i<errorNumber;i++)
                        num[i]=num[i]-1;
                    vectorLenght = 2;
                    tentativa++;
                    //posErrorVector[0] = posError;
                    //posErrorVector[1] = posError2 + num[0];

                }
                /*for(i=0;i<errorNumber;i++)
                    printf("%d ",num[i]);
                    printf("tentativa:%d\n",tentativa);*/
                break;
            case 5:
                while(vetunique(num,5))
                {
                    //srand(time(0));
                    posError = rand()%(L*C) + 1;
                    num[0] = posError;
                    num[1] = posError + vprincipal[rand()%8];
                    num[2] = posError + vprincipal[rand()%8];
                    num[3] = posError + vprincipal[rand()%8];
                    num[4] = posError + vprincipal[rand()%8];
                    //printf("\n%d %d %d\n",num[0]-1,num[1]-1,num[2]-1);
                    for(i=2;i<=(C-1);i++)
                    {
                        if(posError==i)
                        {
                            num[1] = posError +vp1[rand()%5];
                            num[2] = posError +vp1[rand()%5];
                            num[3] = posError +vp1[rand()%5];
                            num[4] = posError +vp1[rand()%5];
                        }
                        else if(posError==((L-1)*C)+i)
                        {
                            //printf("\n AQUI 1 %d  %d",num,vp2[rand()%5]);
                            num[1] = posError + vp2[rand()%5];
                            num[2] = posError + vp2[rand()%5];
                            num[3] = posError + vp2[rand()%5];
                            num[4] = posError + vp2[rand()%5];
                        }
                        //num=rand()%5;

                    }
                    for(i=1;i<=L;i++)
                    {
                        if(posError==(i-1)*C +1)
                        {
                            if(posError==1)
                            {
                                num[1] =posError + vp3[rand()%3];
                                num[2] =posError + vp3[rand()%3];
                                num[3] =posError + vp3[rand()%3];
                                num[4] =posError + vp3[rand()%3];
                            }
                            else if(posError==((L-1)*C)+1)
                            {
                                num[1] =posError + vp4[rand()%3];
                                num[2] =posError + vp4[rand()%3];
                                num[3] =posError + vp4[rand()%3];
                                num[4] =posError + vp4[rand()%3];
                            }
                            else
                            {
                                num[1] =posError + vp5[rand()%5];
                                num[2] =posError + vp5[rand()%5];
                                num[3] =posError + vp5[rand()%5];
                                num[4] =posError + vp5[rand()%5];
                            }//printf("\n AQUI 2");
                        }
                        else if(posError==i*C)
                        {
                            if(posError==C)
                            {
                                num[1] =posError + vp6[rand()%3];
                                num[2] =posError + vp6[rand()%3];
                                num[3] =posError + vp6[rand()%3];
                                num[4] =posError + vp6[rand()%3];
                            }
                            else if(posError==(C*L))
                            {
                                num[1] =posError + vp7[rand()%3];
                                num[2] =posError + vp7[rand()%3];
                                num[3] =posError + vp7[rand()%3];
                                num[4] =posError + vp7[rand()%3];
                            }
                            else
                            {
                                num[1] =posError + vp8[rand()%5];
                                num[2] =posError + vp8[rand()%5];
                                num[3] =posError + vp8[rand()%5];
                                num[4] =posError + vp8[rand()%5];
                            }
                            //printf("\n AQUI 2");
                        }
                    }
                    for(i=0;i<errorNumber;i++)
                        num[i]=num[i]-1;
                    vectorLenght = 2;
                    tentativa++;
                    //posErrorVector[0] = posError;
                    //posErrorVector[1] = posError2 + num[0];

                }
                /*for(i=0;i<errorNumber;i++)
                    printf("%d ",num[i]);
                    printf("tentativa:%d\n",tentativa);*/
                break;
            default:
                break;
        }
        testCount++;
    }//end of while
    /*for(i=0;i<L;i++)
        for(j=0;j<C;j++)
            matrix_error[i][j]=0;
    */
    for(i=0;i<num_erros;i++)
    {
        matrix_error[num[i]/C][num[i]%C]=1;
    }
   /* for(i=0;i<L;i++)
    {
        for(j=0;j<C;j++)
            printf("%d ",matrix_error[i][j]);
        printf("\n");
    }*/
    /*for(i=0;i<L;i++)
    {
        vet_error_hex[i]=128*matrix_error[i][0] + 64*matrix_error[i][1] + 32*matrix_error[i][2] + 16*matrix_error[i][3] + 8*matrix_error[i][4] + 4*matrix_error[i][5] + 2*matrix_error[i][6] + 1*matrix_error[i][7];
        printf("%x ",vet_error_hex[i]);
    }*/
    return matrix_error;
}

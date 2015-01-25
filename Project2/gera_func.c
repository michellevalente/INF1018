/* Lucas Hardman Gomes Campos França - 1113567 - 3WB */
/* Michelle Andrade Valente da Silva - 1312828 - 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gera_func.h"

/* Libera Função */
void libera_func (void* func)
{
    free(func);
}

/* Calcula posicao na pilha */
int posicao_pilha(int n, Parametro params[], int posicao)
{
    int i;
    int tam_bytes = 8;
    for(i=0;i<n;i++)
    {
        if((params[i].amarrado == 0) && (params[i].posicao < posicao))
        {
                if ((params[i].tipo) == DOUBLE_PAR)
                    tam_bytes += 8;
                else
                    tam_bytes += 4;  
        }
          
    }
    return tam_bytes;
}

void * gera_func (void* f, int n, Parametro params[])
{
    static unsigned char FuncaoInicio[] = {0x55,0x89,0xe5}; /* push %ebp ; mov %esp, %ebp */
    static unsigned char FimFuncao[] = {0x89,0xec,0x5d,0xc3}; /* mov %ebp, %esp ; pop %ebp ; ret */
    static unsigned char Call[] = {0xe8,0xfc,0xff,0xff,0xff}; /* call generico */
    unsigned char * codigo = (unsigned char*) malloc(sizeof(unsigned char) * 255); /* codigo da funcao gerada */
    unsigned char* parametro;
    int iterador = 0, iterador2 ;
    int instrucoes = 0, CallFuncao, DepoisCall, endFunc, endProximaInstrucao, endCall, tam_bytes;

    /* Coloca o inicio da funcao no codigo[] */
    for(iterador = 0 ; iterador < 3 ; iterador ++ , instrucoes++)
    {
        codigo[iterador] = FuncaoInicio[iterador];
    }

    /* Loop para percorrer todos os parametros */
    for( iterador = n - 1; iterador >= 0 ; iterador--)
    {
        /* Se o parametro for amarrado */
        if(params[iterador].amarrado)
        {
	    /* mov $valoramarrado, %ecx */
            codigo[instrucoes] = 0xb9 ; 

            if((params[iterador].tipo) == CHAR_PAR)
            {
                /* Armazenando o char no codigo[] */
                codigo[instrucoes + 1] = params[iterador].valor.v_char;

                /* Colocando o padding no codigo[] */
                for(iterador2 = 2 ; iterador2 < 5; iterador2++)
                {
                    codigo[instrucoes + iterador2] = 0;
                }

                instrucoes += 5;
            }

            else if((params[iterador].tipo) == INT_PAR)
            {
                
		parametro = (unsigned char *) &params[iterador].valor.v_int;

		/* Armazenando o int no codigo[] */
		for(iterador2 = 0; iterador2 < 4 ; iterador2++)
		{
			codigo[instrucoes + (iterador2 +1)] = *(parametro + iterador2);
		}

		instrucoes += 5;

            }

            else if((params[iterador].tipo) == PTR_PAR)
            {
                parametro = (unsigned char*) &params[iterador].valor.v_ptr;

                /* Armazenando o ponteiro no codigo[] */
                for(iterador2 = 0; iterador2 < 4 ; iterador2++)
                {
                    codigo[instrucoes + (iterador2 +1)] = *(parametro + iterador2);
                }
                instrucoes += 5;
            }

            else if((params[iterador].tipo) == DOUBLE_PAR)
            {
                parametro = (unsigned char*) &params[iterador].valor.v_double;

                /* Armazenando o double no codigo[] */
                codigo[instrucoes + 1] = *(parametro + 4);
                codigo[instrucoes + 2] = *(parametro + 5);
                codigo[instrucoes + 3] = *(parametro + 6);
                codigo[instrucoes + 4] = *(parametro + 7);
                codigo[instrucoes + 5] = 0x51; 
                codigo[instrucoes + 6] = 0xb9; 
                codigo[instrucoes + 7] = *(parametro + 0);
                codigo[instrucoes + 8] = *(parametro + 1);
                codigo[instrucoes + 9] = *(parametro + 2);
                codigo[instrucoes + 10] = *(parametro + 3);

                instrucoes += 11;
            }

	    /* push %ecx */
            codigo[instrucoes] = 0x51;

            instrucoes++;

        }

        /* Se o parametro nao for amarrado */
        else
        {
            if((params[iterador].tipo) == INT_PAR)
            {
          
                tam_bytes = posicao_pilha(n, params,params[iterador].posicao);
                codigo[instrucoes + 0] = 0x8b; 
                codigo[instrucoes + 1] = 0x4d;
                codigo[instrucoes + 2] = tam_bytes;
                instrucoes += 3;
            }

            else if((params[iterador].tipo) == CHAR_PAR)
            {
              
             	tam_bytes = posicao_pilha(n, params,params[iterador].posicao);
             	codigo[instrucoes + 0] = 0x8b; 
                codigo[instrucoes + 1] = 0x4d;
                codigo[instrucoes + 2] = tam_bytes;
                instrucoes += 3;

            }

            else if((params[iterador].tipo) == PTR_PAR)
            {
		tam_bytes = posicao_pilha(n, params,params[iterador].posicao);
                codigo[instrucoes + 0] = 0x8b; 
                codigo[instrucoes + 1] = 0x4d;
                codigo[instrucoes + 2] = tam_bytes;
                instrucoes += 3;
            }
            else if((params[iterador].tipo) == DOUBLE_PAR)
            {
              	/* Primeira parte do double */
              	tam_bytes = posicao_pilha(n, params,params[iterador].posicao);
		tam_bytes += 4;	
                codigo[instrucoes + 0] = 0x8b;
                codigo[instrucoes + 1] = 0x4d;
                codigo[instrucoes + 2] = tam_bytes;
                codigo[instrucoes + 3] = 0x51;
                  
		/* Segunda parte do double */
                tam_bytes -= 4;
		codigo[instrucoes + 4] = 0x8b; 
                codigo[instrucoes + 5] = 0x4d;
                codigo[instrucoes + 6] = tam_bytes;           
		instrucoes += 7;
            }

            codigo[instrucoes] = 0x51;
            instrucoes++;

        }

    }
   

        /* Armazenando um call generico */
        for(iterador = 0 ; iterador < 5 ; iterador++, instrucoes++)
        {
            codigo[instrucoes] = Call[iterador];
        }


        /* Armazenando o fim do codigo */
        for(iterador = 0 ; iterador < 4 ; iterador++,instrucoes++)
        {
            codigo[instrucoes] = FimFuncao[iterador];
        }


        /* Andando no codigo até inicio do Call */
        iterador=0;
        while(codigo[iterador]!= 0xe8)
        {
            iterador++;
        }
	/* Indice do Call */
        CallFuncao = iterador + 5;

	/* Indice da instrucao apos o call */
        DepoisCall = iterador + 1;

        /* Colocando o endereco da funcao no call */
        endFunc = (int)f;
        endProximaInstrucao = (int)&codigo[CallFuncao];
        endCall = endFunc - endProximaInstrucao;
        *((int*) &codigo[DepoisCall]) = endCall;

    return codigo;
}

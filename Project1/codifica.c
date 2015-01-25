/* Lucas Hardman Gomes Campos França 1113567 */
/* Michelle Andrade Valente da Silva 1312828 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "codifica.h"

int zig(int n);

int codifica (void* valores, char* campos, FILE* arquivo){
    
   unsigned char * val = (unsigned char *) valores;
   unsigned char bitvet2[5];
   unsigned char bitvet;
   unsigned char chave;
        
   int i, j, zigzag,  n, tipo;
   unsigned char padding = 0;
   unsigned int len;
   int nseq=0;
 
   while(*campos != '\0'){

	if(*campos != 's' && *campos!= 'i'){
		return -1;
	}

    if(*campos == 's'){
           tipo = 2;	 /* Declara tipo String */
           nseq++;	 /* Incrementa numero de sequencia */
           bitvet = ( (nseq << 3) | tipo);	 /* Cria a chave */
           fwrite(&bitvet, sizeof(char), 1, arquivo);  /* Escreve a chave no arquivo */
           len = strlen(((char *)val));	 /* Calcula o tamanho da string */ 

		   bitvet2[0] = ( len >> 0 ) & (0x7f);	/* Separa o inteiro em vetores */
           bitvet2[1] = (len >> 7) & (0x7f);
           bitvet2[2] = (len >> 14) & (0x7f);
           bitvet2[3] = (len >> 21) & (0x7f);
           bitvet2[4] = (len >> 28) & (0x7f);

           for(i=0; bitvet2[i] != 0; i++);
           bitvet2[i-1] &= 0x7F;	 /* Coloca o primeiro bit do ultimo byte como zero*/
           if(i!=1){
               for (j =0 ; j< i-1; j++){
                   bitvet2[j] |= 0x80;	 /* Coloca o primeiro bit dos bytes ate o penultimo como 1 */
   
               }   
			}
			fwrite(bitvet2,sizeof(char),i,arquivo); 	/* Escreve tamanho da string no arquivo */
			fwrite(val, sizeof(char), len, arquivo);    /* Escreve a string no arquivo */
            val += ++len;						/* desloca o ponteiro */
            padding = 4 - ((len-padding) % 4);  /* calcula novo padding */
            if (padding == 4) padding = 0;   
            if (*(campos + 1) != 's') {   /* se o próximo elemento for int */
                val += padding;
                padding = 0;    /* zera o padding no final da sequência de strings */
            }
       }

       else if(*campos == 'i'){
           tipo = 0;	 /* Declara tipo inteiro */
           nseq++;	 /* Incrementa n��mero de sequÊncia */  
           n = (*((int *) val));	 /* Guarda valor inteiro */
           zigzag = zig(n);	 /* Faz o ZigZag */
           chave = ( (nseq << 3) | tipo);	 /* Cria a chave */
           fwrite(&chave, sizeof(char), 1, arquivo);	/* Escreve a chave no arquivo */

           bitvet2[0] = ( zigzag >> 0 ) & (0x7f);	/* Separa o inteiro em vetores */
           bitvet2[1] = (zigzag >> 7) & (0x7f);
           bitvet2[2] = (zigzag >> 14) & (0x7f);
           bitvet2[3] = (zigzag >> 21) & (0x7f);
           bitvet2[4] = (zigzag >> 28) & (0x7f);

           for(i=0; bitvet2[i] != 0; i++);
           bitvet2[i-1] &= 0x7F;	 /* Coloca o primeiro bit do ultimo byte como zero*/
           if(i!=1){
               for (j =0 ; j< i-1; j++){
                   bitvet2[j] |= 0x80;	 /* Coloca o primeiro bit dos bytes até o penultimo como 1 */
   
               }   
           }
 
           fwrite(bitvet2,sizeof(char),i,arquivo); /* Escreve os bytes modificados do int no arquivo */

           val +=4;	 /* Avança para o próximo valor */

       }

		campos++;
        
   }
   return 1;

}

int zig(int n){
   return (n << 1) ^ (n >> 31);
}
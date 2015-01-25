/* Lucas Hardman Gomes Campos França 1113567 */
/* Michelle Andrade Valente da Silva 1312828 */
#include <stdio.h>
#include "codifica.h"

typedef struct {
    int i1;
    int i2;
} S1;

typedef struct {
    int i1;
    int i2;
    char s1[3];
    int i3;
} S2;

typedef struct {
    int n1;
    char s1[4];
} S3;

int main (void) {
    S1 e1 = {1, -1};
    S2 e2 = {150, -150, "oi", 65632};
    S3 e3 = {-1,"abc"};
    FILE *fp1 = fopen("output1.bin", "wb");
    FILE *fp2 = fopen("output2.bin", "wb");
    FILE *fp3 = fopen("output3.bin", "wb");
    
    codifica(&e1, "ii", fp1);
    fclose(fp1);
    codifica(&e2, "iisi", fp2);
    fclose(fp2);
    codifica(&e3, "is", fp3);
    fclose(fp3);
    return 0;
}
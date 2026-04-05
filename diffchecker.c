#include <stdio.h>

int main()
{
    char arq1nome[32];
    char arq2nome[32];

    scanf("%s %s", arq1nome, arq2nome);
    FILE* arq1;
    FILE* arq2;

    arq1 = fopen(arq1nome, "r");
    arq2 = fopen(arq2nome, "r");

    char poop;
    for(int i = 0; i < 46; i++)
    {
        poop = fgetc(arq1);
        printf("%d ", poop);
    }
    printf("\n");
    for(int i = 0; i < 47; i++)
    {
        poop = fgetc(arq2);
        printf("%d ", poop);
    }
    fclose(arq1);
    fclose(arq2);
}
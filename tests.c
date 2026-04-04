#include <stdio.h>
#include <string.h>

int main()
{
    char entrada[] = "frase1,,aposdelim";
    char* copia_entrada = entrada;
    char* campo;
    campo = strsep(&copia_entrada, ",");
    campo = strsep(&copia_entrada, ",");
    printf("%x\n", *campo);
    printf("%x\n", 0);
    printf("%x\n", 12);
}
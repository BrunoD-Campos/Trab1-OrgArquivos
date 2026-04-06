/*
Implementação da funcionalidade 5 de Inserção 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Estruturas.c"
#include "FuncAuxiliares.c"
#include "fornecidas.c"

void INSERT(){
    // Lendo valores do input
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome);

    int nroInserts;
    scanf("%d", &nroInserts);

    FILE* arqBIN;
    if(!(arqBIN = fopen(arqBIN_nome, "rb+")))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    
}
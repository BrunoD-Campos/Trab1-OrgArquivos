/*
Implementando funcionalidade de inserção 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include "FuncAuxiliares.c"
#include "Estruturas.c"
#include "fornecidas.c"

void lerRegistroTerminal(REGISTRO* novoRegistro)
{
    char strAtual[50] = "";
    ScanQuoteString(strAtual);
    
}

void INSERT()
{
    char arqBIN_nome[32];
    scanf("%s",arqBIN_nome);
    
    FILE* arqBIN;
    
    if(!(arqBIN = fopen(arqBIN_nome,"r")))   // Abre para leitura em binário
    {
        printf("Falha no processamento do arquivo->\n");
        return;
    }

    int qtdInserts;
    scanf("%d", &qtdInserts);


}
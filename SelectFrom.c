/*
Implementação da funcionalidade 2 de Select 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include "Estruturas.c"


// Função para ler o registro de cabeçalho de um arquivo e armazenar em uma struct CABECALHO

void SELECT(){
    char arqBIN_nome[32];
    scanf("%s",arqBIN_nome);
    
    FILE* arqBIN;
    
    if(!(arqBIN = fopen(arqBIN_nome,"r")))   // Abre para leitura em binário
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    int PosicaoRRN = 0; // Começa no registro 0 já que irá ler o arquivo inteiro    
    REGISTRO reg;   // Registro 

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    
    // Caso não existam registros no arquivo.
    if(cabecalho.proxRRN == 0)
    {
        printf("Registro inexistente.");
        return;
    }

    while(PosicaoRRN < cabecalho.proxRRN){
        LerRegistroBin(arqBIN, &reg, PosicaoRRN);
        imprimirRegistro(&reg);
        printf("\r\n");
        PosicaoRRN++;
    }
    
    fclose(arqBIN); 
}
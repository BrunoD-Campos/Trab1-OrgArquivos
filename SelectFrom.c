/*
Implementação da funcionalidade 2 de Select 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include "FuncAuxiliares.c"
#include "Estruturas.c"

// Função para ler o registro de cabeçalho de um arquivo e armazenar em uma struct CABECALHO

void SELECT(){
    char arqBIN_nome[32];
    scanf("%s",arqBIN_nome);
    
    FILE* arqBIN;
    
    if(!(arqBIN = fopen(arqBIN_nome,"rb")))   // Abre para leitura em binário
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

    // Flag para definir se todos os registros não estão removidos
    bool existe_um = false;

    while(PosicaoRRN < cabecalho.proxRRN){
        LerRegistroBin(arqBIN, &reg, PosicaoRRN);
        if(reg.removido == '0')
        {
            imprimirRegistro(&reg);
            existe_um = true;
        }
        PosicaoRRN++;
    }
    if(!existe_um)
        printf("Registro inexistente.");

    fclose(arqBIN); 
}
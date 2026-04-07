/*
Implementando funcionalidade de inserção 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include "FuncAuxiliares.c"
#include "Estruturas.c"
//#include "fornecidas.c"

void lerRegistroTerminal(REGISTRO* novoRegistro)
{
    novoRegistro->removido = '0';
    novoRegistro->proximo = -1;
    char strAtual[50] = "";
    ScanQuoteString(strAtual);
    leIntCampoBusca(&novoRegistro->codEstacao, strAtual);
    ScanQuoteString(strAtual);
    leStringCampoBusca(&novoRegistro->tamNomeEstacao, &novoRegistro->nomeEstacao, strAtual);
    ScanQuoteString(strAtual);
    leIntCampoBusca(&novoRegistro->codLinha, strAtual);
    ScanQuoteString(strAtual);
    leStringCampoBusca(&novoRegistro->tamNomeLinha, &novoRegistro->nomeLinha, strAtual);
    ScanQuoteString(strAtual);
    leIntCampoBusca(&novoRegistro->codProxEstacao, strAtual);
    ScanQuoteString(strAtual);
    leIntCampoBusca(&novoRegistro->distProxEstacao, strAtual);
    ScanQuoteString(strAtual);
    leIntCampoBusca(&novoRegistro->codLinhaIntegra, strAtual);
    ScanQuoteString(strAtual);
    leIntCampoBusca(&novoRegistro->codEstIntegra, strAtual);

    imprimirRegistro(novoRegistro);
    printf("\n");
    return;
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

    int contador = 0;

    while(contador < qtdInserts)
    {
        REGISTRO registroInserido;
        lerRegistroTerminal(&registroInserido);
        free(registroInserido.nomeEstacao);
        free(registroInserido.nomeLinha);
        contador++;
    }
    
    fclose(arqBIN);

    return;
}
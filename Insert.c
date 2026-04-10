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

    return;
}

void INSERT()
{
    char arqBIN_nome[32];
    scanf("%s",arqBIN_nome);
    
    FILE* arqBIN;
    
    if(!(arqBIN = fopen(arqBIN_nome,"rb+")))   // Abre para leitura em binário
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    rewind(arqBIN);

    // Define o arquivo como inconsistente após abri-lo para escrita
    char status = '0';
    fwrite(&status, sizeof(char), 1, arqBIN);

    int qtdInserts;
    scanf("%d", &qtdInserts);

    int contador = 0;

    while(contador < qtdInserts)
    {
        REGISTRO registroInserido;
        lerRegistroTerminal(&registroInserido);
        
        if(cabecalho.topo == -1)
        {
            fseek(arqBIN, TAM_CABECALHO+cabecalho.proxRRN*TAM_REGISTRO, SEEK_SET);
            EscreverRegistroBin(arqBIN, &registroInserido);
            cabecalho.proxRRN++;
        }
        else
        {
            fseek(arqBIN, TAM_CABECALHO+cabecalho.topo*TAM_REGISTRO+1, SEEK_SET);
            int regRemovidoProximo;
            fread(&regRemovidoProximo, sizeof(int), 1, arqBIN);
            fseek(arqBIN, -5, SEEK_CUR);
            EscreverRegistroBin(arqBIN, &registroInserido);
            cabecalho.topo = regRemovidoProximo;
        }

        // free() seguro com base em como leRegistroTerminal foi implementado
        free(registroInserido.nomeEstacao);
        free(registroInserido.nomeLinha);

        contador++;
    }
    cabecalho.status = '1';
    atualizarCabecalho(arqBIN, &cabecalho);

    fclose(arqBIN);

    return;
}
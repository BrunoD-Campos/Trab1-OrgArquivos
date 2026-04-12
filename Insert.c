/*
Implementação da funcionalidade 4 de Insert 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include "FuncGerais.c"
#include "Estruturas.c"

// Função usada para ler todos os campos na ordem certa diretamente do terminal onde o 
// código está sendo rodado
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

// Trecho principal da funcionalidade de Insert, que adiciona um novo registro
// determinado pelo terminal em um arquivo binário, a partir da abordagem dinâmica 
// de reaproveitamento de espaço.
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

    // Define quantos registros iremos inserir
    int qtdInserts;
    scanf("%d", &qtdInserts);

    int contador = 0;

    while(contador < qtdInserts)
    {
        REGISTRO registroInserido;
        lerRegistroTerminal(&registroInserido);
        
        // Caso não exista nenhum registro já removido, apenas inserimos na posição proxRRN
        if(cabecalho.topo == -1)
        {
            fseek(arqBIN, TAM_CABECALHO+cabecalho.proxRRN*TAM_REGISTRO, SEEK_SET);
            EscreverRegistroBin(arqBIN, &registroInserido);
            cabecalho.proxRRN++;
        }
        else    // Caso exista um removido, inserimos naquele espaço e desempilhamos um registro do campo topo no cabeçalho
        {
            fseek(arqBIN, TAM_CABECALHO+cabecalho.topo*TAM_REGISTRO+1, SEEK_SET);
            int regRemovidoProximo;
            fread(&regRemovidoProximo, sizeof(int), 1, arqBIN);
            fseek(arqBIN, -5, SEEK_CUR);
            EscreverRegistroBin(arqBIN, &registroInserido);
            cabecalho.topo = regRemovidoProximo;
        }

        free(registroInserido.nomeEstacao);
        free(registroInserido.nomeLinha);

        contador++;
    }
    cabecalho.status = '1';

    recalcularContadores(arqBIN, &cabecalho);

    atualizarCabecalho(arqBIN, &cabecalho);

    fclose(arqBIN);

    BinarioNaTela(arqBIN_nome);
    
    return;
}
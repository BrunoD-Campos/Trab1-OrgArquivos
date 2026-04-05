/*
Implementação da funcionalidade 2 de Select 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include "fundamentals.c"

// Função para ler o registro de cabeçalho de um arquivo e armazenar em uma struct CABECALHO
void lerCabecalhoBin(FILE* arqBIN, CABECALHO* cabecalho)
{
    fseek(arqBIN, 0, SEEK_SET);

    fread(&cabecalho->status, sizeof(char), 1, arqBIN);
    fread(&cabecalho->topo, sizeof(int), 1, arqBIN);
    fread(&cabecalho->proxRRN, sizeof(int), 1, arqBIN);
    fread(&cabecalho->nroEstacoes, sizeof(int), 1, arqBIN);
    fread(&cabecalho->nroParesEstacoes, sizeof(int), 1, arqBIN);
    
    return;
}

void LerRegistroBin(FILE* arqBIN, REGISTRO* reg, int PosicaoRRN){
    fseek(arqBIN, 17 + (PosicaoRRN * 80),SEEK_SET); // Coloca o ponteiro em qual RRN estiver em PosicaoRRN
        
    fread(&reg->removido,sizeof(char) ,1 ,arqBIN);
    fread(&reg->proximo, sizeof(int), 1, arqBIN);
    fread(&reg->codEstacao, sizeof(int), 1, arqBIN);
    fread(&reg->codLinha, sizeof(int), 1, arqBIN);
    fread(&reg->codProxEstacao, sizeof(int), 1, arqBIN);
    fread(&reg->distProxEstacao, sizeof(int), 1, arqBIN);   
    fread(&reg->codLinhaIntegra, sizeof(int), 1, arqBIN);
    fread(&reg->codEstacaoIntegra, sizeof(int), 1, arqBIN);
    
    // Para os campos variáveis
    // Nome de estação (agora que lembrei que este campo nunca é nulo logo não precisa de verificação)
    fread(&reg->tamNomeEstacao, sizeof(int),1, arqBIN);
    if(reg->tamNomeEstacao)
    {
        reg->nomeEstacao = (char*) malloc(reg->tamNomeEstacao + 1);
        fread(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, arqBIN);
        reg->nomeEstacao[reg->tamNomeEstacao] = '\0';
    }
    else
    {
        reg->nomeEstacao = malloc(LEN_CAMPO_NULO);
        strcpy(reg->nomeEstacao, CAMPO_NULO);
    } 

    // Nome da linha
    fread(&reg->tamNomeLinha, sizeof(int), 1, arqBIN);
    if(reg->tamNomeLinha){
        reg->nomeLinha = (char*) malloc(reg->tamNomeLinha + 1);
        fread(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, arqBIN);
        reg->nomeLinha[reg->tamNomeLinha] = '\0';
    }
    else
    {
        reg->nomeLinha = malloc(LEN_CAMPO_NULO);
        strcpy(reg->nomeLinha, CAMPO_NULO);
    }
    
    return;
}

// Mini-função auxiliar para imprimir um campo de inteiro que pode ter valor nulo
void imprimeCampoInt(int campo)
{
    if(campo != -1)
        printf("%d ", campo);
    else printf("%s ", CAMPO_NULO);
    return;
}

void imprimirRegistro(REGISTRO* reg)
{
    printf("%d %s ", reg->codEstacao, reg->nomeEstacao);
    
    imprimeCampoInt(reg->codLinha);

    if(reg->tamNomeLinha)
        printf("%s ", reg->nomeLinha);
    else printf("%s ", CAMPO_NULO);

    imprimeCampoInt(reg->codProxEstacao);
    imprimeCampoInt(reg->distProxEstacao);
    imprimeCampoInt(reg->codLinhaIntegra);

    // Não usando a função para não imprimir um espaço no final
    if(reg->codEstacaoIntegra != -1)
        printf("%d", reg->codEstacaoIntegra);
    else printf("%s", CAMPO_NULO);

    return;
}


void SELECT(){
    char arqBIN_nome[32];
    scanf("%s",arqBIN_nome);
    
    FILE* arqBIN;
    
    if(!(arqBIN = fopen(arqBIN_nome,"r")))   // Abre para leitura em binário
    {
        printf("Falha no processamento do arquivo->\n");
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
    
    fclose(arqBIN); // fechar o arquivo no final das operações
}
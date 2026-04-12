/*
Implementação da funcionalidade 1 de Criação de Tabela
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "Estruturas.c"
#include <stdio.h>

// Função auxiliar com proteção contra falha de segmentação
void lerIntCSV(int* campoRegistro, char** separationPtr)
{
    char* campo;
    campo = strsep(separationPtr, ",");
    // "campo &&" previne o SegFault se strsep retornar NULL
    *campoRegistro = (campo && *campo != '\0' && *campo != '\n' && *campo != '\r') ? atoi(campo) : -1;
    return;
}

// Função para ler e armazenar um registro do formato .csv
void LerRegistroCSV(char *linha, REGISTRO *reg)
{
    char *ptr = linha;
    char *campo;
    
    campo = strsep(&ptr, ",");  
    reg->codEstacao = atoi(campo);  
    
    campo = strsep(&ptr, ",");
    reg->tamNomeEstacao = strlen(campo);
    //Alocação de memória antes do strcpy
    reg->nomeEstacao = malloc(reg->tamNomeEstacao + 1);
    strcpy(reg->nomeEstacao, campo);
    
    lerIntCSV(&reg->codLinha, &ptr);
    
    campo = strsep(&ptr, ",");
    //Verificação segura do ponteiro e da string
    if(campo && *campo != '\0' && *campo != '\n' && *campo != '\r')
    {
        reg->tamNomeLinha = strlen(campo);
        //Alocação de memória antes do strcpy
        reg->nomeLinha = malloc(reg->tamNomeLinha + 1);
        strcpy(reg->nomeLinha, campo);
    }
    else    
    {
        reg->tamNomeLinha = 0;  
        reg->nomeLinha = NULL; // Garante que não aponta para lixo
    }
    
    lerIntCSV(&reg->codProxEstacao, &ptr);    
    lerIntCSV(&reg->distProxEstacao, &ptr);
    lerIntCSV(&reg->codLinhaIntegra, &ptr);
    lerIntCSV(&reg->codEstIntegra, &ptr);

    return;
}

void CREATE_TABLE()
{
    char arqCSV_nome[32], arqBIN_nome[32];
    scanf("%s %s", arqCSV_nome, arqBIN_nome);

    FILE* arqCSV = fopen(arqCSV_nome, "r");
    FILE* arqBIN = fopen(arqBIN_nome, "wb");
    
    if(arqCSV == NULL || arqBIN == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    CABECALHO cabecalhoInicial;
    cabecalhoInicial.status = '0'; //Consistência deve ser o char '0', não o int 0
    cabecalhoInicial.topo = -1;
    cabecalhoInicial.proxRRN = 0;
    cabecalhoInicial.nroEstacoes = 0;
    cabecalhoInicial.nroParesEstacoes = 0;
    atualizarCabecalho(arqBIN, &cabecalhoInicial);

    REGISTRO RegTransferencia;
    RegTransferencia.removido = '0';
    RegTransferencia.proximo = -1;
    
    char linha[TAM_MAX_LINHA_CSV];
    fgets(linha, TAM_MAX_LINHA_CSV, arqCSV); 

    char** listaNomeEstacao = NULL;
    int tamListaNome = 0;

    ParEstacoes* listaParEstacao = NULL;
    int tamListaPar = 0;
    
    while(fgets(linha, TAM_MAX_LINHA_CSV, arqCSV) != NULL)
    {
        // Evita que em algum momento armazene uma quebra de linha no final de uma linha ou no último registro do .csv
        linha[strcspn(linha, "\r\n")] = '\0';

        LerRegistroCSV(linha, &RegTransferencia);
        EscreverRegistroBin(arqBIN, &RegTransferencia);
        cabecalhoInicial.proxRRN++;

        // Abaixo, decidimos não usar a nossa função recalcularContadores() pois no create table já estamos 
        // acessando registro por registro, então seria um desperdício usar ela e acessar todos desde o começo
        // ao invés de ir contabilizando ao longo da criação do .bin
        char igualEncontrado = '0';
        for(int i = 0; i < tamListaNome; i++)
        {
            if(!strcmp(RegTransferencia.nomeEstacao, listaNomeEstacao[i]))
            {
                igualEncontrado = '1';
                break;
            }
        }
        
        if(igualEncontrado == '0')
        {
            // Realocação em blocos de 32 registros para minimizar o número de chamadas de realloc()
            if(!(tamListaNome % 32))
                listaNomeEstacao = realloc(listaNomeEstacao, (tamListaNome + 32) * sizeof(char*)); 
            listaNomeEstacao[tamListaNome] = (char*)calloc(strlen(RegTransferencia.nomeEstacao) + 1, sizeof(char)); 
            strcpy(listaNomeEstacao[tamListaNome], RegTransferencia.nomeEstacao);   
            tamListaNome++;
            cabecalhoInicial.nroEstacoes++;
        }

        ParEstacoes atual;
        atual.origem = RegTransferencia.codEstacao;
        atual.destino = RegTransferencia.codProxEstacao;
        
            
            
        // Libera a memória das strings recém-lidas do CSV nesta iteração
        free(RegTransferencia.nomeEstacao);
        if (RegTransferencia.nomeLinha != NULL) {
            free(RegTransferencia.nomeLinha);
        }
    }

    cabecalhoInicial.status = '1';
    //Gravar o cabeçalho de volta no disco para consolidar as alterações!
    atualizarCabecalho(arqBIN, &cabecalhoInicial);

    // Libera todo o array dinâmico da lista auxiliar
    for(int i = 0; i < tamListaNome; i++) {
        free(listaNomeEstacao[i]);
    }
    free(listaNomeEstacao);

    fclose(arqCSV);
    fclose(arqBIN);

    BinarioNaTela(arqBIN_nome);

    return;
}
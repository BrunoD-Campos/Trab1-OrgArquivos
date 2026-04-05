#ifndef fundamentals
#define fundamentals

// Arquivo com estruturas fundamentais para o projeto

#define TAM_CABECALHO 17
#define TAM_REGISTRO 80
#define TAM_MAX_LINHA_CSV 128 
#define CAMPO_NULO "NULO"
#define LEN_CAMPO_NULO 5


typedef struct
{
    char status;
    int topo;
    int proxRRN;
    int nroEstacoes;
    int nroParesEstacoes;
} CABECALHO;

typedef struct
{
    char removido;
    int proximo;
    int codEstacao; 
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstacaoIntegra;
    int tamNomeEstacao;
    char* nomeEstacao;  
    int tamNomeLinha;
    char* nomeLinha;
} REGISTRO;

typedef struct
{
    char** nomeEstacao;
    int tamLista;
} ListaNomesEstacao;

#endif
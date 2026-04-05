/*
Implementando funcionalidade de select where 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "fornecidas.c"
#include "fundamentals.c"
#include "SelectFrom.c"
#include <stdio.h>



void BuscaStr(FILE* arqBIN, REGISTRO* reg, char* string){

}

void BuscaInt(FILE* arqBIN, REGISTRO* reg, int valor){
    
}

void SELECT_WHERE(){
    char arqBIN_nome[32];
    int qtdBuscas = 0;  // O "n" na primeira entrada do terminal
    char* nomeCampos;
    scanf("%s %d", arqBIN_nome, qtdBuscas);
    FILE* arqBIN = fopen(arqBIN_nome,"r");
    
    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    
    REGISTRO regLido;
    int contador = 0;
    int qtdCampos = 0;  // O "m" em cada busca
    
    // Loop de buscas no geral (n)
    // O arquivo inteiro é percorrido em cada iteração deste while
    while(contador++ != qtdBuscas)
    {
        // Usaremos este registro para atribuir os valores que queremos buscar em cada iteração
        // Apenas usaremos para comparação campos com valores válidos
        REGISTRO regBusca;
        regBusca.codEstacao = -2;
        regBusca.nomeEstacao = NULL;
        regBusca.codLinha = -2;
        regBusca.nomeLinha = NULL;
        regBusca.codProxEstacao = -2;
        regBusca.distProxEstacao = -2;
        regBusca.codLinhaIntegra = -2;
        regBusca.codEstacaoIntegra = -2;

        // Eu preciso alocar dinamicamente o nome do campo.
        scanf("%d",qtdCampos);
        for(int i = 0; i < qtdCampos; i++)  // Daqui começamos a lógica para definir quais campos estamos buscando
        {
            scanf("%s", nomeCampos);
            
            // Dependendo do nome do campo, o scanf muda 
            if(strcmp(nomeCampos,"NomeEstacao") == 0 || strcmp(nomeCampos,"NomeLinha") == 0){
                scanf("%s ",); // captura string
            }
            else{
                scanf("%d ",); // captura inteiro
            }
            
        }
        
    }
    
    if
    
}
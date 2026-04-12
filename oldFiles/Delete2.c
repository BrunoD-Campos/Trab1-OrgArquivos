/*
Implementação da funcionalidade 4 de Delete 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Estruturas.c"
#include "FuncGerais.c"

// Percorre arqBIN até encontrar o primeiro regLido que seja igual à regBusca
void outroBuscaRegistro(FILE* arqBIN, CABECALHO* cabecalho, REGISTRO* regBusca, REGISTRO* regLido, int* RRN) 
{
    while(*RRN < cabecalho->proxRRN)
    {
        LerRegistroBin(arqBIN, &regLido, RRN);
        *RRN++;

        // Se encontramos o registro procurado
        if(ComparaRegistros(&regBusca, &regLido) && regLido->removido == '0') 
            return;
    }
    return;
}

void SELECT_WHERE(){
    
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome); 
    
    FILE* arqBIN = fopen(arqBIN_nome, "rb+");
    
    if (arqBIN == NULL) {
        printf("Falha ao abrir o arquivo\n");
        return;
    }

    int qtdDeletes;  
    scanf("%d", &qtdDeletes);

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);

    // Marca o arquivo como incosistento durante o processo
    cabecalho.status = '0';
    fseek(arqBIN, 0, SEEK_SET);
    fwrite(&cabecalho.status, sizeof(char), 1, arqBIN);
    fseek(arqBIN, 0, SEEK_SET);

    // Registro para cada registro lido do arquivo
    REGISTRO regLido;
    // Registro usado para comparar cada registro lido com os campos da entrada
    REGISTRO regBusca;
    
    int contador = 0;
    while (contador < qtdDeletes)
    {
        int RRN = 0;

        int qtdCampos = 0;
        scanf("%d", &qtdCampos);

        // Settando o regBusca devidamente, colocando todos os 
        // valores inválidos e então lendo os campos do terminal
        initRegBusca(&regBusca, qtdCampos);

        while(RRN < cabecalho.proxRRN)
        {
            outroBuscaRegistro(arqBIN, &cabecalho, &regBusca, &regLido, &RRN);   
            
            // Verificação necessária pois é possível que BuscaRegistro já tenha passado de proxRRN
            if(RRN < cabecalho.proxRRN)
            {
                removerRegistro(arqBIN, RRN, cabecalho.topo);
                cabecalho.topo = RRN;
            }
            
            if(regLido.nomeEstacao) free(regLido.nomeEstacao);
            if(regLido.nomeLinha) free(regLido.nomeLinha);
        }

        if(regBusca.nomeEstacao) free(regBusca.nomeEstacao);
        if(regBusca.nomeLinha) free(regBusca.nomeLinha);

        contador++;
    }

    cabecalho.status = '1';
    recalcularContadores(arqBIN, &cabecalho);
    atualizarCabecalho(arqBIN, &cabecalho);

    fclose(arqBIN);   
}
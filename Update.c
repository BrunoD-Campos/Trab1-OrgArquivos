/*
Implementação da funcionalidade 6 de Update 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "FuncGerais.c"
#include "Estruturas.c"

// parte principal da funcionalidade de Update, que atualiza campos
// específicos de registros conforme o que foi pedido na busca.
void UPDATE() {
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome);
    
    FILE* arqBIN;
    
    if(!(arqBIN = fopen(arqBIN_nome, "rb+"))) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    
    // Define o arquivo como inconsistente durante as alterações
    cabecalho.status = '0';
    atualizarCabecalho(arqBIN, &cabecalho);

    int qtdUpdates;
    scanf("%d", &qtdUpdates);

    // Loop executado n vezes, conforme solicitado na entrada
    for(int i = 0; i < qtdUpdates; i++) {
        
        int m;
        scanf("%d", &m);
        REGISTRO regBusca;
        initRegBusca(&regBusca, m);

        int p;
        scanf("%d", &p);
        REGISTRO regAtualiza;
        initRegBusca(&regAtualiza, p);

        // faz busca linear no arquivo para atualizar os registros, caso eles sejam iguais
        for(int RRN = 0; RRN < cabecalho.proxRRN; RRN++) {
            REGISTRO regLido;
            LerRegistroBin(arqBIN, &regLido, RRN);

            if(regLido.removido == '0' && ComparaRegistros(&regBusca, &regLido)) {
                
                // Macros locais para deixar o código mais legível
                #define ATUALIZA_INT(campo) \
                    if(regAtualiza.campo != -2) regLido.campo = regAtualiza.campo

                #define ATUALIZA_STR(campo, tamCampo) \
                    if(regAtualiza.campo != NULL) { \
                        free(regLido.campo); \
                        regLido.tamCampo = regAtualiza.tamCampo; \
                        regLido.campo = malloc(regAtualiza.tamCampo + 1); \
                        strcpy(regLido.campo, regAtualiza.campo); \
                    }

                // Fazendo as atualizações 
                ATUALIZA_INT(codEstacao);
                ATUALIZA_INT(codLinha);
                ATUALIZA_INT(codProxEstacao);
                ATUALIZA_INT(distProxEstacao);
                ATUALIZA_INT(codLinhaIntegra);
                ATUALIZA_INT(codEstIntegra);

                ATUALIZA_STR(nomeEstacao, tamNomeEstacao);
                ATUALIZA_STR(nomeLinha, tamNomeLinha);

                // Destroi os macros, já que não serão mais usados
                #undef ATUALIZA_INT
                #undef ATUALIZA_STR

                // Volta o ponteiro e sobescreve os registros
                fseek(arqBIN, TAM_CABECALHO + (RRN * TAM_REGISTRO), SEEK_SET);
                EscreverRegistroBin(arqBIN, &regLido);
            }

            #define LIBERA_MEM(registro, campo) \
                if(registro.campo) { \
                    free(registro.campo); \
                    registro.campo = NULL; \
                }
            
            LIBERA_MEM(regLido, nomeEstacao);
            LIBERA_MEM(regLido, nomeLinha);

        }
        
        LIBERA_MEM(regBusca, nomeEstacao);
        LIBERA_MEM(regBusca, nomeLinha);
            
        LIBERA_MEM(regAtualiza, nomeEstacao);
        LIBERA_MEM(regAtualiza, nomeLinha);

        #undef LIBERA_MEM

    }

    cabecalho.status = '1';
    recalcularContadores(arqBIN, &cabecalho);
    atualizarCabecalho(arqBIN, &cabecalho);

    fclose(arqBIN);

    BinarioNaTela(arqBIN_nome);
}
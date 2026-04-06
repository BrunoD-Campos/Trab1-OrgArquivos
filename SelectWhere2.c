/*
Implementação da funcionalidade 3 de Select Where 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Estruturas.c"
#include "FuncAuxiliares.c"

void BuscaRegistro(int operacao, char* arqBIN, CABECALHO* cabecalho) {
    // Registro para cada registro lido do arquivo
    REGISTRO regLido;
    // Registro usado para comparar cada registro lido com os campos da entrada
    REGISTRO regBusca;
    
    int qtdCampos = 0;
    scanf("%d", &qtdCampos);
    
    // Settando o regBusca devidamente, colocando todos os 
    // valores inválidos e então lendo os campos do terminal
    initRegBusca(&regBusca, qtdCampos);
    
    int RRN = 0;
    bool encontrou = false; 

    rewind(arqBIN); 

    while(RRN < cabecalho->proxRRN)
    {
        LerRegistroBin(arqBIN, &regLido, RRN);  

        if(regLido.removido == '0' && ComparaRegistros(&regBusca, &regLido)) 
        {
            
            encontrou = true;
            
            if(operacao == 1){
                imprimirRegistro(&regLido);
                printf("\n");
            }        
            else if (operacao == 2) {
                long OffsetRegAtual = 17 + (RRN * 80);
                fseek(arqBIN, OffsetRegAtual, SEEK_SET);

                regLido.removido = '1'; 
                regLido.proximo = cabecalho->topo;
                fwrite(&regLido.removido, sizeof(char), 1, arqBIN);
                fwrite(&regLido.proximo, sizeof(int), 1, arqBIN);

                cabecalho->topo = RRN;
                
                // Se tiver outra estação com mesmo nome não podemos diminuir isto
                cabecalho->nroEstacoes--;
                
                // Mesma lógica nesta parte eu acho
                if(regLido.codProxEstacao != -1) {
                    cabecalho->nroParesEstacoes--;
                }
                
            }
        }
        
        
        if(regLido.nomeEstacao) free(regLido.nomeEstacao);
        if(regLido.nomeLinha) free(regLido.nomeLinha);
        
        RRN++;
    }
    
    atualizarCabecalho(arqBIN, &cabecalho);
    
    if(!encontrou && operacao == 1) {
        printf("Registro inexistente.\n");
    }
    
    if(operacao == 1) {
        printf("\n"); 
    }
    
    if(regBusca.nomeEstacao) free(regBusca.nomeEstacao);
    if(regBusca.nomeLinha) free(regBusca.nomeLinha);

    
    if (operacao == 2) {
        cabecalho->status = '1';
        atualizarCabecalho(arqBIN, &cabecalho);
    }

    fclose(arqBIN);
    return;
}


void SELECT_WHERE(){
    
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome); 
    
    FILE* arqBIN = fopen(arqBIN_nome, "rb");
    
    if (arqBIN == NULL) {
        printf("Falha ao abrir o arquivo\n");
        return;
    }

    int qtdBuscas;  
    scanf("%d", &qtdBuscas);

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);

    // Mantendo aqui por sla pq
    // if (operacao == 2) {
    //     cabecalho.status = '0';
    //     fseek(arqBIN, 0, SEEK_SET);
    //     fwrite(&cabecalho.status, sizeof(char), 1, arqBIN);
    // }
    
    int contador = 0;
    while (contador < qtdBuscas)
    {
        

        BuscaRegistro(1, arqBIN_nome);   
        
        
        contador++;
    }
    
}
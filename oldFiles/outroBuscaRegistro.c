#include "Estruturas.c"
#include <stdio.h>
#include <stdbool.h>

void BuscaRegistro(FILE* arqBIN, CABECALHO* cabecalho, REGISTRO* regBusca, REGISTRO* regLido, int* RRN) 
{
    bool encontrou = false; 

    while(*RRN < cabecalho->proxRRN)
    {
        LerRegistroBin(arqBIN, &regLido, RRN);

        // Se encontramos o registro procurado
        if(ComparaRegistros(&regBusca, &regLido) && regLido->removido == '0') 
        
        RRN++;
    }
    return;
}
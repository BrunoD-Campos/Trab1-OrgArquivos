/*
Trabalho 1 de SCC0215 - Organização de Arquivos
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

// Bibliotecas padrao do C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Definições de tipos
#include "Estruturas.c" 

// Funções que todos os outros módulos podem usar
#include "fornecidas.c"
#include "FuncAuxiliares.c" 

// 6 Funcionalidades
#include "CreateTable.c"
#include "SelectFrom.c"
#include "SelectWhere.c"
#include "Remove.c"
#include "Insert.c"
#include "Update.c"

int main(){
    //Lendo entrada da funcionalidade
    int comando;
    scanf("%d", &comando);
    
    switch (comando){
        case 1:
            CREATE_TABLE();
            break;
        case 2:
            SELECT();
            break;
        case 3:
            SELECT_WHERE();
            break;
        case 4:
            REMOVE();
            break;
        case 5:
            INSERT();
            break;
        case 6:
            UPDATE();
            break;
        default:
            printf("Entrada inválida!");
            break;
    }
}
/*
Implementação da funcionalidade 3 de Select Where 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

// Lembrar de remover estes includes, está apenas como placeholder para evitar squiggles distraindo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// Tanto este enum quanto a função campoParaEnum() foram feitas para 
// auxiliar em definir qual campo está servindo de critério
typedef enum
{
    codEstacao,         // 0
    nomeEstacao,        // 1
    codLinha,           // 2
    nomeLinha,          // 3
    codProxEstacao,     // 4
    distProxEstacao,    // 5
    codLinhaIntegra,    // 6
    codEstIntegra       // 7
}CRITERIO;

// Lê a strin nomeCampo e define qual é seu valor no tipo enum CRITERIO e o retorna
CRITERIO campoParaEnum(char* nomeCampo)
{
    if(!strcmp(nomeCampo, "codEstacao")) return codEstacao;
    else if(!strcmp(nomeCampo, "nomeEstacao")) return nomeEstacao;
    else if(!strcmp(nomeCampo, "codLinha")) return codLinha;
    else if(!strcmp(nomeCampo, "nomeLinha")) return nomeLinha;
    else if(!strcmp(nomeCampo, "codProxEstacao")) return codProxEstacao;
    else if(!strcmp(nomeCampo, "distProxEstacao")) return distProxEstacao;
    else if(!strcmp(nomeCampo, "codLinhaIntegra")) return codLinhaIntegra;
    else return codEstIntegra;
}

// Mini-função para lermos o inteiro após o nome do campo e armazená-lo no registro de busca
void leIntCampoBusca(int* valorCampo, char* strInput)
{
    // Agora verifica explicitamente a string "NULO"
    if(strcmp(strInput, "NULO") == 0 || strInput[0] == '\0')    
        *valorCampo = -1;
    else
        *valorCampo = atoi(strInput);
    return;
}

// Mini-função com mesmo conceito da anterior mas para strings
void leStringCampoBusca(int* tamCampo, char** stringCampo, char* strInput)
{
    if(strInput[0] == '\0') // É NULO
    {
        *tamCampo = 0;
        *stringCampo = malloc(5); // Tamanho da palavra "NULO" + \0
        strcpy(*stringCampo, "NULO");
    }
    else
    {
        *tamCampo = strlen(strInput);
        *stringCampo = malloc(*tamCampo + 1);
        strcpy(*stringCampo, strInput);
    }
    return;
}

// Compara dois registros e retorna true caso sejam, e false caso contrário
// Se estiver sendo usado em SelectWhere, coloque o regBusca no primeiro parâmetro
bool ComparaRegistros(REGISTRO* filtro, REGISTRO* comparado) {
    
    // Macro dentro do escopo da função para comparar strings
    // Se o filtro não for nulo (-1) e for diferente do comparado, retorna falso.
    #define VERIFICA_INT(campo) \
        if (filtro->campo != -2 && filtro->campo != comparado->campo) return false

    // Macro dentro do escopo da função para comparar strings
    // Se o filtro não for nulo e for diferente do comparado, retorna falso.
    #define VERIFICA_STR(campo) \
        if (filtro->campo != NULL && strcmp(filtro->campo, comparado->campo) != 0) return false

    VERIFICA_INT(codEstacao);
    VERIFICA_INT(codLinha);
    VERIFICA_INT(codProxEstacao);
    VERIFICA_INT(distProxEstacao);
    VERIFICA_INT(codLinhaIntegra);
    VERIFICA_INT(codEstIntegra);

    VERIFICA_STR(nomeEstacao);
    VERIFICA_STR(nomeLinha);

    return true;

    #undef VERIFICA_INT
    #undef VERIFICA_STR
}

void SELECT_WHERE(){
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome);
    FILE* arqBIN = fopen(arqBIN_nome,"rb");
    
    int qtdBuscas;  // O "n" na primeira entrada do terminal
    scanf("%d", &qtdBuscas);

    // Pegando os dados do registro de cabeçalho
    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    
    REGISTRO regLido;

    int contador = 0;   // Contador de quantas buscas do arquivo já foram feitas
    
    // Loop de buscas no geral (n)
    // O arquivo inteiro é percorrido em cada iteração deste while
    while(contador++ != qtdBuscas)
    {
        
        REGISTRO regBusca;
        
        regBusca.codEstacao = -2;
        regBusca.codLinha = -2;
        regBusca.codProxEstacao = -2;
        regBusca.distProxEstacao = -2;
        regBusca.codLinhaIntegra = -2;
        regBusca.codEstIntegra = -2;
        regBusca.nomeEstacao = NULL;
        regBusca.nomeLinha = NULL;
        
        int qtdCampos = 0;  // O "m" em cada busca
        scanf("%d", &qtdCampos);
        
        // muda conforme a quantidade campos que o usuário queira buscar
        for(int i = 0; i < qtdCampos; i++)  
        {
            char idCampo[32]; 
            scanf("%s", idCampo);
            
            CRITERIO nomeCampo = campoParaEnum(idCampo);
            char stringBuscada[50] = "";  
            
            if (nomeCampo == nomeEstacao || nomeCampo == nomeLinha) {
                ScanQuoteString(stringBuscada);
            } else {
                scanf("%s", stringBuscada);
            }

            switch(nomeCampo)
            {
                case nomeEstacao:
                    leStringCampoBusca(&regBusca.tamNomeEstacao, &regBusca.nomeEstacao, stringBuscada);
                    break;

                
                case nomeLinha:
                    leStringCampoBusca(&regBusca.tamNomeLinha, &regBusca.nomeLinha, stringBuscada);
                    break;

                
                case codEstacao:
                    leIntCampoBusca(&regBusca.codEstacao, stringBuscada);
                    break;
                    
                
                case codLinha:
                    leIntCampoBusca(&regBusca.codLinha, stringBuscada);
                    break;
                
                case codProxEstacao:
                    leIntCampoBusca(&regBusca.codProxEstacao, stringBuscada);
                    break;
                
                case distProxEstacao:
                    leIntCampoBusca(&regBusca.distProxEstacao, stringBuscada);
                    break;
                
                case codEstIntegra:
                    leIntCampoBusca(&regBusca.codEstIntegra, stringBuscada);
                    break;
                
                case codLinhaIntegra:
                    leIntCampoBusca(&regBusca.codLinhaIntegra, stringBuscada);
                    break;
                
                default:
                    printf("Falha de processamento do arquivo\n");
                    return;
            }
        }  

        int RRN = 0;
        bool encontrou = false; 

        while(RRN < cabecalho.proxRRN)
        {
            LerRegistroBin(arqBIN, &regLido, RRN);  
            
            if(regLido.removido == '0') {
                if(ComparaRegistros(&regBusca, &regLido)) {
                    imprimirRegistro(&regLido);
                    printf("\n");
                    encontrou = true;
                }
            }
            
            
            if(regLido.nomeEstacao) free(regLido.nomeEstacao);
            if(regLido.nomeLinha) free(regLido.nomeLinha);
            
            RRN++;
        }

        if(!encontrou) {
            printf("Registro inexistente.\n");
        }
        
        printf("\n"); 

        if(regBusca.nomeEstacao){
            free(regBusca.nomeEstacao);
        } 
        
        if(regBusca.nomeLinha){
            free(regBusca.nomeLinha);
        }
    }
    fclose(arqBIN);
    return;
}
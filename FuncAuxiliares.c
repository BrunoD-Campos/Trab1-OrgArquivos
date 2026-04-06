#ifndef FUNC_AUX
#define FUNC_AUX

void imprimeCampoInt(int campo)
{
    if(campo != -1)
        printf("%d ", campo);
    else 
        printf("%s ", CAMPO_NULO);
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
    if(reg->codEstIntegra != -1)
        printf("%d", reg->codEstIntegra);
    else printf("%s", CAMPO_NULO);

    return;
}



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
    fread(&reg->codEstIntegra, sizeof(int), 1, arqBIN);
    
    // Para os campos variáveis
    // Nome de estação -> MUDAR ISSO, POIS Os CAMPOs nomeEstacao e nomeLinha NUNCA SÃO NULOS!!!!!!!!!!!!!!!!!!!
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

void BuscaRegistro(int operacao, char* arqBIN_nome) {
    
    FILE* arqBIN = fopen(arqBIN_nome, "rb+");
    
    if (arqBIN == NULL) {
        printf("Falha ao abrir o arquivo\n");
        return;
    }

    int qtdBuscas;  
    scanf("%d", &qtdBuscas);

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    
    // Marca o arquivo como inconsistente durante a operação
    if (operacao == 2) {
        cabecalho.status = '0';
        fseek(arqBIN, 0, SEEK_SET);
        fwrite(&cabecalho.status, sizeof(char), 1, arqBIN);
    }

    REGISTRO regLido;
    int contador = 0;

    // Loop principal de buscas
    while(contador < qtdBuscas)
    {
        contador++;
        REGISTRO regBusca;
        
        regBusca.codEstacao = -2;
        regBusca.codLinha = -2;
        regBusca.codProxEstacao = -2;
        regBusca.distProxEstacao = -2;
        regBusca.codLinhaIntegra = -2;
        regBusca.codEstIntegra = -2;
        regBusca.nomeEstacao = NULL;
        regBusca.nomeLinha = NULL;
        
        int qtdCampos = 0;
        scanf("%d", &qtdCampos);
        
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

        rewind(arqBIN); 

        while(RRN < cabecalho.proxRRN)
        {
            LerRegistroBin(arqBIN, &regLido, RRN);  
            
            if(qtdCampos == 0) {
                break;
            }

            if(regLido.removido == '0' && ComparaRegistros(&regBusca, &regLido)) {
                
                encontrou = true; 
                
                if(operacao == 1){ 
                    imprimirRegistro(&regLido);
                    printf("\n");
                }        
                else if (operacao == 2) { 
                    int antigo_topo = cabecalho.topo;

                    long OffsetRegAtual = 17 + (RRN * 80);
                    fseek(arqBIN, OffsetRegAtual, SEEK_SET);

                    char MarcaRemovido = '1';
                    fwrite(&MarcaRemovido, sizeof(char), 1, arqBIN);
                    fwrite(&antigo_topo, sizeof(int), 1, arqBIN);

                    cabecalho.topo = RRN;
                    
                    
                    cabecalho.nroEstacoes--;
                    
                    if(regLido.codProxEstacao != -1) {
                        cabecalho.nroParesEstacoes--;
                    }
                    
                    regLido.removido = '1'; 
                }
            }
            
            if(regLido.nomeEstacao) free(regLido.nomeEstacao);
            if(regLido.nomeLinha) free(regLido.nomeLinha);
            
            RRN++;
        }

        
        if(!encontrou && operacao == 1) {
            printf("Registro inexistente.\n");
        }
        
        if(operacao == 1) {
            printf("\n"); 
        }

        
        if(regBusca.nomeEstacao) free(regBusca.nomeEstacao);
        if(regBusca.nomeLinha) free(regBusca.nomeLinha);
        
    } 

    
    if (operacao == 2) {
        cabecalho.status = '1';
        fseek(arqBIN, 0, SEEK_SET);
        
        fwrite(&cabecalho.status, sizeof(char), 1, arqBIN);
        fwrite(&cabecalho.topo, sizeof(int), 1, arqBIN);
        fwrite(&cabecalho.proxRRN, sizeof(int), 1, arqBIN);
        fwrite(&cabecalho.nroEstacoes, sizeof(int), 1, arqBIN);
        fwrite(&cabecalho.nroParesEstacoes, sizeof(int), 1, arqBIN);
    }

    fclose(arqBIN);
    return;
}

#endif
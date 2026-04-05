/*
Implementação da funcionalidade 1 de Criação de Tabela
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/



// Função para sobrescrever o registro de cabeçalho do arquivo
void atualizarCabecalho(FILE* arqBIN, CABECALHO* regCabecalho)
{
    fseek(arqBIN, 0, SEEK_SET);
    fwrite(&regCabecalho->status, sizeof(char), 1, arqBIN);
    fwrite(&regCabecalho->topo, sizeof(int), 1, arqBIN);
    fwrite(&regCabecalho->proxRRN, sizeof(int), 1, arqBIN);
    fwrite(&regCabecalho->nroEstacoes, sizeof(int), 1, arqBIN);
    fwrite(&regCabecalho->nroParesEstacoes, sizeof(int), 1, arqBIN);
    return;
}

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

// Função para escrever um registro no arquivo binário
void EscreverRegistroBin(FILE *arqBIN, REGISTRO *reg)
{
    int BytesEscritos = 0;

    fwrite(&reg->removido, sizeof(char), 1, arqBIN);
    fwrite(&reg->proximo, sizeof(int), 1, arqBIN);
    BytesEscritos += 5; 

    fwrite(&reg->codEstacao, sizeof(int), 1, arqBIN);
    fwrite(&reg->codLinha, sizeof(int), 1, arqBIN);
    fwrite(&reg->codProxEstacao, sizeof(int), 1, arqBIN);
    fwrite(&reg->distProxEstacao, sizeof(int), 1, arqBIN);
    fwrite(&reg->codLinhaIntegra, sizeof(int), 1, arqBIN);
    fwrite(&reg->codEstIntegra, sizeof(int), 1, arqBIN);
    BytesEscritos += 24; 

    fwrite(&reg->tamNomeEstacao, sizeof(int), 1, arqBIN);
    fwrite(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, arqBIN);
    fwrite(&reg->tamNomeLinha, sizeof(int), 1, arqBIN);
    
    if (reg->tamNomeLinha > 0 && reg->nomeLinha != NULL) {
        fwrite(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, arqBIN);
    }
    
    BytesEscritos += 8 + reg->tamNomeEstacao + reg->tamNomeLinha;

    for(int i = 0; i < TAM_REGISTRO - BytesEscritos; i++)
        fputc('$', arqBIN);
    
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

    ListaNomesEstacao lista;
    lista.nomeEstacao = NULL;
    lista.tamLista = 0;

    while(fgets(linha, TAM_MAX_LINHA_CSV, arqCSV) != NULL)
    {
        //Higiene vital que evita o Segfault no último campo do CSV
        linha[strcspn(linha, "\r\n")] = '\0';

        LerRegistroCSV(linha, &RegTransferencia);
        EscreverRegistroBin(arqBIN, &RegTransferencia);
        cabecalhoInicial.proxRRN++;

        char igualEncontrado = '0';
        for(int i = 0; i < lista.tamLista; i++)
        {
            if(!strcmp(RegTransferencia.nomeEstacao, lista.nomeEstacao[i]))
            {
                igualEncontrado = '1';
                break;
            }
        }
        
        if(igualEncontrado == '0')
        {
            lista.nomeEstacao = realloc(lista.nomeEstacao, (lista.tamLista + 1) * sizeof(char*)); 
            lista.nomeEstacao[lista.tamLista] = (char*)calloc(strlen(RegTransferencia.nomeEstacao) + 1, sizeof(char)); 
            strcpy(lista.nomeEstacao[lista.tamLista], RegTransferencia.nomeEstacao);   
            lista.tamLista++;
            cabecalhoInicial.nroEstacoes++;
        }

        //A próxima estação tem que existir (!= -1) para configurar um par
        if(RegTransferencia.codProxEstacao != -1){
            cabecalhoInicial.nroParesEstacoes++;
        }
            
            
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
    for(int i = 0; i < lista.tamLista; i++) {
        free(lista.nomeEstacao[i]);
    }
    free(lista.nomeEstacao);

    fclose(arqCSV);
    fclose(arqBIN);

    BinarioNaTela(arqBIN_nome);

    return;
}
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

#endif

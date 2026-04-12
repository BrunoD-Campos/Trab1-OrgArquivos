/*
Implementação da funcionalidade 4 de Delete
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

// Trecho principal da funcionalidade 4 de Delete, onde registros que 
// tenham campos de valor especifiacdo são removidos com a abordagem
// dinâmica de reaproveitamento de espaço

void DELETE(){
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome); 
    
    // A flag de operação 2 para esta função remove os registros que baterem com a busca
    BuscaRegistro(2, arqBIN_nome); 
    
    BinarioNaTela(arqBIN_nome);
}
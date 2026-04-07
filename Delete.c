/*
Implementando funcionalidade remoção 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

void DELETE(){
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome); 
    
    BuscaRegistro(2, arqBIN_nome); 
    
    BinarioNaTela(arqBIN_nome);
}
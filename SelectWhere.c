/*
Implementação da funcionalidade 3 de Select Where 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

// Trecho principal da funcionalidade Select Where, onde apenas registros
// com campos de valores específicados são impressos
void SELECT_WHERE(){
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome); 
    
    // Primeira aparição da nossa função BuscaRegistro(), que decidimos
    // agregar a todas funcionalidades que tenham funcionamento parecido
    BuscaRegistro(1, arqBIN_nome);   
}
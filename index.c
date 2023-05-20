/****************************************************
2 *                                                 *
3 * Moisés Silva de Azevedo                         *
4 * RGA 2022.0743.004-6                             *
5 * Implementação 2                                 *
6 * Disciplina: Estruturas de Dados e Programação I *
7 * Professor: Ronaldo Fiorilo                      *
8 *                                                 *
9 ***************************************************/

#include <stdio.h>
#include <string.h>
#include "avl.h"

int main(void) {
  Palavra *arvore;
  int sair, contador_de_entrada,mudou_altura;
  char leitura[100], op[12], palavra[30], sinonimo[30];
  arvore = NULL;
  arvore = carregar(arvore, "dados.txt");
  sair = 0;
  do {
    fgets(leitura, sizeof(leitura), stdin);
    contador_de_entrada = sscanf(leitura, "%s %s %s", op, palavra, sinonimo);
    
    if(strcmp(op, "insere") == 0) {
      arvore = inserirPalavra(arvore, palavra, &mudou_altura);
      inserirSinonimoAssociado(arvore, palavra, sinonimo);
      mudou_altura = 0;
      arvore = inserirPalavra(arvore, sinonimo, &mudou_altura);
      inserirSinonimoAssociado(arvore, sinonimo, palavra);
    }

    if(strcmp(op, "busca") == 0) {
      Palavra *temp;
      temp = buscaPalavra(arvore, palavra);
      if(temp) {
        if(temp->arvore_sinonimos) {
          mostrarArvoreDeSinonimosEmOrdem(temp->arvore_sinonimos);
        }
      } else {
        printf("hein?\n");
      }
    }

    if(strcmp(op, "remove") == 0) {
      if(contador_de_entrada == 3) {
        Palavra *temp;
        arvore = removerSinonimoAssociado(arvore, palavra, sinonimo);
        arvore = removerSinonimoAssociado(arvore, sinonimo, palavra);
        temp = buscaPalavra(arvore, palavra);
        if(temp->arvore_sinonimos == NULL) {
          arvore = removerPalavra(arvore, palavra, &mudou_altura);
        }
        temp = buscaPalavra(arvore, sinonimo);
        if(temp->arvore_sinonimos == NULL) {
          arvore = removerPalavra(arvore, sinonimo, &mudou_altura);
        }
      } else if(contador_de_entrada == 2) {
        arvore = removerPalavra(arvore, palavra, &mudou_altura);
      }
    }

    if(strcmp(op, "lista") == 0) {
      lista(palavra, sinonimo, arvore);
    }

    if(strcmp(op, "fim") == 0) {
      sair = 1;
    }
  } while (sair != 1);
  
  salvarNoArquivo(arvore, "dados.txt");
  liberaArvoreDePalavras(arvore);
  return 0;
}
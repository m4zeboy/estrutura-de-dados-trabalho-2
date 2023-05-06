#include <stdio.h>
#include <string.h>
#include "avl.h"

int main(void) {
  Palavra *arvore;
  int sair, contador_de_entrada,mudou_altura;
  char leitura[100], op[12], palavra[30], sinonimo[30];
  arvore = lerDoArquivo(NULL, "dados.txt");
  sair = 0;
  mostrarArvoreDePalavrasEmPreOrdem(arvore);
  do {
    fgets(leitura, sizeof(leitura), stdin);
    contador_de_entrada = sscanf(leitura, "%s %s %s", op, palavra, sinonimo);
    
    if(strcmp(op, "insere") == 0) {
      arvore = inserirPalavra(arvore, palavra, sinonimo, &mudou_altura);
      arvore = inserirPalavra(arvore, sinonimo, palavra, &mudou_altura);
    }

    if(strcmp(op, "busca") == 0) {
      Palavra *temp;
      temp = buscaPalavra(arvore, palavra);
      if(temp) {
        printf("%s\n", temp->palavra);
        if(temp->arvore_sinonimos) {
          mostrarArvoreDeSinonimosEmOrdem(temp->arvore_sinonimos);
        }
      }
    }

    if(strcmp(op, "remove") == 0) {
      if(contador_de_entrada == 3) {
        
      }
    }

    if(strcmp(op, "fim") == 0) {
      sair = 1;
    }
  } while (sair != 1);
  
  salvarNoArquivo(arvore, "dados.txt");
  return 0;
}
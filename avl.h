#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A estrutura 'sinonimo' representa um nó de uma árvore de sinônimos. */
typedef struct sinonimo {
  char *sinonimo;
  int fb;
  struct sinonimo *esq, *dir;
} Sinonimo;

/* A função criarSinonimo() aloca memória dinâmicamente para um sinônimo e retorna o endereço de memória alocado. */
Sinonimo *criarSinonimo(char *sinonimo);
Sinonimo *criarSinonimo(char *sinonimo) {
  Sinonimo *novo = (Sinonimo *) malloc(sizeof(Sinonimo));
  if(novo == NULL) return NULL;
  novo->sinonimo = (char *) malloc(strlen(sinonimo) + 1);
  if(novo->sinonimo == NULL) return NULL;
  strcpy(novo->sinonimo, sinonimo);
  novo->dir = NULL; novo->esq = NULL;
  novo->fb = 0;
  return novo;
}

/* A função liberarSinonimo() libera a memoria alocada do sinonimo passado por argumento. */
void liberarSinonimo(Sinonimo *no);
void liberarSinonimo(Sinonimo *no) {
  if(no) {
    free(no->sinonimo);
    free(no);
  }
}

/* A função liberarArvoreDeSinônimos() percorre a árvore de sinônimos em pré-ordem e libera a memória alocada para cada nó. */
void liberarArvoreDeSinônimos(Sinonimo *raiz) {
  Sinonimo *temp = raiz->dir;
  if(raiz) {
    liberarArvoreDeSinônimos(raiz);
    liberarSinonimo(raiz);
    liberarArvoreDeSinônimos(temp);
  }
}

/* As funções abaixo realizam operações de rotação para a árvore de SINONIMOS. */
/* Rotação Simples para Direita. */
Sinonimo *RSD_SINONIMO(Sinonimo *raiz) {
  Sinonimo *u, *t2;
  u = raiz->esq;
  t2 = u->dir;

  u->dir = raiz;
  raiz->esq = t2;

  raiz->fb = 0;
  return u;
}
/* Rotação Dupla para Direita. */
Sinonimo *RDD_SINONIMO(Sinonimo *raiz) {
  Sinonimo *u, *z, *t2, *t3;
  u = raiz->esq; z = u->dir;
  t2 = z->esq; t3 = z->dir;

  raiz->esq = t3;
  u->dir = t2;
  z->esq = u; z->dir = raiz;
  if(z->fb == -1) raiz->fb = 1;
  else raiz->fb = 0;

  if(z->fb == 1) u->fb =-1; 
  else u->fb = 0;

  return z;
}
/* Rotação Simples para Esquerda. */
Sinonimo *RSE_SINONIMO(Sinonimo *raiz) {
  Sinonimo *u, *t2;
  u = raiz->dir;
  t2 = u->esq;

  u->esq = raiz;
  raiz->dir = t2;
  raiz->fb = 0;
  
  return u;
}
/* Rotação Dupla para Esquerda. */
Sinonimo *RDE_SINONIMO(Sinonimo *raiz) {
  Sinonimo *b, *c, *t2, *t3;

  b = raiz->dir; c = b->esq;
  t2 = c->esq; t3 = c->dir;

  raiz->dir = t2;
  b->esq = t3;

  c->esq = raiz;
  c->dir = b;

  if(c->fb == 1) raiz->fb = -1;
  else raiz->fb = 0;

  if(c->fb == -1) b->fb = 1;
  else b->fb = 0;

  return c;
}

/* A função inserirSinonimo() tenta inserir um sinonimo na estrutra, e realiza as operações de balanceamento caso necessário. */
Sinonimo *inserirSinonimo(Sinonimo *raiz, char *sinonimo, int *mudou_altura) {
  if(raiz == NULL) {
    *mudou_altura = 1;
    return criarSinonimo(sinonimo);
  } else if(strcmp(raiz->sinonimo, sinonimo) == 0) {
    *mudou_altura = 0;
    return raiz; 
  }
  else if(strcmp(sinonimo, raiz->sinonimo) < 0) { 
    raiz->esq = inserirSinonimo(raiz->esq, sinonimo, mudou_altura);
    if(*mudou_altura == 1) {
      switch(raiz->fb) {
        case 1:
          raiz->fb = 0;
          *mudou_altura = 0; 
        break;
        case 0:
          raiz->fb = -1;
        break;
        case -1:
          if(raiz->esq->fb == -1) {
            printf("RSD %s\n", raiz->sinonimo);
            raiz = RSD_SINONIMO(raiz);
          } else {
            printf("RDD %s\n", raiz->sinonimo);
            raiz = RDD_SINONIMO(raiz);
          }
          *mudou_altura = 0;
          raiz->fb = 0;
        break;
      }
    }
    
    return raiz;
  } else {
    raiz->dir = inserirSinonimo(raiz->dir, sinonimo, mudou_altura);
    if(*mudou_altura == 1) {
      switch(raiz->fb) {
        case -1:
          raiz->fb = 0;
          *mudou_altura = 0;
          break;
        case 0:
          raiz->fb = 1;
          break;
        case 1:
          if(raiz->dir->fb == 1) {
            printf("RSE %s\n", raiz->sinonimo);
            raiz = RSE_SINONIMO(raiz);
          } else {
            printf("RDE %s\n", raiz->sinonimo);
            raiz = RDE_SINONIMO(raiz);
          }
          raiz->fb = 0;
          *mudou_altura = 0;
          break;
      }
    }
    return raiz;
  }
}

/* A função removerSinonimo() busca por um sinônimo na árvore em seguida realiza sua remoção. */
Sinonimo *removerSinonimo(Sinonimo *raiz, char *sinonimo, int *mudou_altura) {
  if(raiz == NULL) return NULL;
  else if(strcmp(raiz->sinonimo, sinonimo) == 0) {
    /* nenhum filho */
    if(!raiz->esq && !raiz->dir) {
      *mudou_altura = 1;
      liberarSinonimo(raiz);
      return NULL;
    } else if(raiz->esq && !raiz->dir) {
      /* um filho (esquerdo) */
      Sinonimo *temp = raiz->esq;
      liberarSinonimo(raiz);
      *mudou_altura = 1;
      return temp;
    } else if(!raiz->esq && raiz->dir) {
      /* um filho (direito)*/
      Sinonimo *temp = raiz->dir;
      liberarSinonimo(raiz);
      *mudou_altura = 1;
      return temp;
    } else {
      /* dois filhos */
      Sinonimo *substituto = raiz->esq;
      while(substituto->dir) substituto = substituto->dir;
      /* liberar e realocar memoria para a string pois os tamanhos dos sinonimos podem ser diferentes */
      free(raiz->sinonimo);
      raiz->sinonimo = (char *) malloc(strlen(substituto->sinonimo) + 1);
      strcpy(raiz->sinonimo, substituto->sinonimo);
      raiz->esq = removerSinonimo(raiz->esq, substituto->sinonimo, mudou_altura);
      if(*mudou_altura == 1) {
        switch(raiz->fb) {
          case -1:
            raiz->fb = 0;
          break;
          case 0:
            raiz->fb = 1;
            *mudou_altura = 0;
          break;
          case 1:
            if(raiz->dir->fb == 0) {
              raiz = RSE_SINONIMO(raiz);
              raiz->esq->fb = 1;
              raiz->fb = -1;
              *mudou_altura = 0;
            } else if(raiz->dir->fb == 1) {
              raiz = RSE_SINONIMO(raiz);
              raiz->dir->fb = 0;
              raiz->fb = 0;
            } else {
              raiz = RDE_SINONIMO(raiz);
              raiz->fb = 0;
            }
          break;
        }
     }
      return raiz;
    }
  } else if(strcmp(sinonimo, raiz->sinonimo) < 0) {
    raiz->esq = removerSinonimo(raiz->esq, sinonimo, mudou_altura);
    if(*mudou_altura == 1) {
      switch(raiz->fb) {
        case -1:
          raiz->fb = 0;
        break;
        case 0:
          raiz->fb = 1;
          *mudou_altura = 0;
        break;
        case 1:
          if(raiz->dir->fb == 0) {
            raiz = RSE_SINONIMO(raiz);
            raiz->dir->fb = 1;
            raiz->fb = -1;
            *mudou_altura = 0;
          } else if(raiz->dir->fb == 1) {
            raiz = RSE_SINONIMO(raiz);
            raiz->dir->fb = 0;
            raiz->fb = 0;
          } else {
            raiz = RDE_SINONIMO(raiz);
            raiz->fb = 0;
          }
        break;
      }
    }
    return raiz;
  } else {
    raiz->dir = removerSinonimo(raiz->dir, sinonimo, mudou_altura);
    if(*mudou_altura == 1) {
      switch(raiz->fb) {
        case 1:
          raiz->fb = 0;
        break;
        case 0:
          raiz->fb = -1;
          *mudou_altura = 0;
        break;
        case -1:
          if(raiz->esq->fb == 0) {
            raiz = RSD_SINONIMO(raiz);
            raiz->fb = -1;
            raiz->dir->fb = 1;
            *mudou_altura = 0;
          } else if(raiz->esq->fb == -1) {
            raiz = RSD_SINONIMO(raiz);
            raiz->fb = 0;
            raiz->dir->fb = 0;
          } else if(raiz->esq->fb == 1) {
            raiz = RDD_SINONIMO(raiz);
            raiz->fb = 0;
          }
        break;
      }
    }
    return raiz;
  }
}

/* A função mostrarEmOrdem() percorre a árvore de sinônimos em ordem simétrica e mostra o sinonimo guardadas em cada nó. */
void mostrarArvoreDeSinonimosEmOrdem(Sinonimo *raiz);
void mostrarArvoreDeSinonimosEmOrdem(Sinonimo *raiz) {
  if(raiz->esq) mostrarArvoreDeSinonimosEmOrdem(raiz->esq);
  if(raiz) printf("\t%s\n", raiz->sinonimo);
  if(raiz->dir) mostrarArvoreDeSinonimosEmOrdem(raiz->dir);
}

/* A função mostrarArvoreDeSinonimosEmPreOrdem() percorre a árvore de sinonimos em pré ordem e mostra o sinonimo guardado em cada nó. */
void mostrarArvoreDeSinonimosEmPreOrdem(Sinonimo *raiz);
void mostrarArvoreDeSinonimosEmPreOrdem(Sinonimo *raiz){
  if(raiz) printf("%s | %d\n", raiz->sinonimo, raiz->fb);
  if(raiz->esq) mostrarArvoreDeSinonimosEmPreOrdem(raiz->esq);
  if(raiz->dir) mostrarArvoreDeSinonimosEmPreOrdem(raiz->dir);
}

/* A função totalDePalavras() percorre a estrutura e retorna quantos nós existem na arvóre de sinônimos. */
unsigned int totalSinonimos(Sinonimo *raiz) {
  if(raiz == NULL) return 0;
  return 1 + totalSinonimos(raiz->esq) + totalSinonimos(raiz->dir);
}

/* A estrutura 'palavra' representa um nó de uma árvore de palavras avl. */
typedef struct palavra {
  char *palavra;
  Sinonimo *arvore_sinonimos;
  struct palavra *esq, *dir;
  int fb;
} Palavra;

/* A função criarPalavra() aloca memória dinâmicamente para uma palavra e retorna o endereço de memória alocado. */
Palavra *criarPalavra(char *palavra);
Palavra *criarPalavra(char *palavra) {
  Palavra *nova = (Palavra *) malloc(sizeof(Palavra));
  if(nova == NULL) return NULL;
  nova->palavra = (char *) malloc(strlen(palavra) + 1);
  if(nova->palavra == NULL) return NULL;
  strcpy(nova->palavra, palavra);
  nova->dir = NULL; nova->esq = NULL;
  nova->arvore_sinonimos = NULL;
  nova->fb = 0;
  return nova;
}

/* A função liberarPalavra() libera a memória alocada para a palavra em questão */
void liberarPalavra(Palavra *no);
void liberarPalavra(Palavra *no) {
  if(no) {
    free(no->palavra);
    liberarArvoreDeSinônimos(no->arvore_sinonimos);
    free(no);
  }
}

/* As funções abaixo realizam operações de rotação para a árvore de PALAVRAS. */
/* Rotação Simples para Direita. */
Palavra *RSD_PALAVRA(Palavra *raiz) {
  Palavra *u, *t2;
  u = raiz->esq;
  t2 = u->dir;

  u->dir = raiz;
  raiz->esq = t2;

  raiz->fb = 0;
  return u;
}
/* Rotação Dupla para Direita. */
Palavra *RDD_PALAVRA(Palavra *raiz) {
  Palavra *u, *z, *t2, *t3;
  u = raiz->esq; z = u->dir;
  t2 = z->esq; t3 = z->dir;

  raiz->esq = t3;
  u->dir = t2;
  z->esq = u; z->dir = raiz;
  if(z->fb == -1) raiz->fb = 1;
  else raiz->fb = 0;

  if(z->fb == 1) u->fb =-1; 
  else u->fb = 0;

  return z;
}
/* Rotação Simples para Esquerda. */
Palavra *RSE_PALAVRA(Palavra *raiz) {
  Palavra *u, *t2;
  u = raiz->dir;
  t2 = u->esq;

  u->esq = raiz;
  raiz->dir = t2;
  raiz->fb = 0;
  
  return u;
}
/* Rotação Dupla para Direita. */
Palavra *RDE_PALAVRA(Palavra *raiz) {
  Palavra *b, *c, *t2, *t3;

  b = raiz->dir; c = b->esq;
  t2 = c->esq; t3 = c->dir;

  raiz->dir = t2;
  b->esq = t3;

  c->esq = raiz;
  c->dir = b;

  if(c->fb == 1) raiz->fb = -1;
  else raiz->fb = 0;

  if(c->fb == -1) b->fb = 1;
  else b->fb = 0;

  return c;
}

/* A função inserirPalavra() tenta inserir uma palavra na estrutra, e realiza as operações de balanceamento caso necessário. */
Palavra *inserirPalavra(Palavra *raiz, char *palavra, char *sinonimo, int *mudou_altura) {
  int mudou_altura_sinonimo = 0;
  if(raiz == NULL) {
    Palavra *nova;
    nova = criarPalavra(palavra);
    nova->arvore_sinonimos = inserirSinonimo(nova->arvore_sinonimos, sinonimo, &mudou_altura_sinonimo);
    *mudou_altura = 1;
    return nova;
  } else if(strcmp(palavra, raiz->palavra) == 0) {
    *mudou_altura = 0;
    raiz->arvore_sinonimos = inserirSinonimo(raiz->arvore_sinonimos, sinonimo, &mudou_altura_sinonimo);
    return raiz;
  } else if(strcmp(palavra, raiz->palavra) < 0 ) {
    raiz->esq = inserirPalavra(raiz->esq, palavra, sinonimo, mudou_altura);
    if(*mudou_altura == 1) {
      switch(raiz->fb) {
        case 1:
          raiz->fb = 0;
          *mudou_altura = 0; 
        break;
        case 0:
          raiz->fb = -1;
        break;
        case -1:
          if(raiz->esq->fb == -1) {
            raiz = RSD_PALAVRA(raiz);
          } else {
            raiz = RDD_PALAVRA(raiz);
          }
          *mudou_altura = 0;
          raiz->fb = 0;
        break;
      }
    }
    return raiz;
  } else {
    raiz->dir = inserirPalavra(raiz->dir, palavra,sinonimo, mudou_altura);
    if(*mudou_altura == 1) {
      switch(raiz->fb) {
        case -1:
          raiz->fb = 0;
          *mudou_altura = 0;
          break;
        case 0:
          raiz->fb = 1;
          break;
        case 1:
          if(raiz->dir->fb == 1) {
            raiz = RSE_PALAVRA(raiz);
          } else {
            raiz = RDE_PALAVRA(raiz);
          }
          raiz->fb = 0;
          *mudou_altura = 0;
          break;
      }
    }
    return raiz;
  }
}

/* A função buscaPalavra() procura por uma determinada palavra na estrutura e retrona o seu ponteiro. Caso não seja encontrado o retorno é NULL. */
Palavra *buscaPalavra(Palavra *raiz, char *palavra) {
  if(raiz == NULL) return NULL;
  else if(strcmp(palavra, raiz->palavra) == 0) return raiz;
  else if(strcmp(palavra, raiz->palavra) < 0) return buscaPalavra(raiz->esq, palavra);
  else return buscaPalavra(raiz->dir, palavra);
}

/* A função removerPalavra() busca por uma palavra na árvore em seguida realiza sua remoção. */
Palavra *removerPalavra(Palavra *raiz, char *palavra, int *mudou_altura) {
  if(raiz == NULL) return NULL;
  else if(strcmp(raiz->palavra, palavra) == 0) {
    /* nenhum filho */
    if(!raiz->esq && !raiz->dir) {
      *mudou_altura = 1;
      liberarPalavra(raiz);
      return NULL;
    } else if(raiz->esq && !raiz->dir) {
      /* um filho (esquerdo) */
      Palavra *temp = raiz->esq;
      liberarPalavra(raiz);
      *mudou_altura = 1;
      return temp;
    } else if(!raiz->esq && raiz->dir) {
      /* um filho (direito)*/
      Palavra *temp = raiz->dir;
      liberarPalavra(raiz);
      *mudou_altura = 1;
      return temp;
    } else {
      /* dois filhos */
      Palavra *substituto = raiz->esq;
      while(substituto->dir) substituto = substituto->dir;
      /* liberar e realocar memoria para a string pois os tamanhos das palavras podem ser diferentes */
      free(raiz->palavra);
      raiz->palavra = (char *) malloc(strlen(substituto->palavra) + 1);
      strcpy(raiz->palavra, substituto->palavra);
      raiz->esq = removerPalavra(raiz->esq, substituto->palavra, mudou_altura);
      if(*mudou_altura == 1) {
        switch(raiz->fb) {
          case -1:
            raiz->fb = 0;
          break;
          case 0:
            raiz->fb = 1;
            *mudou_altura = 0;
          break;
          case 1:
            if(raiz->dir->fb == 0) {
              raiz = RSE_PALAVRA(raiz);
              raiz->esq->fb = 1;
              raiz->fb = -1;
              *mudou_altura = 0;
            } else if(raiz->dir->fb == 1) {
              raiz = RSE_PALAVRA(raiz);
              raiz->dir->fb = 0;
              raiz->fb = 0;
            } else {
              raiz = RDE_PALAVRA(raiz);
              raiz->fb = 0;
            }
          break;
        }
     }
      return raiz;
    }
  } else if(strcmp(palavra, raiz->palavra) < 0) {
    raiz->esq = removerPalavra(raiz->esq, palavra, mudou_altura);
    if(*mudou_altura == 1) {
      switch(raiz->fb) {
        case -1:
          raiz->fb = 0;
        break;
        case 0:
          raiz->fb = 1;
          *mudou_altura = 0;
        break;
        case 1:
          if(raiz->dir->fb == 0) {
            raiz = RSE_PALAVRA(raiz);
            raiz->dir->fb = 1;
            raiz->fb = -1;
            *mudou_altura = 0;
          } else if(raiz->dir->fb == 1) {
            raiz = RSE_PALAVRA(raiz);
            raiz->dir->fb = 0;
            raiz->fb = 0;
          } else {
            raiz = RDE_PALAVRA(raiz);
            raiz->fb = 0;
          }
        break;
      }
    }
    return raiz;
  } else {
    raiz->dir = removerPalavra(raiz->dir, palavra, mudou_altura);
    if(*mudou_altura == 1) {
      switch(raiz->fb) {
        case 1:
          raiz->fb = 0;
        break;
        case 0:
          raiz->fb = -1;
          *mudou_altura = 0;
        break;
        case -1:
          if(raiz->esq->fb == 0) {
            raiz = RSD_PALAVRA(raiz);
            raiz->fb = -1;
            raiz->dir->fb = 1;
            *mudou_altura = 0;
          } else if(raiz->esq->fb == -1) {
            raiz = RSD_PALAVRA(raiz);
            raiz->fb = 0;
            raiz->dir->fb = 0;
          } else if(raiz->esq->fb == 1) {
            raiz = RDD_PALAVRA(raiz);
            raiz->fb = 0;
          }
        break;
      }
    }
    return raiz;
  }
}

Palavra *removerSinonimoAssociado(Palavra *raiz, char *palavra, char *sinonimo) {
  if(raiz == NULL) {
    return NULL;
  } else if(strcmp(raiz->palavra, palavra) == 0) {
    int mudou_altura = 0;
    raiz->arvore_sinonimos = removerSinonimo(raiz->arvore_sinonimos, sinonimo, &mudou_altura);
    return raiz;
  } else if(strcmp(palavra, raiz->palavra) < 0) {
    return removerSinonimoAssociado(raiz->esq, palavra, sinonimo);
  }
}
/* A função mostrarArvoreDePalavrasEmPreOrdem() percorre a árvore de palavras em pré ordem e mostra a palavra guardada em cada nó. */
void mostrarArvoreDePalavrasEmPreOrdem(Palavra *raiz){
  if(raiz) {
    printf("%s\n", raiz->palavra);
    mostrarArvoreDeSinonimosEmOrdem(raiz->arvore_sinonimos);
  }
  if(raiz->esq) mostrarArvoreDePalavrasEmPreOrdem(raiz->esq);
  if(raiz->dir) mostrarArvoreDePalavrasEmPreOrdem(raiz->dir);
}

/* A função totalDePalavras() percorre a estrutura e retorna quantos nós existem na arvóre de palavras. */
unsigned int totalDePalavras(Palavra *raiz) {
  if(raiz == NULL) return 0;
  return 1 + totalDePalavras(raiz->esq) + totalDePalavras(raiz->dir);
}
/* A função escreveSinonimos() escreve a árvore de sinônimos no arquivo. */
void escreveSinonimos(FILE *arq, Sinonimo *raiz) {
  if(raiz) {
    escreveSinonimos(arq,raiz->esq);
    fprintf(arq, "%s\n", raiz->sinonimo);
    escreveSinonimos(arq,raiz->dir);
  }
} 

/* A função escrevePalavras(), para cada palavra escreve no arquivo a palavra, o número total de sinônimos e por fim a árvore de sinônimos. */
void escrevePalavras(FILE *arq, Palavra *raiz) {
  if(raiz) {
    int total_sinonimos = totalSinonimos(raiz->arvore_sinonimos);
    escrevePalavras(arq, raiz->esq);
    fprintf(arq, "%s\n", raiz->palavra);
    fprintf(arq, "%d\n", total_sinonimos);
    escreveSinonimos(arq, raiz->arvore_sinonimos);
    escrevePalavras(arq, raiz->dir);
  }
}

/* A funçao salvarNoArquivo() abre o arquivo passado por parâmetro como arquivo de texto e escreve nele a estrutura. */
void salvarNoArquivo(Palavra *raiz, char *arquivo) {
  FILE *arq;
  arq = fopen(arquivo, "w");
  if(!arq) {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", arquivo);
  } else {
    int total_palavras = totalDePalavras(raiz);
    fprintf(arq, "%d\n", total_palavras);
    escrevePalavras(arq, raiz);
    fclose(arq);
  }
}

/* A função lerPalavras() lê os dados do arquivo e insere na estrutura, retornando a estrutura preenchida */
Palavra *lerPalavras(FILE *arq, Palavra *raiz, int total_palavras) {
  int i;
  for(i = 0; i < total_palavras; i++) {
    char palavra[30];
    int total_sinonimos, j;
    fscanf(arq, "%s\n", palavra);
    /* Quantos sinônimos precisam ser adicionados na palavra atual. */
    fscanf(arq, "%d\n", &total_sinonimos);
    for(j = 0; j < total_sinonimos; j++) {
      char sinonimo[30];
      int mudou_altura;
      mudou_altura = 0;
      fscanf(arq, "%s\n",sinonimo);
      raiz = inserirPalavra(raiz, palavra, sinonimo, &mudou_altura);
    }
  }
  return raiz;
}
/* A função carregar() abre o arquivo passado por parâmetro no modo de leitura e chama a função de lerPalavras() e retorna a estrutura preenchida. */
Palavra *carregar(Palavra *raiz, char *arquivo) {
  FILE *arq;
  arq = fopen(arquivo, "r");
  if(!arq) {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", arquivo);
  } else {
    int total_palavras;
    /* Quantas vezes o laço de inserção de palavras deve rodar. */
    fscanf(arq, "%d\n", &total_palavras);
    raiz = lerPalavras(arq, raiz, total_palavras);
    fclose(arq);
  }
  return raiz;
}

#endif /* AVL_H */

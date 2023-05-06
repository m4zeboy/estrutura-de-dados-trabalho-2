/* A estrutura 'palavra' representa um nó de uma árvore de palavras avl. */
typedef struct palavra {
  char *palavra;
  Sinonimo *arvore_sinonimos;
  struct palavra *esq, *dir;
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

/* A função inserir() insere uma palavra na árvore avl se ela não estiver na estrutura. o parâmetro h serve para avisar que houve uma alteração na altura da arvore, assim pode seguir com as rotações necessárias. */
Palavra *inserirPalavra(Palavra *raiz, char *palavra);
Palavra *inserirPalavra(Palavra *raiz, char *palavra) {
  if(raiz == NULL) { return criarPalavra(palavra); } 
  else if(strcmp(palavra, raiz->palavra) == 0) return NULL;
  else if(strcmp(palavra, raiz->palavra) < 0) {
    raiz->esq = inserirPalavra(raiz->esq, palavra);
    return raiz;
  } else {
    raiz->dir = inserirPalavra(raiz->dir, palavra);
    return raiz;
  }
}

/* A função mostrarEmOrdem() percorre a árvore de palavras em ordem simétrica e mostra as palavras guardadas em cada nó. */
void mostrarArvoreDePalavrasEmOrdem(Palavra *raiz);
void mostrarArvoreDePalavrasEmOrdem(Palavra *raiz){
  if(raiz->esq) mostrarArvoreDePalavrasEmOrdem(raiz->esq);
  if(raiz) printf("%s\n", raiz->palavra);
  if(raiz->dir) mostrarArvoreDePalavrasEmOrdem(raiz->dir);
}

/* A função buscarPalavra() realiza uma busca binária na estrutura por uma palavra e retorna o seu respectivo endereço de memória */
Palavra *buscarPalavra(Palavra *raiz, char *palavra);
Palavra *buscarPalavra(Palavra *raiz, char *palavra) {
  if(raiz == NULL) return NULL;
  else if(strcmp(raiz->palavra, palavra) == 0) return raiz;
  else if(strcmp(palavra, raiz->palavra) < 0) return buscarPalavra(raiz->esq, palavra);
  else return buscarPalavra(raiz->dir, palavra);
}

Palavra *inserirPar(Palavra *raiz, char *palavra, char *sinonimo) {
  Palavra *temp;
  raiz = inserirPalavra(raiz, palavra);
  temp = buscarPalavra(raiz, palavra);
  if(temp) {

  }
  return raiz;
}
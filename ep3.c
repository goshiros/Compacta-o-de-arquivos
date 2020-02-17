/* ************************************************************************** */
/* Nome: Giovana Oshiro da Silva                                              */
/* Numero USP: 8022103                                                        */
/*                                                                            */
/* Nome: Lucas Freitas Bastos                                                 */
/* Numero USP: 9783118                                                        */
/*                                                                            */
/* ExercÌÅcio-programa 3                                                       */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct reg{
  int frequencia;
  unsigned char letra;
  struct reg *esq, *dir;
};

typedef struct reg celula;

/* Funcao "subir" para um heap adaptada para um vetor do tipo celula, com dois
ponteiros e dois conteudos. Nesse caso, todas essas estruturas tambem devem ser
trocadas quando uma posicao do vetor eh trocada.
A casa 0 do vetor eh ocupada por -1 pois ela nao vai ser usada. Para um heap, o
vetor deve comecar na casa 1.*/

void subir_heap (int i, celula heap []){
  int j, aux_frequencia;
  char aux_letra;
  celula *aux_dir, *aux_esq;
  j = i/2;

  if (j < i){
    if (heap[i].frequencia > heap[j].frequencia && heap[i].frequencia != -1 && heap[j].frequencia != -1){
      aux_frequencia = heap[i].frequencia;
      heap[i].frequencia = heap[j].frequencia;
      heap[j].frequencia = aux_frequencia;

      aux_letra = heap[i].letra;
      heap[i].letra = heap[j].letra;
      heap[j].letra = aux_letra;

      aux_dir = heap[i].dir;
      heap[i].dir = heap[j].dir;
      heap[j].dir = aux_dir;

      aux_esq = heap[i].esq;
      heap[i].esq = heap[j].esq;
      heap[j].esq = aux_esq;

      subir_heap (j, heap);
    }
  }
}

/* A funcao "construir_heap" constroi um vetor em heap a partir da funcao
"subir" */

void construir_heap (int n, celula heap []){
  int i;
  for (i = 2; i < n; i++) subir_heap (i, heap);
}

/* A funcao minimo recebe um vetor de celula ordenado em heap e o seu tamanho,
procura a casa com a menor das frequÍncias e troca todo o conte˙do dessa casa
com a ultima posicao do vetor. */

void minimo (celula heap [], int n){
  int i, minimo = 1, aux_frequencia;
  char aux_letra;
  celula *aux_dir, *aux_esq;

  for (i = 2; i < n; i++)
    if (heap[i].frequencia < heap[minimo].frequencia)
      minimo = i;

  aux_frequencia = heap[minimo].frequencia;
  heap[minimo].frequencia = heap[n-1].frequencia;
  heap[n-1].frequencia = aux_frequencia;

  aux_letra = heap[minimo].letra;
  heap[minimo].letra = heap[n-1].letra;
  heap[n-1].letra = aux_letra;

  aux_dir = heap[minimo].dir;
  heap[minimo].dir = heap[n-1].dir;
  heap[n-1].dir = aux_dir;

  aux_esq = heap[minimo].esq;
  heap[minimo].esq = heap[n-1].esq;
  heap[n-1].esq = aux_esq;

}

/* A funcao "monta_matriz" faz uma tabela de todos os caminhos da raiz de uma
arvore ate suas folhas, adotando o criterio 0 para a esquerda e 1 para a direita
ao percorrer a arvore. */

void monta_matriz (celula *noh, int binario [256][50], int altura, int caminho [], int direcao){
  int j, asc;

  if (noh == NULL) return;

  if (direcao != -1){
    caminho [altura] = direcao;
    altura++;
  }

  if (noh->esq == NULL && noh->dir == NULL){
    asc = noh->letra;
    for (j = 0; j < altura; j++) binario [asc][j] = caminho [j];
  }

  direcao = 0;
  monta_matriz (noh->esq, binario, altura, caminho, direcao);

  direcao = 1;
  monta_matriz (noh->dir, binario, altura, caminho, direcao);
}

/* A funcao "bin_asc" recebe um numero binario e o transforma em um caractere. */

unsigned char bin_asc (int v_binario [8]){
  int dec = 0, bin = 0, i;
  unsigned char bin_asc;

  for (i = 0; i < 8; i++) bin = bin + v_binario [i] * pow(10,(7-i));

  for(i = 0; bin > 0; i++){
    dec = dec + pow(2, i) * (bin % 10);
    bin = bin / 10;
  }

  bin_asc = dec;

  return bin_asc;
}

/* A funcao "asc_bin" recebe um caractere e o transforma em um binario. */

void asc_bin (unsigned char caractere, int v_binario []){
  int dec, i;

  for (i = 0; i < 8; i++) v_binario [i] = 0; /* "zerar" vetor */

  dec = caractere;

  for (i = 7; dec != 0; i--){
    v_binario [i] = dec % 2;
    dec = dec / 2;
  }
}

int main (){

  FILE * entrada;
  FILE * saida;
  int comando, i, frequencias [256], leitura = 0, asc, n = 1, j, minimo1, minimo2;
  int binario[256][50], caminho [50], altura = 0, direcao = -1; /* funÁ„o monta_matriz */
  int t = 0, v_binario [8], aux_binario [300], p, flag = 0;
  int frequencia;
  char arquivo_entrada [50], arquivo_saida [50];
  unsigned char caractere, letra1, letra2, aux;
  celula *heap, *esq1, *esq2, *dir1, *dir2, *noh, *raiz;

  printf ("Digite 1 para compactar ou 2 para descompactar:\n");

  scanf ("%d", &comando);

  if (comando == 1){

    printf ("Entre com o nome do arquivo a ser compactado:\n");
    scanf ("%s", arquivo_entrada);

    printf ("Entre com o nome do arquivo de saida:\n");
    scanf ("%s", arquivo_saida);

    entrada = fopen (arquivo_entrada, "r");
    saida = fopen (arquivo_saida, "w");

    for (i = 0; i < 256; i++) frequencias [i] = 0; /* zerar vetor */

    while (leitura != EOF){
      leitura = fscanf (entrada, "%c", &caractere);
      asc = caractere;
      frequencias [asc] ++;
    }

    for (i = 0; i < 256; i++) /* verificar quantas casas de frequencias[] estao
                                                                    zeradas */
      if (frequencias [i] != 0)
        n ++;

    heap = malloc (n*sizeof(celula));

    /* vetor heap, que contem apenas as casas nao zeradas de frequencias [], ou
    seja, apenas os caracteres presentes no arquivo, eh criado e inicializado */

    for (i = 0; i < n; i++){ /* zerar vetor */
      heap[i].frequencia = 0;
      heap[i].esq = NULL;
      heap[i].dir = NULL;
      heap[i].letra = '_';
    }

    heap[0].frequencia = -1; /* casa 0 do vetor nao eh considerada (nao satisfaz
                                                                    o heap) */

    j = 1;

    for (i = 0; i < 256; i++){ /* frequencias sao armazenadas em heap [] */
      if (frequencias [i] != 0){
        heap [j].frequencia = frequencias [i];
        heap [j].letra = i;
        j++;
      }
    }

    construir_heap (n, heap);

    for (i = 1; i < n-1; i ++){ /* imprimir arvore no arquivo de saida */
      fprintf (saida, "%d=%d ", heap[i].letra, heap[i].frequencia);
    }
    fprintf (saida, "%d=%d\n", heap[n-1].letra, heap[n-1].frequencia);

    /* O programa entra, entao, num laco para construir a arvore de Huffman. As
    menores frequencias, duas a duas, do vetor heap [] sao retiradas do vetor e
    a soma das frequÍncias eh inserida no vetor. Os ponteiros *esq e *dir apontam
    para as frequencias retiradas (as frequencias minimas). Isto eh, cada soma de
    frequencia inserida no vetor eh um no que eh pai das duas frequencias minimas. */

    while (n > 2){

      minimo (heap, n);
      minimo1 = heap[n-1].frequencia;
      letra1 = heap[n-1].letra;
      esq1 = heap[n-1].esq;
      dir1 = heap[n-1].dir;
      n--; /* a cada interacao uma casa do vetor eh desconsiderada */


      minimo (heap, n);
      minimo2 = heap[n-1].frequencia;
      letra2 = heap[n-1].letra;
      esq2 = heap[n-1].esq;
      dir2 = heap[n-1].dir;

      heap[n-1].frequencia = minimo1 + minimo2;
      heap[n-1].letra = '_'; /* a letra de cada no criado eh '_' */

      heap[n-1].esq = malloc (sizeof(celula));
      heap[n-1].esq->frequencia = minimo2;
      heap[n-1].esq->letra = letra2;
      heap[n-1].esq->esq = esq2;
      heap[n-1].esq->dir = dir2;

      heap[n-1].dir = malloc (sizeof(celula));
      heap[n-1].dir->frequencia = minimo1;
      heap[n-1].dir->letra = letra1;
      heap[n-1].dir->esq = esq1;
      heap[n-1].dir->dir = dir1;

      subir_heap (n-1, heap); /* vetor eh rearranjado */

    }

    /* Vetor "caminho" vai ser preenchido com os caminhos da raiz da arvore ate
    cada uma de suas folhas, uma de cada vez. Ele eh inicializado com -1.*/
    for (i = 0; i < 50; i++) caminho [i] = -1;

    /* A matriz "binario" possui todos os caminhos da raiz da arvore ate as suas
    folhas. Ela eh inicializado com -1.*/
    for (i = 0; i < 256; i++)
      for (j = 0; j < 50; j++)
        binario [i][j] = -1;

    /* Tabela com todos os caminhos da raiz ate as suas folhas eh criada. */
    monta_matriz (&heap[1], binario, altura, caminho, direcao);

    rewind (entrada); /* arquivo vai ser lido novamente */

    leitura = 0;

    /* O programa entra num laco em que cada caractere do arquivo de entrada vai
    ser lido e transformado em seu respectivo valor de 0 ou 1 da tabela de caminhos.
    A cada 8 "zeros" ou "uns", esse conjunto eh transformado em seu respectivo
    caractere da tabela ASCII. Sao utilizados dois vetores para esse processo,
    um maior auxiliar e outro menor, que armazena apenas o binario a ser transformado
    em caractere por interacao e eh impresso no arquivo de saida. */

    for (p = 0; p < 8; p++) v_binario [p] = -1; /* "zerar" vetor */

    for (p = 0; p < 256; p++) aux_binario [p] = -1; /* "zerar" vetor */

    i = 0;

    while (leitura != EOF){

      leitura = fscanf (entrada, "%c", &caractere);
      asc = caractere;

      t = 0;

      for (j = 0; binario [asc][j] != -1; j++) aux_binario [j] = binario [asc][j];

      while (t < j && aux_binario [t] != -1){

        if (i <= 7){
          v_binario [i] = aux_binario [t];
          i++;
          t++;
        }

        if (i > 7){
          i = 0;
          fprintf (saida, "%c", bin_asc (v_binario));
          for (p = 0; p < 8; p++) v_binario [p] = -1; /* "zerar" vetor*/
        }

      } /* while (t < j && aux_binario [t] != -1) */

      for (p = 0; p < 256; p++) aux_binario [p] = -1;

    } /* while (leitura != EOF) */

    /* Como nem sempre o arquivo transformado em binario possuira um numero de
    caracteres multiplo de 8, havera um numero entre 1 e 7 de caracteres restantes.
    Nesse caso, resolvemos completar o binario de 8 caracteres colocando zeros
    a esquerda. */

    flag = 0;

    for (p = 0; p < 8 && flag == 0; p++){
      if (v_binario [p] == -1){
        flag = 1;
        for (i = 0; i < p; i++) v_binario [i + 8 - p] = v_binario [i];
        for (i = 0; i < 8 - p; i++) v_binario [i] = 0;
        fprintf (saida, "%c", bin_asc (v_binario));
      }
    } /* for (p = 0; p < 8 && flag == 0; p++) */

    fclose (entrada);
    fclose (saida);

    printf ("Verifique o arquivo de saÌda para visualizar o arquivo compactado.\n");

  } /* if (comando == 1) */

  if (comando == 2){

    printf ("Entre com o nome do arquivo a ser descompactado:\n");
    scanf ("%s", arquivo_entrada);

    printf ("Entre com o nome do arquivo de saÌda:\n");
    scanf ("%s", arquivo_saida);

    entrada = fopen (arquivo_entrada, "r");
    saida = fopen (arquivo_saida, "w");

    for (i = 0; i < 256; i++) frequencias [i] = 0; /* zerar vetor */

    /* O arquivo de entrada possui na primeira linha a arvore impressa, com os
    caracteres e suas respectivas frequÍncias. Vamos ler a arvore e remontar
    o vetor frequencias []. Depois disso, o programa eh executado exatamente da
    mesma forma que na compactacao, ate a arvore ser remontada. */

    while (caractere != '\n'){
      fscanf (entrada, "%d", &asc);
      fscanf (entrada, "%c", &caractere);
      fscanf (entrada, "%d", &frequencia);
      frequencias [asc] = frequencia;
      fscanf (entrada, "%c", &caractere);
    }

    for (i = 0; i < 256; i++)
      if (frequencias [i] != 0)
        n ++;

    heap = malloc (n*sizeof(celula));

    for (i = 0; i < n; i++){ /* zerar vetor */
      heap[i].frequencia = 0;
      heap[i].esq = NULL;
      heap[i].dir = NULL;
      heap[i].letra = '_';
    }

    heap[0].frequencia = -1;

    j = 1;

    for (i = 0; i < 256; i++){
      if (frequencias [i] != 0){
        heap[j].frequencia = frequencias [i];
        heap[j].letra = i;
        j++;
      }
    }

    construir_heap (n, heap);

    while (n > 2){

      minimo (heap, n);
      minimo1 = heap[n-1].frequencia;
      letra1 = heap[n-1].letra;
      esq1 = heap[n-1].esq;
      dir1 = heap[n-1].dir;
      n--;


      minimo (heap, n);
      minimo2 = heap[n-1].frequencia;
      letra2 = heap[n-1].letra;
      esq2 = heap[n-1].esq;
      dir2 = heap[n-1].dir;

      heap[n-1].frequencia = minimo1 + minimo2;
      heap[n-1].letra = '_';

      heap[n-1].esq = malloc (sizeof(celula));
      heap[n-1].esq->frequencia = minimo2;
      heap[n-1].esq->letra = letra2;
      heap[n-1].esq->esq = esq2;
      heap[n-1].esq->dir = dir2;

      heap[n-1].dir = malloc (sizeof(celula));
      heap[n-1].dir->frequencia = minimo1;
      heap[n-1].dir->letra = letra1;
      heap[n-1].dir->esq = esq1;
      heap[n-1].dir->dir = dir1;

      subir_heap (n-1, heap);

    } /* while (n > 2) */

    for (i = 0; i < 8; i++) v_binario [i] = -1; /* vetor eh "zerado" */

    /* O programa le os caracteres da segunda linha do arquivo compactado um a
    um e percorre a arvore da seguinte forma: se o caractere for 0, o ponteiro
    passara para o proximo no a esquerda; se for 1, o ponteiro passa para o proximo
    no a direita. Entao, o programa checa se esse no eh uma folha. Caso seja, ele
    imprime o caractere contido nessa folha e o ponteiro retorna para a raiz. O
    processo eh repetido ate o arquivo acabar.
    O ultimo caractere nao eh impresso nesse momento. */

    raiz = &heap[1];
    noh = raiz;

    leitura = fscanf (entrada, "%c", &caractere);

    while (leitura != EOF){

      aux = caractere;
      leitura = fscanf (entrada, "%c", &caractere);

      if (leitura != EOF){
        asc_bin (aux, v_binario);
        for (i = 0; i < 8; i++){
          if (v_binario[i] == 0) noh = noh->esq;
          if (v_binario[i] == 1) noh = noh->dir;
          if (noh->esq == NULL && noh->dir == NULL){
            fprintf (saida, "%c", noh->letra);
            noh = raiz;
          }
        }
        for (i = 0; i < 8; i++) v_binario [i] = -1;
      } /* if (leitura != EOF) */

    } /* while (leitura != EOF) */

    /* Como completamos o binario do ultimo caractere com zeros a esquerda, ao
    ler esse ultimo caractere, so levamos em conta o que esta depois do primeiro
    1. */

    p = 0;

    while (v_binario [p] == 0) p++;

    for (i = p; i < 8; i++){
      if (v_binario [i] == 0) noh = noh->esq;
      if (v_binario[i] == 1) noh = noh->dir;
      if (noh->esq == NULL && noh->dir == NULL) fprintf (saida, "%c", noh->letra);
    }

    fclose (entrada);
    fclose (saida);

    printf ("Verifique o arquivo de saida para visualizar o arquivo descompactado.\n");


  } /* if (comando == 2) */

  return 0;

}

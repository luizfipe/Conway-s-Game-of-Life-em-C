#include <stdio.h>
#include <stdlib.h>
// #include <curses.h>
#include <unistd.h>
#include <string.h>

#define reset "\x1b[0m" // definindo as cores usadas no Trabalho
#define pisca "\x1b[5m"
#define graf "\x1b[7m"
#define back "\x1b[0;100m"
#define green "\x1b[32m"

typedef long long ll;

typedef struct { // struct para armazenar as matrizes
  char life[100];
  char copy[100];
  char first[100];
} Vida;

int cord[2][8] = // coordenadas para andar por X e Y na matriz
    {
        {-1, -1, -1, 0, 1, 1, 1, 0},
        {-1, 0, 1, 1, 1, 0, -1, -1},
};

void limpador() // função para limpar a tela do jogo
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

int check(int x, int y, int l, int c) { // função para checar se as coordenadas de análise x e y da matriz são viáveis
  if (x < 0 || x >= c || y < 0 || y >= l)
    return 0;
  else
    return 1;
}

void simulacao(Vida *p, int c, int l, int pop, long long q) { // função que vai executar todas as simulações prontas ou criadas pelo usuário
  int i, j, k;
  int x, y;     // coordenada em volta da posição
  int vid;      // contador da célula vivas adjacentes
  int gera = 0; // quantidade de geraçõe executadas
  limpador();

  printf("Geracao: %d\n", gera); // printar geração 0
  printf("Populacao: %d\n", pop);
  for (i = 0; i < l; i++) {
    printf(back);

    for (j = 0; j < c; j++) {
      if (p[i].life[j] == 'O') {
        printf(green "%c " reset, p[i].life[j]);
        printf(back);
      } else
        printf("%c ", p[i].life[j]);
    }
    printf(reset "\n");
  }

  while (q--) { // copia a matriz p.life para p.copy
    gera++;
    for (i = 0; i < l; i++) {
      for (j = 0; j < c; j++) {
        p[i].copy[j] = p[i].life[j];
      }
    }

    for (i = 0; i < l; i++) {
      for (j = 0; j < c; j++) {
        // analisa os espaços adjacentes ao ponto atual, a matriz deve ser
        // copiada pois a matriz p.life será alterada durante a análise
        vid = 0;
        if (p[i].life[j] == 'O') { // analisa caso seja uma célula viva
          for (k = 0; k < 8; k++) {
            x = i + cord[0][k];
            y = j + cord[1][k];
            if (check(x, y, l, c) && p[x].copy[y] == 'O') {
              vid++;
            }
          }
          if (vid <= 1 || vid > 3) {
            p[i].life[j] = '.';
            pop--;
          }
        } else { // analisa caso seja uma célula morta
          for (k = 0; k < 8; k++) {
            x = i + cord[0][k];
            y = j + cord[1][k];
            if (check(x, y, l, c) && p[x].copy[y] == 'O') {
              vid++;
            }
          }
          if (vid == 3) {
            p[i].life[j] = 'O';
            pop++;
          }
        }
      }
    }

    usleep(500000);

    limpador();
    printf("Geracao: %d\n",
           gera); // printa o resultado da nova geração da matriz
    printf("Populacao: %d\n", pop);
    for (i = 0; i < l; i++) {
      printf(back);

      for (j = 0; j < c; j++) {
        if (p[i].life[j] == 'O') {
          printf(green "%c " reset, p[i].life[j]);
          printf(back);
        } else
          printf("%c ", p[i].life[j]);
      }
      printf(reset "\n");
    }
  }

  printf("Fim da simulacao, pressione enter para voltar");
  getchar();
}

void salvos() {
  int i = 2, j, l, c, pop;
  
  FILE *pFile = fopen("nomes.txt", "rt"); //abrir arquivos dos nomes
  FILE *bin; //abrir arquivos binarios
  FILE *mat;
  
  char nome[110], *tam;
  fflush(stdin);

  if(pFile == NULL) { //caso ainda não tenha nada salvo
    printf("Nenhum arquivo encontrado\n");
    printf("Pressione enter para sair\n");
    getchar();
    return;
  }
  
  while(1) {  
    limpador(); //lista de arquivos
    printf("Escreva o nome de um arquivo salvo valido, ou '000' para voltar\n");
    printf("1 - ");

    c = fgetc(pFile);
    while(c != EOF) {
      putchar(c);
      c = fgetc(pFile);
      if(c == '\n') {
        putchar(c);
        c = fgetc(pFile);
        if (c != EOF) {
          printf("%d - ", i);
          i++;
        }
      }
    }
    
    scanf("%s%*c", nome); //escolhe o arquivo

    if(nome[0] == '0' && nome[1] == '0' && nome[2] == '0') {
      fclose(pFile);
      return;
    }

    tam = (char*)calloc(strlen(nome)+1,sizeof(char));
    
    strcpy(tam, nome); 
    
    tam[strlen(tam)] = 'a';
    printf("%s\n", tam);

    bin = fopen(tam, "rb");
    if (bin == NULL) {
      printf("Arquivo inexistente, pressioene enter para continuar");
      getchar();
      fclose(bin);
    }
    else break;
  }
  
  int gera; //abre as linhas colunas e populacao
  fread(&l, sizeof(int), 1, bin);
  fread(&c, sizeof(int), 1, bin);
  fread(&pop, sizeof(int), 1, bin);
  mat = fopen(nome, "rt");
  
  Vida *p;

  p = (Vida*)malloc(l*sizeof(Vida)); //abre o arquivo de texto da matriz

  for(i = 0; i < l; i++) {
    for(j = 0; j < c; j++) {
      p[i].life[j] = fgetc(mat);
    }
  }
  
  
  while (1) { // pega uma quantidade válida de gerações
    limpador();
    
    printf("Escreva a quantidade de simulacoes para a geracao: ");
    scanf("%d%*c", &gera);
    if (gera >= 0)
      break;
    else {
      printf("Geracao invalida. Aperte enter para continuar");
      getchar();
    }
  }
  
  simulacao(p, c, l, pop, gera); //envia a simulacao salva
  free(p);
  fclose(pFile);
  fclose(mat);
  fclose(bin);
  free(tam);
}

void salvar(Vida *p, int c, int l, int pop) { //funcao para salvar as simulacoes criadas
  int i, j;
  FILE *pFile = fopen("nomes.txt", "at"); //arquivo de nomes
  char nome[110];
  
  
  while(1) {
    limpador(); //cadiciona o nome do arquivo na pasta nomes
    printf("digite o nome da simulacao que deseja salvar, ela não deve conter espaco e deve ter menos de 100 caracteres\n");
    printf("ATENCAO: NOMES EXISTENTES SERAO SUBSTITUIDOS!!!\n");
    fgets(nome, 110, stdin);
    nome[strlen(nome)-1] = '\0';
    if(strlen(nome) > 100) continue;
    else break;
  }
  
  for(i = 0; i < strlen(nome); i++) {
    fputc(nome[i], pFile);
  }
  fputc('\n', pFile);
  fclose(pFile);
  
  pFile = fopen(nome, "wt"); //abre arquivo da matriz

  for(i = 0; i < l; i++) {
    for(j = 0; j < c; j++) {
      fputc(p[i].first[j], pFile);
    }
  }
  fclose(pFile);
      
  
  nome[strlen(nome)] = 'a'; //mudar o nome de tam para mudar os arquivos
  
  int *t = (int*)malloc(3*sizeof(int)); //salvar as linhas e as colunas
  t[0] = l;
  t[1] = c;
  t[2] = pop;

  pFile = fopen(nome, "wb");
  fwrite(t, sizeof(int) , 3, pFile);
  fclose(pFile);

  
}

// função do modo de criação da sua própria simulação
void sandbox() {
  int l, c;    // dimensões da matriz
  int x, y;    // coordenada da matriz
  int pop = 0; // população da matriz
  while (1) {  // pega a quantidade de colunas
    limpador();
    printf("Digite as dimensões (de 1 - 100) de sua Matriz de Conway\n");
    printf("X: ");
    scanf("%d", &c);

    if (c > 0 && c <= 100)
      break;
  }

  while (1) { // pega a quantidade de linhas
    limpador();
    printf("Digite as dimensões (de 1 - 100) de sua Matriz de Conway\n");
    printf("X: %d\n", c);
    printf("Y: ");
    scanf("%d%*c", &l);

    if (l > 0 && l <= 100)
      break;
  }

  printf("Pressione enter para continuar");
  scanf("%*c");

  Vida *p =
      (Vida *)malloc(l * sizeof(Vida)); // cria a matriz por um ponteiro Vida
  int i, j;

  for (i = 0; i < l; i++) { // define todos os elementos da matriz como "."
    for (j = 0; j < c; j++) {
      p[i].life[j] = '.';
    }
  }

  while (1) {
    limpador();
    for (i = 0; i < l; i++) {
      printf(back);
      for (j = 0; j < c; j++) { // printa o estado atual da matriz
        if (p[i].life[j] == '.')
          printf("%c ", p[i].life[j]);
        else
          printf(green "%c " reset, p[i].life[j]);
        printf(back);
      }
      printf("%d", i + 1);
      printf(reset "\n");
    }

    printf("Escolha as coordenada para criar uma celula viva(insira \"0 0\" "
           "para sair da escolha)\n");
    scanf("%d %d%*c", &x,
          &y); // pega as coordenadas para adicionar uma celula viva
    if (x == 0 && y == 0)
      break;
    else if (x < 1 || x > c || y < 1 ||
             y > l) { // não adiciona a coordenada invalida na matriz
      printf("coordenada invalida, pressione enter para continuar");
      getchar();
      continue;
    } else {
      x--;
      y--;
      p[y].life[x] = 'O';
      pop++; // aumenta a população
    }
  }
  int gera;
  while (1) { // pega uma quantidade válida de gerações
    limpador();
    printf("Escreva a quantidade de simulacoes para a geracao: ");
    scanf("%d%*c", &gera);
    if (gera >= 0)
      break;
    else {
      printf("Geracao invalida. Aperte enter para continuar");
      getchar();
    }
  }
  for (i = 0; i < l; i++) {
    for (j = 0; j < c; j++) {
      p[i].first[j] = p[i].life[j];
    }
  }

  simulacao(p, c, l, pop, gera); // envia a matriz pelo ponteiro p, junto com as linhas,
                   // colunas, a população e a quantidade de gerações
  printf("voce gostaria de salvar sua simulacao? pressione 'c' caso deseje salvar o comeco, ou '0' para voltar ao menu\n");
  char salv;
  scanf("%c%*c", &salv);
      
  fflush(stdin);
  if(salv == 'c' || salv == 'C') salvar(p, c, l, pop);
  free(p); // libera o ponteiro Vida
}

int menu() { // função do menu, ele retorna um número que vai para a main no
             // switch
  int select;
  limpador();
  printf(graf);
  printf("           ▐▄▄▄       ▄▄ •           ·▄▄▄▄   ▄▄▄·      ▌ ▐·▪  ·▄▄▄▄   ▄▄▄·     ·▄▄▄▄  ▄▄▄ .     ▄▄·        ▐ ▄ ▄▄▌ ▐ ▄▌ ▄▄▄·  ▄· ▄▌\n");
  printf("▪           ·██▪     ▐█ ▀ ▪▪         ██▪ ██ ▐█ ▀█     ▪█·█▌██ ██▪ ██ ▐█ ▀█     ██▪ ██ ▀▄.▀·    ▐█ ▌▪▪     •█▌▐███· █▌▐█▐█ ▀█ ▐█▪██▌\n");
  printf(" ▄█▀▄     ▪▄ ██ ▄█▀▄ ▄█ ▀█▄ ▄█▀▄     ▐█· ▐█▌▄█▀▀█     ▐█▐█•▐█·▐█· ▐█▌▄█▀▀█     ▐█· ▐█▌▐▀▀▪▄    ██ ▄▄ ▄█▀▄ ▐█▐▐▌██▪▐█▐▐▌▄█▀▀█ ▐█▌▐█▪\n");
  printf("▐█▌.▐▌    ▐▌▐█▌▐█▌.▐▌▐█▄▪▐█▐█▌.▐▌    ██. ██ ▐█ ▪▐▌     ███ ▐█▌██. ██ ▐█ ▪▐▌    ██. ██ ▐█▄▄▌    ▐███▌▐█▌.▐▌██▐█▌▐█▌██▐█▌▐█ ▪▐▌ ▐█▀·.\n");
  printf(" ▀█▄▀▪     ▀▀▀• ▀█▄▀▪·▀▀▀▀  ▀█▄▀▪    ▀▀▀▀▀•  ▀  ▀     . ▀  ▀▀▀▀▀▀▀▀•  ▀  ▀     ▀▀▀▀▀•  ▀▀▀     ·▀▀▀  ▀█▄▀▪▀▀ █▪ ▀▀▀▀ ▀▪ ▀  ▀   ▀ •\n");
  printf(reset pisca);
  printf("                                                        1) Sandbox\n");
  printf("                                                        2) Lexico\n");
  printf("                                                     3) Sobre o Jogo\n");
  printf("                                                       4) Creditos\n");
  printf("                                                        5) Salvos\n");
  printf("                                                         0) Sair\n" reset);
  scanf("%d%*c", &select);
  return select;
}

void natmort() { // função das simulações estátias
  char C;
  int c;
  int i, j;
  FILE *pFile = fopen("natmort.txt", "r+");
  while (1) {
    Vida *p = NULL; // cria um ponteiro Vida para as simulações prontas
    limpador();
    printf("Essa secao apresenta os blocos de vida estaticos\n");
    printf("1- Bloco\n");
    printf("2- Colmeia\n");
    printf("3- Pao\n");
    printf("4- Barco\n");
    printf("5- Cano\n");
    printf("0- Voltar\n");
    scanf("%d%*c", &c);
    switch (c) {
    case 1:
      fseek(pFile, 0, SEEK_SET);
      p = (Vida *)malloc(4 * sizeof(Vida));
      for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
          p[i].life[j] = fgetc(pFile);
        }
        C = fgetc(pFile);
      }
      
      simulacao(p, 4, 4, 4, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 2:
      fseek(pFile, 20, SEEK_SET);
      p = (Vida *)malloc(4 * sizeof(Vida));
      for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
          p[i].life[j] = fgetc(pFile);
        }
        C = fgetc(pFile);
      }
      
      simulacao(p, 4, 4, 6, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 3:
      fseek(pFile, 40, SEEK_SET);
      p = (Vida *)malloc(4 * sizeof(Vida));
      for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
          p[i].life[j] = fgetc(pFile);
        }
        C = fgetc(pFile);
      }
      
      simulacao(p, 4, 4, 7, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 4:
      fseek(pFile, 60, SEEK_SET);
      p = (Vida *)malloc(3 * sizeof(Vida));
      for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
          p[i].life[j] = fgetc(pFile);
        }
        C = fgetc(pFile);
      }
      
      simulacao(p, 3, 3, 5, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 5:
      fseek(pFile, 72, SEEK_SET);
      p = (Vida *)malloc(3 * sizeof(Vida));
      for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
          p[i].life[j] = fgetc(pFile);
        }
        C = fgetc(pFile);
      }
      
      simulacao(p, 3, 3, 4, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 0:
      return;
    }
  }
  fclose(pFile);
}
void oscila() {   // função das simulações osciladoras
  FILE *pFile = fopen("oscila.txt", "r+"); //ponteiro do arquivo oscila
  Vida *p = NULL; // cria um ponteiro Vida para as simulações prontas
  int c;
  int i, j;
  while (1) {
    limpador();
    printf("Essa sessao aprensenta alguns padroes de jogo que oscilam em um "
           "padrao\n");
    printf("1- Blinker (2 periodos)\n");
    printf("2- Sapo (2 periodos)\n");
    printf("3- Farol (2 periodos)\n");
    printf("4- Pulsar (3 periodos)\n");
    printf("5- Penta-decathlon (15 periodos)\n");
    printf("0- Sair\n");
    scanf("%d%*c", &c);
    switch (c) {
    case 1:
      fseek(pFile, 0, SEEK_SET);
      p = (Vida *)malloc(3 * sizeof(Vida));
      for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
          p[i].life[j] = getc(pFile);
      
      simulacao(p, 3, 3, 3, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 2:
      fseek(pFile, 9, SEEK_SET);
      
      p = (Vida *)malloc(4 * sizeof(Vida));
      for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
          p[i].life[j] = getc(pFile);
      
      simulacao(p, 4, 4, 6, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 3:
      fseek(pFile, 25, SEEK_SET);
      
      p = (Vida *)malloc(4 * sizeof(Vida));
      for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
          p[i].life[j] = getc(pFile);

      simulacao(p, 4, 4, 6, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 4:
      fseek(pFile, 41, SEEK_SET);
      
      p = (Vida *)malloc(16 * sizeof(Vida));
      for (i = 0; i < 16; i++)
        for (j = 0; j < 16; j++)
          p[i].life[j] = getc(pFile);
      
      simulacao(p, 16, 16, 48, 10); // envia a simulação para ser executada
      free(p);
      break;

    case 5:
      fseek(pFile, 297, SEEK_SET);
      
      p = (Vida *)malloc(16 * sizeof(Vida));
      for (i = 0; i < 16; i++)
        for (j = 0; j < 16; j++)
          p[i].life[j] = getc(pFile);
      
      simulacao(p, 16, 16, 20, 30); // envia a simulação para ser executada
      free(p);
      break;
    
    case 0:
      return;
    }
  }
  fclose(pFile);
}

void naves() {    // simulacoes de nave
  FILE *pFile = fopen("naves.txt", "r+");
  Vida *p = NULL; // cria um ponteiro Vida para as simulações prontas
  int c;
  int i, j;
  while (1) {
    limpador();
    printf(
        "Essa sessao apresenta um pardao de celulas que se move pela matriz\n");
    printf("1- Glider\n");
    printf("2- Light weight Spaceship (LWSS)\n");
    printf("3- Middle weight Spaceship (MWSS)\n");
    printf("4- Heavy weight Spaceship (HWSS)\n");
    printf("0- Sair\n");
    scanf("%d%*c", &c);
    switch (c) {
    case 1:
      fseek(pFile, 0, SEEK_SET);
      
      p = (Vida *)malloc(10 * sizeof(Vida));
      for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
          p[i].life[j] = fgetc(pFile);
      
      simulacao(p, 10, 10, 5, 25); // envia a simulação para ser executada
      free(p);
      break;

    case 2:
      fseek(pFile, 100, SEEK_SET);
      
      p = (Vida *)malloc(20 * sizeof(Vida));
      for (i = 0; i < 20; i++)
        for (j = 0; j < 20; j++)
          p[i].life[j] = fgetc(pFile);
      
      simulacao(p, 20, 20, 9, 20); // envia a simulação para ser executada
      free(p);
      break;

    case 3:
      fseek(pFile, 500, SEEK_SET);
      
      p = (Vida *)malloc(20 * sizeof(Vida));
      for (i = 0; i < 20; i++)
        for (j = 0; j < 20; j++)
          p[i].life[j] = fgetc(pFile);
      
      simulacao(p, 20, 20, 11, 20); // envia a simulação para ser executada
      free(p);
      break;

    case 4:
      fseek(pFile, 900, SEEK_SET);
      
      p = (Vida *)malloc(20 * sizeof(Vida));
      for (i = 0; i < 20; i++)
        for (j = 0; j < 20; j++)
          p[i].life[j] = fgetc(pFile);

      simulacao(p, 20, 20, 13, 20); // envia a simulação para ser executada
      free(p);
      break;

    case 0:
      return;
    }
  }
  fclose(pFile);
}

void simulacoes() { // função do menu com algumas simulações pré prontas
  while (1) {
    int c;
    limpador();
    printf(graf);
    printf("▄▄▌  ▄▄▄ .▐▄• ▄ ▪   ▄▄·           ·▄▄▄▄   ▄▄▄·      ▌ ▐·▪  ·▄▄▄▄   "
           "▄▄▄· \n");
    printf("██•  ▀▄.▀· █▌█▌▪██ ▐█ ▌▪▪         ██▪ ██ ▐█ ▀█     ▪█·█▌██ ██▪ ██ "
           "▐█ ▀█ \n");
    printf("██▪  ▐▀▀▪▄ ·██· ▐█·██ ▄▄ ▄█▀▄     ▐█· ▐█▌▄█▀▀█     ▐█▐█•▐█·▐█· "
           "▐█▌▄█▀▀█ \n");
    printf("▐█▌▐▌▐█▄▄▌▪▐█·█▌▐█▌▐███▌▐█▌.▐▌    ██. ██ ▐█ ▪▐▌     ███ ▐█▌██. ██ "
           "▐█ ▪▐▌\n");
    printf(".▀▀▀  ▀▀▀ •▀▀ ▀▀▀▀▀·▀▀▀  ▀█▄▀▪    ▀▀▀▀▀•  ▀  ▀     . ▀  ▀▀▀▀▀▀▀▀•  "
           "▀  ▀ \n");
    printf(reset);
    printf("Aqui temos alguns tipos de criacoes conhecidas do jogo da vida de "
           "Conway\n");
    printf("1- Natureza morta\n");
    printf("2- Osciladores\n");
    printf("3- Naves\n");
    printf("0- Sair\n");
    scanf("%d", &c);
    switch (c) {
    case 1:
      natmort();
      break;
    case 2:
      oscila();
      break;
    case 3:
      naves();
      break;
    case 0:
      return;
    }
  }
}

void regras() { // função das regras
  
  limpador();
  printf(graf);
  printf("▄▄▄  ▄▄▄ . ▄▄ • ▄▄▄   ▄▄▄· .▄▄ · \n");
  printf("▀▄ █·▀▄.▀·▐█ ▀ ▪▀▄ █·▐█ ▀█ ▐█ ▀. \n");
  printf("▐▀▀▄ ▐▀▀▪▄▄█ ▀█▄▐▀▀▄ ▄█▀▀█ ▄▀▀▀█▄\n");
  printf("▐█•█▌▐█▄▄▌▐█▄▪▐█▐█•█▌▐█ ▪▐▌▐█▄▪▐█\n");
  printf(".▀  ▀ ▀▀▀ ·▀▀▀▀ .▀  ▀ ▀  ▀  ▀▀▀▀ \n");
  printf(reset);
  printf("John Hortan Conway foi um matematico britanico ativo na teoria "
         "combinatoria dos jogos, na teoria dos numeros e na teoria dos "
         "codigos\n");
  printf("Ele tambem foi o responsavel por criar o jogo da vida, o qual esse "
         "programa replica\n\n");

  printf("Conway baseou seu jogo em uma matriz M por N contendo em cada bloco "
         "da matriz celulas vivas " green "(verdes)" reset " e mortas " back
         "(brancas)" reset "\n");
  printf("Nessa matriz de células reinam as 4 regras: \n");
  printf("1- Qualquer celula viva com menos de 2 vizinhos vivos " back
         "morre" reset " por subpopulacao\n");
  printf("2- Celulas com 2 ou 3 vizinhos vivos " green "sobrevivem" reset
         " para a proxima geracao\n");
  printf("3- Celulas com mais de 3 vizinhos vivos " back "morrem" reset
         " por superpopulacao\n\n");
  printf("4- Celulas mortas com exatamente 3 vizinhos vivos se tornam uma "
         "celula " green "viva" reset " na proxima geracao\n\n");
  printf("Voce pode criar uma simulacao propria no modo sandbox ou ver algumas "
         "estruturas prontas conheci das na opcao Lexico\n");
  printf("Pressione enter para continuar");
  getchar();
}

void creditos() { // função dos creditos
  limpador();
  usleep(1000000);
  printf("Trabalho realizado por:\n");
  usleep(1000000);
  printf("Luiz Felipe Brilhante dos Santos Mendes\n");
  usleep(1000000);
  printf("Matricula: 202401806\n\n");
  usleep(1000000);
  printf("Jogo baseado no jogo da vida de Conway\n");
  usleep(1000000);
  printf("Site gerador do titulo: "
         "https://patorjk.com/software/taag/#p=display&f=Graffiti&t=\n");
  usleep(1000000);
  printf("Pressione enter para continuar");
  getchar();
}

int main() {
  printf("Recomendo que jogue o jogo em tela cheia\n");
  printf("Pressione enter para continuar\n");
  getchar();

  while (1)
    switch (menu()) { // base do menu
    case 1:
      sandbox();
      break;
    case 2:
      simulacoes();
      break;
    case 3:
      regras();
      break;
    case 4:
      creditos();
      break;
    case 5:
      salvos();
      break;
    case 0:
      return 0;
    }
}
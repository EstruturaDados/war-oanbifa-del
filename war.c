// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //biblioteca para usar função rand
// --- Constantes Globais ---
#define TAM_STRING 80
//--- VARIÁVEL GLOBAL ---
char *missoes[] = {"Vencer 3 batalhas seguidas", "Eliminar o exercito branco",
                   "Vencer 2 batalhas seguidas", "Zerar tropas de 2 inimigos",
                   "Reduzir 1 inimigo a 0 tropas"};

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
};
// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
// Limpando Buffer de entrada
void limparBufferEntrada();
void cadTerritorios(struct Territorio *territorios, int qtd);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
void exibirMenu();
// alocando memoria para o mapa
struct Territorio *alocarMapa(int qtd);
void exibirMapa(const struct Territorio *territorios, int qtd);
void faseDeAtaque(struct Territorio *territorios, int qtd);
void atacar(struct Territorio *atacante, struct Territorio *defensor);
void exibirMissao(char *missao);
void liberarMemoria(struct Territorio *territorios) {
  free(territorios);}
void verificarVitoria(char *missao, struct Territorio *territorios, int qtd);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
int main() {
  setlocale (LC_ALL,"Portuguese");
  int qtd;
  srand(time(NULL)); // iniciando gerador de números aleatórios

  printf("Quantos territórios deseja cadastrar? ");
  scanf("%d", &qtd);
  limparBufferEntrada();

  struct Territorio *territorios = alocarMapa(qtd);

  // VERIFICAÇÃO: é importante verificar se a alocação foi bem-sucedida
  if (territorios == NULL) {
    printf("Erro na alocação de memória!\n");
    return 1; // Retorna 1 para indicar um erro
  }

  cadTerritorios(territorios, qtd);

  char missaoJogador[TAM_STRING];

  atribuirMissao(missaoJogador, missoes, 5);

  exibirMissao(missaoJogador);

  int opcao;
    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
  do {
    exibirMapa(territorios, qtd);
    exibirMenu();
    scanf("%d", &opcao);
    limparBufferEntrada();

    switch (opcao) {
    case 1: {
      faseDeAtaque(territorios, qtd);
      break;
    }
    case 2: {
      verificarVitoria(missaoJogador, territorios, qtd);
      break;
    }
    case 0:
      printf("\nSaindo do sistema...\n");
      break;

    default:
      printf("\nOpão inválida! Pressione Enter para tentar novamente.");
      getchar();
      break;
    }
  } while (opcao != 0);
  // Chama a função liberar memória
  liberarMemoria(territorios);
  return 0;
}
// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
// Função Buffer de entrada
void limparBufferEntrada() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
// --- Função Cadastro ---
void cadTerritorios(struct Territorio *territorios, int qtd) {
  printf("\n===========================\n");
  printf("CADASTRO DOS TERRITÓRIOS\n");
  printf("===========================\n\n");

  for (int i = 0; i < qtd; i++) {

    printf("Território %d\n", i + 1);

    printf("Digite o nome do território: ");
    fgets(territorios[i].nome, TAM_STRING, stdin);
    territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

    printf("Digite a cor do território: ");
    fgets(territorios[i].cor, TAM_STRING, stdin);
    territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

    printf("Digite o número de tropas: ");
    scanf("%d", &territorios[i].tropas);
    limparBufferEntrada();

    printf("\n");
  }
}
//Alocar Mapa
struct Territorio *alocarMapa(int qtd) {
  struct Territorio *t = calloc(qtd, sizeof(struct Territorio));
  if (t == NULL) {
    printf("Erro na alocacao!\n");
  }
  return t;
}
// Função Exibir Mapa
void exibirMapa(const struct Territorio *territorios, int qtd) {
  printf("\n==============================\n");
  printf("MAPA DO MUNDO\n");
  printf("==============================\n\n");
  for (int i = 0; i < qtd; i++) {
    printf("%d - ", i + 1);
    printf("%s - ", territorios[i].nome);
    printf("Exército %s, ", territorios[i].cor);
    printf("Tropas: %d\n", territorios[i].tropas);
  }
}
// Funçao Atribuir Missão
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
  int sorteio = rand() % totalMissoes;
  strcpy(destino, missoes[sorteio]);
}
// Função exibir Missão
void exibirMissao(char *missao) {
  printf("--- SUA MISSAO ---\n");
  printf("%s\n", missao);
}
// Função Exibir Menu
void exibirMenu() {

  printf("\n--- MENU DE AÇÕES ---\n");
  printf("1 - Atacar\n");
  printf("2 - Verificar Missao\n");
  printf("0 - Sair\n");
  printf("Escolha sua ação: \n");
}
// função FASE DE ATAQUE
void faseDeAtaque(struct Territorio *territorios, int qtd) {
  int atk, def;

  printf("\n--- FASE DE ATAQUE ---\n");
  printf("Escolha o atacante (1 a %d): ", qtd);
  scanf("%d", &atk);
  printf("Escolha o defensor (1 a %d): ", qtd);
  scanf("%d", &def);
  // Verifica se os dados são válidos
  if (atk < 1 || atk > qtd || def < 1 || def > qtd) {
      printf("Territorio invalido!\n");
      return;
  }
  limparBufferEntrada();
  atacar(&territorios[atk - 1], &territorios[def - 1]);
}

// Função ATACAR
void atacar(struct Territorio *atacante, struct Territorio *defensor) {

  if (strcmp(atacante->cor, defensor->cor) == 0) {
    printf("Nao e possivel atacar um territorio do mesmo exercito!\n");
    return;
  }

  int dadoAtacante = rand() % 6 + 1;
  int dadoDefensor = rand() % 6 + 1;

  printf("O atacante %s tirou: %d no dado\n", atacante->nome, dadoAtacante);
  printf("O defensor %s tirou: %d no dado\n", defensor->nome, dadoDefensor);

  printf("\n--- RESULTADO DA BATALHA ---\n");

  if (dadoAtacante > dadoDefensor) {

    printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");

    if (defensor->tropas > 0) {
        defensor->tropas--;
    }

    if (defensor->tropas <= 0) {
      printf("O territorio %s foi conquistado pelo exercito %s!\n",
             defensor->nome, atacante->cor);
    }

  } else {

    printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");

    if (atacante->tropas > 0) {
        atacante->tropas--;
    }

    if (atacante->tropas <= 0) {
      printf("O territorio %s foi conquistado pelo exercito %s!\n",
             atacante->nome, defensor->cor);
    }
  }
}

// Função Verificar Missão
void verificarVitoria(char *missao, struct Territorio *territorios, int qtd) {

  printf("\n--- VERIFICANDO MISSAO ---\n");

  // Exemplo 1: eliminar exercito branco
  if (strcmp(missao, "Eliminar o exercito branco") == 0) {

    int encontrou = 0;

    for (int i = 0; i < qtd; i++) {
      if (strcmp(territorios[i].cor, "branco") == 0 &&
          territorios[i].tropas > 0) {
        encontrou = 1;
      }
    }

    if (encontrou == 0)
      printf("Missao cumprida!\n");
    else
      printf("Missao ainda nao cumprida.\n");
  }

  // Exemplo 2: zerar tropas de 2 inimigos
  else if (strcmp(missao, "Zerar tropas de 2 inimigos") == 0) {

    int zerados = 0;

    for (int i = 0; i < qtd; i++) {
      if (territorios[i].tropas <= 0)
        zerados++;
    }

    if (zerados >= 2)
      printf("Missao cumprida!\n");
    else
      printf("Missao ainda nao cumprida.\n");
  }

  else {
    printf("Verificacao nao implementada para essa missao ainda.\n\n");
  }
}

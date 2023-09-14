/*
 * Universidade Federal do Amazonas
 * Engenharia da Computação
 * Autora: Caroline Soares Braz
 * Professor: Eduardo Feitosa
 * Última modificação: 13 de setembro de 2023 às 21:15
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PROCESSOS 1000

int processos[NUM_PROCESSOS][4];

// A função verifica se existe alguma processo que ainda tem precisa de tempo
// para processar
int verificaSobraDeTempo() {
  int flag = 0;
  for (int i = 0; i < NUM_PROCESSOS; i++) {
    if (processos[i][1] > 0) {
      flag = 1;
    }
  }
  return flag;
}

// Na fila de processos, retorna aquele que possui menor tempo restante de
// processamento
int indexMenorTempo(int fila[], int processosNaFila) {
  int indexMenorTempo = fila[0];
  for (int i = 0; i < processosNaFila; i++) {
    if (processos[fila[i]][1] < processos[indexMenorTempo][1]) {
      indexMenorTempo = fila[i];
    }
  }
  return indexMenorTempo;
}

// Na fila de processos, retorna aquele que possui maior prioridade
int indexMaiorPrioridade(int fila[], int processosNaFila) {
  int indexMaiorPrioridade = fila[0];
  for (int i = 0; i < processosNaFila; i++) {
    if (processos[fila[i]][2] > processos[indexMaiorPrioridade][2]) {
      indexMaiorPrioridade = fila[i];
    }
  }
  return indexMaiorPrioridade;
}

// Recebe dois processos da fila e troca eles de lugar
void trocaPosicaoNoVetor(int fila[], int index1, int index2) {
  int aux[4];

  // Tranfere dados de 1 para aux
  aux[0] = fila[0];
  aux[1] = fila[1];
  aux[2] = fila[2];
  aux[3] = fila[3];

  // Tranfere dados de 2 para 1
  fila[0] = fila[0];
  fila[1] = fila[1];
  fila[2] = fila[2];
  fila[3] = fila[3];

  // Tranfere dados de aux para 2
  fila[0] = aux[0];
  fila[1] = aux[1];
  fila[2] = aux[2];
  fila[3] = aux[3];
}

// Ordena a fila por ordem de chegada
void ordenaOrdemChegada(int fila[], int processosNaFila) {
  int indexAtual = fila[0];
  for (int i = 0; i < processosNaFila; i++) {
    if (processos[fila[i]][0] < processos[indexAtual][0]) {
      trocaPosicaoNoVetor(fila, i, indexAtual);
    }
  }
}

// Confere todos os processos que ainda necessitam de tempo de processamento e
// cujo tempo é maior que o tempo atual de contagem
int colocaNaFila(int fila[], int tempo) {
  int processosNaFila = 0;
  for (int i = 0; i < NUM_PROCESSOS; i++) {
    int tempoSobrando = processos[i][1] == 0 ? 0 : 1;
    if (tempoSobrando == 1 && (processos[i][0] <= tempo)) {
      fila[processosNaFila] = i;
      processosNaFila++;
    }
  }
  return processosNaFila;
}

// Para o escalonamento cooperativo, verifica-se por quanto tempo um processo
// precisa executar e o executa até o final, no fim, retorna por quanto tempo
// processou
int escalonaCooperativo(int indexProcesso) {
  int tempo = 0;
  int aux = processos[indexProcesso][1];
  for (int w = 0; w < aux; w++) {
    printf("P%d ", indexProcesso + 1);
    processos[indexProcesso][1]--;
    tempo++;
  }
  return tempo;
}

// No escalonamento preempitivo, enquanto ainda é necessário tempo para o
// processo executar o processou menos que o quantum ele continua processando,
// no fim, retorna por quanto tempo processou
int escalonaPreemptivo(int indexProcesso, int quantum) {
  int tempo = 0;
  int aux = processos[indexProcesso][1];
  int w = 0;
  while (processos[indexProcesso][1] > 0 && w < quantum) {
    printf("P%d ", indexProcesso + 1);
    processos[indexProcesso][1]--;
    tempo++;
    w++;
  }
  return tempo;
}

// O primeiro processo da fila é executado até o final
void FCFS(int fila[], int quantidadeProcessos) {

  // Soma os tempos de espera e execução pra tirar a média
  double somaTempoProcessos = 0;
  double somaTempoEspera = 0;

  // Tempo de execução
  int tempo = 0;

  // Quantidade de processos na fila
  int processosNaFila = 0;

  // Flag para verificar se algum processo ainda precisa executar
  int flag = verificaSobraDeTempo();

  printf("\n\n===== ORDEM DE EXECUCAO PARA FCFS =====\n\n");

  // Enquanto existirem processos para executar, continua executando
  while (flag == 1) {

    // Coloca os processos na fila de execução
    processosNaFila = colocaNaFila(fila, tempo);

    if (processosNaFila == 0) {
      // Se não houverem processos na fila, apenas incrementa o tempo
      tempo++;
    } else if (processosNaFila == 1) {
      // Se houver um único processo na fila, executa ele
      int tempoPassado = escalonaCooperativo(fila[0]);
      tempo += tempoPassado;
      // Soma o tempo usado para processar
      somaTempoProcessos += tempoPassado;
    } else {
      // Se houver mais de um processo na fila, ordena os processos por ordem de
      // chegada
      ordenaOrdemChegada(fila, processosNaFila);
      // Escalona e vê quanto tempo se passou
      int tempoPassado = escalonaCooperativo(fila[0]);
      tempo += tempoPassado;
      // Soma o tempo passado em cada processo da fila
      somaTempoProcessos += (processosNaFila * tempoPassado);
      // Soma o tempo de espera pros processos da fila, menos o que foi
      // processado
      somaTempoEspera += ((processosNaFila - 1) * tempoPassado);
    }

    // Zera a quantidade de processos da fila, para reorganizar
    processosNaFila = 0;

    // Verifica se há processos para executar
    flag = verificaSobraDeTempo();
  }

  // Tira a média de tempo de execução
  double mediaTempoProcessos = somaTempoProcessos / quantidadeProcessos;
  printf("\n\nTEMPO MÉDIO DE PROCESSAMENTO (Tt): %.2f\n", mediaTempoProcessos);

  // Tira a média de tempo de espera
  double mediaTempoEspera = somaTempoEspera / quantidadeProcessos;
  printf("\nTEMPO MÉDIO DE ESPERA (Tw): %.2f\n\n", mediaTempoEspera);
}

// Executa os processos da fila pelo tempo de quantum em ordem de chegada
void RR(int fila[], int quantum, int quantidadeProcessos) {

  // Soma os tempos de espera e execução pra tirar a média
  double somaTempoProcessos = 0;
  double somaTempoEspera = 0;

  // Tempo de execução
  int tempo = 0;

  // Quantidade de processos na fila
  int processosNaFila = 0;

  // Flag para verificar se algum processo ainda precisa executar
  int flag = verificaSobraDeTempo();

  printf("\n\n===== ORDEM DE EXECUCAO PARA RR =====\n\n");

  // Enquanto existirem processos para executar, continua executando
  while (flag == 1) {

    // Coloca os processos na fila de execução
    processosNaFila = colocaNaFila(fila, tempo);

    if (processosNaFila == 0) {
      // Se não houverem processos na fila, apenas incrementa o tempo
      tempo++;

    } else if (processosNaFila == 1) {
      // Se houver um único processo na fila, executa ele
      int tempoPassado = escalonaCooperativo(fila[0]);
      tempo += tempoPassado;
      // Soma o tempo usado para processar
      somaTempoProcessos += tempoPassado;
    } else {
      // Se houver mais de um processo na fila, ordena os processos por ordem de
      // chegada
      ordenaOrdemChegada(fila, processosNaFila);
      // Coloca toda a fila para processar preemptivamente
      for (int i = 0; i < processosNaFila; i++) {
        int tempoPassado = escalonaPreemptivo(fila[i], quantum);
        tempo += tempoPassado;
        // Soma o tempo passado em cada processo da fila
        somaTempoProcessos += (processosNaFila * tempoPassado);
        // Soma o tempo de espera pros processos da fila, menos o que foi
        // processado
        somaTempoEspera += ((processosNaFila - 1) * tempoPassado);
      }
    }

    // Zera a quantidade de processos da fila, para reorganizar
    processosNaFila = 0;

    // Verifica se há processos para executar
    flag = verificaSobraDeTempo();
  }

  // Tira a média de tempo de execução
  double mediaTempoProcessos = somaTempoProcessos / quantidadeProcessos;
  printf("\n\nTEMPO MÉDIO DE PROCESSAMENTO (Tt): %.2f\n", mediaTempoProcessos);

  // Tira a média de tempo de espera
  double mediaTempoEspera = somaTempoEspera / quantidadeProcessos;
  printf("\nTEMPO MÉDIO DE ESPERA (Tw): %.2f\n\n", mediaTempoEspera);
}

// Executa o processo que possui menor tempo de execução completamente
void SJF(int fila[], int quantidadeProcessos) {

  // Soma os tempos de espera e execução pra tirar a média
  double somaTempoProcessos = 0;
  double somaTempoEspera = 0;

  // Tempo de execução
  int tempo = 0;

  // Quantidade de processos na fila
  int processosNaFila = 0;

  // Flag para verificar se algum processo ainda precisa executar
  int flag = verificaSobraDeTempo();

  printf("\n\n===== ORDEM DE EXECUCAO PARA SJF =====\n\n");

  // Enquanto existirem processos para executar, continua executando
  while (flag == 1) {

    // Coloca os processos na fila de execução
    processosNaFila = colocaNaFila(fila, tempo);

    if (processosNaFila == 0) {
      // Se não houverem processos na fila, apenas incrementa o tempo
      tempo++;
    } else {
      // Busca o index do elemento que precisa de menos tempo de processamento
      // total
      int indexParaProcessar = indexMenorTempo(fila, processosNaFila);
      // Coloca esse processo para executar
      int tempoPassado = escalonaCooperativo(indexParaProcessar);
      tempo += tempoPassado;
      // Soma o tempo passado em cada processo da fila
      somaTempoProcessos += (processosNaFila * tempoPassado);
      // Soma o tempo de espera pros processos da fila, menos o que foi
      // processado
      somaTempoEspera += ((processosNaFila - 1) * tempoPassado);
    }

    // Zera a quantidade de processos da fila, para reorganizar
    processosNaFila = 0;

    // Verifica se há processos para executar
    flag = verificaSobraDeTempo();
  }

  // Tira a média de tempo de execução
  double mediaTempoProcessos = somaTempoProcessos / quantidadeProcessos;
  printf("\n\nTEMPO MÉDIO DE PROCESSAMENTO (Tt): %.2f\n", mediaTempoProcessos);

  // Tira a média de tempo de espera
  double mediaTempoEspera = somaTempoEspera / quantidadeProcessos;
  printf("\nTEMPO MÉDIO DE ESPERA (Tw): %.2f\n\n", mediaTempoEspera);
}

// Executa pelo tempo do quantum o processo que possui menor tempo de execução
void STRF(int fila[], int quantum, int quantidadeProcessos) {

  // Soma os tempos de espera e execução pra tirar a média
  double somaTempoProcessos = 0;
  double somaTempoEspera = 0;

  // Tempo de execução
  int tempo = 0;

  // Quantidade de processos na fila
  int processosNaFila = 0;

  // Flag para verificar se algum processo ainda precisa executar
  int flag = verificaSobraDeTempo();

  printf("\n\n===== ORDEM DE EXECUCAO PARA STRF =====\n\n");

  // Enquanto existirem processos para executar, continua executando
  while (flag == 1) {

    // Coloca os processos na fila de execução
    processosNaFila = colocaNaFila(fila, tempo);

    if (processosNaFila == 0) {
      // Se não houverem processos na fila, apenas incrementa o tempo
      tempo++;
    } else {
      // Busca o index do elemento que precisa de menos tempo de processamento
      // total
      int indexParaProcessar = indexMenorTempo(fila, processosNaFila);
      // Coloca esse processo para executar pelo tempo do quantum
      int tempoPassado = escalonaPreemptivo(indexParaProcessar, quantum);
      tempo += tempoPassado;
      // Soma o tempo passado em cada processo da fila
      somaTempoProcessos += (processosNaFila * tempoPassado);
      // Soma o tempo de espera pros processos da fila, menos o que foi
      // processado
      somaTempoEspera += ((processosNaFila - 1) * tempoPassado);
    }

    // Zera a quantidade de processos da fila, para reorganizar
    processosNaFila = 0;

    // Verifica se há processos para executar
    flag = verificaSobraDeTempo();
  }
  // Tira a média de tempo de execução
  double mediaTempoProcessos = somaTempoProcessos / quantidadeProcessos;
  printf("\n\nTEMPO MÉDIO DE PROCESSAMENTO (Tt): %.2f\n", mediaTempoProcessos);

  // Tira a média de tempo de espera
  double mediaTempoEspera = somaTempoEspera / quantidadeProcessos;
  printf("\nTEMPO MÉDIO DE ESPERA (Tw): %.2f\n\n", mediaTempoEspera);
}

// Executa o processo com maior prioridade completamente
void PRIOC(int fila[], int quantidadeProcessos) {

  // Soma os tempos de espera e execução pra tirar a média
  double somaTempoProcessos = 0;
  double somaTempoEspera = 0;

  // Tempo de execução
  int tempo = 0;

  // Quantidade de processos na fila
  int processosNaFila = 0;

  // Flag para verificar se algum processo ainda precisa executar
  int flag = verificaSobraDeTempo();

  printf("\n\n===== ORDEM DE EXECUCAO PARA PRIOC =====\n\n");

  // Enquanto existirem processos para executar, continua executando
  while (flag == 1) {

    // Coloca os processos na fila de execução
    processosNaFila = colocaNaFila(fila, tempo);

    // Se não houverem processos na fila, apenas incrementa o tempo
    if (processosNaFila == 0) {
      tempo++;
    } else {
      // Busca o index do elemento com maior prioridade
      int indexParaProcessar = indexMaiorPrioridade(fila, processosNaFila);
      // Coloca esse processo para executar
      int tempoPassado = escalonaCooperativo(indexParaProcessar);
      tempo += tempoPassado;
      // Soma o tempo passado em cada processo da fila
      somaTempoProcessos += (processosNaFila * tempoPassado);
      // Soma o tempo de espera pros processos da fila, menos o que foi
      // processado
      somaTempoEspera += ((processosNaFila - 1) * tempoPassado);
    }

    // Zera a quantidade de processos da fila, para reorganizar
    processosNaFila = 0;

    // Verifica se há processos para executar
    flag = verificaSobraDeTempo();
  }

  // Tira a média de tempo de execução
  double mediaTempoProcessos = somaTempoProcessos / quantidadeProcessos;
  printf("\n\nTEMPO MÉDIO DE PROCESSAMENTO (Tt): %.2f\n", mediaTempoProcessos);

  // Tira a média de tempo de espera
  double mediaTempoEspera = somaTempoEspera / quantidadeProcessos;
  printf("\nTEMPO MÉDIO DE ESPERA (Tw): %.2f\n\n", mediaTempoEspera);
}

// Executa pelo tempo do quantum o processo com maior prioridade
void PRIOP(int fila[], int quantum, int quantidadeProcessos) {

  // Soma os tempos de espera e execução pra tirar a média
  double somaTempoProcessos = 0;
  double somaTempoEspera = 0;

  // Tempo de execução
  int tempo = 0;

  // Quantidade de processos na fila
  int processosNaFila = 0;

  // Flag para verificar se algum processo ainda precisa executar
  int flag = verificaSobraDeTempo();

  printf("\n\n===== ORDEM DE EXECUCAO PARA PRIOP =====\n\n");

  // Enquanto existirem processos para executar, continua executando
  while (flag == 1) {

    // Coloca os processos na fila de execução
    processosNaFila = colocaNaFila(fila, tempo);

    if (processosNaFila == 0) {
      // Se não houverem processos na fila, apenas incrementa o tempo
      tempo++;
    } else {
      // Busca o index do elemento que precisa de menos tempo de processamento
      // total
      int indexParaProcessar = indexMaiorPrioridade(fila, processosNaFila);
      // Coloca esse processo para executar pelo tempo do quantum
      int tempoPassado = escalonaPreemptivo(indexParaProcessar, quantum);
      tempo += tempoPassado;
      // Soma o tempo passado em cada processo da fila
      somaTempoProcessos += (processosNaFila * tempoPassado);
      // Soma o tempo de espera pros processos da fila, menos o que foi
      // processado
      somaTempoEspera += ((processosNaFila - 1) * tempoPassado);
    }

    // Zera a quantidade de processos da fila, para reorganizar
    processosNaFila = 0;

    // Verifica se há processos para executar
    flag = verificaSobraDeTempo();
  }

  // Tira a média de tempo de execução
  double mediaTempoProcessos = somaTempoProcessos / quantidadeProcessos;
  printf("\n\nTEMPO MÉDIO DE PROCESSAMENTO (Tt): %.2f\n", mediaTempoProcessos);

  // Tira a média de tempo de espera
  double mediaTempoEspera = somaTempoEspera / quantidadeProcessos;
  printf("\nTEMPO MÉDIO DE ESPERA (Tw): %.2f\n\n", mediaTempoEspera);
}

int main() {

  printf("\n----------------------\n\n");
  printf(
      "NESTE PROGRAMA O VALOR MÁXIMO DEFINIDO DE PROCESSOS PARA "
      "PROCESSAMENTO É DE %d PROCESSOS, SE FOR NECESSÁRIO MAIS PROCESSOS, É "
      "NECESSÁRIO ALTERAR O VALOR DA CONSTANTE NUM_PROCESSOS NA LINHA 13\n\n",
      NUM_PROCESSOS);
  printf("----------------------\n\n");

  FILE *fptr;

  int countProcesso = 0;

  char linha[100];
  char nomeArquivo[100];

  int flagEncontrou = 0;

  printf("Informe o arquivo para escalonar: ");
  scanf("%s", nomeArquivo);

  // Abrir arquivo
  fptr = fopen(nomeArquivo, "r");

  while (flagEncontrou == 0) {
    // Se existir
    if (fptr != NULL) {
      // Lê conteúdo do arquivo
      while (fgets(linha, 100, fptr)) {
        int countDado = 0;
        char *pt;
        pt = strtok(linha, " ");
        // Separa a linha nos espaços e salva no vetor de processos
        while (pt) {
          pt = strtok(NULL, " ");
          if (pt != NULL) {
            processos[countProcesso][countDado] = atoi(pt);
            countDado++;
          }
        }
        countProcesso++;
      }
      flagEncontrou = 1;
      // Se não existir
    } else {
      printf("\n\n ===== ARQUIVO NÃO EXISTE =====\n\n");
      printf("Informe o arquivo para escalonar: ");
      scanf("%s", nomeArquivo);

      // Abrir arquivo
      fptr = fopen(nomeArquivo, "r");
    }
  }

  // Fecha o arquivo
  fclose(fptr);

  // Começar escalonamento

  // Se a quantidade de processos no arquivo for maior que a constante
  // NUM_PROCESSOS, o contador de processos recebe NUM_PROCESSOS
  if (countProcesso > NUM_PROCESSOS) {
    countProcesso = NUM_PROCESSOS;
  }

  // Vetor da fila de processos
  int fila[countProcesso];

  // Tipo dos processos para executar
  int tipoProcesso = processos[0][3];

  // Recomendação dos escalonadores baseadp no tipo de processos
  printf("\n\nEscalonadores recomendados: \n\n");
  if (tipoProcesso == 1) {
    printf("(1)FCFS (2)SJF (3)PRIOC");
  } else if (tipoProcesso == 2) {
    printf("(4)RR (5)STRF (6)PRIOP");
  } else {
    printf("(1)FCFS (2)SJF (3)PRIOC (4)RR (5)STRF (6)PRIOP");
  }

  int escolha;

  printf("\n\nSUA ESCOLHA: ");
  scanf("%d", &escolha);

  printf("\n");

  int quantum;

  // Realiza escalonamento com base na escolha do usuário
  switch (escolha) {
  case 1:
    FCFS(fila, countProcesso);
    break;
  case 2:
    SJF(fila, countProcesso);
    break;
  case 3:
    PRIOC(fila, countProcesso);
    break;
  case 4:
    printf("INFORME O QUANTUM: ");
    scanf("%d", &quantum);
    RR(fila, quantum, countProcesso);
    break;
  case 5:
    printf("INFORME O QUANTUM: ");
    scanf("%d", &quantum);
    STRF(fila, quantum, countProcesso);
    break;
  case 6:
    printf("INFORME O QUANTUM: ");
    scanf("%d", &quantum);
    PRIOP(fila, quantum, countProcesso);
    break;
  }

  return 0;
}
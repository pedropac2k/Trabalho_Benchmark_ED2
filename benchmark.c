#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

//Declaração das constantes e variáveis globais usadas durante os testes.
#define TAM        4
#define TAM_BOZ	   4
#define SEED_1     1
#define SEED_0	   0
#define PORC       10
#define QTD_TESTES 30
#define NUM_ARQ    8

const int tamanhos[TAM] = {10, 100, 1000, 10000};
const int tamanhosBozo[TAM] = {4, 8, 10, 12};

typedef struct t_request {
	int user_id;   // chave principal
	int chegada;   // timestamp para verificação de estabilidade
} acesso_request;


char *arquivos[NUM_ARQ] = {
	"BubbleSort.csv",
	"ShellSort.csv",
	"HeapSort.csv",
	"InsertionSort.csv",
	"SelectionSort.csv",
	"MergeSort.csv",
	"BozoSort.csv",
	"QuickSort.csv",
};


int mostraMenu(int escolha) {
	escolha++;

	printf("\n--- Menu de Testes ---");
	printf("\nEscolha qual o metodo a ser testado (ou 0 para sair): ");
	printf("\n\t1 - Bolha Inteligente\n\t2 - Insercao\n\t3 - Selecao\n\t4 - MergeSort\n\t5 - HeapSort\n\t6 - ShellSort\n\t7 - BozoSort\n\t8 - QuickSort");
	printf("\nOpcao: %d", escolha);
	//scanf("%d", &escolha);

	return escolha;
}


void escolhaDeTestes() {
	int escolha = 0;

	printf("\n##### Iniciando rotina de testes #####\n");
	escolha = mostraMenu(escolha);
	do {
		// Execução dos testes baseado na escolha do usuário
		switch (escolha) {
			case 1:
				testeBolhaInteligente();
				break;
			case 2:
				testeInsercao();
				break;
			case 3:
				testeSelecao();
				break;
			case 4:
				testeMergeSort();
				break;
			case 5:
				testeHeapSort();
				break;
			case 6:
				testeShellSort();
				break;
			case 7:
				testeBozoSort();
				break;
			case 8:
				testeQuickSort();
				break;
			default:
				printf("\n#### Opcao invalida! Tente novamente ####\n");
				break;
		}
		printf("\n----- Bateria de testes concluida -----\n");
		escolha = mostraMenu(escolha);
	}while (escolha < 9);
	printf("\n##### Encerrando rotina de testes #####\n");
}


void inicializaArquivoCSV(char *nomeArq) {
	FILE *arq = fopen(nomeArq, "w");
	if (arq != NULL) {
		fprintf(arq, "DataSet,Tamanho,Comparacoes,Movimentacoes,Tempo_ms\n");
		fclose(arq);
	}
}


double obterTempo() {
	LARGE_INTEGER frequencia, contador;
	QueryPerformanceFrequency(&frequencia);
	QueryPerformanceCounter(&contador);
	return (double)contador.QuadPart * 1000.0 / (double)frequencia.QuadPart;
}


void registrarResultadoCSV(char *nomeMetodo, char *dataset, int tamanho, met *m, double tempo) {
	char nomeArquivo[50];
	strcpy(nomeArquivo, nomeMetodo);

	FILE *arq = fopen(nomeArquivo, "a");
	if (arq == NULL)
		return;

	fprintf(arq, "%s,%d,%llu,%llu,%.4f\n", dataset, tamanho, ((acesso_metricas*)m)->comparacoes, ((acesso_metricas*)m)->movimentacoes,tempo);

	fclose(arq);
}


void registrarMediasCSV(char *nomeMetodo, char *dataset, int tamanho, unsigned long long mediaComp, unsigned long long mediaMov, double tempo) {
	char nomeArquivo[50];
	strcpy(nomeArquivo, nomeMetodo);

	FILE *arq = fopen(nomeArquivo, "a");
	if (arq == NULL)
		return;

	fprintf(arq, "%s,%d,%llu,%llu,%.4f\n", dataset, tamanho, mediaComp, mediaMov,tempo);

	fclose(arq);
}


void trataNomeArquivo(char *nomeArq) {
	int tam = strlen(nomeArq);

	nomeArq[tam - 1] = '\0';
	nomeArq[tam - 2] = '\0';
	nomeArq[tam - 3] = '\0';
	nomeArq[tam - 4] = '\0';
}


int verificaEstabilidade(char *nomeArq, int tam) {
	FILE *arq = fopen(nomeArq, "r");
	if (arq == NULL) {
		printf("Erro ao abrir o arquivo para verificação.\n");
		exit(-1);
	}

	int id_atual, ts_atual;
	int id_prox, ts_prox;

	while (fscanf(arq, "%d\t%d", &id_prox, &ts_prox) == 2) {
		if (id_atual == id_prox && ts_atual > ts_prox)
				return 0;
		id_atual = id_prox;
		ts_atual = ts_prox;
	}

	fclose(arq);
	return 1;
}


void executarTesteGenerico(AlgoritmoOrdenacao func, char *nomeAlgoritimo) {
	r *requisicao;
	met *metricas;
	double inicio;
	double fim;
	unsigned long long mediaComp;
	unsigned long long mediaMov;
	double mediaTempo;
	int estavel = 0;
	char nomeArquivo[50];


	strcpy(nomeArquivo, nomeAlgoritimo);
	trataNomeArquivo(nomeArquivo);


	printf("\n##########################################");
	printf("\n-------- DataSet Decrescente --------\n");
	printf("##########################################\n");
	for(int i=0; i<TAM; i++) {
		requisicao = geraDecrescente(tamanhos[i], SEED_1);

		inicio = obterTempo();
		metricas = func(requisicao, tamanhos[i]);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		registrarResultadoCSV(nomeAlgoritimo, "Decrescente", tamanhos[i], metricas, (fim - inicio));

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Ordenado --------\n");
	printf("##########################################\n");
	for(int i=0; i<TAM; i++) {
		requisicao = geraOrdenados(tamanhos[i], SEED_1);

		inicio = obterTempo();
		metricas = func(requisicao, tamanhos[i]);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		registrarResultadoCSV(nomeAlgoritimo, "Ordenado", tamanhos[i], metricas, (fim - inicio));

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Quase Ordenado --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i=0; i<TAM; i++) {
		mediaComp = 0;
		mediaMov = 0;
		mediaTempo = 0;
		for(int j = 0; j < QTD_TESTES; j++) {
			requisicao = geraQuaseOrdenados(tamanhos[i], SEED_0, PORC);

			inicio = obterTempo();
			metricas = func(requisicao, tamanhos[i]);
			fim = obterTempo();

			mediaComp += ((acesso_metricas*)metricas)->comparacoes;
			mediaMov += ((acesso_metricas*)metricas)->movimentacoes;
			mediaTempo += (fim - inicio);

			liberaVetor(requisicao);
			liberaMetricas(metricas);
		}
		mediaComp /= QTD_TESTES;
		mediaMov /= QTD_TESTES;
		mediaTempo /= QTD_TESTES;

		registrarMediasCSV(nomeAlgoritimo, "QuaseOrdenado", tamanhos[i], mediaComp, mediaMov, mediaTempo);

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Aleatorio --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i=0; i<TAM; i++) {
		mediaComp = 0;
		mediaMov = 0;
		mediaTempo = 0;
		for(int j=0; j<QTD_TESTES; j++) {
			requisicao = geraAleatorios(tamanhos[i], SEED_0);

			inicio = obterTempo();
			metricas = func(requisicao, tamanhos[i]);
			fim = obterTempo();

			mediaComp += ((acesso_metricas*)metricas)->comparacoes;
			mediaMov += ((acesso_metricas*)metricas)->movimentacoes;
			mediaTempo += (fim - inicio);

			if(i == 2 && j == 0) {
				salvaVetor(requisicao, tamanhos[i], nomeArquivo);
				estavel = verificaEstabilidade(nomeArquivo, tamanhos[i]);
			}

			liberaVetor(requisicao);
			liberaMetricas(metricas);
		}
		mediaComp /= QTD_TESTES;
		mediaMov /= QTD_TESTES;
		mediaTempo /= QTD_TESTES;

		registrarMediasCSV(nomeAlgoritimo, "Aleatorio", tamanhos[i], mediaComp, mediaMov, mediaTempo);


		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}
	if(estavel)
		printf("\n----- Algoritimo %s e estavel -----\n", nomeArquivo);
	else
		printf("\n----- Algoritimo %s nao e estavel -----\n", nomeArquivo);
}


void executarTesteParametrizado(AlgoritmoParametrizado func, char *nomeAlgoritimo) {
	r *requisicao;
	met *metricas;
	double inicio;
	double fim;
	unsigned long long mediaComp = 0;
	unsigned long long mediaMov = 0;
	double mediaTempo;
	int estavel = 0;
	char nomeArquivo[50];

	strcpy(nomeArquivo, nomeAlgoritimo);
	trataNomeArquivo(nomeArquivo);

	printf("\n##########################################");
	printf("\n-------- DataSet Decrescente --------\n");
	printf("##########################################\n");
	for(int i=0; i<TAM; i++) {
		requisicao = geraDecrescente(tamanhos[i], SEED_1);
		metricas = alocaMetricas();

		inicio = obterTempo();
		func(requisicao, 0, tamanhos[i] - 1, metricas);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		registrarResultadoCSV(nomeAlgoritimo, "Decrescente", tamanhos[i], metricas, (fim - inicio));

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Ordenado --------\n");
	printf("##########################################\n");
	for(int i=0; i<TAM; i++) {
		requisicao = geraOrdenados(tamanhos[i], SEED_1);
		metricas = alocaMetricas();

		inicio = obterTempo();
		func(requisicao, 0, tamanhos[i] - 1, metricas);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		registrarResultadoCSV(nomeAlgoritimo, "Ordenado", tamanhos[i], metricas, (fim - inicio));

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Quase Ordenado --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i=0; i<TAM; i++) {
		mediaComp = 0;
		mediaMov = 0;
		mediaTempo = 0;
		for(int j=0; j<QTD_TESTES; j++) {
			requisicao = geraQuaseOrdenados(tamanhos[i], SEED_0, PORC);
			metricas = alocaMetricas();

			inicio = obterTempo();
			func(requisicao, 0, tamanhos[i] - 1, metricas);
			fim = obterTempo();

			mediaComp += ((acesso_metricas*)metricas)->comparacoes;
			mediaMov += ((acesso_metricas*)metricas)->movimentacoes;
			mediaTempo += (fim - inicio);

			liberaVetor(requisicao);
			liberaMetricas(metricas);
		}
		mediaComp /= QTD_TESTES;
		mediaMov /= QTD_TESTES;
		mediaTempo /= QTD_TESTES;

		registrarMediasCSV(nomeAlgoritimo, "QuaseOrdenado", tamanhos[i], mediaComp, mediaMov, mediaTempo);

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Aleatorio --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i=0; i<TAM; i++) {
		mediaComp = 0;
		mediaMov = 0;
		mediaTempo = 0;
			for(int j=0; j<QTD_TESTES; j++) {
			requisicao = geraAleatorios(tamanhos[i], SEED_0);
			metricas = alocaMetricas();

			inicio = obterTempo();
			func(requisicao, 0, tamanhos[i] - 1, metricas);
			fim = obterTempo();

			mediaComp += ((acesso_metricas*)metricas)->comparacoes;
			mediaMov += ((acesso_metricas*)metricas)->movimentacoes;
			mediaTempo += (fim - inicio);

			if(i == 2 && j == 0) {
				salvaVetor(requisicao, tamanhos[i], nomeArquivo);
				estavel = verificaEstabilidade(nomeArquivo, tamanhos[i]);
			}

			liberaVetor(requisicao);
			liberaMetricas(metricas);
		}
		mediaComp /= QTD_TESTES;
		mediaMov /= QTD_TESTES;
		mediaTempo /= QTD_TESTES;

		registrarMediasCSV(nomeAlgoritimo, "Aleatorio", tamanhos[i], mediaComp, mediaMov, mediaTempo);

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}
	if(estavel)
		printf("\n----- Algoritimo %s e estavel -----\n", nomeArquivo);
	else
		printf("\n----- Algoritimo %s nao e estavel -----\n", nomeArquivo);
}


void executarTesteBozo(AlgoritmoOrdenacao func, char *nomeAlgoritimo) {
	r *requisicao;
	met *metricas;
	double inicio;
	double fim;
	unsigned long long mediaComp;
	unsigned long long mediaMov;
	double mediaTempo;
	int estavel = 0;
	char nomeArquivo[50];


	strcpy(nomeArquivo, nomeAlgoritimo);
	trataNomeArquivo(nomeArquivo);


	printf("\n##########################################");
	printf("\n-------- DataSet Decrescente --------\n");
	printf("##########################################\n");
	for(int i=0; i<TAM_BOZ; i++) {
		requisicao = geraDecrescente(tamanhosBozo[i], SEED_1);

		inicio = obterTempo();
		metricas = func(requisicao, tamanhosBozo[i]);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhosBozo[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		registrarResultadoCSV(nomeAlgoritimo, "Decrescente", tamanhosBozo[i], metricas, (fim - inicio));

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Ordenado --------\n");
	printf("##########################################\n");
	for(int i=0; i<TAM_BOZ; i++) {
		requisicao = geraOrdenados(tamanhosBozo[i], SEED_1);

		inicio = obterTempo();
		metricas = func(requisicao, tamanhosBozo[i]);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhosBozo[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		registrarResultadoCSV(nomeAlgoritimo, "Ordenado", tamanhosBozo[i], metricas, (fim - inicio));

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Quase Ordenado --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i=0; i<TAM_BOZ; i++) {
		mediaComp = 0;
		mediaMov = 0;
		mediaTempo = 0;
		for(int j = 0; j < QTD_TESTES; j++) {
			requisicao = geraQuaseOrdenados(tamanhosBozo[i], SEED_0, PORC);

			inicio = obterTempo();
			metricas = func(requisicao, tamanhosBozo[i]);
			fim = obterTempo();

			mediaComp += ((acesso_metricas*)metricas)->comparacoes;
			mediaMov += ((acesso_metricas*)metricas)->movimentacoes;
			mediaTempo += (fim - inicio);

			liberaVetor(requisicao);
			liberaMetricas(metricas);
		}
		mediaComp /= QTD_TESTES;
		mediaMov /= QTD_TESTES;
		mediaTempo /= QTD_TESTES;

		registrarMediasCSV(nomeAlgoritimo, "QuaseOrdenado", tamanhosBozo[i], mediaComp, mediaMov, mediaTempo);

		printf("\n--- Resultados para %d elementos ---", tamanhosBozo[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Aleatorio --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i=0; i<TAM_BOZ; i++) {
		mediaComp = 0;
		mediaMov = 0;
		mediaTempo = 0;
		for(int j=0; j<QTD_TESTES; j++) {
			requisicao = geraAleatorios(tamanhosBozo[i], SEED_0);

			inicio = obterTempo();
			metricas = func(requisicao, tamanhosBozo[i]);
			fim = obterTempo();

			mediaComp += ((acesso_metricas*)metricas)->comparacoes;
			mediaMov += ((acesso_metricas*)metricas)->movimentacoes;
			mediaTempo += (fim - inicio);

			if(i == 2 && j == 0) {
				salvaVetor(requisicao, tamanhosBozo[i], nomeArquivo);
				estavel = verificaEstabilidade(nomeArquivo, tamanhosBozo[i]);
			}

			liberaVetor(requisicao);
			liberaMetricas(metricas);
		}
		mediaComp /= QTD_TESTES;
		mediaMov /= QTD_TESTES;
		mediaTempo /= QTD_TESTES;

		registrarMediasCSV(nomeAlgoritimo, "Aleatorio", tamanhosBozo[i], mediaComp, mediaMov, mediaTempo);


		printf("\n--- Resultados para %d elementos ---", tamanhosBozo[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}
	if(estavel)
		printf("\n----- Algoritimo %s e estavel -----\n", nomeArquivo);
	else
		printf("\n----- Algoritimo %s nao e estavel -----\n", nomeArquivo);
}


void testeBolhaInteligente() {
	inicializaArquivoCSV(arquivos[0]);
	executarTesteGenerico(bolhaInteligente, arquivos[0]);
}


void testeShellSort() {
	inicializaArquivoCSV(arquivos[1]);
    executarTesteGenerico(shellSort, arquivos[1]);
}


void testeHeapSort() {
	inicializaArquivoCSV(arquivos[2]);
    executarTesteGenerico(heapSort, arquivos[2]);
}


void testeInsercao() {
	inicializaArquivoCSV(arquivos[3]);
	executarTesteGenerico(insercao, arquivos[3]);
}


void testeSelecao() {
	inicializaArquivoCSV(arquivos[4]);
	executarTesteGenerico(selecao, arquivos[4]);
}


void testeMergeSort() {
	inicializaArquivoCSV(arquivos[5]);
	executarTesteParametrizado(mergeSort, arquivos[5]);
}


void testeBozoSort() {
	inicializaArquivoCSV(arquivos[6]);
	executarTesteBozo(bozoSort, arquivos[6]);
}


void testeQuickSort() {
	inicializaArquivoCSV(arquivos[7]);
	executarTesteParametrizado(quickSort, arquivos[7]);
}
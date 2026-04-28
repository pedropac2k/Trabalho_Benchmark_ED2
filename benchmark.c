#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

//Declação das constantes usadas durante os testes.
#define TAM        5
#define SEED       1
#define PORC       10
#define QTD_TESTES 10
const int tamanhos[TAM] = {1000, 10000, 100000, 1000000, 10000000};

typedef struct t_metricas {
	unsigned long long comparacoes;
	unsigned long long movimentacoes;
}acesso_metricas;

int mostraMenu() {
	int escolha;

	printf("\n--- Menu de Testes ---");
	printf("\nEscolha qual o metodo a ser testado (ou 0 para sair): ");
	printf("\n\t1 - Bolha Inteligente\n\t2 - Insercao\n\t3 - Selecao\n\t4 - MergeSort\n\t5 - QuickSort Tradicional\n\t6 - ShellSort\n\t7 - HeapSort");
	printf("\nOpcao: ");
	scanf("%d", &escolha);

	return escolha;
}


void escolhaDeTestes() {
	int escolha;

	printf("\n##### Iniciando rotina de testes #####\n");
	escolha = mostraMenu();
	do {
		// Execução do teste baseado na escolha
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
				testeQuickSortTradicional();
				break;
			case 6:
				testeShellSort();
				break;
			case 7:
				testeHeapSort();
				break;
			default:
				printf("\n#### Opcao invalida! Tente novamente ####\n");
				break;
		}
		printf("\n----- Bateria de testes concluida -----\n");
		escolha = mostraMenu();
	}while (escolha);
	printf("\n##### Encerrando rotina de testes #####\n");
}


double obterTempo() {
	LARGE_INTEGER frequencia, contador;
	QueryPerformanceFrequency(&frequencia);
	QueryPerformanceCounter(&contador);
	return (double)contador.QuadPart * 1000.0 / (double)frequencia.QuadPart;
}


void executarTesteGenerico(AlgoritmoOrdenacao func) {
	r *requisicao;
	met *metricas;
	double inicio;
	double fim;
	unsigned long long mediaComp;
	unsigned long long mediaMov;
	double mediaTempo;

	printf("\n##########################################");
	printf("\n-------- DataSet Decrescente --------\n");
	printf("##########################################\n");
	for(int i = 0; i < TAM; i++) {
		requisicao = geraDecrescente(tamanhos[i], SEED);

		inicio = obterTempo();
		metricas = func(requisicao, tamanhos[i]);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Ordenado --------\n");
	printf("##########################################\n");
	for(int i = 0; i < TAM; i++) {
		requisicao = geraOrdenados(tamanhos[i], SEED);

		inicio = obterTempo();
		metricas = func(requisicao, tamanhos[i]);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Quase Ordenado --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i = 0; i < TAM; i++) {
		mediaComp = 0;
		mediaMov = 0;
		mediaTempo = 0;
		for(int j = 0; j < QTD_TESTES; j++) {
			requisicao = geraQuaseOrdenados(tamanhos[i], SEED, PORC);

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

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}

	// 4. DataSet Aleatorio
	printf("\n##########################################");
	printf("\n-------- DataSet Aleatorio --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i=0; i<TAM; i++) {
		mediaComp = 0;
		mediaMov = 0;
		mediaTempo = 0;
		for(int j = 0; j < QTD_TESTES; j++) {
			requisicao = geraAleatorios(tamanhos[i], SEED);

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

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}
}


void executarTesteParametrizado(AlgoritmoParametrizado func) {
	r *requisicao;
	met *metricas;
	double inicio;
	double fim;
	unsigned long long mediaComp = 0;
	unsigned long long mediaMov = 0;
	double mediaTempo;

	printf("\n##########################################");
	printf("\n-------- DataSet Decrescente --------\n");
	printf("##########################################\n");
	for(int i=0; i<TAM; i++) {
		requisicao = geraDecrescente(tamanhos[i], SEED);
		metricas = alocaMetricas();

		inicio = obterTempo();
		func(requisicao, 0, tamanhos[i] - 1, metricas);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Ordenado --------\n");
	printf("##########################################\n");
	for(int i=0; i<TAM; i++) {
		requisicao = geraOrdenados(tamanhos[i], SEED);
		metricas = alocaMetricas();

		inicio = obterTempo();
		func(requisicao, 0, tamanhos[i] - 1, metricas);
		fim = obterTempo();

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nTempo de processamento: %.4fms\n", (fim - inicio));
		imprimeMetricas(metricas);

		liberaVetor(requisicao);
		liberaMetricas(metricas);
	}

	printf("\n##########################################");
	printf("\n-------- DataSet Quase Ordenado --------\n");
	printf("Numero de testes realizados para obter a media: %d", QTD_TESTES);
	printf("\n##########################################\n");
	for(int i=0; i<TAM; i++) {
		mediaComp = 0; mediaMov = 0; mediaTempo = 0;
		for(int j=0; j<QTD_TESTES; j++) {
			requisicao = geraQuaseOrdenados(tamanhos[i], SEED, PORC);
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
		mediaComp = 0; mediaMov = 0; mediaTempo = 0;
			for(int j=0; j<QTD_TESTES; j++) {
			requisicao = geraAleatorios(tamanhos[i], SEED);
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

		printf("\n--- Resultados para %d elementos ---", tamanhos[i]);
		printf("\nMedia das Comparacoes: %llu", mediaComp);
		printf("\nMedia das Movimentacoes: %llu", mediaMov);
		printf("\nTempo medio de processamento: %.4fms\n", mediaTempo);
	}
}


void testeBolhaInteligente() {
	executarTesteGenerico(bolhaInteligente);
}


void testeShellSort() {
    executarTesteGenerico(shellSort);
}


void testeHeapSort() {
    executarTesteGenerico(heapSort);
}


void testeInsercao() {
   executarTesteGenerico(insercao);
}


void testeSelecao() {
	executarTesteGenerico(selecao);
}


void testeMergeSort() {
	executarTesteParametrizado(mergeSort);
}


void testeQuickSortTradicional() {
	executarTesteParametrizado(quickSort);
}
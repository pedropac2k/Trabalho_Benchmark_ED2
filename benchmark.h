//
// Created by Pedro on 4/27/2026.
//

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "ordenacao.h"

//Função auxiliar para a plotagem do menu ao usuário
int mostraMenu();


//Permite ao usuário definir a rotina de testes
void escolhaDeTestes();


//Mede o tempo de processamento dos testes com precisão
double obterTempo();


//Define um tipo para passar as funções de ordenação como parâmetro
typedef met* (*AlgoritmoOrdenacao)(r*, int); //É usado em funções que retornam as métricas
typedef void (*AlgoritmoParametrizado)(r*, int, int, met*);//É usado em funções que alteram as métricas por parâmetro


//Conversão para 'arquivo.csv' dos dados obtidos
void registrarResultadoCSV(char *nomeMetodo, char *dataset, int tamanho, met *m, double tempo);
void registrarMediasCSV(char *nomeMetodo, char *dataset, int tamanho, unsigned long long mediaComp, unsigned long long mediaMov, double tempo);


void inicializaArquivoCSV(char *nomeArq);


//Recebe algorítimo genérico e aplica os testes
//Criada para evitar a repetição de trechos de código
void executarTesteGenerico(AlgoritmoOrdenacao func, char *nomeAlgoritimo);//Tratam funções que retornam as métricas como resultado
void executarTesteParametrizado(AlgoritmoParametrizado func, char *nomeAlgoritimo);//Tratam funções que alteram as métricas por parâmetro


//Realiza a bateria de testes utilizando o algorítimo 'Bubble Sort'
void testeBolhaInteligente();


//Realiza a bateria de testes utilizando o algorítimo 'Insertion Sort'
void testeInsercao();


//Realiza a bateria de testes utilizando o algorítimo 'Selection Sort'
void testeSelecao();


//Realiza a bateria de testes utilizando o algorítimo 'Merge Sort'
void testeMergeSort();


//Realiza a bateria de testes utilizando o algorítimo 'Quick Sort'
void testeQuickSortTradicional();


//Realiza a bateria de testes utilizando o algorítimo 'Shell Sort'
void testeShellSort();


//Realiza a bateria de testes utilizando o algorítimo 'Heap Sort'
void testeHeapSort();

#endif //BENCHMARK_H

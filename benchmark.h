//
// Created by Pedro on 4/27/2026.
//

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "ordenacao.h"

//Estrutura para a acumular resultados das métricas(Types Cast Forçado)
typedef struct t_metricas {
	unsigned long long comparacoes;
	unsigned long long movimentacoes;
}acesso_metricas;


//Função auxiliar para a plotagem do menu ao usuário
int mostraMenu(int escolha);


//Permite ao usuário definir a rotina de testes
void escolhaDeTestes();


//Mede o tempo de processamento dos testes com precisão
double obterTempo();


//Define um tipo para passar as funções de ordenação como parâmetro
typedef met* (*AlgoritmoOrdenacao)(r*, int); //É usado em funções que retornam as métricas
typedef void (*AlgoritmoParametrizado)(r*, int, int, met*);//É usado em funções que alteram as métricas por parâmetro


//Conversão para 'arquivo.csv' dos dados de performance obtidos
void registrarResultadoCSV(char *nomeMetodo, char *dataset, int tamanho, met *m, double tempo);
void registrarMediasCSV(char *nomeMetodo, char *dataset, int tamanho, unsigned long long mediaComp, unsigned long long mediaMov, double tempo);


//Garante que os arquivos CSVs não acumulam resultados de execuções diferentes do programa
void inicializaArquivoCSV(char *nomeArq);


//Lê o arquivo com os dados do vetor de cada ordenação e verifica se ela é estável
int verificaEstabilidade(char *nomeArq, int tam);


//Altera o nome do arquivo para armazenar o vetor ordenado para cada algorítimo de ordenação, para a verificação de estabilidade
void trataNomeArquivo(char *nomeArq);


//Recebe algorítimo genérico e aplica os testes
//Criada para evitar a repetição de trechos de código
void executarTesteGenerico(AlgoritmoOrdenacao func, char *nomeAlgoritimo);//Tratam funções que retornam as métricas como resultado
void executarTesteParametrizado(AlgoritmoParametrizado func, char *nomeAlgoritimo);//Tratam funções que alteram as métricas por parâmetro
void executarTesteBozo(AlgoritmoOrdenacao func, char *nomeAlgoritimo);


//Realiza a bateria de testes utilizando o algorítimo 'Bubble Sort'
void testeBolhaInteligente();


//Realiza a bateria de testes utilizando o algorítimo 'Insertion Sort'
void testeInsercao();


//Realiza a bateria de testes utilizando o algorítimo 'Selection Sort'
void testeSelecao();


//Realiza a bateria de testes utilizando o algorítimo 'Merge Sort'
void testeMergeSort();


//Realiza a bateria de testes utilizando o algorítimo 'Quick Sort'
void testeQuickSort();


//Realiza a bateria de testes utilizando o algorítimo 'Shell Sort'
void testeShellSort();


//Realiza a bateria de testes utilizando o algorítimo 'Heap Sort'
void testeHeapSort();


//Realiza a bateria de testes utilizando o algorítimo 'Bozo Sort'
void testeBozoSort();

#endif //BENCHMARK_H
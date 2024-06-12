#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cidades.h"

// Função comparar cidades por suas posições
int compareCities(const void *a, const void *b){
    return ((Cidade *)a)->Posicao - ((Cidade *)b)->Posicao;
}

// Analisa se todas as restrições são atendidas
int verificarRestricoes(Estrada *estrada){
    // Analisa se T e N estão dentro dos limites permitidos
    if(estrada->T < 3 || estrada->T > 106 || estrada->N < 2 || estrada->N > 104){
        printf("Erro: Restricoes nao atendidas para T ou N.\n");
        return 0;
    }

    // Analisa se todas as posições das cidades estão dentro dos limites e são diferentes umas das outras
    for(int i = 0; i < estrada->N; i++){
        if (estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= estrada->T){
            printf("Erro: Restricoes nao atendidas para as posicoes das cidades.\n");
            return 0;
        }
        for(int j = i + 1; j < estrada->N; j++){
            if(estrada->C[i].Posicao == estrada->C[j].Posicao){
                printf("Erro: Restricoes nao atendidas para Xi ≠ Xj.\n");
                return 0;
            }
        }
    }

    return 1; // Todas as restrições são atendidas
}

// Inicializa cidades no TAD Estrada
Estrada *getEstrada(const char *nomeArquivo){
    FILE *file = fopen(nomeArquivo, "r");
    if(!file){
        printf("Erro ao abrir o arquivo %s\n", "teste01.txt");
        return NULL;
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if(!estrada){
        printf("Erro ao alocar memoria para Estrada.\n");
        fclose(file);
        return NULL;
    }

    if(fscanf(file, "%d", &estrada->T) != 1){
        printf("Erro ao ler o comprimento da estrada(T).\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    if(fscanf(file, "%d", &estrada->N) != 1){
        printf("Erro ao ler o numero de cidades(N).\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        printf("Erro ao alocar memoria para o vetor de cidades.\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    for(int i = 0; i < estrada->N; i++){
        if (fscanf(file, "%d %[^\n]", &estrada->C[i].Posicao, estrada->C[i].Nome) != 2){
            printf("Erro ao ler os dados da cidade %d.\n", i+1);
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);

    if(!verificarRestricoes(estrada)){
        free(estrada->C);
        free(estrada);
        return NULL;
    }

    qsort(estrada->C, estrada->N, sizeof(Cidade), compareCities);
    return estrada;
}

// Calcula a menor vizinhança
double calcularMenorVizinhanca(const char *nomeArquivo){
    Estrada *estrada = getEstrada(nomeArquivo);
    if(!estrada) return -1;

    double menorVizinhanca = estrada->T;
    for(int i = 0; i < estrada->N - 1; i++){
        double vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
        if(vizinhanca < menorVizinhanca){
            menorVizinhanca = vizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

// Retorna a cidade que tem a menor vizinhança
char *cidadeMenorVizinhanca(const char *nomeArquivo){
    Estrada *estrada = getEstrada(nomeArquivo);
    if(!estrada) return NULL;

    double menorVizinhanca = estrada->T;
    char *cidade = NULL;

    for(int i = 0; i < estrada->N - 1; i++){
        double vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
        if(vizinhanca < menorVizinhanca){
            menorVizinhanca = vizinhanca;
            cidade = estrada->C[i].Nome;
        }
    }
    char *result = (char *)malloc((strlen(cidade) + 1) * sizeof(char));
    if(result){
        strcpy(result, cidade);
    }
    free(estrada->C);
    free(estrada);
    return result;
}

// Função principal para testar as funções
int main(){
    const char *nomeArquivo = "cidadeTeste.txt";

    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada){
        printf("Estrada inicializada com sucesso.\n");
        printf("Comprimento total da estrada: %d\n", estrada->T);
        printf("Numero de cidades: %d\n", estrada->N);
        for (int i = 0; i < estrada->N; i++) {
            printf("Cidade: %s, Posicao: %d\n", estrada->C[i].Nome, estrada->C[i].Posicao);
        }
        free(estrada->C);
        free(estrada);
    } else{
        printf("Erro ao inicializar a estrada.\n");
    }

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    if(menorVizinhanca != -1){
        printf("Menor vizinhanca: %.2f\n", menorVizinhanca);
    }else{
        printf("Erro ao calcular a menor vizinhanca.\n");
    }

    char *cidade = cidadeMenorVizinhanca(nomeArquivo);
    if(cidade){
        printf("Cidade com menor vizinhanca: %s\n", cidade);
        free(cidade);
    }else{
        printf("Erro ao encontrar a cidade com menor vizinhanca.\n");
    }

    return 0;
}
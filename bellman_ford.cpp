#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

// Onde tem o nome dos vertices (a,b,c,d,e)
typedef struct state{
    char stateName[100];
}stateName;
stateName estadoGrafo[100];
int numeroEstados;

// Onde sera armazenado os valores associados aos vertices
typedef struct vertices{
    char pai[100];
    char filho[100];
    int peso;
}vertices;

vertices relacoes[100];
int numeroDeVertices;

// Esta eh a tabela importante, onde serao armazenados os valores das iteracoes
typedef struct SCP{
    char stateName[100];
    int buscaCusto;
    int infinito;                   //1 or 0
    char pai[100];
}SCP;
SCP tabelaReduzida[100];

char aux[100], aux1[100],c;


void pegarGrafo(){
    int i, j, l, s;
    FILE *InputFile;

    InputFile = fopen("inputN.txt", "r");
    fgets(aux, 255, InputFile);
    j = 0;
    s = 0;
    char virgula = ',';

    for(i= 0; i < strlen(aux); i++){
        c = aux[i];
        if(c != virgula){
            if(c != '\n'){
                estadoGrafo[s].stateName[j] = c;
                j++;
            }
        }else{
            j=0;
            s++;
        }
    }
    numeroEstados = s+1;
    cout<<"Estatos do Grafo: ";
    for(int i =0; i<numeroEstados; i++){
        cout<<"("<<estadoGrafo[i].stateName<<")";
    }
    cout<<"\nO estado inicial eh: "<<estadoGrafo[0].stateName<<endl;

    cout<<"Vertices do grafo: \n";
    j=0;
    numeroDeVertices=0;

    while(!feof(InputFile)){
        fscanf(InputFile,"%s %s %d", relacoes[j].pai, relacoes[j].filho, &relacoes[j].peso);
        j++;
    }
    numeroDeVertices = j;

    for(i=0; i < numeroDeVertices; i++){
        cout<<relacoes[i].pai <<"|"<< relacoes[i].filho<<"|"<<relacoes[i].peso<<endl;
    }
    fclose(InputFile);
}

void comecarRelaxamento(){
    int i;
    strcpy(tabelaReduzida[0].stateName, estadoGrafo[0].stateName);
    tabelaReduzida[0].infinito = 0;
    tabelaReduzida[0].buscaCusto = 0;
    for(i=1; i<= numeroEstados; i++){
        strcpy(tabelaReduzida[i].stateName, estadoGrafo[i].stateName);
        tabelaReduzida[i].infinito = 1;
    }
}
int tentaRelaxar(char pre[], char sec[], int w){
    int changed,i, preIndex, secIndex;
    int preCustoInfinito, secCustoInfinito;
    int preCustoAtual, secCustoAtual;
    changed = 0;

    for(i = 0; i < numeroEstados; i++){
        if(strcmp(pre, tabelaReduzida[i].stateName)==0){
            preIndex = i;
            if(tabelaReduzida[i].infinito == 1){
                preCustoInfinito=1;
            }else{
                preCustoInfinito=0;
                preCustoAtual = tabelaReduzida[i].buscaCusto;
            }
        }
        if(strcmp(sec, tabelaReduzida[i].stateName)==0){
            secIndex = i;
            if(tabelaReduzida[i].infinito == 1){
                secCustoInfinito=1;
            }else{
                secCustoInfinito=0;
                secCustoAtual = tabelaReduzida[i].buscaCusto;
            }
        }
    }
    if(preCustoInfinito == 0 && secCustoInfinito == 1){
        tabelaReduzida[secIndex].buscaCusto = preCustoAtual + w;
        tabelaReduzida[secIndex].infinito = 0;
        strcpy(tabelaReduzida[secIndex].pai, pre);
        changed = 1;
    }
    if(preCustoInfinito == 0 && secCustoInfinito == 0){
        if(secCustoAtual > (preCustoAtual + w)){
            tabelaReduzida[secIndex].buscaCusto = preCustoAtual + w;
            strcpy(tabelaReduzida[secIndex].pai, pre);
            changed = 1;
        }
    }
    return changed;
}
void relaxamento(){
    int i, j;
    for(i = 0; i < numeroEstados; i++){
        for(j = 0; j < numeroDeVertices; j++){
            if(strcmp(tabelaReduzida[i].stateName, relacoes[j].pai) == 0){
                tentaRelaxar(tabelaReduzida[i].stateName, relacoes[j].filho, relacoes[j].peso);
            }
        }
    }
}

void menorCaminho(){
    int i,b,j,existeCicloNegativo=0;
    FILE *OutputFile;
    OutputFile = fopen("output.txt", "w");
    fputs("state | pai | buscaCusto\n", OutputFile);

    for(i=0; i < numeroEstados; i++){
        if(tabelaReduzida[i].infinito == 1){
            fprintf(OutputFile, "%s | %s infinito\n", tabelaReduzida[i].stateName, tabelaReduzida[i].pai);
        }else{
            fprintf(OutputFile, "%s | %s %d\n", tabelaReduzida[i].stateName, tabelaReduzida[i].pai, tabelaReduzida[i].buscaCusto);
        }
    }

    // verifica se existe um ciclo negativo
    existeCicloNegativo =0;
    for(i=0; i<numeroEstados; i++){
        for(j=0; j<numeroDeVertices; j++){
            if(strcmp(estadoGrafo[i].stateName, relacoes[j].pai) == 0){
                existeCicloNegativo = tentaRelaxar(estadoGrafo[i].stateName, relacoes[j].filho, relacoes[j].peso);
            }
        }
    }
    if(existeCicloNegativo == 1){
        cout<<"\nExiste um ciclo negativo"<<endl;
        fprintf(OutputFile, "Existe um ciclo negativo\n");
    }else{
        cout<<"\nNao existe um ciclo negativo"<<endl;
        fprintf(OutputFile, "Nao Existe um ciclo negativo\n");
    }
    fclose(OutputFile);
}


main(){
    int i;
    cout<<"Algoritmo de Bellman-ford"<<endl;
    pegarGrafo();
    comecarRelaxamento();
    for(i=0; i < numeroEstados-1; i++){
        relaxamento();
    }
    menorCaminho();
}
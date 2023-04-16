// Nomes: Caio Pedroso Gazen, Vitor sepulveda Monteiro, Rafael Cruz Ferreira, Hyasmim Boechat

#include <stdio.h>
#include <time.h>
#include <stdlib.h> 


struct RegItens{       //Struct para armazenar os itens
    int Ganho;
    int Peso;
    double Densidade;
};



void ImprimirMochila(struct RegItens Item[], int *Mochila, int NumeroItensMochila){  //Declaração da função que imprime a mochila
    int t = 0, tg = 0; //Declaração das variáveis que armazenam o peso e o ganho da mochila
    printf("\n"); 
    printf("         Item   Peso   Ganho");
    for(int i = 0; i < NumeroItensMochila; i++){    //Loop que imprime os itens da mochila
        printf("\nMochila:  %d ---- %d ---- %d", Mochila[i], Item[Mochila[i]].Peso, Item[Mochila[i]].Ganho);
    }
    for(int i = 0; i < NumeroItensMochila; i++){    //Loop que calcula o peso da mochila
        t += Item[Mochila[i]].Peso;
    }
    printf("\n  Peso total: %d", t);    //Imprime o peso da mochila
    for(int i = 0; i < NumeroItensMochila; i++){    //Loop que calcula o ganho da mochila
        tg += Item[Mochila[i]].Ganho;
    }
    printf("\n  Ganho total: %d", tg);  //Imprime o ganho da mochila
    printf("\n");
}



void LerMochila(int *Capacidade, int *NumeroDeItens, FILE *fMochila){   //Declaracao da função que coleta os dados do arquivo
    fscanf(fMochila, "%d", Capacidade);      //Leitura da capacidade da mochila
    fscanf(fMochila, "%d", NumeroDeItens);   //Leitura do numero de itens no arquivo
}



void LerItens(struct RegItens Item[],int *NumeroDeItens, FILE *fMochila){   //Declaracao da função que coleta os dados dos itens
    for(int i = 0; i < *NumeroDeItens; i++){    //Loop que coleta os dados dos itens
        fscanf(fMochila, "%d", &Item[i].Ganho); //leitura do ganho do item
        fscanf(fMochila, "%d", &Item[i].Peso);  //leitura do peso do item
        Item[i].Densidade = (double)Item[i].Ganho/(double)Item[i].Peso; //calculo da densidade do item
    }
}



void BuscaLocal(struct RegItens Item[],int NumeroDeItens,int *NumeroItensMochila, int *Mochila, int *PesoMochila, int Capacidade){  //Declaracao da função de busca local
    int lista[NumeroDeItens];   //Declaracao da lista de itens para a busca local
    int temp;   //Declaracao da variavel temporaria para a troca de itens
    for(int i = 0; i < NumeroDeItens; i++){ //loop que preenche a lista de itens
        lista[i] = i;
    }

    for(int i = 0; i < NumeroDeItens; i++){ //loop faz um bubble sort
        for(int j = 0; j < NumeroDeItens - 1; j++){ //segundo loop do bubble sort
            if(Item[lista[j]].Densidade < Item[lista[j+1]].Densidade){  //condição de ordenação do bubble sort de acorco com a densidade dos itens
                temp = lista[j];        //troca da posicao dos itens
                lista[j] = lista[j+1];
                lista[j+1] = temp;
            }
        }
    }

    for(int i = 0; i < NumeroDeItens ;i++){ //loop que adiciona os itens na mochila
        if(*PesoMochila + Item[lista[i]].Peso <= Capacidade){   //condição para verificar se o item não ultrapassa a capacidade da mochila
            int* TempMochila = realloc(Mochila, *NumeroItensMochila * sizeof(int) + sizeof(int)); //realocação da mochila para adicionar o item
            if(TempMochila == NULL){    //verificação de erro de alocacao de memoria
                printf("erro de alocacão de memoria");
                return;
            }
            else{ //realocação realizada com sucesso
                Mochila = TempMochila;
            }
            Mochila[*NumeroItensMochila] = lista[i];    //adição do item na mochila
            *PesoMochila += Item[lista[i]].Peso;        //atualização do peso da mochila
            *NumeroItensMochila += 1;                   //atualização do numero de itens na mochila
        }
    }
    ImprimirMochila(Item, Mochila, *NumeroItensMochila);    //chamada da função que imprime a mochila
}



int Redundancia(int NumeroItensMochila, int *Mochila, int RandItem){    //função que verifica se o item ja esta na mochila
    for(int i = 0; i < NumeroItensMochila; i++){    //loop que percorre a mochila
        if(Mochila[i] == RandItem){ //se o item aletorio ja esta na mochila
            return 0; //retorna 0, pois o item ja esta na mochila
        }
    }
    return 1; // o item aletorio nao esta na mochila, retorna 1
}

void Ils(struct RegItens Item[],int NumeroDeItens,int *NumeroItensMochila, int *Mochila, int *PesoMochila, int Capacidade){ //Declaracao da função ILS
    int RandMochila = rand() % (*NumeroItensMochila + 2);   //gera um numero aleatorio que vai de 0 até o numero de itens na mochila + 1,
    int RandItem = rand() % (NumeroDeItens);    //gera um numero aleatorio que vai de 0 até o numero de itens
    if(RandMochila == *NumeroItensMochila && Redundancia(*NumeroItensMochila, Mochila, RandItem) && (*PesoMochila + Item[RandItem].Peso) <= Capacidade){ //se o numero aleatorio da mochila for igual ao numero de itens na mochila, tentar adicionar um item na mochila,antes de adicionar o item aletorio na mochila , verifica se o item aletorioa a ser adicionado ja esta na mochila e se o peso da mochila + peso do item aletorio nao ultrapassa a capacidade
        int* TempMochila = realloc(Mochila, *NumeroItensMochila * sizeof(int) + sizeof(int)); //realocação da mochila para adicionar um item
            if(TempMochila == NULL){    //verificação de erro de alocacao de memoria
                printf("erro de alocacão de memoria");
                return;
            }
            else{   //realocação realizada com sucesso
                Mochila = TempMochila;
            }
            Mochila[*NumeroItensMochila] = RandItem;    //adição do item aleatorio na mochila
            *PesoMochila += Item[RandItem].Peso;        //atualização do peso da mochila
            *NumeroItensMochila += 1;                   //atualização do numero de itens na mochila
    }

    if(RandMochila == (*NumeroItensMochila + 1) && *NumeroItensMochila > 1){    //se o numero aleatorio da mochila for igual ao numero de itens na mochila + 1 e se a mochila tem mais de um item, remover um item aleatorio da mochila
        int Randremover  = rand() % (*NumeroItensMochila); //escolhe um item para remover da mochila
        *PesoMochila -= Item[Mochila[Randremover]].Peso; //atualiza o peso da mochila
        Mochila[Randremover]  = Mochila[*NumeroItensMochila - 1]; //troca o item removido pelo ultimo item da mochila
        *NumeroItensMochila -= 1; //atualiza o numero de itens na mochila
        int* TempMochila = realloc(Mochila, (*NumeroItensMochila * sizeof(int)));   //realocação da mochila para remover um item
            if(TempMochila == NULL){
                printf("erro de alocacão de memoria");
                return;
            }
            else{ //realocação realizada com sucesso
                Mochila = TempMochila;
            }
    }
    if (RandMochila < *NumeroItensMochila && ((*PesoMochila - Item[Mochila[RandMochila]].Peso) +  Item[RandItem].Peso) <= Capacidade && Redundancia(*NumeroItensMochila, Mochila, RandItem)){ //se o numero aleatorio da mochila for menor que o numero de itens na mochila, se o peso da mochila - peso do item aleatorio a ser removido da mochila + peso do item aletorio a ser adicionado na mochila nao ultrapassa a capacidade e se o item aleatorio a ser adicionado não esta na mochila
        *PesoMochila -= Item[Mochila[RandMochila]].Peso; //atualiza o peso da mochila
        *PesoMochila += Item[RandItem].Peso;            //atualiza o peso da mochila
        Mochila[RandMochila] = RandItem;                //troca o item removido pelo item aletorio
    }
} 






int main(void){             //Funcao principal
    srand((unsigned int)time(NULL)); //seed para gerar numeros aleatorios baseada na funcao time

    FILE *fMochila = fopen("Mochila.txt","r");  //abertura do arquivo de entrada
    if(fMochila == NULL){   //verifica se o arquivo foi aberto com sucesso
        printf("\n\n Erro Na leitura do arquivo\n\n"); //imprime mensagem de erro
        return 2; //retorna 2, codigo de erro 
    }

    int Capacidade, NumeroDeItens;  //declaracao das variaveis de entrada
    
    LerMochila(&Capacidade, &NumeroDeItens, fMochila);  //leitura das propriedades da mochila
    struct RegItens Item[NumeroDeItens - 1];
    LerItens(Item, &NumeroDeItens, fMochila);           //leitura dos itens
   
    fclose(fMochila);   //fecha o arquivo de entrada

    int PesoMochila = 0;    //decracao da variavel que vai guardar o peso da mochila
    int NumeroItensMochila = 0;     //declaracao da variavel que vai guardar o numero de itens na mochila
    int* Mochila = (int*)calloc(1, sizeof(int));    //alocacao dinamica para a mochila
  
    BuscaLocal(Item, NumeroDeItens, &NumeroItensMochila , Mochila, &PesoMochila, Capacidade); //chamada da funcao de busca local

    int tempMochilaBusca[NumeroItensMochila];   //guarda a copia da mochila para imprimir depois
    int tempNItensBusca = NumeroItensMochila;
    for (int i = 0; i < tempNItensBusca; i++){
            tempMochilaBusca[i] = Mochila[i];
    }

    printf("cap: %d NumeroDeItens: %d\n", Capacidade, NumeroDeItens); //imprime as propriedades da mochila
    for(int i = 0; i < NumeroDeItens; i++){  //imprime todos os itens
        printf("%d", Item[i].Ganho);
        printf("  %d  ", Item[i].Peso);
        printf("%f", Item[i].Densidade);
        printf(" %d \n", i );
    }

    int NumeroAlteracoesPorIteracao = 10, NumeroIteracoes =100; //Declara o numero de Alteracoes por Iteracao e o numero de Iteracoes que a Ils vai executar

    for(int i = 0; i < NumeroIteracoes; i++){
        int tempMochila[NumeroItensMochila];    //declara um vetor para armazenar uma copia da melhor mochila
        int tempNItens = NumeroItensMochila;    //guarda o numero de itens na melhor mochila
        int tempPeso =  PesoMochila;            //guarda o peso da melhoro mochila
        int tempGanho = 0;                      //declara uma variavel para armazenar o ganho da melhor mochila 
        int Ganho = 0;                          //declara uma variavel para armazenar o ganho da mochila
        for (int j = 0; j < NumeroItensMochila; j++){ //calcula o ganho da mochila
            tempGanho += Item[Mochila[j]].Ganho;
        }
        for (int j = 0; j < NumeroItensMochila; j++){ //preenche o vetor com a copia da mochila
            tempMochila[j] = Mochila[j];
        }
        for(int j = 0; j < NumeroAlteracoesPorIteracao; j++){   //chama a ILS na mochila atual
            Ils(Item, NumeroDeItens, &NumeroItensMochila , Mochila, &PesoMochila, Capacidade);
        }

        for (int j = 0; j < NumeroItensMochila; j++){   //calcula o ganho da mochila depois da ILS
            Ganho += Item[Mochila[j]].Ganho;
        }

        if (Ganho < tempGanho){         //se o ganho da mochila depois da ILS for menor que o ganho da melhor mochila, retorna a melhor mochila
            for (int j = 0; j < tempNItens; j++){   //loop que retorna a melhor mochila
                Mochila[j] = tempMochila[j];
            }
            NumeroItensMochila = tempNItens;    //retorna o numero de itens na mochila
            PesoMochila = tempPeso;             //retorna o peso da mochila
        }

        ImprimirMochila(Item, Mochila, NumeroItensMochila); //imprime a melhor mochila atual
    }
    printf("\n\nCombinacao encontrada pelo algoritimo guloso\n");   //imprime a melhor mochila encontrada pelo algoritimo guloso
    ImprimirMochila(Item, tempMochilaBusca, tempNItensBusca);

    printf("\n\nMelhor Combinacao encontrada pelo algoritimo ILS\n");   //imprime a melhor mochila encontrada pelo algoritimo ILS
    ImprimirMochila(Item, Mochila, NumeroItensMochila);
}
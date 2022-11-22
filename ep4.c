#include <stdio.h>
#include <stdlib.h>
#define PRETA 1
#define AZUL -1
#define FALSE 0
#define TRUE 1



int escolheJogada (int *** tab, int n, int cor, int *lin, int *col);
int getZ(int *** tab, int n, int *lin, int *col);
int checaGanhador(int *** tab, int n, int cor);
int podeGanhar(int ***tab, int n, int cor, int *lin, int *col);
int jogarDiagonal(int ***tab, int n, int cor, int *lin, int *col);
int escolheQualquerUma(int ***tab, int n, int cor, int *lin, int *col);
int bestPlay(int ***tab, int n, int cor, int *lin, int *col);
int naoCaiNoBait(int ***tab, int n, int cor, int *lin, int *col);


int main(){
    int n;
    int ***tab;
    int jogadorHumano;
    int jogadorMaquina;
    int fim = FALSE;
    int vez = PRETA;
    int lin;
    int col;
    int z;

    printf("Digite o valor de N: ");
    scanf("%d", &n);

    /* Criando o tabuleiro */
    /* tab[x][y][z] */
    /* tab[lin][col][z] */
    tab = malloc(sizeof(int**)* n);
    for(int i = 0; i < n; i++){
        tab[i] = malloc(sizeof(int*)* n);
        for(int j = 0; j < n; j++)
            tab[i][j] = malloc(sizeof(int) * n);
    }

    printf("Você quer jogar como PRETAs[1] ou Azuis[-1]? ");
    scanf("%d", &jogadorHumano);

    /* O jogadorMaquina é do time contrário ao do jogadorHumano*/
    jogadorMaquina = -1 * jogadorHumano;

    /*Enquanto ninguém ganhar nem preenher tudo, pegar as jogadas*/
    while(!fim){
        /*Recebe a jogada do jogadorHumano*/
        if(vez == jogadorHumano){
            printf("Digite a linha[x] e a coluna[y] que deseja jogar: ");
            scanf("%d %d", &lin, &col);
            z = getZ(tab, n, &lin, &col);
            tab[lin][col][z] = jogadorHumano;
        }
        /*Recbe a jogada do jogadorMAquina e printa ela*/
        else{
            z = escolheJogada(tab, n , jogadorMaquina, &lin, &col);
            /* Se a escolheJogada retorar n, então o jogo acabou*/
            if(z == n)
                fim = TRUE;
            else{
                tab[lin][col][z] = jogadorMaquina;
                printf("Maquina jogou %d %d \n\n", lin , col);
            }
        }
        /*Checa a cada rodada se alguém ganhou*/
        fim = checaGanhador(tab,n, vez);
        if(fim) printf("Fim do jogo!\n");

        /*Passa pra vez do próximo jogador*/
        vez *= -1;
    
    }
    
    return 0;

}

int getZ(int *** tab, int n,  int *lin, int *col){
    /* Retorna o plano z em que a pecinha ira cair
    de acordo com a linha e a coluna informadas*/

    /*Vai subindo de z enquanto não achar uma posição vazia */
    int z = 0;
    while(tab[*lin][*col][z] != 0)
        z++;
    return z;
}

int escolheJogada (int *** tab, int n, int cor, int *lin, int *col){
    /* Função que pensa no melhor lugar para se jogar,
    seguindo uma prioridade de estratégias que eu pensei
    quando fui jogando o 3x3x3 e o 4x4x4 contra outras pessoas*/

    /* Se falta só uma posição pra eu ganhar,
    jogar nessa posição e ganhar*/
    if(podeGanhar(tab, n, cor,lin, col)){
        printf("vou ganhar hahaha\n");
        return getZ(tab, n, lin, col);
    }
    /* Se falta só uma posição pro oponente ganhar,
    jogar nessa posição e impedir ele*/
    if(podeGanhar(tab, n, -1 * cor ,lin, col)){
        printf("Vai ganhar nao cara\n");
        return getZ(tab, n, lin, col);}
    

    /* Quando o tabuleiro for ímpar, jogar na posição central*/
    if(n%2 == 1){
        printf("Deixa eu colocar na central aqui\n");
        if(tab[n/2][n/2][0] == 0){
            *lin = n/2;
            *col = n/2;
            return getZ(tab, n, lin, col);
        }
    }

    /* Verifcar se o jogador está prestes a fazer uma armadilha
    e impedir (ler descrição da função)*/
    if(naoCaiNoBait(tab,n,-1*cor,lin,col)){
        printf("Que isso ta biruleibe? Eu nao caio em bait\n");
        return getZ(tab, n, lin, col);}
    
    /* Jogar nos cantos que não estejam diagonalmente opostos 
    a uma peça do oponente*/
    printf("Cantooo livree\n");
    if(tab[0][0][0] == 0 && tab[n-1][n-1][0] != -1 * cor){
        *lin = 0;
        *col = 0;
        return getZ(tab, n, lin, col);
    }
    if(tab[0][n-1][0] == 0 && tab[n-1][0][0] != -1 * cor){
        *lin = 0;
        *col = n-1;
        return getZ(tab, n, lin, col);
    }

    /* Joga preenchendo posições para ganhar na diagonal,
    quando possivel (ler descrição da função)*/
    if(jogarDiagonal(tab, n, cor,lin, col)){
        printf("Preencher umas diagonais aqui e ali\n");
        return getZ(tab, n, lin, col);}


    printf("Cantooooo\n");
    /* Se ainda houver cantos livres no plano z = 0, jogar neles*/
    if(tab[0][0][0] == 0){
        *lin = 0;
        *col = 0;
        return getZ(tab, n, lin, col);
    }
    if(tab[0][n-1][0] == 0){
        *lin = 0;
        *col = n-1;
        return getZ(tab, n, lin, col);
    }
    if(tab[n-1][0][0] == 0){
        *lin = n-1;
        *col = 0;
        return getZ(tab, n, lin, col);
    }
    if(tab[n-1][n-1][0] == 0){
        *lin = n-1;
        *col = n-1;
        return getZ(tab, n, lin, col);
    }

    /* Se acabou as estrategias, jogar em fileiras
    que já tenham pessas minhas e não tenham nenhuma do oponente
    (ler descrição da função)*/
    if(bestPlay(tab,n,cor,lin,col)){
        printf("Eu sou o mió\n");
        return getZ(tab, n, lin, col);}
    
    /* Se a função de cima não achou nenhuma fileira não bloqueada,
    jogar na primeira posição válida que achar*/
    if(escolheQualquerUma(tab,n,cor,lin,col)){
        printf("vou escolher qualquer uma fds\n");
        return getZ(tab, n, lin, col);}
    
    /* Se a função acima não achou nenhuma posição válida, acabaram as*/
    return n;
    
}

int checaGanhador(int *** tab, int n, int cor){
    /*Passa por todas as possibilidades de se alinhas N
    em um tabuleiro NXNxN e checa se a cor dada ganhou*/


    /* Checa todas as linhas retas (não diagonais)
    em todas as faces do cubo(tabuleiro)*/
    /*O cubo N faces em cada uma das 3 direções (x,y,z)*/
    int ganhou = TRUE;
    
    /*Checando na direção z (o k em tab[i][j][k] varia)*/
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            for(int k = 0; k < n; k++){
                if(tab[i][j][k] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou) return ganhou;
        }
    }
    
    /*Checando na direção x (o k em tab[k][i][j] varia)*/
    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            for(int k = 0; k < n; k++){
                if(tab[k][i][j] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou) return ganhou;
        }
    }
    
    /*Checando na direção y (o k em tab[i][k][j] varia)*/
    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            for(int k = 0; k < n; k++){
                if(tab[i][k][j] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou) return ganhou;
        }
    }

    /* Checando as 4 diagonais internas do cubo */
    /* Começando da posição (startLin,startCol,0) e indo até a posição diagonalmente oposta no cubo*/
    /* Diagonal interna 1 */
    int startLin = 0;
    int startCol = 0;

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] != cor){
            ganhou = FALSE;
            break;
        }
        startLin++;
        startCol++;
    }
    if(ganhou) return ganhou;

    /* Diagonal interna 2 */
    startLin = 0;
    startCol = n-1;

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] != cor){
            ganhou = FALSE;
            break;
        }
        startLin++;
        startCol--;
    }
    if(ganhou) return ganhou;

    /* Diagonal interna 3 */
    startLin = n-1;
    startCol = 0;

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] != cor){
            ganhou = FALSE;
            break;
        }
        startLin--;
        startCol++;
    }
    if(ganhou) return ganhou;

    /* Diagonal interna 4 */
    startLin = n-1;
    startCol = n-1;

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] != cor){
            ganhou = FALSE;
            break;
        }
        startLin--;
        startCol--;
    }
    if(ganhou) return ganhou;

     /* Checa todas as diagonais em todas as faces do cubo(tabuleiro)
    O cubo N faces em cada uma das 3 direções (x,y,z)
    Cada face tem 2 diagonais */

    /*Faces na direção x */
    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k++;
        }
        if(ganhou) return ganhou;

        ganhou = TRUE;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k--;
        }
        if(ganhou) return ganhou;
    }

    /*Faces na direção y */
    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        for(int a = 0; a < n; a++){
            if(tab[j][i][k]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k++;
        }
        if(ganhou) return ganhou;

        ganhou = TRUE;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[j][i][k]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k--;
        }
        if(ganhou) return ganhou;
    }

    /*Faces na direção z */
    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        for(int a = 0; a < n; a++){
            if(tab[k][j][i]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k++;
        }
        if(ganhou) return ganhou;

        ganhou = TRUE;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[k][j][i]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k--;
        }
        if(ganhou) return ganhou;
    }
    return ganhou;

}

int jogarDiagonal(int ***tab, int n, int cor, int *lin, int *col){
    /* Utiliza a parte de checagem das diagonais da função checaGanahdor
    Ele checa se a posição embaixo da posição que compõe a diagonal está ocupada
    com uma bolinha. Se estiver, ele joga nessa posição, e a bolinha cairá
    numa posição que faz parte de uma diagonal. Essa função busca preencher 
    diagonais e ganhar nelas .*/

    int startLin = 1;
    int startCol = 1;
    int ganhou = FALSE;

    ganhou = TRUE;
    if(tab[startLin-1][startCol-1][0] == cor){
    for(int i = 1; i < n; i++){
        if(tab[startLin][startCol][i-1] != 0 && tab[startLin][startCol][i] == 0){
                    ganhou = TRUE;
                    *lin = startLin;
                    *col = startCol;
                    return ganhou;
                }
        startLin++;
        startCol++;
    }
    }

    startLin = 1;
    startCol = n-2;
    if(tab[startLin-1][startCol+1][0] == cor){
    ganhou = TRUE;
    for(int i = 1; i < n; i++){
        if(tab[startLin][startCol][i-1] != 0 && tab[startLin][startCol][i] == 0){
                    ganhou = TRUE;
                    *lin = startLin;
                    *col = startCol;
                    return ganhou;
                }
        startLin++;
        startCol--;
    }

    }

    startLin = n-2;
    startCol = 1;
    if(tab[startLin+1][startCol - 1][0] == cor){
    ganhou = TRUE;
    for(int i = 1; i < n; i++){
        if(tab[startLin][startCol][i-1] != 0 && tab[startLin][startCol][i] == 0){
                    ganhou = TRUE;
                    *lin = startLin;
                    *col = startCol;
                    return ganhou;
                }
        startLin--;
        startCol++;
    }

    startLin = n-2;
    startCol = n-2;
    if(tab[startLin+1][startCol+1][0] == cor){
    ganhou = TRUE;
    for(int i = 1; i < n; i++){
        if(tab[startLin][startCol][i-1] != 0 && tab[startLin][startCol][i] == 0){
                    ganhou = TRUE;
                    *lin = startLin;
                    *col = startCol;
                    return ganhou;
                }
        startLin--;
        startCol--;
    }
    }
    }

    //Checando diagonais das  9 faces
    ganhou = TRUE;
    
    for(int i = 1; i < n; i++){
        int j = 1;
        int k = 1;
        ganhou = TRUE;
        if(tab[i][j][0] == cor){
        for(int a = 1; a < n; a++){
            if(tab[i][j][k-1] != 0 && tab[i][j][k-1] == 0){
                    ganhou = TRUE;
                    *lin = i;
                    *col = j;
                    return ganhou;
                }
            j++;
            k++;
        }
    }
    }

    
    ganhou = TRUE;
    for(int i = 1; i < n; i++){
        int j = 1;
        int k = 1;
        ganhou = TRUE;
        if(tab[i][j][0] == cor){
        for(int a = 1; a < n; a++){
            if(tab[i][j][k-1] != 0 && tab[i][j][k-1] == 0){
                    ganhou = TRUE;
                    *lin = i;
                    *col = j;
                    return ganhou;
                }
            j++;
            k--;
        }
    }
    }
    
   
    ganhou = TRUE;
    for(int i = 1; i < n; i++){
        int j = 1;
        int k = 1;
        ganhou = TRUE;
         if(tab[j][i][0] == cor){
        for(int a = 1; a < n; a++){
            if(tab[j][i][k-1] != 0 && tab[j][i][k-1] == 0){
                    ganhou = TRUE;
                    *lin = j;
                    *col = i;
                    return ganhou;
                }
            j++;
            k++;
        }
    }

    
    ganhou = TRUE;
    for(int i = 1; i < n; i++){
        int j = 1;
        int k = 1;
        ganhou = TRUE;
        if(tab[j][i][0] == cor){
        for(int a = 1; a < n; a++){
            if(tab[j][i][k-1] != 0 && tab[j][i][k-1] == 0){
                    ganhou = TRUE;
                    *lin = j;
                    *col = i;
                    return ganhou;
                }
            j++;
            k--;
        }
    }
    }

    return ganhou;
    }
}

int podeGanhar(int ***tab, int n, int cor, int *lin, int *col){
    /* Função que utiliza a passagem por alinhamentos da função checaGanhador
    e indica caso falte apenas uma bolinha para completar o alinhamento 
    da cor selecionada a casinha correspondente*/

    int zeroCount;
    //Checando Rows
    int ganhou = TRUE;
    zeroCount = 0;
    
   
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){
                if(tab[i][j][k] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[i][j][k] == 0 ){
                    zeroCount++;
                    *lin = i;
                    *col = j;
                    if(zeroCount > 1 || !(k == 0 ||tab[i][j][k-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            }
            if(ganhou && zeroCount == 1) return ganhou;
        }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){
                if(tab[k][i][j] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[k][i][j] == 0){
                    zeroCount++;
                    *lin = k;
                    *col = i;
                    if(zeroCount > 1 || !(j == 0 ||tab[k][i][j-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            }
            if(ganhou && zeroCount == 1) return ganhou;
        }
    }
    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){
                if(tab[i][k][j] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[i][k][j] == 0 ){
                    zeroCount++;
                    *lin = i;
                    *col = k;
                    if(zeroCount > 1 || !(j == 0 ||tab[i][k][j-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            }
            if(ganhou && zeroCount == 1) return ganhou;
        }
    }

    //Checando as 4 diagonais do cubo
    int startLin = 0;
    int startCol = 0;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[startLin][startCol][i] == 0){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    if(zeroCount > 1 || !(i == 0 ||tab[startLin][startCol][i-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
        startLin++;
        startCol++;
    }
    if(ganhou && zeroCount == 1) return ganhou;

    startLin = 0;
    startCol = n-1;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 ){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    if(zeroCount > 1 || !(i == 0 ||tab[startLin][startCol][i-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
        startLin++;
        startCol--;
    }
    if(ganhou && zeroCount == 1) return ganhou;

    startLin = n-1;
    startCol = 0;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[startLin][startCol][i] == 0){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    if(zeroCount > 1 || !(i == 0 ||tab[startLin][startCol][i-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
        startLin--;
        startCol++;
    }
    if(ganhou && zeroCount == 1) return ganhou;

    startLin = n-1;
    startCol = n-1;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 ){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    if(zeroCount > 1 || !(i == 0 ||tab[startLin][startCol][i-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
        startLin--;
        startCol--;
    }
    if(ganhou && zeroCount == 1) return ganhou;

    //Checando diagonais das  9 faces
    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[i][j][k] == 0){
                    zeroCount++;
                    *lin = i;
                    *col = j;
                    if(zeroCount > 1 || !(k == 0 ||tab[i][j][k-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            j++;
            k++;
        }
        if(ganhou && zeroCount == 1) return ganhou;
        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[i][j][k] == 0){
                    zeroCount++;
                    *lin = i;
                    *col = j;
                    if(zeroCount > 1 || !(k == 0 ||tab[i][j][k-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            j++;
            k--;
        }
        if(ganhou && zeroCount == 1) return ganhou;
    }
    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[j][i][k] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[j][i][k] == 0  ){
                    zeroCount++;
                    *lin = j;
                    *col = i;
                    if(zeroCount > 1 || ! (k == 0 ||tab[j][i][k-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            j++;
            k++;
        }
        if(ganhou && zeroCount == 1) return ganhou;if(ganhou) return ganhou;
        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[j][i][k] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[j][i][k] == 0){
                    zeroCount++;
                    *lin = j;
                    *col = i;
                    if(zeroCount > 1 || !(k == 0 ||tab[j][i][k-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            j++;
            k--;
        }
        if(ganhou && zeroCount == 1) return ganhou;
    }
    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[k][j][i] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[k][j][i]  == 0 ){
                    zeroCount++;
                    *lin = k;
                    *col = j;
                    if(zeroCount > 1 || !(i == 0 ||tab[k][j][i-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            j++;
            k++;
        }
        if(ganhou && zeroCount == 1) return ganhou;
        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[k][j][i] == -1 *cor){
                    ganhou = FALSE;
                    break;
                }
                if(tab[k][j][i]  == 0){
                    zeroCount++;
                    *lin = k;
                    *col = j;
                    if(zeroCount > 1 || !(i == 0 ||tab[k][j][i-1] != 0)){
                        ganhou = FALSE;
                        break;
                    }
                }
            j++;
            k--;
        }
        if(ganhou && zeroCount == 1) return ganhou;
    }
    return ganhou;
}

int naoCaiNoBait(int ***tab, int n, int cor, int *lin, int *col){
    /* Essa função utiliza a mesma estrutura da função podeGanhar, passando
    por todos os alinhamentos possíveis e vendo se o jogador tem N-2 bolinha em 
    um mesmo alunhamento que não esteja impedido (Não tenha uma bolinha minha).
    Se ele tiver pelo menos dois alinhamentos assim, há chances de ele colocar
    uma armadilha para poder ganhar em dois lugares, então a função coloca uma peça
    para bloquear um desses alinhamentos*/

    int zeroCount;
    int tempLin;
    int tempCol;
    int minCount = n + 1;
    int minCountCount = 0;
    //Checando Rows
    int ganhou = TRUE;
    zeroCount = 0;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){

                if(tab[i][j][k] == -1 *cor || !(k == 0 ||tab[i][j][k-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[i][j][k] == 0 && (k == 0 ||tab[i][j][k-1] != 0)){
                    zeroCount++;
                    *lin = i;
                    *col = j;
                    
                }
            }
            if(zeroCount != 0 && zeroCount < minCount){
                minCount = zeroCount;
                tempLin = i;
                tempCol = j;
            }
        }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){
                if(tab[k][i][j] == -1 *cor || !(j == 0 ||tab[k][i][j-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[k][i][j] == 0 && (j == 0 ||tab[k][i][j-1] != 0)){
                    zeroCount++;
                    *lin = k;
                    *col = i;
                }
            }
            if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
                minCount = zeroCount;
                tempLin = *lin;
                tempCol = *col;
            }
        }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){
                if(tab[i][k][j] == -1 *cor || !(j == 0 ||tab[i][k][j-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[i][k][j] == 0 && (j == 0 ||tab[i][k][j-1] != 0)){
                    zeroCount++;
                    *lin = i;
                    *col = k;
                    
                }
            }
            if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
                minCount = zeroCount;
                tempLin = *lin;
                tempCol = *col;
            }
        }
    }

    //Checando as 4 diagonais do cubo
    int startLin = 0;
    int startCol = 0;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor || !(i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 && (i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    
                }
        startLin++;
        startCol++;
    }
    if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
        minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

    startLin = 0;
    startCol = n-1;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor || !(i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 && (i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    
                }
        startLin++;
        startCol--;
    }
    if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

    startLin = n-1;
    startCol = 0;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor || !(i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 && (i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    
                }
        startLin--;
        startCol++;
    }
    if(zeroCount != 0 && zeroCount < minCount){
            minCount = zeroCount;
            tempLin = *lin;
            tempCol = *col;
        }

    startLin = n-1;
    startCol = n-1;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor || !(i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 && (i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    
                }
        startLin--;
        startCol--;
    }
    if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

    //Checando diagonais das  9 faces
    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k] == -1 *cor || !(k == 0 ||tab[i][j][k-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[i][j][k] == 0 && (k == 0 ||tab[i][j][k-1] != 0)){
                    zeroCount++;
                    *lin = i;
                    *col = j;
    
                }
            j++;
            k++;
        }
        if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k] == -1 *cor || !(k == 0 ||tab[i][j][k-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[i][j][k] == 0 && (k == 0 ||tab[i][j][k-1] != 0)){
                    zeroCount++;
                    *lin = i;
                    *col = j;
                
                }
            j++;
            k--;
        }
        if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[j][i][k] == -1 *cor || !(k == 0 ||tab[j][i][k-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[j][i][k] == 0 && (k == 0 ||tab[j][i][k-1] != 0)){
                    zeroCount++;
                    *lin = j;
                    *col = i;
                    
                }
            j++;
            k++;
        }
       if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[j][i][k] == -1 *cor || !(k == 0 ||tab[j][i][k-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[j][i][k] == 0 && (k == 0 ||tab[j][i][k-1] != 0)){
                    zeroCount++;
                    *lin = j;
                    *col = i;
                    
                }
            j++;
            k--;
        }
        if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[k][j][i] == -1 *cor || !(i == 0 ||tab[k][j][i-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[k][j][i]  == 0 && (i == 0 ||tab[k][j][i-1] != 0)){
                    zeroCount++;
                    *lin = k;
                    *col = j;
                    
                }
            j++;
            k++;
        }
       if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[k][j][i] == -1 *cor || !(i == 0 ||tab[k][j][i-1] != 0)){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[k][j][i]  == 0 && (i == 0 ||tab[k][j][i-1] != 0)){
                    zeroCount++;
                    *lin = k;
                    *col = j;
                    
                }
            j++;
            k--;
        }
        if(zeroCount == minCount) minCountCount++;
            if(zeroCount != 0 && zeroCount < minCount){
                minCountCount = 0;
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }
    }
   
    if(minCount > n - 2 || minCountCount < 2) return FALSE;
    *lin = tempLin;
    *col = tempCol;
    return TRUE;
}

int escolheQualquerUma(int ***tab, int n, int cor, int *lin, int *col){
    /* Escolhe a primeira posição válida que encontrar. No entanto , checa 
    se o oponente pode ganhar na próxima jogada com a jogada escolhida e 
    muda em caso positivo*/

    int tempLin;
    int tempCol;
    int achou = FALSE;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(tab[i][j][n-1] == 0){
                achou = TRUE;
                tempLin = i;
                tempCol = j;
                tab[i][j][getZ(tab, n, &i,&j)] = cor;
                if(!podeGanhar(tab,n, -1 * cor, lin, col)){
                    *lin = i;
                    *col = j;
                    tab[i][j][getZ(tab, n, &i,&j)] = 0;
                    return TRUE;
                }
                tab[i][j][getZ(tab, n, &i,&j)] = 0;
            }
        }
    }
    *lin = tempLin;
    *col = tempCol;
    return achou;
}

int bestPlay(int ***tab, int n, int cor, int *lin, int *col){
    /*Função que utiliza a passagem pelos alinhamentos da função
    checaGanhador e analisa quais alinhamentos ainda não estão impedidos (com
    uma bolinha do oponente) e escolhe o com mais bolinhas minhas para jogar*/

    int zeroCount;
    int tempLin;
    int tempCol;
    int minCount = 0;
    //Checando Rows
    int ganhou = TRUE;
    zeroCount = 0;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){
                if(tab[i][j][k] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[i][j][k] == 0 && (k == 0 ||tab[i][j][k-1] != 0)){
                    zeroCount++;
                    *lin = i;
                    *col = j;
                    
                }
            }
            if(zeroCount > minCount){
                minCount = zeroCount;
                tempLin = i;
                tempCol = j;
            }
        }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){
                if(tab[k][i][j] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[k][i][j] == 0 && (j == 0 ||tab[k][i][j-1] != 0)){
                    zeroCount++;
                    *lin = k;
                    *col = i;
                }
            }
            if(zeroCount > minCount){
                minCount = zeroCount;
                tempLin = *lin;
                tempCol = *col;
            }
        }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ganhou = TRUE;
            zeroCount = 0;
            for(int k = 0; k < n; k++){
                if(tab[i][k][j] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[i][k][j] == 0 && (j == 0 ||tab[i][k][j-1] != 0)){
                    zeroCount++;
                    *lin = i;
                    *col = k;
                    
                }
            }
            if(zeroCount > minCount){
                minCount = zeroCount;
                tempLin = *lin;
                tempCol = *col;
            }
        }
    }

    //Checando as 4 diagonais do cubo
    int startLin = 0;
    int startCol = 0;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 && (i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    
                }
        startLin++;
        startCol++;
    }
    if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

    startLin = 0;
    startCol = n-1;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 && (i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    
                }
        startLin++;
        startCol--;
    }
    if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

    startLin = n-1;
    startCol = 0;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 && (i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    
                }
        startLin--;
        startCol++;
    }
    if(zeroCount > minCount){
            minCount = zeroCount;
            tempLin = *lin;
            tempCol = *col;
        }

    startLin = n-1;
    startCol = n-1;

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        if(tab[startLin][startCol][i] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[startLin][startCol][i] == 0 && (i == 0 ||tab[startLin][startCol][i-1] != 0)){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    
                }
        startLin--;
        startCol--;
    }
    if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

    //Checando diagonais das  9 faces
    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[i][j][k] == 0 && (k == 0 ||tab[i][j][k-1] != 0)){
                    zeroCount++;
                    *lin = i;
                    *col = j;
    
                }
            j++;
            k++;
        }
        if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[i][j][k] == 0 && (k == 0 ||tab[i][j][k-1] != 0)){
                    zeroCount++;
                    *lin = i;
                    *col = j;
                
                }
            j++;
            k--;
        }
        if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[j][i][k] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[j][i][k] == 0 && (k == 0 ||tab[j][i][k-1] != 0)){
                    zeroCount++;
                    *lin = j;
                    *col = i;
                    
                }
            j++;
            k++;
        }
        if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[j][i][k] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[j][i][k] == 0 && (k == 0 ||tab[j][i][k-1] != 0)){
                    zeroCount++;
                    *lin = j;
                    *col = i;
                    
                }
            j++;
            k--;
        }
        if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }
    }

    ganhou = TRUE;
    zeroCount = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        ganhou = TRUE;
        zeroCount = 0;
        for(int a = 0; a < n; a++){
            if(tab[k][j][i] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[k][j][i]  == 0 && (i == 0 ||tab[k][j][i-1] != 0)){
                    zeroCount++;
                    *lin = k;
                    *col = j;
                    
                }
            j++;
            k++;
        }
        if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }

        ganhou = TRUE;
        zeroCount = 0;
        j = 0;
        k = n-1;
        for(int a = 0; a < n; a++){
            if(tab[k][j][i] == -1 *cor){
                    ganhou = FALSE;
                    zeroCount = 0;
                    break;
                }
                if(tab[k][j][i]  == 0 && (i == 0 ||tab[k][j][i-1] != 0)){
                    zeroCount++;
                    *lin = k;
                    *col = j;
                    
                }
            j++;
            k--;
        }
        if(zeroCount > minCount){
        minCount = zeroCount;
        tempLin = *lin;
        tempCol = *col;
    }
    }
    if(minCount == 0) return FALSE;
    *lin = tempLin;
    *col = tempCol;
    return TRUE;
}

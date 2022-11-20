#include <stdio.h>
#include <stdlib.h>
#define preta 1
#define azul -1
#define FALSE 0
#define TRUE 1

int escolheJogada (int *** tab, int n, int cor, int *lin, int *col);
int getZ(int *** tab, int n, int *lin, int *col);
int checaGanhador(int *** tab, int n, int cor);
int podeGanhar(int ***tab, int n, int cor, int *lin, int *col);
int jogarDiagonal(int ***tab, int n, int cor, int *lin, int *col);

int main(){
    int n;
    int ***tab;
    int jogador;
    int pc;
    int fim = FALSE;
    int vez = preta;
    int lin;
    int col;
    int z;

    printf("Digite o valor de N: ");
    scanf("%d", &n);

    /* Criando o tabuleiro */
    /* tab[x][y][z] */
    tab = malloc(sizeof(int**)* n);
    for(int i = 0; i < n; i++){
        tab[i] = malloc(sizeof(int*)* n);
        for(int j = 0; j < n; j++)
            tab[i][j] = malloc(sizeof(int) * n);
    }

    printf("Você quer jogar como Pretas[1] ou Azuis[-1]? ");
    scanf("%d", &jogador);
    pc = -1 * jogador;

    int count = 0;
    while(!fim){
        count++;
        if(vez == jogador){
            //Perguntar o Carlinhos se o user vai colocar de 0 a n-1 ou de 1 a n
            //Tenho que considerar jogadas inválidas?
            printf("Digite a linha[x] e a coluna[y] que deseja jogar: ");
            scanf("%d %d", &lin, &col);
            z = getZ(tab, n, &lin, &col);
            tab[lin][col][z] = jogador;
        }
        else{
            z = escolheJogada(tab, n , pc, &lin, &col);
            if(z == n)
                fim = TRUE;
            else{
                tab[lin][col][z] = pc;
                printf("Jogou %d %d \n", lin, col);
            }
        }
        fim = checaGanhador(tab,n, vez);
        if(fim) printf("Fim do jogo!\n");
        vez *= -1;
    
    }
    printf("\n%d", tab[0][0][0]);
    printf("\n%d", tab[0][0][1]);

    return 0;

}

int getZ(int *** tab, int n,  int *lin, int *col){
    int i = 0;
    //printf("\n%d %d", *lin, *col);
    while(tab[*lin][*col][i] != 0)
        i++;
    return i;
}

int escolheJogada (int *** tab, int n, int cor, int *lin, int *col){
    int cantosX[4] = {0, 0, n-1, n-1};
    int cantosY[4] = {0, n-1, 0, n-1};
    
    //checar se tem n³ bolinhas
    //Se puder ganhar , ganhe
    if(podeGanhar(tab, n, cor,lin, col))
        return getZ(tab, n, lin, col);
    if(podeGanhar(tab, n, -1 * cor ,lin, col)){
        printf("Vai ganhar não cara\n");
        return getZ(tab, n, lin, col);}
    
    //Se o oponente for ganhar, impessa
    //Se n for impar, jogar na posição central
    if(n%2 == 1){
        if(tab[n/2][n/2][0] == 0){
            *lin = n/2;
            *col = n/2;
            return getZ(tab, n, lin, col);
        }
    }
    
    //Checando os cantos livres não bloqueados na diagonal
    if(tab[0][0][0] == 0 && tab[n-1][n-1][0] == 0){
        *lin = 0;
        *col = 0;
        return getZ(tab, n, lin, col);
    }
    if(tab[0][n-1][0] == 0 && tab[n-1][0][0] == 0){
        *lin = 0;
        *col = n-1;
        return getZ(tab, n, lin, col);
    }

    if(jogarDiagonal(tab, n, cor,lin, col))
        return getZ(tab, n, lin, col);
        
    //checando cantos livres
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
    
    
    
    printf("Não consegui escolher uma jogada :(\n");
    return n;
}

int jogarDiagonal(int ***tab, int n, int cor, int *lin, int *col){
    //Checando as 4 diagonais do cubo
    int startLin = 1;
    int startCol = 1;
    int ganhou = FALSE;
    printf("Vish1\n");
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
                if(tab[i][j][k] == 0){
                    zeroCount++;
                    *lin = i;
                    *col = j;
                    if(zeroCount > 1){
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
                    if(zeroCount > 1){
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
                if(tab[i][k][j] == 0){
                    zeroCount++;
                    *lin = i;
                    *col = k;
                    if(zeroCount > 1){
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
                    if(zeroCount > 1){
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
                if(tab[startLin][startCol][i] == 0){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    if(zeroCount > 1){
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
                    if(zeroCount > 1){
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
                if(tab[startLin][startCol][i] == 0){
                    zeroCount++;
                    *lin = startLin;
                    *col = startCol;
                    if(zeroCount > 1){
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
                    if(zeroCount > 1){
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
                    if(zeroCount > 1){
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
                if(tab[j][i][k] == 0){
                    zeroCount++;
                    *lin = j;
                    *col = i;
                    if(zeroCount > 1){
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
                    if(zeroCount > 1){
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
                if(tab[k][j][i]  == 0){
                    zeroCount++;
                    *lin = k;
                    *col = j;
                    if(zeroCount > 1){
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
                    if(zeroCount > 1){
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

int checaGanhador(int *** tab, int n, int cor){
    //Checando Rows
    //printf("Rows1\n");
    int ganhou = TRUE;
    
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
    
    //printf("Rows2\n");
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
    //printf("Rows3\n");
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

    //Checando as 4 diagonais do cubo
    //printf("Diagonais do cubo\n");
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

    //Checando diagonais das  9 faces
    //printf("Diagonais da face\n");
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
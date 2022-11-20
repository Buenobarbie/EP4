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
            printf("check");
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
    //checar se tem n³ bolinhas
    //Se puder ganhar , ganhe
    if(podeGanhar(tab, n, cor,lin, col))
        return getZ(tab, n, lin, col);
    if(podeGanhar(tab, n, -1 * cor ,lin, col))
        return getZ(tab, n, lin, col);
    
    //Se o oponente for ganhar, impessa
    //Se n for impar, jogar na posição central
    if(n%2 == 1){
        if(tab[n/2+1][n/2+1][0] == 0){
            *lin = n/2 + 1;
            *col = n/2 + 1;
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
    return n;
}

int podeGanhar(int ***tab, int n, int cor, int *lin, int *col){
    //Checando Rows
    int ganhou = TRUE;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n-1; k++){
                if(tab[i][j][k] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou && tab[i][j][n-1] == 0){
                *lin = i;
                *col = j;
                return ganhou;
            }
        }
    }

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n-1; k++){
                if(tab[k][i][j] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou && tab[n-1][i][j] == 0){
                *lin = n-1;
                *col = i;
                return ganhou;
            }
        }
    }

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n -1; k++){
                if(tab[i][k][j] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou && tab[i][n-1][j] == 0){
                *lin = i;
                *col = n-1;
                return ganhou;
            }
        }
    }

    //Checando as 4 diagonais do cubo
    int startLin = 0;
    int startCol = 0;

    ganhou = TRUE;
    for(int i = 0; i < n-1; i++){
        if(tab[startLin][startCol][i] != cor){
            ganhou = FALSE;
            break;
        }
        startLin++;
        startCol++;
    }
    if(ganhou && tab[startLin + 1][startCol + 1][n -1] == 0){
                *lin = startLin + 1;
                *col = startCol + 1;
                return ganhou;
    }

    startLin = 0;
    startCol = n-1;

    ganhou = TRUE;
    for(int i = 0; i < n - 1; i++){
        if(tab[startLin][startCol][i] != cor){
            ganhou = FALSE;
            break;
        }
        startLin++;
        startCol--;
    }
    if(ganhou && tab[startLin + 1][startCol - 1][n -1] == 0){
                *lin = startLin + 1;
                *col = startCol - 1;
                return ganhou;
    }

    startLin = n-1;
    startCol = 0;

    ganhou = TRUE;
    for(int i = 0; i < n-1; i++){
        if(tab[startLin][startCol][i] != cor){
            ganhou = FALSE;
            break;
        }
        startLin--;
        startCol++;
    }
    if(ganhou && tab[startLin - 1][startCol + 1][n -1] == 0){
                *lin = startLin - 1;
                *col = startCol + 1;
                return ganhou;
    }

    startLin = n-1;
    startCol = n-1;

    ganhou = TRUE;
    for(int i = 0; i < n -1; i++){
        if(tab[startLin][startCol][i] != cor){
            ganhou = FALSE;
            break;
        }
        startLin--;
        startCol--;
    }
    if(ganhou && tab[startLin - 1][startCol - 1][n -1] == 0){
                *lin = startLin - 1;
                *col = startCol - 1;
                return ganhou;
    }

    //Checando diagonais das  9 faces
    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        for(int a = 0; a < n; a++){
            if(tab[i][j][k]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k++;
        }
        if(ganhou && tab[i][j+1][k+1] == 0){
                *lin = i;
                *col = j+1;
                return ganhou;
        }

        ganhou = TRUE;
        j = 0;
        k = n-1;
        for(int a = 0; a < n-1; a++){
            if(tab[i][j][k]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k--;
        }
        if(ganhou && tab[i][j+1][k-1] == 0){
                *lin = i;
                *col = j+1;
                return ganhou;
        }
    }

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        for(int a = 0; a < n -1; a++){
            if(tab[j][i][k]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k++;
        }
        if(ganhou && tab[j+1][i][k+1] == 0){
                *lin = j+1;
                *col = i;
                return ganhou;
        }

        ganhou = TRUE;
        j = 0;
        k = n-1;
        for(int a = 0; a < n -1; a++){
            if(tab[j][i][k]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k--;
        }
        if(ganhou && tab[j+1][i][k-1] == 0){
                *lin = j+1;
                *col = i;
                return ganhou;
        }
    }

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
        for(int a = 0; a < n-1; a++){
            if(tab[k][j][i]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k++;
        }
        if(ganhou && tab[k+1][j+1][i] == 0){
                *lin = k+1;
                *col = j+1;
                return ganhou;
        }

        ganhou = TRUE;
        j = 0;
        k = n-1;
        for(int a = 0; a < n -1; a++){
            if(tab[k][j][i]!= cor){
                ganhou = FALSE;
                break;
            }
            j++;
            k--;
        }
        if(ganhou && tab[k-1][j+1][i] == 0){
                *lin = k-1;
                *col = j+1;
                return ganhou;
        }
    }
    return ganhou;
}
int checaGanhador(int *** tab, int n, int cor){
    //Checando Rows
    int ganhou = TRUE;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                if(tab[i][j][k] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou) return ganhou;
        }
    }

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                if(tab[k][i][j] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou) return ganhou;
        }
    }

    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                if(tab[i][k][i] != cor){
                    ganhou = FALSE;
                    break;
                }
            }
            if(ganhou) return ganhou;
        }
    }

    //Checando as 4 diagonais do cubo
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
    ganhou = TRUE;
    for(int i = 0; i < n; i++){
        int j = 0;
        int k = 0;
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
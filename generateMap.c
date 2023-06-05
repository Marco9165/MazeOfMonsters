#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>


void generateMap(STATE *st, char map[st->height][st->width], float probParede, int primeiraPassagem, int segundaPassagem) {
//primeiraPassagem - 1 a 3 
//segundaPassagem - 0 a 2

    // Gera paredes aleatórias no mapa com base no valor de probabilidade
    // Preenche o mapa com paredes nas bordas e com base na probabilidade
    for (int r = 0; r < st->height; r++) { //percorre todas linhas
        for (int c = 0; c < st->width; c++) { //percorre todas colunas
            if (r == 0 || r == st->height - 1 || c == 0 || c == st->width - 1) { // as bordas são sempre paredes
                map[r][c] = *WALL; 
            } else {
                map[r][c] = (drand48() < probParede) ? *WALL : *FLOOR; //Se drand48() (entre 0 e 1) < probParede for verdadeira, ou seja, se o número aleatório for menor 0, pois a probabilidade é 0.4, o local do mapa atual fica como parede se não fica um chão
            }
        }
    }

    // Cada passagem analisa cada célula do mapa e conta o número de paredes nas células vizinhas
    // Aplica a primeira passagem - Ela analisa cada célula do mapa e verifica a quantidade de paredes ao seu redor. 
    char map1[st->height][st->width];
    for (int i = 0; i < primeiraPassagem; i++) {
        for (int r = 1; r < st->height - 1; r++) {
            for (int c = 1; c < st->width - 1; c++) {
                int numWalls1 = 0;
                int numWalls2 = 0;
                // Conta o número de paredes nas células vizinhas
                for (int rr = r - 1; rr <= r + 1; rr++) {
                    for (int cc = c - 1; cc <= c + 1; cc++) {
                        if (map[rr][cc] == *WALL) {
                            numWalls1++;
                        }
                    }
                }
                for (int rr = r - 2; rr <= r + 2; rr++) {
                    for (int cc = c - 2; cc <= c + 2; cc++) {
                        if (map[rr][cc] == *WALL) {
                            numWalls2++;
                        }
                    }
                }
                // Aplica as regras de criação de paredes e espaços vazios
                if (numWalls1 >= 5 || numWalls2 <= 2) { //Se o número de paredes vizinhas (ao redor) for maior ou igual a 5 ou o número de paredes em uma área mais ampla ao redor da célula for menor ou igual a 2 a celula fica como parede se nao fica como chao
                    map1[r][c] = *WALL;
                } else {
                    map1[r][c] = *FLOOR;
                }
            }
        }
        // Atualiza o mapa com as alterações da primeira passagem
        for (int r = 1; r < st->height - 1; r++) {
            for (int c = 1; c < st->width - 1; c++) {
                map[r][c] = map1[r][c];
            }
        }
    }

    // Aplica a segunda passagem
    char map2[st->height][st->width];
    for (int i = 0; i < segundaPassagem; i++) {
        for (int r = 1; r < st->height - 1; r++) {
            for (int c = 1; c < st->width - 1; c++) {
                int numWalls = 0;
                // Conta o número de paredes nas células vizinhas
                for (int rr = r - 1; rr <= r + 1; rr++) {
                    for (int cc = c - 1; cc <= c + 1; cc++) {
                        if (map[rr][cc] == *WALL) {
                            numWalls++;
                        }
                    }
                }
                // Aplica as regras de criação de paredes e espaços vazios
                // Se o número de paredes vizinhas for maior ou igual a 5, a célula atual será uma parede. Caso contrário, será um espaço vazio.
                if (numWalls >= 5) {
                    map2[r][c] = *WALL;
                } else {
                    map2[r][c] = *FLOOR;
                }
            }
        }
        // Atualiza o mapa com as alterações da segunda passagem
        for (int r = 1; r < st->height - 1; r++) {
            for (int c = 1; c < st->width - 1; c++) {
                map[r][c] = map2[r][c];
            }
        }
    }
}

void generateCoins(STATE *st, char map[st->height][st->width], int numCoins){
    for (int i = 0; i < numCoins; i++){
        // Gerar aleatoriamente a posição da moeda
        int x = rand() % st->width; //posição X
        int y = rand() % st->height; //posição Y

        // Verifica se a célula no mapa é um espaço vazio antes de colocar uma moeda
        if (map[y][x] == *FLOOR) {
            map[y][x] = *COIN; // Coloca uma moeda na célula
        }

    }
}

void generateKeyAndFinish(STATE *st, char map[st->height][st->width]) { 
    for (int i = 0; i < st->numKeys; i++){
    // Gerar aleatoriamente a posição da chave (*KEY)
    int keyX = rand() % st->width; //posição X
    int keyY = rand() % st->height; //posição X

    while (map[keyY][keyX] != *FLOOR){ // Encontra uma posição vazia para a chave
        keyX = rand() % st->width;
        keyY = rand() % st->height;
    }

        map[keyY][keyX] = *KEY; // Coloca a chave na posição encontrada e valida
    }
    // Gerar a posição da porta numa das bordas do mapa
    int doorSide = rand() % 4; // Lado da porta: 0 = superior, 1 = inferior, 2 = esquerda, 3 = direita

    int doorX = 0;
    int doorY = 0;

    switch (doorSide) {
        case 0: // Superior
            doorX = rand() % st->width;
            doorY = 0;
            break;
        case 1: // Inferior
            doorX = rand() % st->width;
            doorY = st->height - 1;
            break;
        case 2: // Esquerda
            doorX = 0;
            doorY = rand() % st->height;
            break;
        case 3: // Direita
            doorX = st->width - 1;
            doorY = rand() % st->height;
            break;
    }

    // Cria acesso à porta (sem obstáculos) se não houver   
    if (doorSide == 0){
        for (int r = doorY + 1; r < st->height-1; r++) {
            if (map[r][doorX] == *WALL) {
                map[r][doorX] = *FLOOR;
            } else{
                break;
            }
        }
    } else if(doorSide == 1){
        for (int r = doorY - 1; r > 0; r--) {
            if (map[r][doorX] == *WALL) {
                map[r][doorX] = *FLOOR;
            } else{
                break;
            }
        }
    } else if(doorSide == 2){
        for (int r = doorX + 1; r < st->width; r++) {
            if (map[doorY][r] == *WALL) {
                map[doorY][r] = *FLOOR;
            } else{
                break;
            }
        }
    } else if(doorSide == 3){
        for (int r = doorX - 1; r > 0; r--) {
            if (map[doorY][r] == *WALL) {
                map[doorY][r] = *FLOOR;
            } else{
                break;
            }
        }
    }

    map[doorY][doorX] = *DOOR_LOCK;  // Coloca a porta de saída no mapa
}


void updateDoorLock(STATE *st, char map[st->height][st->width]) {
    // Percorre o mapa para encontrar a posição da porta (*DOOR_LOCK)
    // e verifica se o número de chaves coletadas é igual ao número total de chaves (st->numKeys)
    for (int r = 0; r < st->height; r++) {
        for (int c = 0; c < st->width; c++) {
            // Se encontrar a porta trancada e todas as chaves foram coletadas
            if (map[r][c] == *DOOR_LOCK && st->key == st->numKeys) {
                // Substitui a porta trancada pelo estado de porta destrancada 
                map[r][c] = *DOOR_UNLOCK;
            }
        }
    }
}

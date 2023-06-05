#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "dictionary.h"


void generateMonsters(STATE *st, STATEmonster stMonsters[NUM_MONSTERS], STATEmonster *monsters, char map[st->height][st->width]) {
    srand(time(NULL)); // inicializa o gerador de números aleatórios

    // Gera aleatoriamente a posição dos monstros no mapa
    for (int i = 0; i < st->numMonsters; i++) {
        int x = rand() % st->width;
        int y = rand() % st->height;

        int random = (rand() % NUM_MONSTERS);

        // Encontra uma posição vazia (*FLOOR) no mapa para o monstro, excluindo a posição do jogador
        while (map[y][x] != *FLOOR || (x == st->playerX && y == st->playerY)) {
            x = rand() % st->width;
            y = rand() % st->height;
        }

        if (map[y][x] == *FLOOR && (x != st->playerX || y != st->playerY)) {
            // Atribui as propriedades do monstro a partir do array stMonsters (monstros existentes no jogo)
            monsters[i].id = stMonsters[random].id;
            monsters[i].monsterX = x;
            monsters[i].monsterY = y;
            monsters[i].hp = stMonsters[random].hp;
            monsters[i].distance = stMonsters[random].distance;
            monsters[i].damage = stMonsters[random].damage;

            map[y][x] = monsters[i].id; // atualiza o mapa com a posição do monstro
        }
    }
}


void moveMonsters(STATE *st, STATEmonster *monsters, char map[st->height][st->width]) {
    int playerX = st->playerX;
    int playerY = st->playerY;

    // Move cada monstro no mapa 1 a 1
    for (int i = 0; i < st->numMonsters; i++) {
        int monsterX = monsters[i].monsterX;
        int monsterY = monsters[i].monsterY;
        int dx = playerX - monsterX;
        int dy = playerY - monsterY;
        int distance = sqrt(dx * dx + dy * dy);

        int attack = rand() % 4; //se for diferente 0 vai attack se for 0 nao vai perseguir

        if (distance <= 10 && attack != 0) {
            // O monstro está a menos de 10 blocos de distância do jogador, então ele o persegue
            int newX = monsterX;
            int newY = monsterY;

            if (dx < 0 && dy < 0) {  // Movimentos diagonais
                newX -= 1; // Move para a diagonal superior esquerda
                newY -= 1;
            } else if (dx < 0 && dy > 0) {
                newX -= 1; // Move para a diagonal inferior esquerda
                newY += 1;
            } else if (dx > 0 && dy < 0) {
                newX += 1; // Move para a diagonal superior direita
                newY -= 1;
            } else if (dx > 0 && dy > 0) {
                newX += 1; // Move para a diagonal inferior direita
                newY += 1;
            } else if (dx < 0) {
                newX -= 1; // Move para a esquerda
            } else if (dx > 0) {
                newX += 1; // Move para a direita
            } else if (dy < 0) {
                newY -= 1; // Move para cima
            } else if (dy > 0) {
                newY += 1; // Move para baixo
            }

            // Verifica se a nova posição está dentro dos limites do mapa e é um piso vazio (*FLOOR) e não coincide com a posição do jogador
            if (newX >= 0 && newX < st->width && newY >= 0 && newY < st->height && map[newY][newX] == *FLOOR && !(newX == st->playerX && newY == st->playerY)) {
                map[monsters[i].monsterY][monsters[i].monsterX] = *FLOOR; // atualiza o mapa, a antiga posição do monstro posição do monstro volta a ser chão
                monsters[i].monsterX = newX;
                monsters[i].monsterY = newY;
                map[monsters[i].monsterY][monsters[i].monsterX] = monsters[i].id; // Atualiza o mapa com a nova posição do monstro
            }
        } else {
            // Caso contrário, o monstro se move aleatoriamente
            int randomDirection = rand() % 4; // Escolhe uma direção aleatória (0-3)

            int newX = monsterX;
            int newY = monsterY;

            // Movimentos nas direções escolhidas aleatoriamente
            switch (randomDirection) {
                case 0:
                    newX -= 1; // Move para a esquerda
                    break;
                case 1:
                    newX += 1; // Move para a direita
                    break;
                case 2:
                    newY -= 1; // Move para cima
                    break;
                case 3:
                    newY += 1; // Move para baixo
                    break;
                default:
                    break;
            }

            // Verifica se a nova posição está dentro dos limites do mapa e é um piso vazio (*FLOOR) e não coincide com a posição do jogador
            if (newX >= 0 && newX < st->width && newY >= 0 && newY < st->height && map[newY][newX] == *FLOOR && !(newX == st->playerX && newY == st->playerY)) {
                map[monsters[i].monsterY][monsters[i].monsterX] = *FLOOR; // atualiza o mapa com a posição anterior do monstro
                monsters[i].monsterX = newX;
                monsters[i].monsterY = newY;
                map[monsters[i].monsterY][monsters[i].monsterX] = monsters[i].id; // atualiza o mapa com a nova posição do monstro
            }
        }
    }
}


void attackPlayer(STATE *st, STATEmonster *monsters) { //monstro ataca player
    int playerX = st->playerX;
    int playerY = st->playerY;
    
    // Verifica se o jogador está dentro do raio de ataque de algum monstro
    for (int i = 0; i < st->numMonsters; i++) {
        int monsterX = monsters[i].monsterX;
        int monsterY = monsters[i].monsterY;
        int distance = monsters[i].distance;
        int damage = monsters[i].damage;
        
        // Calcula a distância entre o jogador e o monstro
        float distToPlayer = sqrt(pow(playerX - monsterX, 2) + pow(playerY - monsterY, 2));
        
        // Verifica se o jogador está dentro do raio de ataque do monstro
        if (distToPlayer <= distance) {
            // O monstro ataca o jogador
            if (st->armor > 0) {
                if (damage >= st->armor) {
                    // O dano é maior ou igual ao valor da armor, ajustar a armor para zero
                    damage -= st->armor;
                    st->armor = 0;
                } else {
                    // O dano é menor do que o valor da armor, subtrair o dano da armor
                    st->armor -= damage;
                    damage = 0; // Não resta dano para subtrair da vida
                }
            }

            // Subtrair o dano restante da hp
            st->hp -= damage;

            // Verificar se a hp é menor que 0 e ajustá-la para 0, se necessário
            if (st->hp < 0) {
                st->hp = 0;
            }
        }
    }
}



int findNearestMonster(STATE* st, STATEmonster *monsters, STATEweapons currentWeapon, char map[st->height][st->width]) { //encontrar o monstro mais perto do player
    int nearestIndex = -1; // Índice do monstro mais próximo (-1 se nenhum monstro encontrado)
    int weaponRange = currentWeapon.distance; // Alcance da arma atual do jogador
    int minDistance = currentWeapon.distance + 1; // Distância mínima inicializada com um valor maior que o alcance da arma

    // Loop pelos monstros existentes no jogo
    for (int i = 0; i < st->numMonsters; i++) {
        int monsterX = monsters[i].monsterX;
        int monsterY = monsters[i].monsterY;
        int distance = abs(monsterX - st->playerX) + abs(monsterY - st->playerY);

        // Verifica se o monstro está dentro do alcance da arma ou adjacente ao jogador
        if ((distance <= weaponRange || (distance == 1 && monsterX == st->playerX && monsterY == st->playerY)) &&
            distance < minDistance) {

            // Verifica se há uma parede entre o monstro e o jogador
            int dx = abs(st->playerX - monsterX);
            int dy = abs(st->playerY - monsterY);
            int sx = (monsterX < st->playerX) ? 1 : -1;
            int sy = (monsterY < st->playerY) ? 1 : -1;
            int err = dx - dy;

            while (monsterX != st->playerX || monsterY != st->playerY) {
                if (map[monsterY][monsterX] == *WALL) {
                    // Parede encontrada, monstro não é detectado
                    break;
                }

                int e2 = 2 * err;
                if (e2 > -dy) {
                    err -= dy;
                    monsterX += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    monsterY += sy;
                }
            }

            // Se não houve parede, atualiza o índice do monstro mais próximo
            if (monsterX == st->playerX && monsterY == st->playerY) {
                minDistance = distance;
                nearestIndex = i;
            }
        }
    }

    return nearestIndex; // Retorna o índice do monstro mais próximo (-1 se nenhum monstro encontrado)
}

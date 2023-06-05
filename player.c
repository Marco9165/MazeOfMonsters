#include "dictionary.h"

void findXY(STATE *st, STATEmonster stMonsters[NUM_MONSTERS], char map[st->height][st->width]) { //encontra posição valida para o player nascer
    int checkMonster = 0; // Variável para verificar se há um monstro próximo à posição gerada
    int x, y;

    do {
        // Gera um valor aleatório dentro da largura do mapa
        x = rand() % st->width;
        y = rand() % st->height;
        for (int i = 0; i < NUM_MONSTERS; i++){
            if (map[y][x-1] == stMonsters[i].id || map[y][x+1] == stMonsters[i].id || map[y-1][x] == stMonsters[i].id || map[y+1][x] == stMonsters[i].id || map[y-1][x-1] == stMonsters[i].id || map[y-1][x+1] == stMonsters[i].id || map[y+1][x-1] == stMonsters[i].id || map[y+1][x+1] == stMonsters[i].id){
                checkMonster = 1; // Define a variável checkMonster como 1 se houver um monstro próximo
            }
        }
    } while (map[y][x] != *FLOOR || map[y][x-1] == *WALL || map[y][x+1] == *WALL || map[y-1][x] == *WALL || map[y+1][x] == *WALL || map[y-1][x-1] == *WALL || map[y-1][x+1] == *WALL || map[y+1][x-1] == *WALL || map[y+1][x+1] == *WALL || checkMonster == 1);
    // Repete o processo enquanto a posição gerada não for um espaço vazio no mapa (FLOOR), ou seja, enquanto houver paredes (WALL) ou monstros próximos
    
    //atribui as coordenadas do jogador
    st->playerX = x;
    st->playerY = y;
}

void printPlayer(STATE *st){ //Imprime o personagem na posição atual do jogo.
    if (st->menu == *GAME){
        attron(COLOR_PAIR(12));
        mvprintw(st->playerY, st->playerX, PLAYER); // Imprime o caractere do personagem na posição (st->playerY, st->playerX)
        attroff(COLOR_PAIR(12));
    } 
}

void removeMonster(STATE *st, STATEmonster *monsters, char map[st->height][st->width]) { //remover monstro quando o mesmo morre
    for (int i = 0; i < st->numMonsters; i++) {
        if (monsters[i].hp <= 0) {
            // Obtenha as coordenadas do monstro a ser removido
            int monsterX = monsters[i].monsterX;
            int monsterY = monsters[i].monsterY;

            // Altere o local onde o monstro estava para *FLOOR
            map[monsterY][monsterX] = *FLOOR;

            // Remova o monstro do vetor
            for (int j = i; j < st->numMonsters - 1; j++) {
                monsters[j] = monsters[j+1];
            }
            (st->numMonsters)--;
            i--; // Para verificar novamente o elemento atual após a remoção
        }
    }
}


void playerAttack(STATE *st, char map[st->height][st->width], STATEmonster *monsters, STATEweapons currentWeapon, int indexMonster) { //jogador ataca monstro clicando na tecla a
    // Verifica se o índice do monstro é válido (-1 indica nenhum monstro selecionado)
    if (indexMonster != -1) {
        int areaRadius = currentWeapon.area;
        
        // Aplica o dano ao monstro selecionado
        monsters[indexMonster].hp -= currentWeapon.damage;
         
        // Percorre os monstros próximos dentro do raio da área de ataque
        for (int i = 0; i < st->numMonsters; i++) {
            if (i == indexMonster) {
                continue; // Ignora o monstro selecionado anteriormente
            }
            
            int dx = monsters[i].monsterX - monsters[indexMonster].monsterX;
            int dy = monsters[i].monsterY - monsters[indexMonster].monsterY;
            int distance = sqrt(dx*dx + dy*dy);
            
            if (distance <= areaRadius) {
                // Aplica o dano em área aos monstros próximos
                monsters[i].hp -= currentWeapon.damage;
            }
        }
        
        removeMonster(st, monsters, map); //chama a função para remomver o monstro da lista
    }
}







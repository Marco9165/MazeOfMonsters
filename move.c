#include "dictionary.h"

// Verifica se há uma arma nas coordenadas especificadas (x, y) no mapa
int verifyWeapon(STATE *st, char map[st->height][st->width], int x, int y, STATEweapons stWeapons[]){
    for (int i = 0; i < NUM_WEAPONS; i++){
        if(map[st->playerY+y][st->playerX+x] == stWeapons[i].id){
            return 1;
        }
    }
    return 0;
}

// Verifica se há uma poção nas coordenadas especificadas (x, y) no mapa
int verifyPotion(STATE *st, char map[st->height][st->width], int x, int y, STATEpotions stPotions[]){
    for (int i = 0; i < NUM_POTIONS; i++){
        if(map[st->playerY+y][st->playerX+x] == stPotions[i].id){
            return 1;
        } 
    }
    return 0;
}

// Verifica se o movimento nas coordenadas (x, y) é válido
int verifyMove(STATE *st, char map[st->height][st->width], int x, int y, STATEweapons stWeapons[], STATEpotions stPotions[]){
    if(map[st->playerY+y][st->playerX+x] == *FLOOR ||
       map[st->playerY+y][st->playerX+x] == *DOOR_UNLOCK  ||
       map[st->playerY+y][st->playerX+x] == *KEY  ||
       map[st->playerY+y][st->playerX+x] == *COIN ||
       verifyWeapon(st, map, x, y, stWeapons) ||
       verifyPotion(st, map, x, y, stPotions)
        ){
        return 1; 
    } 
    return 0;
}


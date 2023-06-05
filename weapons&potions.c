#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "dictionary.h"


void generateWeapons(STATE *st, STATEweapons stWeapons[NUM_WEAPONS], STATEweapons *weapons, int numWeapons, char map[st->height][st->width]) {
    for (int i = 0; i < numWeapons; i++){
        // Gera coordenadas aleatórias para a posição da arma
        int x = rand() % st->width;
        int y = rand() % st->height;
        int random = 0;

        // Escolhe aleatoriamente uma arma para ser gerada
        while (random == 0){
            random = (rand() % NUM_WEAPONS);
        }

        // Verifica se a posição gerada é válida (piso vazio)
        while (map[y][x] != *FLOOR){
            x = rand() % st->width;
            y = rand() % st->height;
        }
        
        // Se a posição for válida, gera a arma no mapa
        if (map[y][x] == *FLOOR){
            // Atribui os valores da arma gerada ao vetor de armas
            weapons[i].id = stWeapons[random].id;
            weapons[i].weaponX = x;
            weapons[i].weaponY = y;
            weapons[i].distance = stWeapons[random].distance;
            weapons[i].damage = stWeapons[random].damage;
            weapons[i].area = stWeapons[random].area;

            map[y][x] = weapons[i].id; // atualiza o mapa com a posição do monstro
        } 
    }
}


void generatePotions(STATE *st, STATEpotions stPotions[NUM_POTIONS], STATEpotions *potions, int numPotions, char map[st->height][st->width]) {
    for (int i = 0; i < numPotions; i++){
        // Gera coordenadas aleatórias para a posição da poção
        int x = rand() % st->width;
        int y = rand() % st->height;
        int random = 0;

        // Escolhe aleatoriamente uma poção para ser gerada
        while (random == 0){
            random = (rand() % NUM_POTIONS);
        }
        
        // Verifica se a posição gerada é válida (piso vazio)
        while (map[y][x] != *FLOOR){
            x = rand() % st->width;
            y = rand() % st->height;
        }
        
        // Se a posição for válida, gera a poção no mapa
        if (map[y][x] == *FLOOR){
            potions[i].id = stPotions[random].id;
            potions[i].potionX = x;
            potions[i].potionY = y;

            map[y][x] = potions[i].id; // atualiza o mapa com a posição da poção
        } 
    }
}

void weaponEquipped(STATE *st, STATEweapons stWeapons[NUM_WEAPONS], STATEweapons *currentWeapon){
    for (int i = 0; i < NUM_WEAPONS; i++) {
        if (st->weapon == stWeapons[i].id) {
            // Atribui as propriedades da arma atualmente equipada ao objeto currentWeapon
            currentWeapon->id = stWeapons[i].id;
            currentWeapon->weaponX = 0;
            currentWeapon->weaponY = 0;
            currentWeapon->distance = stWeapons[i].distance;
            currentWeapon->damage = stWeapons[i].damage;
            currentWeapon->area = stWeapons[i].area;
            currentWeapon->bullets = stWeapons[i].bullets;

            break; // Interrompe o loop após encontrar a arma correspondente
        }
    }
}


void bulletsCheck(STATE *st, STATEweapons *currentWeapon) {
    // Verifica se a arma atual tem balas restantes e se não é uma arma corpo a corpo (HAND, KNIFE, SWORD)
    if (currentWeapon->bullets != 0 && st->weapon != *HAND && st->weapon != *KNIFE && st->weapon != *SWORD) {
        // Reduz a quantidade de balas em 1
        currentWeapon->bullets -= 1;
        
        // Verifica se acabaram as balas da arma atual
        if (currentWeapon->bullets == 0) {
            // Define a arma do jogador como HAND (sem arma)
            st->weapon = *HAND;
        }
    }
}

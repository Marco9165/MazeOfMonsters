#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "dictionary.h"

#include "generateMap.c"
#include "vision.c"
#include "monstros.c"
#include "move.c"
#include "weapons&potions.c"
#include "windows.c"
#include "player.c"

extern MENU menu[];
extern int num_options;

void game(void) {
    clear(); // limpa a janela
    srand(time(NULL)); // inicializa o gerador de números aleatórios
    
  //STAT game = {width, height, playerX, playery, hp, armor, armor, weapon,effect, vision, coins, key, menu}
    // estado inicial do jogador
    STATE game = {COLS-21, LINES-1, 0, 0, 100, 100, 'H', 'N', 15, 0, 0, 'G', 0, 0}; 
    
    // Estado inicial da arma atual do jogador
    STATEweapons currentWeapon = {*HAND, 0, 0, 1, 2, 1, 0};

    // Índice do monstro mais próximo, inicialmente definido como -1, ou seja sem nenhum monstro perto
    int nearestMonster = -1;

    // Array dos monstros existentes no jogo
    STATEmonster stMonsters[NUM_MONSTERS] = {{*RAT, 0   , 0, 30, 1, 2},
                                            {*SPIDER, 0, 0, 50, 2, 3},
                                            {*PYTHON, 0, 0, 75, 1, 4},
                                            {*ZOMBIE, 0, 0, 100, 1, 5},
                                            {*DRAGON, 0, 0, 125, 4, 10},
                                            {*TREX, 0, 0, 150, 3, 15}};

    // Array das armas existentes no jogo
    STATEweapons stWeapons[NUM_WEAPONS] = {{*HAND, 0, 0, 1, 2, 1, 0},
                                           {*KNIFE, 0, 0, 1, 10, 1, 0},
                                           {*SWORD, 0, 0, 2, 15, 1, 0},
                                           {*PISTOL, 0, 0, 5, 25, 1, 15},
                                           {*RIFLE, 0, 0, 15, 35, 1, 25},
                                           {*GRANEDE, 0, 0, 10, 20, 3, 5}};

    // Array das poções existentes no jogo
    STATEpotions stPotions[NUM_POTIONS] = {{*NONE, 0, 0},
                                          {*VISION, 0, 0},
                                          {*STRENGTH, 0, 0},
                                          {*RESISTENCE, 0, 0},
                                          {*CURE, 0, 0}}; 

    int key; // Variável para armazenar uma tecla pressionada
    int selected = 0; // Índice do elemento selecionado
    
    // Define o número de monstros/poções/armas/coins/keys aleatoriamente com base nas dimensões do jogo
    game.numMonsters = rand() % ((game.width * game.height) / 150) + 5;
    STATEmonster monsters[game.numMonsters];
    int probWeapons = rand() % ((game.width * game.height) / 100) + 5;
    STATEweapons weapons[probWeapons];
    int probPotions = rand() % ((game.width * game.height) / 100) + 5;
    STATEpotions potions[probPotions];
    int probCoins = rand() % ((game.width * game.height) / 20);
    game.numKeys = rand() % 5 + 1;


    // gera e desenha o mapa aleatório
    char map[game.height][game.width];
    int visited[game.height][game.width];

    // Inicializa o array visited com zeros que futuramente vai ser alterado com 1 conforme é visitado
    for (int r = 0; r < game.height; r++) {
        for (int c = 0; c < game.width; c++) {
                visited[r][c] = 0; 
        }
    }

    generateMap(&game, map, 0.4, 3, 2); // Gerar o mapa aleatório usando a função generateMap com 40% de paredes
    generateWeapons(&game, stWeapons, weapons, probWeapons, map); // Gerar as armas no mapa usando a função generateWeapons
    generateMonsters(&game, stMonsters, monsters, map); // Gerar os monstros no mapa usando a função generateMonsters
    generatePotions(&game, stPotions, potions, probPotions, map); // Gerar as poções no mapa usando a função generatePotions
    generateKeyAndFinish(&game, map); // Gerar a chave e a posição de saída no mapa usando a função generateKeyAndFinish
    generateCoins(&game, map, probCoins); // Gera as moedas no mapa usando a função generateCoins

    findXY(&game, stMonsters, map); // Encontra as posições dos monstros no mapa usando a função findXY
    drawVision(&game, monsters, weapons, probWeapons, potions, probPotions, map, 0, visited); // Desenha o mapa com visão limitada usando a função drawVision
    
    printPlayer(&game); // Desenha o jogador na posição inicial
    printSTAT(&game, monsters, nearestMonster, &currentWeapon); // Mostra as estatísticas do jogador, monstros e arma atual usando a função printSTAT

    refresh(); //Atualiza a tela
    
    time_t effectActivatedTime = 0; // Inicializa a variável para armazenar o tempo em que o efeito foi ativado

    // loop principal do jogo
    while ((key = tolower(getch())) != 'q') { // sai do jogo se pressionar 'q' tolower para 'Q' ser igual a 'q'
        switch(key) {
            case KEY_UP:
                // Movimento para cima no jogo ou seleção para cima no menu de compras
                if (game.menu == *GAME && verifyMove(&game, map, 0, -1, stWeapons, stPotions)){
                    game.playerY--;                    
                } else if (game.menu == *SHOP){
                    selected--;
                    if(selected < 0) {
                        selected = num_options - 1;
                    }
                }
                break;
            case KEY_DOWN:
                // Movimento para baixo no jogo ou seleção para baixo no menu de compras
                if (game.menu == *GAME && verifyMove(&game, map, 0, 1, stWeapons, stPotions)) {
                    game.playerY++;
                } else if (game.menu == *SHOP){
                   selected++;
                    if (selected >= num_options) {
                        selected = 0;
                    }
                }
                break;
            case KEY_LEFT:
                // Movimento para a esquerda no jogo
                if (game.menu == *GAME && verifyMove(&game, map, -1, 0, stWeapons, stPotions)) {
                    game.playerX--;
                }
                break;
            case KEY_RIGHT:
                // Movimento para a direita no jogo
                if (game.menu == *GAME && verifyMove(&game, map, 1, 0, stWeapons, stPotions)) {
                    game.playerX++;
                }
                break;
            case 'e':  
                // Equipamento no jogo
                if (game.menu == *GAME && map[game.playerY][game.playerX] == *KEY){
                    game.key += 1;
                    map[game.playerY][game.playerX] = *FLOOR;
                } else if(game.menu == *GAME && (verifyPotion(&game, map, 0, 0, stPotions) || verifyWeapon(&game, map, 0, 0, stWeapons))){
                    if (!verifyWeapon(&game, map, 0, 0, stWeapons)){
                        if (map[game.playerY][game.playerX] == *STRENGTH && game.effect != *STRENGTH){
                            currentWeapon.damage *= 2;
                        }
                        game.effect = map[game.playerY][game.playerX];
                        map[game.playerY][game.playerX] = *FLOOR;
                        effectActivatedTime = time(NULL); // Armazena o tempo em que o efeito foi ativado
                    } else {
                        game.weapon = map[game.playerY][game.playerX];
                        map[game.playerY][game.playerX] = *FLOOR;
                        weaponEquipped(&game, stWeapons, &currentWeapon);
                    }
                } else if (game.menu == *SHOP){
                    game.menu = *GAME; //se tiver na loja vai fechar
                    effectActivatedTime = time(NULL); // Armazena o tempo em que o jogadar sai da loja
                }
                break;
            case 's': 
                // Abre a loja no jogo
                if (game.menu == *GAME){
                    game.menu = *SHOP; //abrir loja, muda o game.menu para SHOP e depois na drawVision vai mudar
                }
                break;
            case 'w':
                map[game.playerY][game.playerX] = *STRENGTH;
                break;
            case 'b':
                // Compra um item na loja
                if (game.menu == *SHOP){
                    if (game.coins - list[selected].price < 0 || (selected == 0 && game.effect == *STRENGTH) || (selected == 1 && game.armor == 100) || (selected == 3 && game.hp == 100) || (selected == 4 && game.vision == 100)){ //se nao tiver moedas suficientes ou algum efeito já ativo do mesmo efeito ou hp ou armor no maximo...
                            break;
                    } else {
                        game.coins -= list[selected].price;
                    }

                    //todos casos na loja ao comprar
                    if (selected == 0) {
                        game.effect = *STRENGTH;
                        currentWeapon.damage += currentWeapon.damage; //duplica o efeito 
                        effectActivatedTime = time(NULL)+999999999999999;
                    } else if (selected == 1) {
                        game.armor += 10;
                        effectActivatedTime = time(NULL)+999999999999999; 
                    } else if(selected == 2){
                        game.effect = *VISION;
                        effectActivatedTime = time(NULL)+999999999999999; 
                    } else if(selected == 3){
                        game.hp = 100;
                    } else if(selected == 4){
                        game.vision += 5;
                    } else if(selected == 5){
                        game.weapon = *KNIFE;
                    } else if(selected == 6){
                        game.weapon = *SWORD;
                    } else if(selected == 7){
                        game.weapon = *PISTOL;
                    } else if(selected == 8){
                        game.weapon = *RIFLE;
                    } else if(selected == 9){
                        game.weapon = *GRANEDE;
                    } else if(selected == 10){
                    }
                    weaponEquipped(&game, stWeapons, &currentWeapon);
                }
                break;
            case 'a':
                // Ataca um monstro no jogo
                playerAttack(&game, map, monsters, currentWeapon, nearestMonster);
                bulletsCheck(&game, &currentWeapon);
                if (currentWeapon.bullets == 0){
                    weaponEquipped(&game, stWeapons, &currentWeapon);    
                }
               break;
            default:
                break;
        }

        //se o player passar por cima de uma coisa recebe automaticamente
        if (map[game.playerY][game.playerX] == *COIN){
            game.coins += 1; 
            map[game.playerY][game.playerX] = *FLOOR; //local antigo da moeda passa para chao ou seja a moeda desaparece
        }
        if (map[game.playerY][game.playerX] == *DOOR_UNLOCK){ //se o jogador "calcar" a porta aberta ele ganha o jogo e vai para a janela de vitoria que é desenhada na função win
            win();
            break;
        }
        if (game.hp <= 0){ //se o jogador morrer ou seja tiver 0 ou menos de vida, o jogo acaba e vai para a janela de derrota que é desenhada na função defeat
            defeat();
            break;
        }
        
        clear();
        if (game.menu == *GAME){ //se estiver no jogo atualiza os mobs se estiver na loja os mosntros nao se movem nem atacam
            moveMonsters(&game, monsters, map);
            attackPlayer(&game, monsters);
            nearestMonster = findNearestMonster(&game, monsters, currentWeapon, map);
        }
        
        updateDoorLock(&game, map);
         
        drawVision(&game, monsters, weapons, probWeapons, potions, probPotions, map, selected, visited); // Redesenha o mapa com visão limitada usando a função drawVision
        printPlayer(&game); // Redesenha o jogador na posição inicial
        printSTAT(&game, monsters, nearestMonster, &currentWeapon); // Mostra as estatísticas do jogador, monstros e arma atual usando a função printSTAT

        refresh(); //atualiza

        //Código para verificar se o efeito ainda está ativo e há mais de 5 segundos desde que foi ativado
        if (game.effect == *VISION && time(NULL) - effectActivatedTime >= 3) {
            game.effect = *NONE; // Desativa o efeito
        }else if (game.effect == *STRENGTH && time(NULL) -effectActivatedTime >= 5) {
            currentWeapon.damage /= 2;
            game.effect = *NONE; // Desativa o efeito
        } else if (game.effect == *CURE) {
            game.hp = 100;
            game.effect = *NONE; // Desativa o efeito
        } else if (game.effect == *RESISTENCE){
            game.armor += 10;
            game.effect = *NONE;
        }
        
    }
}

void mapa(void){
    clear();
    STATE main = {COLS-1, LINES-1, 0, 0, 0, 0, 'H', 'N', 0, 0, 0, 'M', 0, 0}; // posição inicial do jogador
    
    char map[main.height][main.width];

    clear();
    generateMap(&main, map, 0.4, 3, 2);
    drawMap(&main, map);
    mvprintw(LINES-1, COLS/2-11, "RANDOMLY GENERATED MAP");
    refresh();
    char ch;
    while ((ch = getch()) != 'q');
}

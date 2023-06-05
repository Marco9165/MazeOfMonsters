#include <ncurses.h>
#include <math.h>

#include "dictionary.h"

MENU list[] = {
    {"5 sec with double of STRENGTH", 2, "Potions"},
    {"+10% armor", 5, "Potions"},
    {"100% of VISION for 3 sec", 7, "Potions"},
    {"CURE 100% your life", 10, "Potions"},
    {"+5% VISION", 14, "Potions"},
    {"KNIFE", 10, "Weapons"},
    {"SWORD", 13, "Weapons"},
    {"PISTOL", 15, "Weapons"},
    {"RIFLE", 17, "Weapons"},
    {"GRANEDE", 20, "Weapons"},
    {"FULL AMMO", 15, "Weapons"},
};
int num_options = 11;

void drawMap(STATE *st, char map[st->height][st->width]){
    // Percorre as linhas do mapa
    for (int r = 0; r < st->height; r++) {
        // Percorre as colunas do mapa
        for (int c = 0; c < st->width; c++) {
            // Imprime o caractere correspondente à posição atual do mapa
            addch(map[r][c]);
        }
        // Imprime um caractere de nova linha para passar para a próxima linha do mapa
        addch('\n'); 
    }
}


void drawVision(STATE *st, STATEmonster *monsters, STATEweapons *weapons, int numWeapons, STATEpotions *potions, int numPotions, char map[st->height][st->width], int selected, int visited[st->height][st->width]) {
    if (st->menu == *GAME){
            int diagonal = round(sqrt(pow(st->height, 2) + pow(st->width, 2)));
            int raio_visivel = round(diagonal/2 * (st->vision/100.0));

        if (st->effect == *VISION){
            drawMap(st,map);
        } else {
            // Desenha a visão do jogador e os elementos visíveis no mapa
            for (int r = 0; r < st->height; r++) {
                for (int c = 0; c < st->width; c++) {
                    attron(COLOR_PAIR(10));
                    if (visited[r][c] == 1){
                        if(map[r][c] == *WALL){
                            mvaddch(r, c, *WALL); 
                        } else if(map[r][c] == *KEY){
                            mvaddch(r, c, *KEY); 
                        } else if(map[r][c] == *DOOR_LOCK){
                            mvaddch(r, c, *DOOR_LOCK); 
                        } else if(map[r][c] == *DOOR_UNLOCK){
                            mvaddch(r, c, *DOOR_UNLOCK); 
                        } else {
                            mvaddch(r, c, *FLOOR); 
                        }
                    attroff(COLOR_PAIR(10));
                    }
                }
            }
            // Desenha a visão circular ao redor do jogador
            for(double angulo = 0; angulo < 360; angulo++) { // Loop para percorrer os ângulos de 0 a 359 graus
                for(int dist = 0; dist <= raio_visivel; dist++) { // Loop para percorrer as distâncias dentro do raio visível
                    int dx = round(dist * cos(angulo * PI / 180));  // Cálculo do deslocamento horizontal com base no ângulo e distância
                    int dy = round(dist * sin(angulo * PI / 180)); // Cálculo do deslocamento vertical com base no ângulo e distância
                    int x = st->playerX + dx; // Posição x do ponto na matriz do mapa
                    int y = st->playerY+ dy; // Posição y do ponto na matriz do mapa

                    if(x < 0 || x >= st->width || y < 0 || y >= st->height) {
                        break; // Sai do loop de distância se estiver fora dos limites do mapa
                    }
                    
                    visited[y][x] = 1; // Marca a posição como visitada

                    //ESTRUTURA DO MAPA
                    attron(COLOR_PAIR(13));
                    if(map[y][x] == *WALL){ // Verifica se a posição contém uma parede
                        mvaddch(y, x, *WALL);
                        break; // Sai do loop de distância, pois a parede bloqueia a visão
                    }
                    
                    if(map[y][x] == *FLOOR) {
                        mvaddch(y, x, *FLOOR); 
                    }
                    attroff(COLOR_PAIR(13));

                    attron(COLOR_PAIR(17));
                    if (map[y][x] == *DOOR_UNLOCK){
                        mvaddch(y, x, *DOOR_UNLOCK); 
                    }

                    if (map[y][x] == *KEY){
                        mvaddch(y, x, *KEY); 
                    }
                    attroff(COLOR_PAIR(17));


                    //MONSTERS
                    attron(COLOR_PAIR(11));
                    for (int i = 0; i < st->numMonsters; i++){
                        if (map[y][x] == monsters[i].id){
                            mvaddch(y, x, monsters[i].id);
                        }
                    }
                    
                    if (map[y][x] == *DOOR_LOCK){
                        mvaddch(y, x, *DOOR_LOCK); 
                    }
                    attroff(COLOR_PAIR(11));

                    //WEAPONS
                    attron(COLOR_PAIR(15));
                    for (int i = 0; i < numWeapons; i++){
                        if(map[y][x] == weapons[i].id) {
                            mvaddch(y, x, weapons[i].id);
                        }
                    }
                    attroff(COLOR_PAIR(15));

                    //POTIONS
                    attron(COLOR_PAIR(16));
                    for (int i = 0; i < numPotions; i++){
                        if(map[y][x] == potions[i].id) {
                            mvaddch(y, x, potions[i].id);
                        }
                    }
                    attroff(COLOR_PAIR(16));

                    //coins
                    attron(COLOR_PAIR(12));
                    if (map[y][x] == *COIN){
                        mvaddch(y, x, *COIN); 
                    }
                    attroff(COLOR_PAIR(12));
                    
                    }
                }
            refresh();
        }
    } else if (st->menu == *SHOP){
        // Desenha a tela da loja
        clear();
        attron(A_BOLD);
        attron(COLOR_PAIR(1));
        mvprintw(2, 2, " __        __   _                            _          _   _            ____ _____ ___  ____  _____ _    ");
        mvprintw(3, 2, " \\ \\      / /__| | ___ ___  _ __ ___   ___  | |_ ___   | |_| |__   ___  / ___|_   _/ _ \\|  _ \\| ____| |   ");
        mvprintw(4, 2, "  \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  | __| '_ \\ / _ \\ \\___ \\ | || | | | |_) |  _| | |   ");
        mvprintw(5, 2, "   \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |_| | | |  __/  ___) || || |_| |  _ <| |___|_|   ");
        mvprintw(6, 2, "    \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/   \\__|_| |_|\\___| |____/ |_| \\___/|_| \\_\\_____(_)   ");
        attroff(COLOR_PAIR(1));
        attroff(A_BOLD);

        int weapons_start = 10; //onde começa a weapons
        int potions_start = 10;
        for(int i = 0; i < num_options; i++) {
            if(strcmp(list[i].category, "Weapons")) {
                if(i == selected) {
                    attron(A_REVERSE);
                }
                mvprintw(weapons_start, 3, "%s:", list[i].option);
                attroff(A_REVERSE);
                attron(COLOR_PAIR(2)); 
                printw(" %d$", list[i].price);
                attroff(COLOR_PAIR(2)); 
                weapons_start++;
            } else if(strcmp(list[i].category, "Potions")) {
                if(i == selected) {
                    attron(A_REVERSE);
                }
                mvprintw(potions_start, 40, "%s:", list[i].option);
                attroff(A_REVERSE);
                attron(COLOR_PAIR(2)); 
                printw(" %d$", list[i].price);
                attroff(COLOR_PAIR(2)); 
                potions_start++;
            }
        }

        attron(A_BOLD);
        attron(COLOR_PAIR(2));
        mvprintw(9, 2, "Potions:");
        mvprintw(9, 39, "Weapons:");
        attroff(COLOR_PAIR(2));
        attroff(A_BOLD);

        refresh();
    }
    
}

void printSTAT(STATE *st, STATEmonster *monsters, int nearestMonster, STATEweapons *currentWeapon) { //imprime a barra de status na lateral
    // Definição de uma estrutura para mapear as entidades do jogo
    typedef struct {
        char id;
        char* description;
    } Entity;

    // Dicionário que associa IDs a descrições das entidades do jogo
    Entity dictionary[] = {
        {'H', "HAND"},
        {'K', "KINFE"},
        {'E', "SWORD"},
        {'U', "PISTOL"},
        {'M', "RIFLE"},
        {'G', "GRANEDE"},
        {'R', "RAT"},
        {'S', "SPIDER"},
        {'P', "PYTHON"},
        {'Z', "ZOMBIE"},
        {'D', "DRAGON"},
        {'T', "TREX"},
        {'N', "NONE"},
        {'V', "VISION"},
        {'F', "STRENGTH"},
        {'A', "RESISTENCE"},
        {'C', "CURE"}
    };
    
    // Desenha a barra lateral esquerda
    attron(COLOR_PAIR(2));
    for (int i = 0; i <= st->height-1; i++) {
        mvprintw(i, st->width + 2, "|");
    }
    attroff(COLOR_PAIR(2));

    if (st->menu == *GAME){
        // Informações personalizadas na barra lateral
        attron(COLOR_PAIR(3));
        mvprintw(1, st->width + 4, "PLAYER POSITION:");
        mvprintw(2, st->width + 5, "(%d,%d)", st->playerX, st->playerY);
        mvprintw(4, st->width + 4, "HEALTH: ");
        // Define a cor do texto de acordo com a percentagem de vida do jogador
            if (st->hp <=  100 / 3) {
                attron(COLOR_PAIR(1));
            } else if (st->hp <= 100 * 2 / 3) {
                attron(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(7));
            }
        mvprintw(5, st->width + 5, "%d%%", st->hp);
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(7));


        mvprintw(7, st->width + 4, "ARMOR: ");
            // Define a cor do texto de acordo com a percentagem de armadura do jogador
            if (st->armor <= 100 / 3) {
                attron(COLOR_PAIR(1));
            } else if (st->armor <= 100 * 2 / 3) {
                attron(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(7));
            }
        mvprintw(8, st->width + 5, "%d%%", st->armor);
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(7));


        mvprintw(10, st->width + 4, "WEAPON: ");
            attron(COLOR_PAIR(5));
        // Procura no dicionário a descrição da arma
        for (int i = 0; i < 17; i++) {
            if (dictionary[i].id == st->weapon) {
                mvprintw(11, st->width + 5, "%s", dictionary[i].description);
                break;
            }
        }
            attroff(COLOR_PAIR(5));

        mvprintw(13, st->width + 4, "DAMAGE: ");
            attron(COLOR_PAIR(1));
        mvprintw(14, st->width + 5, "%d", currentWeapon->damage);
            attroff(COLOR_PAIR(1));
        mvprintw(16, st->width + 4, "AMMO: ");
            attron(COLOR_PAIR(1));
        mvprintw(17, st->width + 5, "%d", currentWeapon->bullets);
            attroff(COLOR_PAIR(1));


        mvprintw(19, st->width + 4, "EFFECT: ");
            attron(COLOR_PAIR(6));
        for (int i = 0; i < 17; i++) {
            if (dictionary[i].id == st->effect) {
                mvprintw(20, st->width + 5, "%s", dictionary[i].description);
                break;
            }
        }
            attroff(COLOR_PAIR(6));

        mvprintw(22, st->width + 4, "COINS: ");
            attron(COLOR_PAIR(2));
        mvprintw(23, st->width + 5, "%d", st->coins);
            attroff(COLOR_PAIR(2));

        mvprintw(25, st->width + 4, "KEY: ");
            if (st->key == 0) {
                attron(COLOR_PAIR(1));
            } else if (st->key == st->numKeys) {
                attron(COLOR_PAIR(7));
            } else {
                attron(COLOR_PAIR(2));
            }
        mvprintw(26, st->width + 5, "%d/%d", st->key, st->numKeys);
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(7));


            attron(COLOR_PAIR(2));
        mvprintw(28, st->width + 3, "------------------");
            attroff(COLOR_PAIR(2));

        mvprintw(30, st->width + 4, "MONSTERS ALIVE: ");
            attron(COLOR_PAIR(1));
        mvprintw(31, st->width + 5, "%d", st->numMonsters);
            attroff(COLOR_PAIR(1));
        if (nearestMonster!=-1){
            mvprintw(33, st->width + 4, "POINT AT: ");
            for (int i = 0; i < 17; i++) {
                if (dictionary[i].id == monsters[nearestMonster].id) {
                        attron(COLOR_PAIR(1));
                    mvprintw(34, st->width + 5, "%s", dictionary[i].description);
                        attroff(COLOR_PAIR(1));
                break;
                }
            }
            mvprintw(36, st->width + 5, "LIFE: %d",  monsters[nearestMonster].hp);
        }
        
        attroff(COLOR_PAIR(3));
    }else if (st->menu == *SHOP) {
            // Informações personalizadas na barra lateral para o menu de compras
            attron(COLOR_PAIR(3));
            // Título do menu
            mvprintw(2, st->width + 4, "SHOP MENU");
                attron(COLOR_PAIR(2));
            mvprintw(3, st->width + 3, "------------------");
                attroff(COLOR_PAIR(2));
            mvprintw(4, st->width + 4, "BUY: press b");
            mvprintw(5, st->width + 4, "EXIT: press e");
            attroff(COLOR_PAIR(3));
            attron(COLOR_PAIR(3));
            mvprintw(6, st->width + 4, "COINS: %d", st->coins);
            attroff(COLOR_PAIR(3));
    }
}

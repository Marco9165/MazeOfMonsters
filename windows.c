#include <ncurses.h>

void commands(void) {
    int x_mid = COLS / 2;
    int y_mid = LINES / 2 + 8;

    clear();
    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    mvprintw(y_mid - 25, x_mid-45, "    __  ______ _____   ______   ____  ______   __  _______  _   _______________________  _____");
    mvprintw(y_mid - 24, x_mid-45, "   /  |/  /   /__  /  / ____/  / __ \\/ ____/  /  |/  / __ \\/ | / / ___/_  __/ ____/ __ \\/ ___/");
    mvprintw(y_mid - 23, x_mid-45, "  / /|_/ / /| | / /  / __/    / / / / /_     / /|_/ / / / /  |/ /\\__ \\ / / / __/ / /_/ /\\__ \\");
    mvprintw(y_mid - 22, x_mid-45, " / /  / / ___ |/ /__/ /___   / /_/ / __/    / /  / / /_/ / /|  /___/ // / / /___/ _, _/___/ /");
    mvprintw(y_mid - 21, x_mid-45, "/_/  /_/_/  |_/____/_____/   \\____/_/      /_/  /_/\\____/_/ |_//____//_/ /_____/_/ |_|/____/");
    attroff(COLOR_PAIR(1)); 
    attroff(A_BOLD);

    mvprintw(y_mid - 19, x_mid - 32, "In Maze Of Monsters, your goal is to survive in a cave facing monsters");
    mvprintw(y_mid - 18, x_mid - 32, "and many challenges. Use weapons, potions and strategy to find the key");
    mvprintw(y_mid - 17, x_mid - 32, "so you can unlock the door, escape the map, and win the game.");


    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    mvprintw(y_mid - 14, x_mid - 10, "CONTROLS");
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);

    mvprintw(y_mid - 13, x_mid - 17, "Arrow up    -> MOVE UP");
    mvprintw(y_mid - 12, x_mid - 17, "Arrow left  -> MOVE LEFT");
    mvprintw(y_mid - 11, x_mid - 17, "Arrow right -> MOVE RIGHT");
    mvprintw(y_mid - 10, x_mid - 17, "Arrow down  -> MOVE DOWN");
    mvprintw(y_mid - 9 , x_mid - 17, "E           -> EQUIP");
    mvprintw(y_mid - 8 , x_mid - 17, "S           -> SHOP");
    mvprintw(y_mid - 7 , x_mid - 17, "A           -> ATTACK");
    mvprintw(y_mid - 6 , x_mid - 17, "Q           -> QUIT");


    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    mvprintw(y_mid - 2, x_mid - 39, "WEAPONS");
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);


    attron(COLOR_PAIR(2));
    mvprintw(y_mid - 1, x_mid - 38, "MELLE");
    attroff(COLOR_PAIR(2));


    mvprintw(y_mid, x_mid - 40, "H         -> HAND");
    mvprintw(y_mid + 1, x_mid - 40, "K         -> KNIFE");
    mvprintw(y_mid + 2, x_mid - 40, "E         -> SWORD");


    attron(COLOR_PAIR(2));
    mvprintw(y_mid + 3, x_mid - 38, "RANGED");
    attroff(COLOR_PAIR(2));


    mvprintw(y_mid + 4, x_mid - 40, "U         -> PISTOL");
    mvprintw(y_mid + 5, x_mid - 40, "M         -> RIFLE");
    mvprintw(y_mid + 6, x_mid - 40, "G         -> GRENADES");

    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    mvprintw(y_mid + 8, x_mid - 39, "EFFECTS (POTIONS)");
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);
    
    mvprintw(y_mid + 9, x_mid - 40, "N         -> NONE");
    mvprintw(y_mid + 10, x_mid - 40, "V         -> VISION");
    mvprintw(y_mid + 11, x_mid - 40, "F         -> STRENGTH");
    mvprintw(y_mid + 12, x_mid - 40, "A         -> ARMOR");
    mvprintw(y_mid + 13, x_mid - 40, "C         -> CURE");

    attron(A_BOLD); 
    attron(COLOR_PAIR(1));
    mvprintw(y_mid - 2, x_mid + 25, "MAP");
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);

    mvprintw(y_mid - 1, x_mid + 24, "#         -> WALL");
    mvprintw(y_mid    , x_mid + 24, ".         -> FLOOR");
    mvprintw(y_mid + 1, x_mid + 24, "+         -> CLOSED DOOR");
    mvprintw(y_mid + 2, x_mid + 24, "-         -> OPEN DOOR");
    mvprintw(y_mid + 3, x_mid + 24, "*         -> KEY");
    mvprintw(y_mid + 4, x_mid + 24, "$         -> COIN");


    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    mvprintw(y_mid + 6, x_mid + 25, "MONSTERS");
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);

    mvprintw(y_mid + 7 , x_mid + 24, "R         -> RAT");
    mvprintw(y_mid + 8 , x_mid + 24, "S         -> SPIDER");
    mvprintw(y_mid + 9 , x_mid + 24, "P         -> PYTHON");
    mvprintw(y_mid + 10, x_mid + 24, "Z         -> ZOMBIE");
    mvprintw(y_mid + 11, x_mid + 24, "D         -> DRAGON");
    mvprintw(y_mid + 12, x_mid + 24, "T         -> T-REX");


    refresh();
    char ch;
    while ((ch = getch()) != 'q');
}


void win(void){
    // O jogador ganhou/completou o jogo
    clear();
    int win_height = 5; // Altura da palavra "WIN"
    int win_top = (LINES - win_height) / 2;

    attron(COLOR_PAIR(7));

    // Imprimindo a palavra "WIN" no centro da tela
    mvprintw(win_top    , (COLS - 23) / 2, " _       _______   __");
    mvprintw(win_top + 1, (COLS - 23) / 2, "| |     / /  _/ | / /");
    mvprintw(win_top + 2, (COLS - 23) / 2, "| | /| / // //  |/ /");
    mvprintw(win_top + 3, (COLS - 23) / 2, "| |/ |/ // // /|  /");
    mvprintw(win_top + 4, (COLS - 23) / 2, "|__/|__/___/_/ |_/");

    attroff(COLOR_PAIR(7));
    mvprintw(win_top + win_height + 2, (COLS-25) / 2, "Press ENTER to leave");

    char ch;
    while ((ch = getch()) != '\n');
}

void defeat(void){
    clear();
    int defeat_height = 5; // Altura da palavra "DEFEAT"
    int defeat_top = (LINES - defeat_height) / 2;


    attron(COLOR_PAIR(1));

    // Imprimindo a palavra "DEFEAT" no centro da tela
    mvprintw(defeat_top    , (COLS - 38) / 2, "    ____  _____________________  ______");
    mvprintw(defeat_top + 1, (COLS - 38) / 2, "   / __ \\/ ____/ ____/ ____/   |/_  __/");
    mvprintw(defeat_top + 2, (COLS - 38) / 2, "  / / / / __/ / /_  / __/ / /| | / /");
    mvprintw(defeat_top + 3, (COLS - 38) / 2, " / /_/ / /___/ __/ / /___/ ___ |/ /");
    mvprintw(defeat_top + 4, (COLS - 38) / 2, "/_____/_____/_/   /_____/_/  |_/_/");

    attroff(COLOR_PAIR(1));
    mvprintw(defeat_top + defeat_height + 2, (COLS - 25) / 2, "Press ENTER to leave ");

    char ch;
    while ((ch = getch()) != '\n');
}

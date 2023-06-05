#include <ncurses.h>
#include <string.h>

#include "jogo.c"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// comando -> gcc menu.c -o Roguelite -lm -lcurses -Wall -Wextra -pedantic -O2
// ./Roguelite
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

int main(void) {
    initscr(); // Inicializa a biblioteca ncurses
    cbreak(); // Habilita o modo de leitura de caracteres imediatos
    noecho(); // desativa a exibição da entrada do usuário
    cbreak();
    curs_set(FALSE); // define a visibilidade do cursor - neste caso esta desativado
    keypad(stdscr, TRUE);

   
    // Verifica se o tamanho atual é menor que o tamanho mínimo
    if (LINES < MIN_HEIGHT || COLS < MIN_WIDTH) {
        endwin(); // Encerra a biblioteca ncurses
        printf("O tamanho mínimo da janela não foi atingido, aumente um pouco mais o tamanho da janela.\n");
        return 1;
    }

    // Define as opções do menu
    const char* opcoes[] = {"   Start   ", "    Map    ", "How to play" ,"    Exit   "};
    int num_opcoes = 4;

    // Define as cores do menu
    start_color(); // inicia o uso de cores
    //(numero da cor, cor letra, cor fundo)
    init_pair(1, COLOR_RED, COLOR_BLACK); //monstros
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); //personagem
    init_pair(3, COLOR_WHITE, COLOR_BLACK); //menu
    init_pair(4, COLOR_BLACK, COLOR_WHITE); //menu
    init_pair(5, COLOR_CYAN, COLOR_BLACK); //armas
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); //potions
    init_pair(7, COLOR_GREEN, COLOR_BLACK);

    //CORES MAPA
    //init_color - criar cor
    //init_pair - criar par de cor (numero, cor de fundo, cor da letra)
    init_color(8, 300, 300, 300); 
    init_color(9, 169, 169, 169); 
    init_pair(10, 8, 9);    //já visitado
    init_pair(11, 91, 8); 
    init_pair(12, 92, 8);
    init_pair(13, 93, 8);
    init_pair(15, 95, 8);
    init_pair(16, 96, 8);
    init_pair(17, 97, 8);
    init_color(91, 900, 0, 0);
    init_color(92, 900, 900, 0);
    init_color(93, 900, 900, 900);
    init_color(95, 0, 900, 900);
    init_color(96, 900, 0, 900);
    init_color(97, 0, 900, 0);

    // Obtém o tamanho da janela do terminal
    int linhas = LINES;
    int colunas = COLS;


    // Calcula a posição central do menu
    int x_menu = (colunas - 36) / 2;
    int y_menu = (linhas - num_opcoes * 2) / 2;

    // Imprime o título do menu
    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    mvprintw(y_menu - 2, x_menu-35, "    __  ______ _____   ______   ____  ______   __  _______  _   _______________________  _____");
    mvprintw(y_menu - 1, x_menu-35, "   /  |/  /   /__  /  / ____/  / __ \\/ ____/  /  |/  / __ \\/ | / / ___/_  __/ ____/ __ \\/ ___/");
    mvprintw(y_menu    , x_menu-35, "  / /|_/ / /| | / /  / __/    / / / / /_     / /|_/ / / / /  |/ /\\__ \\ / / / __/ / /_/ /\\__ \\");
    mvprintw(y_menu + 1, x_menu-35, " / /  / / ___ |/ /__/ /___   / /_/ / __/    / /  / / /_/ / /|  /___/ // / / /___/ _, _/___/ /");
    mvprintw(y_menu + 2, x_menu-35, "/_/  /_/_/  |_/____/_____/   \\____/_/      /_/  /_/\\____/_/ |_//____//_/ /_____/_/ |_|/____/");
    attroff(COLOR_PAIR(1)); 
    attroff(A_BOLD);

    // Imprime o menu na posição central
    for (int i = 0; i < num_opcoes; i++) {
        if (i == 0) {
            attron(COLOR_PAIR(4));
        } else {
            attron(COLOR_PAIR(3));
        }
        mvprintw(y_menu+i+i+4, x_menu, " %s ", opcoes[i]);
        attroff(COLOR_PAIR(3));
        attroff(COLOR_PAIR(4));
    }

    // Lê a opção escolhida pelo usuário
    int escolha = 0;
    while (1) {
        int c = getch();
        switch (c) {
            case KEY_UP:
                escolha--;
                if (escolha < 0) {
                    escolha = num_opcoes - 1;
                }
                break;
            case KEY_DOWN:
                escolha++;
                if (escolha >= num_opcoes) {
                    escolha = 0;
                }
                break;

            case '\n':
                if (escolha == 0) {
                    //Começar Jogo
                    game();
                } else if (escolha == 1) {
                    //Ver mapa
                    mapa();
                } else if(escolha == 2){
                    //Ver comandos
                    commands();
                } else if(escolha == 3){
                    // Sai do jogo
                    endwin();
                    return 0;
                }
                break;
            default:
                break;
        }
        clear();

        // Imprime o título do menu
        attron(A_BOLD);
        attron(COLOR_PAIR(1));
        mvprintw(y_menu - 2, x_menu-35, "    __  ______ _____   ______   ____  ______   __  _______  _   _______________________  _____");
        mvprintw(y_menu - 1, x_menu-35, "   /  |/  /   /__  /  / ____/  / __ \\/ ____/  /  |/  / __ \\/ | / / ___/_  __/ ____/ __ \\/ ___/");
        mvprintw(y_menu    , x_menu-35, "  / /|_/ / /| | / /  / __/    / / / / /_     / /|_/ / / / /  |/ /\\__ \\ / / / __/ / /_/ /\\__ \\");
        mvprintw(y_menu + 1, x_menu-35, " / /  / / ___ |/ /__/ /___   / /_/ / __/    / /  / / /_/ / /|  /___/ // / / /___/ _, _/___/ /");
        mvprintw(y_menu + 2, x_menu-35, "/_/  /_/_/  |_/____/_____/   \\____/_/      /_/  /_/\\____/_/ |_//____//_/ /_____/_/ |_|/____/");
        attroff(COLOR_PAIR(1));
        attroff(A_BOLD);

        // Atualiza o menu com a opção selecionada
        for (int i = 0; i < num_opcoes; i++) {
            if (i == escolha) {
                attron(COLOR_PAIR(4));
            } else {
                attron(COLOR_PAIR(3));
            }
            mvprintw(y_menu+i+i+4, x_menu, " %s ", opcoes[i]);
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(4));
        }   
    }

    // Finaliza a biblioteca ncurses
    endwin();

    return 0;
}

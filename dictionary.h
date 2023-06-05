#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED

#define PI 3.14159265

#define MIN_HEIGHT 40
#define MIN_WIDTH 105


//Menu/display
#define GAME "G"
#define SHOP "S"

//Definir a estrutura do menu da loja
typedef struct menu {
    char* option;
    int price;
    char* category;
} MENU;



//Mapa
#define WALL "#"
#define FLOOR "."
#define DOOR_LOCK "+"
#define DOOR_UNLOCK "-"
#define KEY "*" //chave para abrir porta final
#define COIN "$" 


//Jogador
#define PLAYER "@"

//Definir a estrutura do menu do jogo
typedef struct stGame{

    //map stat
    int width; //COLS-1 = largura
    int height; //LINES-1 = altura
    //player stat
    int playerX;
    int playerY;
    int hp; //0-100
    int armor; //0-100
    char weapon;
    char effect; //efeito jogador possui, exemplo mais visao, mais vida, mais força
    int vision; //percentagem visao do jogador 0-100
    int coins;
    int key;
    char menu;

    int numMonsters;
    int numKeys;
} STATE;


//Armas
#define NUM_WEAPONS 6

//Definir a estrutura das armas
typedef struct stWeapons{
    char id;
    int weaponX;
    int weaponY;
    int distance;
    int damage;
    int area;
    int bullets;
} STATEweapons;

    //Corpo a Corpo
#define HAND "H"    //distancia = 1     dano = 2       area = 1
#define KNIFE "K"   //distancia = 1    dano = 10      area = 1
#define SWORD "E"   //distancia = 2    dano = 15      area = 2

    //Distancia
#define PISTOL "U"     //distancia = 5         dano = 25      area = 1
#define RIFLE "M"   //distancia = 15      dano = 35      area = 1
#define GRANEDE "G" //distancia = 10    dano = 20      area = 5

//Monstros
//Definir a estrutura dos monstros
typedef struct stMonster{
    char id;
    int monsterX;
    int monsterY;
    int hp; // 0-100
    int distance;
    int damage;
} STATEmonster;


#define NUM_MONSTERS 6

#define RAT "R"     //vida = 30     distancia = 1     dano = 2
#define SPIDER "S"  //vida = 50     distancia = 2     dano = 3
#define PYTHON "P"  //vida = 75     distancia = 1     dano = 4
#define ZOMBIE "Z"  //vida = 200    distancia = 1     dano = 5
#define DRAGON "D"  //vida = 400    distancia = 4     dano = 10
#define TREX "T"    //vida = 500    distancia = 3     dan


//Efeitos/poções
//Definir a estrutura das poções/efeitos
typedef struct stPotions{
    char id;
    int potionX;
    int potionY;
} STATEpotions;

#define NUM_POTIONS 5

#define NONE "N"
#define VISION "V"
#define STRENGTH "F" //força = *2
#define RESISTENCE "A" //+10% armor
#define CURE "C" //vida 100


#endif // DICTIONARY_H_INCLUDED

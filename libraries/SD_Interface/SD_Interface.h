#ifndef SD_Interface_h
#define SD_Interface_h

#include <stdio.h>
#include "Arduino.h"

#include <LinkedList.h>

#include "SPI.h"
#include "SD.h"

#include <memorysaver.h>
#include <avr/pgmspace.h>

#define SD_CS           53
#define Pokedex         721
#define nullifier       -1
#define READ            0
#define WRITE           1

#define POKEDEX_ID      0
#define APOKEDEX_ID     1
#define ABILITY_ID      2
#define ATTACK_ID       3
#define ITEM_ID         4
#define NATURE_ID       5

#define POKEDEX         "Database/Pokedex.txt"
#define APOKEDEX        "Database/APokedex.txt"
#define ABILITY         "Database/Ability.txt"
#define ATTACK          "Database/Attack.txt"
#define ITEM            "Database/Item.txt"
#define NATURE          "Database/Nature.txt"

#define POKEDEX_SIZE    23
#define APOKEDEX_SIZE   15
#define ABILITY_SIZE    17
#define ATTACK_SIZE     19
#define ITEM_SIZE       20
#define NATURE_SIZE     12

#define POKEDEX_NAME    11
#define APOKEDEX_NAME   11
#define ABILITY_NAME    14
#define ATTACK_NAME     16
#define ITEM_NAME       16
#define NATURE_NAME     7

typedef struct{
    int                 ID;                 // Database identification
    int                 outListSize;        // The final size of the list
    
    LinkedList<String>  Result;
    LinkedList<String>  SearchID;
    
    //String              Result[4];
    //String              SearchID[4];
    
    LinkedList<String>  wordInput;          // The user provided query
    
}database;

typedef struct{
    int                 sizeDB;
    int                 sizeWord;
    int                 inWordSize;
    
    LinkedList<String>  lineSearch;
    
    LinkedList<char>    wordStorage;
    
}process;

typedef struct {
    int pxlX;     // Number of pixels in the x-direction
    int pxlY;     // Number of pixels in the y-direction
    int posX;     // Position of the left side of the image
    int posY;     // Position of the top side of the image
    int sizeX;    // Base horizontal size of the image
    int sizeY;    // Base vertical size of the image
    int cnt;
    int scale = 1;
    int pixCol;
    
    unsigned int backColor = VGA_SCR_BACK;
    
    String location;
    String num;
    String file;
} graphic;

class SD_Interface{
    
public:

    SD_Interface();
    
    void Test(char* part1);
    
    //
    // Preliminary Loading Information
    //
    
    int         Masuda(int type);
    int         ShellDisplay(int type,int color = nullifier);
    void        GameList(int list[],int size,int type);
    
    // Egg Total Read/Write
    // Hatch Total Read/Write
    
    //
    // Database Manipulation
    //
    
    void        search(database* Data);
    
    void        pokedexRead(char* Name,int NumSel = 1);
    void        abilityRead(char ABIL_HEX_0,char ABIL_HEX_1,char* Ability);
    void        attackRead(char ATK_HEX_0,char ATK_HEX_1,char* Attack);
    int         itemRead(char ITEM_HEX_0,char ITEM_HEX_1,char* Item);
    void        natureRead(char ITEM_NATR_0,char ITEM_NATR_1,char* Nature,int* NatRet);
    
    // Breed History
    // Parent History
    // Parent Directory
    // Box Manipulation
    // Image Retrieval
    
    //
    // Interface Retrievals
    //
    
    //void pokemonInfoRead(int Num,char* Pokemon[],char* PrimeAbil[],char* SecondAbil[],char* HiddenAbil,int GenderType,int AbilityType);
    void        abilityWrite(char* Ability,int type);
    int         infoWrite(int type);
    
    char KeyLayout[40] = {  '1','2','3','4','5','6','7','8','9','0',
                            'A','B','C','D','E','F','G','H','I','J',
                            'K','L','M','N','O','P','Q','R','S','T',
                            'U','V','W','X','Y','Z','.','\'','-',' '};
    
private:
    
    char        PrimeAbil[14];
    char        SecondAbil[14];
    char        HiddenAbil[14];
    
    int         PokemonNum;
    int         GenderType;
    int         AbilityType;
    
    int         i;
    int         j;
    int         k;
};

#endif

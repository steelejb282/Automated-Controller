#ifndef SD_Interface_h
#define SD_Interface_h

#include <stdio.h>
#include "Arduino.h"

#include "SPI.h"
#include "SD.h"

#define SD_CS       53
#define Pokedex     721
#define nullifier   -1
#define READ        0
#define WRITE       1

#define POKEDEX     "Database/Pokedex.txt"
#define ABILITY     "Database/Ability.txt"
#define ATTACK      "Database/Attack.txt"
#define ITEM        "Database/Item.txt"
#define NATURE      "Database/Nature.txt"

class SD_Interface{
    
public:

    SD_Interface();
    
    void Test(char* part1);
    
    //
    // Preliminary Loading Information
    //
    
    int     Masuda(int type);
    int     ShellDisplay(int type,int color = nullifier);
    void    GameList(int list[],int size,int type);
    
    // Egg Total Read/Write
    // Hatch Total Read/Write
    
    //
    // Database Manipulation
    //
    
    void    pokedexRead(char* Name,int NumSel = 1);
    void    abilityRead(char ABIL_HEX_0,char ABIL_HEX_1,char* Ability);
    void    attackRead(char ATK_HEX_0,char ATK_HEX_1,char* Attack);
    int     itemRead(char ITEM_HEX_0,char ITEM_HEX_1,char* Item);
    void    natureRead(char ITEM_NATR_0,char ITEM_NATR_1,char* Nature,int* NatRet);
    
    // Breed History
    // Parent History
    // Parent Directory
    // Box Manipulation
    // Image Retrieval
    
    //
    // Interface Retrievals
    //
    
    //void pokemonInfoRead(int Num,char* Pokemon[],char* PrimeAbil[],char* SecondAbil[],char* HiddenAbil,int GenderType,int AbilityType);
    void    abilityWrite(char* Ability,int type);
    int     infoWrite(int type);
    
private:
    
    char    PrimeAbil[14];
    char    SecondAbil[14];
    char    HiddenAbil[14];
    
    int     PokemonNum;
    int     GenderType;
    int     AbilityType;
    
    int     i;
};

#endif
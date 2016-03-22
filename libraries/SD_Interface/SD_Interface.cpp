#include "SD_Interface.h"

// SD_Interface is a class that is designated to hold all of the dealings with the SD card that the
// main function will use.  For the most part, these functions should work behind the scenese, with
// the primary interface functions being included under the Interface Retrievals header.

File myFile;
File altFile;

SD_Interface::SD_Interface(){
    
    SD.begin(SD_CS);
}

//
// PRELIMINARY LOADING INFORMATION
//
// Information to be called from the bootup of the program
//

int SD_Interface::Masuda(int type) {
    
    // MasudaRead is used to store the number of eggs that have been hatched under the pretense
    // of the Masuda method of breeding, wherein the mother and father of the baby originated from
    // different regions of the world, to the sd card.  type is used to direct Masuda to either
    // read the existing number, or to write to update the stored value.
    
    int MasudaCnt = 0;
    int temp;
    
    myFile = SD.open("Masuda.txt", FILE_READ);
    
    if (myFile) {
        
        while (myFile.available()) {
            
            temp = myFile.read() - '0';
            
            MasudaCnt = MasudaCnt * 10 + temp;
        }
    }
    else {
        
        return -999;
    }
    
    myFile.close();
    
    if (type == READ){
        
        return MasudaCnt;
    }
    else if (type == WRITE){
        
        MasudaCnt++;
        
        myFile = SD.open("Masuda.txt",FILE_WRITE);
        
        if (myFile){
            
            myFile.seek(0);
            myFile.print(MasudaCnt);
        }
        else{
            
            //Serial.println("SD Card could not be found.");
        }
        
        myFile.close();
    }
}
int SD_Interface::ShellDisplay(int type,int color){
    
    char storedShell[2];
    int storedVal;
    
    if (type == READ || color == nullifier){
        
        myFile = SD.open("Shell.txt", FILE_READ);
        
        if (myFile){
            
            for (i=0;i<2;i++){
                
                storedShell[i] = myFile.read()-'0';
            }
        }
        
        if (storedShell[1] >= 0 && storedShell[1] <= 9){
            
            storedVal = 10+storedShell[1];
        }
        else{
            
            storedVal = storedShell[0];
        }
        
        myFile.close();
        
        return storedVal;
    }
    else if (type == WRITE){
        
        SD.remove("Shell.txt");
        myFile = SD.open("Shell.txt", FILE_WRITE);
        
        myFile.print(color);
        
        myFile.close();
    }
}

void SD_Interface::GameList(int list[],int size,int type){
    
    int temp;
    
    if (type == READ){
        
        myFile = SD.open("GameList.txt",FILE_READ);
        
        for (i=0;i<size;i++){
            
            int temp = myFile.read()-'0';
            
            list[i] = temp;
        }
        
        myFile.close();
    }
    else if (type == WRITE){
        
        SD.remove("GameList.txt");
        myFile = SD.open("GameList.txt",FILE_WRITE);
        
        for (i=0;i<size;i++){
            
            if (list[i]){
                myFile.print('1');
            }
            else{
                myFile.print('0');
            }
        }
        
        myFile.close();
    }
    
}

//
// DATABASE MANIPULATION
//
// Functions designed to read and interpret the stored databases
//

void SD_Interface::search(database* Data) {
    
    //
    // This function, search, is designed to accept the entry of a database identifier, as well as
    // a search query.  This function will open the desired database, and search it for words that
    // start with the input query list.  These lists will be concatenated into strings which, along
    // with their corresponding identifiers for later consultation, will be added to a linked list
    // provided within the given structure.
    //
    // This function should be used with a keyboard, to allow for the user to insert search queries,
    // and find the information corresponding to their desired search.
    //
    // At some point in the future, this functino should be altered, such that search queries that
    // begin with '*' will search for the query anywhere within the word.
    //
    
    process sort;
    
    int     nameMod,            // Modifier to search name - some databases are buffered in name size
            wordSize;           // The size of the resulting word in the search
    
    int     Cap         = 1;    // A marker for capitalizing the results in appropriate locations
    int     compCnt     = 0;    // Counter for comparing query to database
    int     IDmod       = 2;    // Modifier to the size of of the identification code
    int     start       = 3;    // Start position of the word in the database
    
    String tempInput    = String("");   // Used to convert the input string list into a char array
    String tempID       = String("");   // Used to convert the search results to a string
    String tempResult   = String("");   // Used to concert the search id to a string
    
    // Use the database ID to open the database, and retrieve sizes, and pertinate modifiers
    switch (Data->ID) {
            
        case POKEDEX_ID:
            myFile = SD.open(POKEDEX, FILE_READ);
            sort.sizeWord = POKEDEX_NAME;
            sort.sizeDB   = POKEDEX_SIZE;
            start   = 4;                                // Starting position of the word
            nameMod = 0;                                // Word size modification
            IDmod   = 3;                                // Size of the identifier
        case ABILITY_ID:
            myFile = SD.open(ABILITY, FILE_READ);
            sort.sizeWord = ABILITY_NAME;
            sort.sizeDB   = ABILITY_SIZE;
            nameMod = 0;                                // Word size modification
            break;
        case ATTACK_ID:
            myFile = SD.open(ATTACK, FILE_READ);
            sort.sizeWord = ATTACK_NAME;
            sort.sizeDB   = ATTACK_SIZE;
            nameMod = 1;                                // Word size modification
            break;
        case ITEM_ID:
            myFile = SD.open(ITEM, FILE_READ);
            sort.sizeWord = ITEM_NAME;
            sort.sizeDB   = ITEM_SIZE;
            nameMod = 1;                                // Word size modification
            break;
        case NATURE_ID:
            myFile = SD.open(NATURE, FILE_READ);
            sort.sizeWord = NATURE_NAME;
            sort.sizeDB   = NATURE_SIZE;
            nameMod = 0;                                // Word size modification
            break;
        default:    // APokedex_ID
            myFile = SD.open(APOKEDEX, FILE_READ);
            sort.sizeWord = APOKEDEX_NAME;
            sort.sizeDB   = APOKEDEX_SIZE;
            start   = 4;                                // Starting position of the word
            nameMod = 0;                                // Word size modification
            IDmod   = 3;                                // Size of the identifier
            break;
    }
    
    // Create a temporary storage array for each entry into the database
    char  SD_IMPORT[sort.sizeDB];
    char  input[sort.sizeWord];
    
    // Define the size of the input query for later comparison
    sort.inWordSize = Data->wordInput.size();
    
    // Clear the outputs to avoid cross-contamination
    Data->SearchID.clear();
    Data->Result.clear();
    
    // Convert the input string list into a char array for comparative purposes
    for (i = 0; i < sort.inWordSize; i++) tempInput += Data->wordInput.get(i);
    
    tempInput.toCharArray(input, sort.inWordSize);
    
    for (i = 0; i < sort.inWordSize; i++) {
        if (tempInput[i] >= 'a' && tempInput[i] <= 'z') tempInput[i] -= 'a' - 'A';
        sort.lineSearch.add(String(tempInput[i]));
    }
    
    if (myFile) {
        
        while (myFile.available()) {
            
            if (sort.inWordSize == 0) break;
            
            // Pull each item in the data base for analysis
            //
            for (i = 0; i < sort.sizeDB; i++) SD_IMPORT[i] = myFile.read();
            
            // Compare the stored word with the converted query, incrementing compCnt to use
            // as an identifier for a match
            //
            for (i = 0; i < sort.inWordSize; i++) if (String(SD_IMPORT[i + start]) == sort.lineSearch.get(i)) compCnt++;
            
            // If compCnt is the same size as the query, move on to storing the needed information
            // from the line
            //
            if (compCnt == sort.inWordSize) {
                
                // Retreive the size of the word stored in the given line.  The stored size will need to be converted from hexadecimal characters to decimal integers.  As 'A'-'F' follow the decimal values, a modifier of 10 will need to be added to the result for accuracy
                
                // nameMod is included as some databases have altered word sizes for the sake of ease in storage
                wordSize = SD_IMPORT[IDmod];
                
                if (wordSize >= 'A') wordSize += 10 + nameMod - 'A';
                else wordSize += nameMod - '0';
                
                // Retrieve the actual word from the line by adding each character of the word from the stored line into Data->lineStorage.  Follow this by converting the word from all capslock to a more standard grammatical form, and storing the results into Data->wordStorage
                for (i = 0; i < wordSize; i++) sort.wordStorage.add(SD_IMPORT[i + start]);
                
                for (i = 0; i < wordSize; i++) {
                    if (Cap) {
                        sort.wordStorage.set(i, SD_IMPORT[i + start]);
                        Cap = 0;
                    }
                    else if (sort.wordStorage.get(i) >= 'A' && sort.wordStorage.get(i) <= 'Z') sort.wordStorage.set(i, SD_IMPORT[i + start] - 'A' + 'a');
                    else sort.wordStorage.set(i, SD_IMPORT[i + start]);
                    
                    if (sort.wordStorage.get(i) == '-' || sort.wordStorage.get(i) == '.' || sort.wordStorage.get(i) == ' ') Cap = 1;
                }
                
                // Retrieve the identification code for the line, for later access and retrieval of additional information stored in the line
                for (i = 0; i < IDmod; i++) tempID += String(SD_IMPORT[i]);
                Data->SearchID.add(tempID);
                
                // Convert Data->wordStorage into a string, and add it to the list of search results
                for (i = 0; i < wordSize; i++) tempResult += String(sort.wordStorage.get(i));
                Data->Result.add(tempResult);
            }
            
            // Reset the accounting variables and storage spaces for the next round of searching, and
            // move to the next line
            sort.wordStorage.clear();
            tempID = String("");
            tempResult = String("");
            compCnt = 0;
            Cap = 1;
            
            myFile.read();
        }
        
        myFile.close();
    }
    else {
        
        //Serial.println("SD Card could not found.");
        
        // Place the "Missing SD" notice on the board
    }
}

// ***
// ***
// ***
//
// The designs of pokedexRead, abilityRead, attackRead, natureRead, and item read are all obsolete.
// When I get to the point where I'm displaying specific pokémon or information, all of these functions
// are to be combined into one single function that handles the same affects and outputs.
//
// ***
// ***
// ***    // Hatch Total Read/Write
    
    //
    // Database Manipulation
    //
    //
    // void        search(database* Data);
    //
    // void        pokedexRead(char* Name,int NumSel = 1);
    // void        abilityRead(char ABIL_HEX_0,char ABIL_HEX_1,char* Ability);
    // void        attackRead(char ATK_HEX_0,char ATK_HEX_1,char* Attack);
    // int         itemRead(char ITEM_HEX_0,char ITEM_HEX_1,char* Item);
    // void        natureRead(char ITEM_NATR_0,char ITEM_NATR_1,char* Nature,int* NatRet);
    //
    // Breed History
    // Parent History
    // Parent Directory
    // Box Manipulation
    // Image Retrieval
    
    //
    // Interface Retrievals
    //
    
    //void pokemonInfoRead(int Num,char* Pokemon[],char* PrimeAbil[],char* SecondAbil[],char* HiddenAbil

void SD_Interface::pokedexRead(char* Name,int NumSel) {
    
    //  pokedexRead is designed to receive a user provided array [Name] and a selected value
    //  [NumSel] representing a Pokémon's National Dex identification number. [NumSel] is
    //  optional, and preset to 1.
    //
    //  The function will receive a 23 character code containing various pieces of information
    //  from a pre-prepared file ("Pokedex.txt") on an sd card.  The function will seek the
    //  code pertaining to [NumSel], and parse the code into information including the Pokémon
    //  itself, it's gender and ability types, and it's abilitys' identification values.
    //
    //  The Pokémon's name will be saved to [Name], [NumSel], gender spread, and ability spread
    //  to the global variables [PokemonNum], [GenderType], and [AbilityType] respectively,
    //  and each of the abilities will be identified and stored through the abilityRead function.
    
    int   Cap = 1;
    int   End = 0;
    
    int   NatNum;
    
    char  temp;
    
    char  SD_POKEDEX_IMPORT[POKEDEX_SIZE];
    
    //  SD_POKEDEX_IMPORT[23] == A[23]
    //
    //  a. A[0]-A[2]   = Number identification
    //  b. A[3]        = Number of characters to retreive from A[4]-A[14]
    //  c. A[4]-A[14]  = Pokemon's name
    //  d. A[15]       = Gender spread identification
    //          0:  M/F
    //          1:  M/-
    //          2:  -/F
    //          3:  -/-
    //  e. A[16]-A[17] = Primary Ability hexadecimal value
    //  f. A[18]-A[19] = Secondary Ability hexadecimal value
    //  g. A[20]-A[21] = Hidden Ability hexadeciaml value
    //  h. A[22]       = Ability spread identification
    //          0:  1/1/1
    //          1:  1/-/1
    //          2:  1/1/-
    //          3:  1/-/-
    //
    // TO BE ADDED:
    //
    // For eventual use in the breeding routine, it would be useful for the program to understand
    // the mechanics of breeding, particularly of the various types Hatched Pokemon, as well as
    // egg group listings for compatibility testing.  Therefore,
    //
    // i. A[23]-A[25]   = Identifier for the base form of the mother/Ditto borne
    // j. A[26]-A[28]   = Identifier for the base form, in the case of use of Incense
    // k. A[29]         = Incense identifier (0 if not compatible to Incense breeding)
    // l. A[30]         = Hexadecimal identifier for Egg Group 1 (ranging from 1-F)
    // m. A[31]         = Hexadecimal identifier for Egg Group 2 (0 for no secondary egg group)
    
    int   SD_POKEDEX_NAME_CNT;
    
    char  ABIL_1_HEX[2];        // Primary ability hex value
    char  ABIL_2_HEX[2];        // Secondary ability hex value
    char  ABIL_3_HEX[2];        // Hidden ability hex value
    
    // Reset the input array, assuming that the longest pokémon name is eleven characters long.
    
    for (i=0;i<POKEDEX_NAME;i++){
        
        Name[i] = NULL;
    }
    
    //  Reset the Ability and Gender flags to the global constants.
    
    AbilityType = 0;
    GenderType  = 0;
    
    myFile = SD.open(POKEDEX, FILE_READ);
    
    if (myFile) {
        
        while (myFile.available()) {
            
            for (i = 0; i < POKEDEX_SIZE; i++) {
                
                SD_POKEDEX_IMPORT[i] = myFile.read();
            }
        
            //  Convert the code identifier (a) to an integer value for comparison to [NumSel].
            
            NatNum = (SD_POKEDEX_IMPORT[0] - '0') * 100 + (SD_POKEDEX_IMPORT[1] - '0') * 10 + (SD_POKEDEX_IMPORT[2] - '0');
            
            if (NatNum == NumSel) {
                
                End = 1;
                
                PokemonNum = NatNum;
                
                SD_POKEDEX_NAME_CNT = SD_POKEDEX_IMPORT[3]  - '0';      //  (b)
                
                GenderType          = SD_POKEDEX_IMPORT[15] - '0';      //  (d)
                AbilityType         = SD_POKEDEX_IMPORT[22] - '0';      //  (h)
                
                //  (e)
                ABIL_1_HEX[0]       = SD_POKEDEX_IMPORT[16];
                ABIL_1_HEX[1]       = SD_POKEDEX_IMPORT[17];
                
                //  (f)
                ABIL_2_HEX[0]       = SD_POKEDEX_IMPORT[18];
                ABIL_2_HEX[1]       = SD_POKEDEX_IMPORT[19];
                
                //  (g)
                ABIL_3_HEX[0]       = SD_POKEDEX_IMPORT[20];
                ABIL_3_HEX[1]       = SD_POKEDEX_IMPORT[21];
                
                //  (c)
                for (i = 0; i < SD_POKEDEX_NAME_CNT; i++) {
                    
                    if (Cap) {
                        
                        Name[i] = SD_POKEDEX_IMPORT[i + 4];
                        
                        Cap = 0;
                    }
                    else if (SD_POKEDEX_IMPORT[i + 4] >= 'A' && SD_POKEDEX_IMPORT[i + 4] <= 'Z') {
                        
                        temp = SD_POKEDEX_IMPORT[i + 4] + 32;
                        
                        Name[i] = temp;
                    }
                    else {
                        
                        Name[i] = SD_POKEDEX_IMPORT[i + 4];
                    }
                    
                    if (SD_POKEDEX_IMPORT[i + 4] == '-' || SD_POKEDEX_IMPORT[i + 4] == ' ') {
                        
                        Cap = 1;
                    }
                }
            }
            
            if (End){
                
                break;
            }
            
            myFile.read();
        }
        
        myFile.close();
        
        // Convert the ability hex codes into the corresponding ability string
        
        abilityRead(ABIL_1_HEX[0],ABIL_1_HEX[1],PrimeAbil);
        abilityRead(ABIL_2_HEX[0],ABIL_2_HEX[1],SecondAbil);
        abilityRead(ABIL_3_HEX[0],ABIL_3_HEX[1],HiddenAbil);
    }
    else {
        
        //Serial.println("SD Card could not found.");
        
        // Place the "Missing SD" notice on the board
    }
}

void SD_Interface::abilityRead(char ABIL_HEX_0,char ABIL_HEX_1,char* Ability) {
    
    // abilityRead is designed to function in the same fashion as pokedexRead.  The file stored
    // containing the list of abilities should be organized with the format of a two digit
    // hexadecimal identifier, followed by the ability name in capital letters.
    //
    // abilityRead recieves two hexadecimal digits, ABIL_HEX_0 and ABIL_HEX_1, and uses them to
    // search for the desired ability.  Upon finding the corresponding code, the name of the
    // ability is converted to a more standard form, before it is written to Ability.
    
    int     Cap = 1;
    int     End = 0;
    
    char    temp;
    
    char    SD_ABILITY_IMPORT[ABILITY_SIZE];
    
    // SD_ABILITY_IMPORT[17] == B[17]
    //
    // a. B[0]          = First hexadecimal identifier
    // b. B[1]          = Second hexadecimal identifier
    // c. B[2]          = Hexadecimal number of characters to retrieve from B[3]-B[16]
    // d. B[3]-B[16]    = Ability Name
    
    int     SD_ABILITY_CNT;
    
    char    AbilHex[2];
    
    // Clear the input array to prevent contamination from past uses
    
    for (i=0;i<ABILITY_NAME;i++){
        
        Ability[i] = NULL;
    }
    
    myFile = SD.open(ABILITY, FILE_READ);
    
    if (myFile) {
        
        while (myFile.available()) {
            
            for (i = 0; i < ABILITY_SIZE; i++) {
                
                SD_ABILITY_IMPORT[i] = myFile.read();
            }
            
            // Search for the correct code using (a) and (b)
            
            if (SD_ABILITY_IMPORT[0] == ABIL_HEX_0 && SD_ABILITY_IMPORT[1] == ABIL_HEX_1) {
                
                End = 1;    // Alerts the search to end, following the retrieval of the ability
                
                // Convert (c) from hexadecimal to decimal
                
                if (SD_ABILITY_IMPORT[2] >= '0' && SD_ABILITY_IMPORT[2] <= '9') {
                    
                    SD_ABILITY_CNT = SD_ABILITY_IMPORT[2] - '0';
                }
                else {
                    
                    SD_ABILITY_CNT = SD_ABILITY_IMPORT[2] - 55;
                }
                
                // Retrieve Ability as per (d), and convert it to it's presentation form
                
                for (i = 0; i < SD_ABILITY_CNT; i++) {
                    
                    if (Cap) {
                        
                        Ability[i] = SD_ABILITY_IMPORT[3 + i];
                        
                        Cap = 0;
                    }
                    else if (SD_ABILITY_IMPORT[3 + i] >= 'A' && SD_ABILITY_IMPORT[3 + i] <= 'Z') {
                        
                        temp = SD_ABILITY_IMPORT[3 + i] + 32;
                        
                        Ability[i] = temp;
                    }
                    else {
                        
                        Ability[i] = SD_ABILITY_IMPORT[3 + i];
                    }
                    
                    if (SD_ABILITY_IMPORT[3 + i] == '-' || SD_ABILITY_IMPORT[3 + i] == ' ') {
                        
                        Cap = 1;
                    }
                }
            }
            
            if (End){
                
                break;
            }
            
            myFile.read();
        }
        
        myFile.close();
    }
    
    else {
        
        //Serial.print("SD Card could not be found.");
    }
}

void SD_Interface::attackRead(char ATK_HEX_0,char ATK_HEX_1,char* Attack) {
    
    // attackRead is designed to function in the same fashion as pokedexRead.  The file stored
    // containing the list of abilities should be organized with the format of a two digit
    // hexadecimal identifier, followed by the ability name in capital letters, evening out with filler
    // characters.
    //
    // attackRead recieves two hexadecimal digits, ATK_HEX_0 and ATK_HEX_1, and uses them to
    // search for the desired attack.  Upon finding the corresponding code, the name of the
    // attack is converted to a more standard form, before it is written to Attack.
    
    int     Cap = 1;
    int     End = 0;
    
    char    temp;
    
    char    SD_ATTACK_IMPORT[ATTACK_SIZE];
    
    // SD_ABILITY_IMPORT[19] == B[19]
    //
    // a. B[0]          = First hexadecimal identifier
    // b. B[1]          = Second hexadecimal identifier
    // c. B[2]          = Hexadecimal number of characters to retrieve from B[3]-B[16] (Add 1)
    // d. B[3]-B[18]    = Attack Name
    
    int     SD_ATTACK_CNT;
    
    char    AtkHex[2];
    
    // Clear the input array to prevent contamination from past uses
    
    for (i=0;i<ATTACK_NAME;i++){
        
        Attack[i] = NULL;
    }
    
    myFile = SD.open(ATTACK, FILE_READ);
    
    if (myFile) {
        
        while (myFile.available()) {
            
            for (i = 0; i < ATTACK_SIZE; i++) {
                
                SD_ATTACK_IMPORT[i] = myFile.read();
            }
            
            // Search for the correct code using (a) and (b)
            
            if (SD_ATTACK_IMPORT[0] == ATK_HEX_0 && SD_ATTACK_IMPORT[1] == ATK_HEX_1) {
                
                End = 1;    // Alerts the search to end, following the retrieval of the ability
                
                // Convert (c) from hexadecimal to decimal
                
                if (SD_ATTACK_IMPORT[2] >= '0' && SD_ATTACK_IMPORT[2] <= '9') {
                    
                    SD_ATTACK_CNT = SD_ATTACK_IMPORT[2] - '0' + 1;
                }
                else {
                    
                    SD_ATTACK_CNT = SD_ATTACK_IMPORT[2] - 54;
                }
                
                // Retrieve Ability as per (d), and convert it to it's presentation form
                
                for (i = 0; i < SD_ATTACK_CNT; i++) {
                    
                    if (Cap) {
                        
                        Attack[i] = SD_ATTACK_IMPORT[3 + i];
                        
                        Cap = 0;
                    }
                    else if (SD_ATTACK_IMPORT[3 + i] >= 'A' && SD_ATTACK_IMPORT[3 + i] <= 'Z') {
                        
                        temp = SD_ATTACK_IMPORT[3 + i] + 32;
                        
                        Attack[i] = temp;
                    }
                    else {
                        
                        Attack[i] = SD_ATTACK_IMPORT[3 + i];
                    }
                    
                    if (SD_ATTACK_IMPORT[3 + i] == '-' || SD_ATTACK_IMPORT[3 + i] == ' ') {
                        
                        Cap = 1;
                    }
                }
            }
            
            if (End){
                
                break;
            }
            
            myFile.read();
        }
        
        myFile.close();
    }
    
    else {
        
        //Serial.print("SD Card could not be found.");
    }
}

int SD_Interface::itemRead(char ITEM_HEX_0,char ITEM_HEX_1,char* Item) {
    
    // itemRead is designed to function in the same fashion as pokedexRead.  The file stored
    // containing the list of abilities should be organized with the format of a two digit
    // hexadecimal identifier, followed by the ability name in capital letters, evening out with filler
    // characters.
    //
    // attackRead recieves two hexadecimal digits, ITEM_HEX_0 and ITEM_HEX_1, and uses them to
    // search for the desired attack.  Upon finding the corresponding code, the name of the
    // item is converted to a more standard form, before it is written to Item.
    
    int     Cap = 1;
    int     End = 0;
    
    char    temp;
    
    char    SD_ITEM_IMPORT[ITEM_SIZE];
    
    // SD_ABILITY_IMPORT[20] == B[20]
    //
    // a. B[0]          = First hexadecimal identifier
    // b. B[1]          = Second hexadecimal identifier
    // c. B[2]          = Hexadecimal number of characters to retrieve from B[3]-B[16] (Add 1)
    // d. B[3]-B[18]    = Item Name
    // e. B[19]         = Item Flag
    
    int     SD_ITEM_CNT;
    
    char    ItemHex[2];
    int     ItemFlag;
    
    // Clear the input array to prevent contamination from past uses
    
    for (i=0;i<ITEM_NAME;i++){
        
        Item[i] = NULL;
    }
    
    myFile = SD.open(ITEM, FILE_READ);
    
    if (myFile) {
        
        while (myFile.available()) {
            
            for (i = 0; i < ITEM_SIZE; i++) {
                
                SD_ITEM_IMPORT[i] = myFile.read();
            }
            
            // Search for the correct code using (a) and (b)
            
            if (SD_ITEM_IMPORT[0] == ITEM_HEX_0 && SD_ITEM_IMPORT[1] == ITEM_HEX_1) {
                
                End = 1;    // Alerts the search to end, following the retrieval of the ability
                
                // Convert (c) from hexadecimal to decimal
                
                if (SD_ITEM_IMPORT[2] >= '0' && SD_ITEM_IMPORT[2] <= '9') {
                    
                    SD_ITEM_CNT = SD_ITEM_IMPORT[2] - '0' + 1;
                }
                else {
                    
                    SD_ITEM_CNT = SD_ITEM_IMPORT[2] - 54;
                }
                
                // Convert (e) from hexadecimal to decimal
                
                if (SD_ITEM_IMPORT[19] >= '0' && SD_ITEM_IMPORT[19] <= '9') {
                    
                    ItemFlag = SD_ITEM_IMPORT[19] - '0';
                }
                else {
                    
                    ItemFlag = SD_ITEM_IMPORT[19] - 55;
                }
                
                // Retrieve Ability as per (d), and convert it to it's presentation form
                
                for (i = 0; i < SD_ITEM_CNT; i++) {
                    
                    if (Cap) {
                        
                        Item[i] = SD_ITEM_IMPORT[3 + i];
                        
                        Cap = 0;
                    }
                    else if (SD_ITEM_IMPORT[3 + i] >= 'A' && SD_ITEM_IMPORT[3 + i] <= 'Z') {
                        
                        temp = SD_ITEM_IMPORT[3 + i] + 32;
                        
                        Item[i] = temp;
                    }
                    else {
                        
                        Item[i] = SD_ITEM_IMPORT[3 + i];
                    }
                    
                    if (SD_ITEM_IMPORT[3 + i] == '-' || SD_ITEM_IMPORT[3 + i] == ' ') {
                        
                        Cap = 1;
                    }
                }
            }
            
            if (End){
                
                break;
            }
            
            myFile.read();
        }
        
        myFile.close();
        
        return ItemFlag;
    }
    
    else {
        
        //Serial.print("SD Card could not be found.");
    }
}

void SD_Interface::natureRead(char NATR_HEX_0,char NATR_HEX_1,char* Nature,int NatRet[]) {
    
    // itemRead is designed to function in the same fashion as pokedexRead.  The file stored
    // containing the list of abilities should be organized with the format of a two digit
    // hexadecimal identifier, followed by the ability name in capital letters, evening out with filler
    // characters.
    //
    // attackRead recieves two hexadecimal digits, ITEM_HEX_0 and ITEM_HEX_1, and uses them to
    // search for the desired attack.  Upon finding the corresponding code, the name of the
    // item is converted to a more standard form, before it is written to Item.
    
    int     Cap = 1;
    int     End = 0;
    
    char    temp;
    
    char    SD_NATR_IMPORT[NATURE_SIZE];
    
    // SD_ABILITY_IMPORT[12] == B[12]
    //
    // a. B[0]          = First hexadecimal identifier
    // b. B[1]          = Second hexadecimal identifier
    // c. B[2]          = Hexadecimal number of characters to retrieve from B[3]-B[16] (Add 1)
    // d. B[3]-B[9]     = Nature Name
    // e. B[10]         = High Stat
    // f. B[11]         = Low Stat
    
    int     SD_NATR_CNT;
    
    char    NatureHex[2];
    
    // Clear the input array to prevent contamination from past uses
    
    for (i=0;i<NATURE_NAME;i++){
        
        Nature[i] = NULL;
    }
    
    myFile = SD.open(NATURE, FILE_READ);
    
    if (myFile) {
        
        while (myFile.available()) {
            
            for (i = 0; i < NATURE_SIZE; i++) {
                
                SD_NATR_IMPORT[i] = myFile.read();
            }
            
            // Search for the correct code using (a) and (b)
            
            if (SD_NATR_IMPORT[0] == NATR_HEX_0 && SD_NATR_IMPORT[1] == NATR_HEX_1) {
                
                End = 1;    // Alerts the search to end, following the retrieval of the ability
                
                SD_NATR_CNT = SD_NATR_IMPORT[2] - '0';
        
                NatRet[0] = SD_NATR_IMPORT[10] - '0';
                NatRet[1] = SD_NATR_IMPORT[11] - '0';
                
                // Retrieve Ability as per (d), and convert it to it's presentation form
                
                for (i = 0; i < SD_NATR_CNT; i++) {
                    
                    if (Cap) {
                        
                        Nature[i] = SD_NATR_IMPORT[3 + i];
                        
                        Cap = 0;
                    }
                    else {
                        
                        temp = SD_NATR_IMPORT[3 + i] + 32;
                        
                        Nature[i] = temp;
                    }
                }
            }
            
            if (End){
                
                break;
            }
            
            myFile.read();
        }
        
        myFile.close();
    }
    
    else {
        
        //Serial.print("SD Card could not be found.");
    }
}

//
// INTERFACE RETRIEVALS
//
// Functions designed to allow the main function to interact with the SD card as simply as possible
//
/*
void SD_Interface::pokemonInfoRead(int Num,char* Pokemon[],char* primeAbil[],char* secondAbil[],char* hiddenAbil,int genderType,int abilityType) {
    
    // pokemonInfoRead is designed to allow for the user to search for a pokémon within the pokédex
    // database directly
    
    if (Num > 0) {
        
        //PokemonNum = Num;
        
        pokedexRead(Pokemon, Num);
        
        abilityWrite(primeAbil, 1);
        abilityWrite(secondAbil, 2);
        abilityWrite(hiddenAbil, 3);
        
        genderType  = infoWrite(1);
        abilityType = infoWrite(2);
    }
}*/

void SD_Interface::abilityWrite(char* Ability,int type){
    
    // abilityWrite is designed to be called externally, to retrieve one of the stored abilities,
    // defined by type, and to be saved to Ability.  As every Pokémon will have at least one ability,
    // the default ability to retrieve will be the Prime Ability.
    
    switch (type){
        
        case 2:
            
            for (i=0;i<14;i++){
                
                Ability[i] = SecondAbil[i];
            }
            break;
            
        case 3:
            
            for (i=0;i<14;i++){
                
                Ability[i] = HiddenAbil[i];
            }
            break;
            
        default:
            
            for (i=0;i<14;i++){
                
                Ability[i] = PrimeAbil[i];
            }
            break;
    }
}

int SD_Interface::infoWrite(int type){
    
    // infoWrite is designed to be called externally, and it is used to retrieve the stored
    // gender or ability spread.
    
    switch (type){
            
        case 1:
            
            return GenderType;
            break;
            
        case 2:
            
            return AbilityType;
            break;
            
        default:
            
            return GenderType;
            break;
    }
}



#include <SD_Interface.h>
#include <ToolBox.h>

#include <UTouch.h>
#include <UTouchCD.h>

#include <UTFT.h>

#include <SPI.h>
#include <SD.h>

char    Pokemon[11];

char    PrimeAbil[14];
char    SecondAbil[14];
char    HiddenAbil[14];

int     PokemonNum;
int     GenderType;
int     AbilityType;

int i;
int j;
int k;

int TFT_X;
int TFT_Y;

UTFT                    GLCD(ILI9325D_16, 38, 39, 40, 41);
UTouch                  myTouch(6, 5, 4, 3, 2);
SD_Interface            sd;
ToolBox                 tools;

void setup() {

  Serial.begin(9600);

  GLCD.InitLCD(PORTRAIT);
  myTouch.InitTouch(PORTRAIT);

  TFT_X = tools.getX();
  TFT_Y = tools.getY();
}

void loop() {

  //initMenu();

  //sd.Test("3");

  nameSearch(POKEDEX_ID);

  while (1);
}

void nameSearch(int databaseID) {

  int scrOpen   = 279;
  int sel;
  int capType   = 0;
  int holdCap   = 1;
  int holdShift = 1;
  int X         = 14;
  int Y         = 164;
  int cnt       = 0;
  int sizeDB;

  char*  Name[sizeDB];
  char*  Search[4][11];

  char   NAME[sizeDB];
  char   SEARCH[sizeDB];

  int    tempName[sizeDB];

  switch(databaseID){

      case POKEDEX_ID:
        sizeDB = POKEDEX_NAME;
        break;
      case ABILITY_ID:
        sizeDB = ABILITY_NAME;
        break;
      case ATTACK_ID:
        sizeDB = ATTACK_NAME;
        break;
      case ITEM_ID:
        sizeDB = ITEM_NAME;
        break;
      case NATURE_ID:
        sizeDB = NATURE_NAME;
        break;
  }

  for (i = 0; i < sizeDB; i++) {

    tempName[i] = NULL;
    SEARCH[i] = NULL;
  }

  GLCD.setBackColor(VGA_SCR_BACK);
  GLCD.setFont(SmallFont);

  pushButton  button;
  keyboard    keys = tools.initKeyboard(X, Y);

  button.arrSize = keys.arrSize;

  for (i = 0; i < button.arrSize; i++) {

    button.text.add(keys.textHigh.get(i));
    button.posX.add(keys.posX.get(i));
    button.posY.add(keys.posY.get(i));
    button.sizeX.add(keys.sizeX.get(i));
    button.sizeY.add(keys.sizeY.get(i));
    button.state.add(keys.state.get(i));
    button.font.add(keys.font.get(i));
    button.textSize.add(keys.textSize.get(i));
    button.textPos.add(keys.textPos.get(i));
  }

  tools.footer(scrOpen - 5, OPEN);
  tools.initMenuSetup(scrOpen);

  GLCD.setColor(VGA_SCR_BACK);
  GLCD.fillRect(X, 34, X + 210, 54);
  GLCD.fillRect(X, 59, X + 210, 79);
  GLCD.fillRect(X, 84, X + 210, 104);
  GLCD.fillRect(X, 109, X + 210, 129);
  GLCD.fillRect(X, Y - 25, X + 210, Y - 5);

  tools.writeButton(&button);

  while (1) {

    sel = readButton(&button);

    delay(200);

    if (keys.shift && sel > 3) {

      if (holdShift) {
        holdShift = 2;
      }
      else {
        keys.shift = 0;
      }
    }

    if (sel == 1) {

      keys.shift = 0;
      keys.caps = !keys.caps;
    }
    else if (sel == 2) {

      keys.shift = !keys.shift;
      keys.caps = 0;

      holdShift = 1;
    }

    if (keys.caps || keys.shift) {

      holdCap = 1;

      for (i = 0; i < button.arrSize; i++) {

        button.text.set(i, keys.textHigh.get(i));
      }

      tools.writeButton(&button);
    }
    else if (holdCap && !keys.shift && !keys.caps) {

      holdCap = 0;

      for (i = 0; i < button.arrSize; i++) {

        button.text.set(i, keys.textLow.get(i));
      }

      tools.writeButton(&button);
    }

    if (sel > 3 && cnt < 11) {

      Name[cnt] = button.text.get(sel);
      tempName[cnt] = sel - 4;
      cnt++;

      GLCD.setColor(VGA_SCR_BACK);
      GLCD.fillRect(X, Y - 25, X + 210, Y - 5);
      GLCD.fillRect(X, 34, X + 210, 54);
      GLCD.fillRect(X, 59, X + 210, 79);
      GLCD.fillRect(X, 84, X + 210, 104);
      GLCD.fillRect(X, 109, X + 210, 129);
      GLCD.setColor(VGA_BLACK);

      for (i = 0; i < cnt; i++) {

        GLCD.print(Name[i], X + 5 + 8 * i, Y - 21);
        NAME[i] = tools.KeyLayout[tempName[i]];
      }

      sd.search(databaseID,NAME, SEARCH, cnt, 0);

      for (i = 0; i < 11; i++) {

        if (tempName[i] == NULL) {

          NAME[i] = NULL;
        }
        else {

          NAME[i] = tools.KeyLayout[tempName[i]];
        }

        GLCD.print(String(SEARCH[i]), X + 5 + i * 8, 113);
      }

      for (i = 0; i < 11; i++) {

        SEARCH[i] = NULL;
      }
    }
    else if (sel == 3 && cnt <= 11) {

      cnt--;
      Name[cnt] = NULL;
      tempName[cnt] = NULL;

      GLCD.setColor(VGA_SCR_BACK);
      GLCD.fillRect(X, Y - 25, X + 210, Y - 5);
      GLCD.fillRect(X, 34, X + 210, 54);
      GLCD.fillRect(X, 59, X + 210, 79);
      GLCD.fillRect(X, 84, X + 210, 104);
      GLCD.fillRect(X, 109, X + 210, 129);
      GLCD.setColor(VGA_BLACK);

      for (i = 0; i < cnt; i++) {

        GLCD.print(Name[i], X + 5 + 8 * i, Y - 21);

        NAME[i] = tools.KeyLayout[tempName[i]];
      }

      sd.search(databaseID,NAME, SEARCH, cnt, 0);

      for (i = 0; i < 11; i++) {

        if (tempName[i] == NULL) {

          NAME[i] = NULL;
        }
        else {

          NAME[i] = tools.KeyLayout[tempName[i]];
        }

        GLCD.print(String(SEARCH[i]), X + 5 + i * 8, 113);
      }
    }

    if (holdShift == 2) {

      holdShift = 0;

      holdCap = 0;

      for (i = 0; i < button.arrSize; i++) {

        button.text.set(i, keys.textLow.get(i));
      }

      tools.writeButton(&button);
    }

    if (sel == 39 || sel == 42) {

      keys.shift  = 1;
      holdShift   = 1;
    }
  }

  sel = NULL;
}

void initBox() {


}

void initMenu() {

  tools.header();

  delay(500);

  mainMenu();
}
void mainMenu() {

  int sel;

  int buff    = standBuff;
  int scrOpen = headerSize + 2 * screenBuff - buff - 1;

  // Prepare the pushButton structure
  tools.clearButtonList();

  pushButton button;

  button.text.add("Routines");
  button.text.add("Tools");
  button.text.add("Settings");

  button.arrSize = button.text.size();

  for (i = 0; i < button.arrSize; i++) {

    // General initializations

    button.sizeX.add(TFT_X - 2 * (innerBuff + outerBuff) - 1);
    button.sizeY.add(30);
    button.posX.add(screenBuff - 1);
    button.posY.add(screenBuff + headerSize + (button.sizeY.get(i) + buff)*i - 1);
    button.state.add(LIVE);
    button.font.add(SMALL);
    button.textSize.add(strlen(button.text.get(i)));
    button.textPos.add(MID_CENTER);

    scrOpen += (button.sizeY.get(i) + buff);
  }

  // Complex initializations

  button.sizeY.set(0, 60);
  button.posY.set(1, 99);
  button.posY.set(2, 134);
  button.font.set(0, LARGE);

  scrOpen += 30;

  // Initiate Display

  tools.footer(scrOpen - 5, OPEN);
  tools.initMenuSetup(scrOpen);

  tools.writeButton(&button);

  // Initiate Touch

  while (1) {

    sel = readButton(&button);

    delay(200);
    tools.footer(CLOSE);
    delay(500);

    switch (sel) {

      case 0:
        gameMenu(0);
        break;
      case 1:
        toolMenu();
        break;
      case 2:
        settingMenu();
        break;
    }
  }
}

void routineMenu() {

  mainMenu();

  /*
      Z - Dead
      ORAS
      XY
      RBGY - Dead
  */

  /*
     Breed
     Release
     Basic Analysis
     Advanced Analysis
     EV Train
     Soft Reset

  */
}

void toolMenu() {

  int sel;

  int buff    = standBuff;
  int scrOpen = headerSize + 2 * screenBuff - buff - 1;

  // Prepare the pushButton structure
  tools.clearButtonList();

  pushButton button;

  button.text.add(" Account Manager");
  button.text.add(" SD Manager");
  button.text.add(" Manual Control");
  button.text.add(" Create Routine");
  button.text.add(" Exit");

  button.arrSize = button.text.size();

  for (i = 0; i < button.arrSize; i++) {

    // General initializations

    button.sizeX.add(TFT_X - 2 * (innerBuff + outerBuff) - 1);
    button.sizeY.add(30);
    button.posX.add(screenBuff - 1);
    button.posY.add(screenBuff + headerSize + (button.sizeY.get(i) + buff)*i - 1);
    button.state.add(DEAD);
    button.font.add(SMALL);
    button.textSize.add(strlen(button.text.get(i)));
    button.textPos.add(MID_LEFT);

    scrOpen += (button.sizeY.get(i) + buff);
  }

  // Complex initializations

  button.state.set(0, LIVE);
  button.state.set(4, LIVE);

  // Initiate Display

  tools.footer(scrOpen - 5, OPEN);
  tools.initMenuSetup(scrOpen);

  tools.writeButton(&button);

  // Initiate Touch

  while (1) {

    sel = readButton(&button);

    delay(200);
    tools.footer(CLOSE);
    delay(500);

    switch (sel) {

      case 0:     // Account Manager
        gameMenu(1);
        break;
      case 1:     // SD Manager
        break;
      case 2:     // Manual Control
        break;
      case 3:     // Create Routine
        break;
      case 4:     // Exit -> mainMenu
        mainMenu();
        break;
    }
  }
}

void settingMenu() {

  int sel;

  int buff    = standBuff;
  int scrOpen = headerSize + 2 * screenBuff - buff - 1;

  // Prepare the pushButton structure
  tools.clearButtonList();

  pushButton button;

  button.text.add(" Game Manager");
  button.text.add(" Routine Manager");
  button.text.add(" Time-Date");
  button.text.add(" Appearance Manager");
  button.text.add(" Exit");

  button.arrSize = button.text.size();

  for (i = 0; i < button.arrSize; i++) {

    // General initializations

    button.sizeX.add(TFT_X - 2 * (innerBuff + outerBuff) - 1);
    button.sizeY.add(30);
    button.posX.add(screenBuff - 1);
    button.posY.add(screenBuff + headerSize + (button.sizeY.get(i) + buff)*i - 1);
    button.state.add(LIVE);
    button.font.add(SMALL);
    button.textSize.add(strlen(button.text.get(i)));
    button.textPos.add(MID_LEFT);

    scrOpen += (button.sizeY.get(i) + buff);
  }

  // Complex initializations

  button.state.set(2, DEAD);

  // Initiate Display

  tools.footer(scrOpen - 5, OPEN);
  tools.initMenuSetup(scrOpen);

  tools.writeButton(&button);

  // Initiate Touch

  while (1) {

    sel = readButton(&button);

    delay(200);
    tools.footer(CLOSE);
    delay(500);

    switch (sel) {

      case 0:       // Game Manager
        gameMenu(2);
        break;
      case 1:       // Routine Manager
        gameMenu(3);
        break;
      case 2:       // Time-Date
        break;
      case 3:       // Appearance Manager
        appearanceMenu();
        break;
      case 4:       // Exit -> mainMenu
        mainMenu();
        break;
    }
  }
}

void gameMenu(int call) {

  // The game menu will be accessed by the Routines call (0), the Account Manager (1), the Game Manager (2), and the Routine Manager (3)

  int sel;

  int buff    = standBuff;
  int scrOpen = headerSize + 2 * screenBuff - buff + 210;

  // Prepare the pushButton structure
  tools.clearButtonList();

  pushButton button;

  button.text.add(" Sun");
  button.text.add(" Moon");
  button.text.add(" O.Ruby");
  button.text.add(" A.Sapphire");
  button.text.add(" X");
  button.text.add(" Y");
  button.text.add(" Yellow");
  button.text.add(" Red");
  button.text.add(" Blue");
  if (call == 2) {
    button.text.add("Save");
    button.text.add("Exit");
  }
  else {
    button.text.add(" Exit");
  }

  button.arrSize = button.text.size();

  int GameSelect[button.arrSize - 1];
  sd.GameList(GameSelect, button.arrSize, READ);

  for (i = 0; i < button.arrSize; i++) {

    // General initializations

    button.sizeX.add(TFT_X - 2 * (innerBuff + outerBuff) - 1);
    button.sizeY.add(30);
    button.posX.add(screenBuff - 1);
    button.posY.add(screenBuff + headerSize + (button.sizeY.get(i) + buff)*i - 1);
    if (call == 2) {
      button.state.add(LIVE);
    }
    else if (call != 3) {
      button.state.add(GameSelect[i]);
    }
    else {
      button.state.add(DEAD);
    }
    button.font.add(SMALL);
    button.textSize.add(strlen(button.text.get(i)));
    button.textPos.add(MID_LEFT);
  }

  // Complex initializations

  // Sun / Moon
  button.sizeX.set(0, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2) - 1);  // Sun
  button.sizeX.set(1, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2));      // Moon
  button.posX.set(1, floor(TFT_X / 2) + 1);
  button.posY.set(0, 34);
  button.posY.set(1, 34);

  // O. Ruby / A. Sapphire
  button.sizeX.set(2, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2) - 1);  // O. Ruby
  button.sizeX.set(3, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2));      // A. Sapphire
  button.posX.set(3, floor(TFT_X / 2) + 1);
  button.posY.set(2, 69);
  button.posY.set(3, 69);

  // X / Y
  button.sizeX.set(4, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2) - 1);  // X
  button.sizeX.set(5, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2));      // Y
  button.posX.set(5, floor(TFT_X / 2) + 1);
  button.posY.set(4, 104);
  button.posY.set(5, 104);

  // Yellow
  button.posY.set(6, 139);

  // Red / Blue
  button.sizeX.set(7, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2) - 1);  // Red
  button.sizeX.set(8, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2));      // Blue
  button.posX.set(8, floor(TFT_X / 2) + 1);
  button.posY.set(7, 174);
  button.posY.set(8, 174);

  button.posY.set(9, 209);

  if (call == 2) {
    button.sizeX.set(button.arrSize - 2, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2) - 1);
    button.sizeX.set(button.arrSize - 1, floor((TFT_X - 2 * (innerBuff + outerBuff) - 4) / 2));
    button.posX.set(button.arrSize - 1, floor(TFT_X / 2) + 1);
    button.posY.set(button.arrSize - 1, button.posY.get(button.arrSize - 2));
    button.state.set(button.arrSize - 2, LIVE);
    button.state.set(button.arrSize - 1, LIVE);
    button.textPos.set(button.arrSize - 2, MID_CENTER);
    button.textPos.set(button.arrSize - 1, MID_CENTER);
  }
  else {
    button.state.set(button.arrSize - 1, LIVE);
  }

  // Initiate Display

  tools.footer(scrOpen - 5, OPEN);
  tools.initMenuSetup(scrOpen);

  tools.writeButton(&button);

  if (call == 2) {
    for (i = 0; i < button.arrSize; i++) {

      if (GameSelect[i] == LIVE) {

        tools.writeButtonPress(button.posX.get(i), button.posY.get(i), button.sizeX.get(i), button.sizeY.get(i), NO_PRESS);
      }
      else if (GameSelect[i] == DEAD) {

        tools.writeButtonPress(button.posX.get(i), button.posY.get(i), button.sizeX.get(i), button.sizeY.get(i), BLOCK_PRESS);
      }
    }
  }

  // Initiate Touch

  while (1) {

    sel = readButton(&button);

    if (call == 2) {

      delay(100);

      for (i = 0; i < button.arrSize; i++) {

        if (GameSelect[i] == LIVE) {

          tools.writeButtonPress(button.posX.get(i), button.posY.get(i), button.sizeX.get(i), button.sizeY.get(i), NO_PRESS);
        }
        else if (GameSelect[i] == DEAD) {

          tools.writeButtonPress(button.posX.get(i), button.posY.get(i), button.sizeX.get(i), button.sizeY.get(i), BLOCK_PRESS);
        }
      }

      sel = readButton(&button);

      if (sel == button.arrSize - 2 || sel == button.arrSize - 1) {

        delay(200);
        tools.footer(CLOSE);
        delay(500);

        switch (sel) {

          case 9:
            sd.GameList(GameSelect, button.arrSize - 1, WRITE);
            mainMenu();
            break;
          case 10:    // Exit -> settingMenu
            settingMenu();
            break;
        }
      }
      else {

        GameSelect[sel] = !GameSelect[sel];

        if (GameSelect[sel]) {

          tools.writeButtonPress(button.posX.get(i), button.posY.get(i), button.sizeX.get(i), button.sizeY.get(i), NO_PRESS);
        }
        else {

          tools.writeButtonPress(button.posX.get(i), button.posY.get(i), button.sizeX.get(i), button.sizeY.get(i), BLOCK_PRESS);
        }
      }
    }
    else {

      delay(200);
      tools.footer(CLOSE);
      delay(500);

      switch (sel) {

        case 0:       // Sun
          break;
        case 1:       // Moon
          break;
        case 2:       // Omega Ruby
          break;
        case 3:       // Alpha Sapphire
          break;
        case 4:       // X
          break;
        case 5:       // Y
          break;
        case 6:       // Yellow
          break;
        case 7:       // Red
          break;
        case 8:       // Blue
          break;
        case 9:
          switch (call) {

            case 0:   // Routine -> mainMenu
              mainMenu();
              break;
            case 1:   // Account Manager -> toolMenu
              toolMenu();
              break;
            case 3:   // Routine Manager -> settingMenu
              settingMenu();
              break;
          }
          break;
      }
    }
  }
}
void routineManageMenu() {


}
void timeDateMenu() {


}
void appearanceMenu() {

  int temp;

  int sel     = 0;
  int hold    = 1;
  int buff    = 4;
  int scrOpen = 245; // Due to the nature of the menu, the grid is locked.  Therefore, as it cannot easily be adjusted, scrOpen can be declared outright
  int cnt     = 0;

  int colSize[2] = {0, 20};

  // Prepare the pushButton structure
  tools.clearButtonList();

  pushButton button;

  for (i = 0; i < 20; i++) {
    button.text.add("");
  }
  button.text.add("Save");
  button.text.add("Exit");

  button.arrSize = button.text.size();

  for (i = 0; i < button.arrSize; i++) {

    // General initializations

    button.sizeX.add((TFT_X - 2 * (innerBuff + outerBuff) - 3 * buff) / 4);
    button.sizeY.add(30);
    button.state.add(LIVE);
    button.font.add(SMALL);
    button.textSize.add(strlen(button.text.get(i)));
    button.textPos.add(MID_CENTER);
  }

  // Complex initializations

  button.sizeX.set(button.arrSize - 2, (TFT_X - 2 * (innerBuff + outerBuff) - buff) / 2);
  button.sizeX.set(button.arrSize - 1, button.sizeX.get(button.arrSize - 2));

  for (i = 0; i < 5; i++) {
    for (j = 0; j < 4; j++) {

      button.posX.add(innerBuff + outerBuff + j * (button.sizeX.get(cnt) + buff) - 1);
      button.posY.add(headerSize + innerBuff + outerBuff + i * (button.sizeY.get(cnt) + buff) - 1);

      cnt++;
    }
  }

  button.posX.add(innerBuff + outerBuff - 1);
  button.posY.add(headerSize + innerBuff + outerBuff + 5 * (button.sizeY.get(0) + buff) - 1);
  button.posX.add(innerBuff + outerBuff + button.sizeX.get(button.arrSize - 2) + buff - 1);
  button.posY.add(button.posY.get(button.arrSize - 2));

  // Initiate Display

  tools.footer(scrOpen - 5, OPEN);
  tools.initMenuSetup(scrOpen);
  tools.writeButton(&button);
  tools.displayColor(&button, colSize);
  tools.writeButtonPress(button.posX.get(sd.ShellDisplay(READ)), button.posY.get(sd.ShellDisplay(READ)), button.sizeX.get(sd.ShellDisplay(READ)), button.sizeY.get(sd.ShellDisplay(READ)), BLOCK_PRESS);

  // Initiate Touch

  while (1) {

    temp = sel;
    sel = readButton(&button);

    if (hold == 1) {

      tools.writeButtonPress(button.posX.get(sd.ShellDisplay(READ)), button.posY.get(sd.ShellDisplay(READ)), button.sizeX.get(sd.ShellDisplay(READ)), button.sizeY.get(sd.ShellDisplay(READ)), NO_PRESS);
      GLCD.setColor(VGA_BUTTON);
      GLCD.setBackColor(VGA_BUTTON);
      GLCD.fillRect(button.posX.get(button.arrSize - 2), button.posY.get(button.arrSize - 2) + 1, button.posX.get(button.arrSize - 2) + button.sizeX.get(button.arrSize - 2), button.posY.get(button.arrSize - 2) + button.sizeY.get(button.arrSize - 2) - 1);
      GLCD.drawPixel(button.posX.get(button.arrSize - 2) + button.sizeX.get(button.arrSize - 2) - 1, button.posY.get(button.arrSize - 2));
      GLCD.setColor(VGA_BLACK);
      GLCD.drawLine(button.posX.get(button.arrSize - 2) + 1, button.posY.get(button.arrSize - 2) + button.sizeY.get(button.arrSize - 2), button.posX.get(button.arrSize - 2) + button.sizeX.get(button.arrSize - 2) - 1, button.posY.get(button.arrSize - 2) + button.sizeY.get(button.arrSize - 2));
      GLCD.drawPixel(button.posX.get(button.arrSize - 2), button.posY.get(button.arrSize - 2) + button.sizeY.get(button.arrSize - 2) - 1);
      GLCD.setFont(SmallFont);
      GLCD.setColor(VGA_AQUA);
      GLCD.print(button.text.get(button.arrSize - 2), button.posX.get(button.arrSize - 2) + floor(button.sizeX.get(button.arrSize - 2) / 2) - 4 * strlen(button.text.get(button.arrSize - 2)), button.posY.get(button.arrSize - 2) + floor((button.sizeY.get(button.arrSize - 2) - 1) / 2) - 6);
      GLCD.setColor(VGA_WHITE);
      tools.writeButtonPress(button.posX.get(button.arrSize - 2), button.posY.get(button.arrSize - 2), button.sizeX.get(button.arrSize - 2), button.sizeY.get(button.arrSize - 2), NO_PRESS);
      button.state.set(button.arrSize - 2, LIVE);
    }

    if (sel < 20) {

      tools.writeButtonPress(button.posX.get(temp), button.posY.get(temp), button.sizeX.get(temp), button.sizeY.get(temp), NO_PRESS);
      tools.writeButtonPress(button.posX.get(sel), button.posY.get(sel), button.sizeX.get(sel), button.sizeY.get(sel), BLOCK_PRESS);

      hold++;
    }
    else {

      delay(200);
      tools.footer(CLOSE);
      delay(500);

      switch (sel) {

        case 20:
          sd.ShellDisplay(WRITE, temp);
          initMenu();
          break;
        case 21:
          settingMenu();
          break;
      }
    }
  }
}

void passwordManageMenu() {


}

/*
    Radio Button (circle):

    input:
      pos[x][y],
      sizes[x][y]
      char* array[numValueY][numValueX],
      buffer[x][y] (space between outer limits of array items)

    initiate array[size] within function, and place preconstructed buttons according to pos and buffer.  set all cells within array to zero, then set the first
    cell to 1.

    the cell to hold one is signified on screen, and the rest are zero.  if another button is pressed within the array, run through the array, set temp to x,y,
    set previous value to zero, and set temp to one.  change the screen to reflect

    Switch Button (square):

    formatted the same way as the radio button, the switch button allows for multiple buttons to be toggled on and off within the array, as opposed to just one.

    Switch:

    operates as a radio button limited to two or three values and different imagery.

    Slide:

    read the bar for the lengthwise value and compare it to the diminsions of the bar.  map this to the array of values which the slide is tied to.  snap to
    appropriate position after slider is released. (Lists)


    needed icons:
    male-female-genderless
    arrows
    miniature pushbuttons (increase/decrease egg or party count mid operation, for example)
*/
/*

  //
  // TEMPLATE FOR MENU CREATION
  //

  void mainMenu() {

  int sel;

  int buff    = standBuff;
  int scrOpen = headerSize + 2 * screenBuff - buff - 1;

  // Prepare the pushButton structure
  tools.clearButtonList();

  pushButton button;

  button.text.add("Routines");
  button.text.add("Tools");
  button.text.add("Settings");

  button.arrSize = button.text.size();

  for (i = 0; i < button.arrSize; i++) {

    // General initializations

    button.sizeX.add(TFT_X - 2 * (innerBuff + outerBuff) - 1);
    button.sizeY.add(30);
    button.posX.add(screenBuff - 1);
    button.posY.add(screenBuff + headerSize + (button.sizeY.get(i) + buff)*i - 1);
    button.state.add(LIVE);
    button.font.add(SMALL);
    button.textSize.add(strlen(button.text.get(i)));
    button.textPos.add(MID_CENTER);

    scrOpen += (button.sizeY.get(i) + buff);
  }

  // Complex initializations

  button.sizeY.set(0, 60);
  button.posY.set(1, 99);
  button.posY.set(2, 134);
  button.state.set(0, DEAD);
  button.font.set(0, LARGE);

  scrOpen += 30;

  // Initiate Display

  tools.footer(scrOpen - 5, OPEN);
  tools.initMenuSetup(scrOpen);

  tools.writeButton(&button);

  // Initiate Touch

  while (1) {

    sel = readButton(&button);

    delay(200);
    tools.footer(CLOSE);
    delay(500);

    switch (sel) {

      case 0:
        mainMenu();//routineMenu();
        break;
      case 1:
        mainMenu();//toolMenu();
        break;
      case 2:
        mainMenu();//settingMenu();
        break;
    }
  }
  }
*/

void pokemonInfoRead(int Num, char* Pokemon) {

  // pokemonInfoRead is designed to allow for the user to search for a pokémon within the pokédex
  // database directly

  if (Num > 0) {

    PokemonNum = Num;

    sd.pokedexRead(Pokemon, Num);

    sd.abilityWrite(PrimeAbil, 1);
    sd.abilityWrite(SecondAbil, 2);
    sd.abilityWrite(HiddenAbil, 3);

    GenderType  = sd.infoWrite(1);
    AbilityType = sd.infoWrite(2);
  }
}
int readButton(pushButton * info) {

  int hold = 1;

  int touchX;
  int touchY;

  int sel;

  while (hold) {

    for (i = 0; i < info->arrSize; i++) {

      if (myTouch.dataAvailable()) {

        myTouch.read();

        touchX = myTouch.getX();
        touchY = myTouch.getY();

        if (touchX >= info->posX.get(i) && touchY >= info->posY.get(i)) {

          if (touchX <= info->posX.get(i) + info->sizeX.get(i) && touchY <= info->posY.get(i) + info->sizeY.get(i)) {

            if (info->state.get(i)) {

              sel = i;
              i = info->arrSize;
              hold = 0;

              tools.writeButtonPress(info->posX.get(sel), info->posY.get(sel), info->sizeX.get(sel), info->sizeY.get(sel), PRESS);
            }
          }
        }
      }
    }
  }

  delay(250);

  tools.writeButtonPress(info->posX.get(sel), info->posY.get(sel), info->sizeX.get(sel), info->sizeY.get(sel), NO_PRESS);

  return sel;
}

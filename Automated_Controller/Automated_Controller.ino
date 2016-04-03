#include <MenuManager.h>
#include <SD_Interface.h>
#include <ToolBox.h>

#include <UTouch.h>
#include <UTouchCD.h>

#include <UTFT.h>

#include <SPI.h>
#include <SD.h>

#include <Wire.h>
#include "Sodaq_DS3231.h"

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

bool startup = true;

UTFT                    GLCD(ILI9325D_16, 38, 39, 40, 41);
UTouch                  Touch(6, 5, 4, 3, 2);
SD_Interface            sd;
ToolBox                 tools;
MenuManage              menu;

File file;

//void header(String text);

int tempMin = -1;
int Hour;
int Minute;
int Second;
int timeX;
int timeY;

int returnGameSel = 0;

String timeString;

DateTime Time;

void setup() {

  Serial.begin(9600);

  GLCD.InitLCD(PORTRAIT);
  Touch.InitTouch(PORTRAIT);

  TFT_X = tools.getX();
  TFT_Y = tools.getY();

  Wire.begin();
  rtc.begin();

  menu.begin(TFT_X, TFT_Y);

  timeX = TFT_X - 46;
  timeY = 5;
}

void loop() {

  initMenu();

  while (1);
}

void initBox() {


}

void initMenu() {

  tools.initHeader(&startup);

  delay(500);

  mainMenu();
}
void mainMenu() {

  int sel;

  pushButton button;
  menu.init_mainMenu(&button);
  button.frame--;

  // Initiate Display

  tools.footer(button.frame, OPEN);
  tools.initMenuSetup(button.frame);
  tools.writeButton(&button);

  // Initiate Touch

  while (1) {

    sel = readButton(&button, mainMenu);

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


  //    SM - Dead
  //    ORAS
  //    XY
  //    RBGY - Dead



  //   Breed
  //   Release
  //   Basic Analysis
  //   Advanced Analysis
  //   EV Train
  //   Soft Reset

}

void toolMenu() {

  int sel;

  pushButton button;
  menu.init_toolMenu(&button);
  button.frame++;

  // Initiate Display

  tools.footer(button.frame, OPEN);
  tools.initMenuSetup(button.frame);
  tools.writeButton(&button);

  // Initiate Touch

  while (1) {

    sel = readButton(&button, mainMenu);

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
    }
  }
}

void settingMenu() {

  int sel;

  pushButton button;
  menu.init_settingMenu(&button);
  button.frame++;

  // Initiate Display

  tools.footer(button.frame, OPEN);
  tools.initMenuSetup(button.frame);
  tools.writeButton(&button);

  // Initiate Touch

  while (1) {

    sel = readButton(&button, mainMenu);

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
    }
  }
}

void gameMenu(int call) {

  // The game menu will be accessed by the Routines call (0), the Account Manager (1), the Game Manager (2), and the Routine Manager (3)

  int sel;

  int buff    = standBuff;
  int scrOpen = headerSize + 2 * screenBuff - buff + 210;

  pushButton button;

  button.text.add("Sun");
  button.text.add("Moon");
  button.text.add("O.Ruby");
  button.text.add("A.Sapphire");
  button.text.add("X");
  button.text.add("Y");
  button.text.add("Yellow");
  button.text.add("Red");
  button.text.add("Blue");
  if (call == 2) button.text.add("Save");

  button.frame = 120;
  if (call == 2) button.frame += 25;

  button.arrSize = button.text.size();

  int GameSelect[button.arrSize];
  sd.GameList(GameSelect, button.arrSize, READ);

  for (i = 0; i < button.arrSize; i++) {

    // General initializations

    button.sizeX.add(TFT_X - 2 * (innerBuff + outerBuff) - 1);
    button.sizeY.add(30);
    button.posX.add(screenBuff - 1);
    button.posY.add(screenBuff + headerSize + (button.sizeY.get(i) + standBuff)*i);

    if (call == 2) button.state.add(LIVE);
    else if (call != 3) button.state.add(GameSelect[i]);
    else button.state.add(DEAD);

    button.font.add(SMALL);
    button.textSize.add(button.text.get(i).length());
    button.textPos.add(MID_LEFT);
  }

  // Complex initializations

  // Sun / Moon
  button.sizeX.set(0, floor((TFT_X - (2 * screenBuff) - 4) / 2) - 1);  // Sun
  button.sizeX.set(1, floor((TFT_X - (2 * screenBuff) - 4) / 2));      // Moon
  button.posX.set(1, floor(TFT_X / 2) + 1);
  button.posY.set(0, headerSize + screenBuff);
  button.posY.set(1, button.posY.get(0));

  // O. Ruby / A. Sapphire
  button.sizeX.set(2, floor((TFT_X - (2 * screenBuff) - standBuff - 1) / 2) - 1);  // O. Ruby
  button.sizeX.set(3, floor((TFT_X - (2 * screenBuff) - standBuff - 1) / 2));      // A. Sapphire
  button.posX.set(3, floor(TFT_X / 2) + 1);
  button.posY.set(2, button.posY.get(0)+button.sizeY.get(0)+standBuff);
  button.posY.set(3, button.posY.get(2));

  // X / Y
  button.sizeX.set(4, floor((TFT_X - (2 * screenBuff) - standBuff - 1) / 2) - 1);  // X
  button.sizeX.set(5, floor((TFT_X - (2 * screenBuff) - standBuff - 1) / 2));      // Y
  button.posX.set(5, floor(TFT_X / 2) + 1);
  button.posY.set(4, 104);
  button.posY.set(5, 104);

  // Yellow
  button.posY.set(6, 139);

  // Red / Blue
  button.sizeX.set(7, floor((TFT_X - (2 * screenBuff) - standBuff - 1) / 2) - 1);  // Red
  button.sizeX.set(8, floor((TFT_X - (2 * screenBuff) - standBuff - 1) / 2));      // Blue
  button.posX.set(8, floor(TFT_X / 2) + 1);
  button.posY.set(7, 174);
  button.posY.set(8, 174);

  // Save
  if (call == 2) {
    button.sizeX.set(button.arrSize - 1, TFT_X - (2 * screenBuff) - standBuff);
    button.posX.set(button.arrSize - 1, floor(TFT_X / 2) + 1);
    button.posY.set(button.arrSize - 1, button.posY.get(button.arrSize - 2));
    button.state.set(button.arrSize - 1, LIVE);
    button.textPos.set(button.arrSize - 1, MID_CENTER);
  }
  else {
    button.state.set(button.arrSize - 1, LIVE);
  }

  // Initiate Display

  tools.footer(button.frame, OPEN);
  tools.initMenuSetup(button.frame);

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

    sel = readButton(&button, mainMenu);

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

      sel = readButton(&button, mainMenu);

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

  pushButton button;

  int temp = sd.ShellDisplay(READ);

  int sel       = 0;
  int hold      = 0;
  int cnt = 0;
  int holdTemp = 0;

  int colSize[2] = {0, 20};

  //menu.init_settingMenu(&button);

  button.frame  = 245; // Due to the nature of the menu, the grid is locked.  Therefore, as it cannot easily be adjusted, scrOpen can be declared outright

  for (i = 0; i < 20; i++) button.text.add("");
  button.text.add("Save");

  button.arrSize = button.text.size();

  for (i = 0; i < button.arrSize; i++) {

    // General initializations

    button.sizeX.add((TFT_X - 2 * (innerBuff + outerBuff) - 3 * (standBuff - 1)) / 4);
    button.sizeY.add(30);
    button.state.add(LIVE);
    button.font.add(SMALL);
    button.textSize.add(button.text.get(i).length());
    button.textPos.add(MID_CENTER);
  }

  // Complex initializations
  button.sizeX.set(button.arrSize - 1, TFT_X - (2 * screenBuff) - innerBuff);

  for (i = 0; i < 5; i++) {
    for (j = 0; j < 4; j++) {

      button.posX.add(innerBuff + outerBuff + j * (button.sizeX.get(cnt) + standBuff - 1) - 1);
      button.posY.add(headerSize + innerBuff + outerBuff + i * (button.sizeY.get(cnt) + standBuff - 1) - 1);

      cnt++;
    }
  }

  button.posX.add(innerBuff + outerBuff - 1);
  button.posY.add(headerSize + innerBuff + outerBuff + 5 * (button.sizeY.get(0) + standBuff - 1) - 1);

  // Initiate Display

  tools.footer(button.frame, OPEN);
  tools.initMenuSetup(button.frame);
  tools.writeButton(&button);
  tools.displayColor(&button, colSize);
  tools.writeButtonPress(button.posX.get(temp), button.posY.get(temp), button.sizeX.get(temp), button.sizeY.get(temp), BLOCK_PRESS);

  // Initiate Touch

  while (1) {

    if (holdTemp) temp = sel;
    sel = readButton(&button, settingMenu);

    if (sel < 20) {

      tools.writeButtonPress(button.posX.get(temp), button.posY.get(temp), button.sizeX.get(temp), button.sizeY.get(temp), NO_PRESS);
      tools.writeButtonPress(button.posX.get(sel), button.posY.get(sel), button.sizeX.get(sel), button.sizeY.get(sel), BLOCK_PRESS);

      holdTemp = 1;
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
      }
    }
  }
}

void passwordManageMenu() {


}


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
int readButton(pushButton* info, void function()) {

  headerInfo  head;

  int hold = 1;

  int touchX;
  int touchY;

  int sel;

  head.foreColor = GLCD.getColor();
  head.backColor = GLCD.getBackColor();

  while (hold) {

    for (i = 0; i < info->arrSize; i++) {

      headerDraw(&head);

      if (Touch.dataAvailable()) {

        Touch.read();

        touchX = Touch.getX();
        touchY = Touch.getY();

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
        else if (touchX >= 4 && touchX <= 39) {
          if (touchY >= 3 && touchY <= 18) {
            if (touchX <= 19) {   // Exit
              tools.writeButtonPress(4, 3, 15, 15, PRESS);
              delay(250);
              tools.writeButtonPress(4, 3, 15, 15, NO_PRESS);
              tools.footer(CLOSE);
              touchX = -1;
              touchY = -1;
              delay(500);
              function();
            }
            if (touchX >= 24) {   // Home
              tools.writeButtonPress(24, 3, 15, 15, PRESS);
              delay(250);
              tools.writeButtonPress(24, 3, 15, 15, NO_PRESS);
              tools.footer(CLOSE);
              touchX = -1;
              touchY = -1;
              delay(500);
              mainMenu();
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

void headerDraw(headerInfo* info) {

  DateTime Time = rtc.now();

  Minute  = Time.minute();

  if (Minute != tempMin) {
    tempMin = Minute;

    Hour    = Time.hour();

    timeString = String("");

    if (Hour > 12) Hour -= 12;
    else if (Hour == 0) Hour = 12;

    GLCD.setFont(SmallFont);
    GLCD.setColor(VGA_WHITE);
    GLCD.setBackColor(VGA_BUTTON);

    if (Hour < 10) timeString = String(" ") + String(Hour) + String(":");
    else timeString = String(Hour) + String(":");
    if (Minute < 10) timeString += String("0") + String(Minute);
    else timeString += String(Minute);

    GLCD.print(timeString, timeX, timeY);

    GLCD.setColor(info->foreColor);
    GLCD.setBackColor(info->backColor);
  }
}

String search(byte ID) {

  int sel;
  int touchX;
  int touchY;

  int choice = -1;
  int holdChoice = 1;

  pushButton  button;
  database    Data;
  keyboard    keys;
  graphic     image;

  button.listMod = 4;
  Data.ID = ID;
  Data.skipSize = button.listMod;
  keys.anchorX = 15;
  keys.anchorY = 132;

  sd.initDatabase(&Data);

  char tempResult[Data.sizeWord];

  keys.searchBar.add(keys.anchorX - 1);
  keys.searchBar.add(keys.anchorY);
  keys.searchBar.add(keys.anchorX + 133);
  keys.searchBar.add(keys.anchorY + 20);

  GLCD.setBackColor(VGA_SCR_BACK);
  GLCD.setFont(SmallFont);

  for (i = 0; i < button.listMod; i++) {
    button.text.add(String("-"));
    if (Data.image)   button.sizeX.add(keys.searchBar.get(2) - keys.searchBar.get(0));
    else              button.sizeX.add(TFT_X - 30);
    button.sizeY.add(20);
    button.posX.add(keys.searchBar.get(0));
    button.posY.add(headerSize + outerBuff + innerBuff - 1 + i * (button.sizeY.get(i) + 1));
    button.state.add(LIVE);
    button.font.add(SMALL);
    button.textSize.add(1);
    button.textPos.add(MID_LEFT);
  }

  image.posX = keys.searchBar.get(2) + 9;
  image.posY = button.posY.get(0) + button.listMod + 3;
  image.scale = 2;

  tools.initKeyboard(&keys, &button);

  tools.footer(keys.scrOpen - 5, OPEN);
  tools.initMenuSetup(keys.scrOpen + 10);
  tools.writeButton(&button);

  GLCD.setColor(VGA_SCR_BACK);
  GLCD.fillRoundRect(keys.searchBar.get(0), keys.searchBar.get(1), keys.searchBar.get(2), keys.searchBar.get(3));
  if (Data.image) GLCD.fillRoundRect(keys.searchBar.get(2) + 6, button.posY.get(0) + button.listMod, keys.searchBar.get(2) + 76, button.posY.get(0) + button.listMod + 70);

  while (1) {

    sel = readButton(&button, initMenu);

    if (sel >= button.listMod && sel <= 3 + button.listMod) {

      switch (sel) {

        case 4:     // Enter
          if (choice >= 0) return Data.SearchID.get(choice);
          break;
        case 5:     // Clear
          choice     = -1;
          holdChoice = 1;
          keys.shift = 1;

          Data.wordInput.clear();
          Data.SearchID.clear();
          Data.Result.clear();

          if (button.state.get(4 + button.listMod) == DEAD) for (i = 4 + button.listMod; i < button.arrSize; i++) button.state.set(i, LIVE);
          for (i = 0; i < button.listMod; i++) button.text.set(i, String("-"));
          for (i = button.listMod; i < button.arrSize; i++) button.text.set(i, keys.textHigh.get(i - button.listMod));
          tools.writeButton(&button);

          GLCD.fillRoundRect(keys.searchBar.get(0), keys.searchBar.get(1), keys.searchBar.get(2), keys.searchBar.get(3));
          if (Data.image) GLCD.fillRoundRect(keys.searchBar.get(2) + 6, button.posY.get(0) + button.listMod, keys.searchBar.get(2) + 76, button.posY.get(0) + button.listMod + 70);
          break;
        case 6:     // Shift
          keys.shift = !keys.shift;

          switch (keys.shift) {
            case 0:
              for (i = button.listMod; i < button.arrSize; i++) button.text.set(i, keys.textLow.get(i - button.listMod));
              break;
            case 1:
              for (i = button.listMod; i < button.arrSize; i++) button.text.set(i, keys.textHigh.get(i - button.listMod));
              break;
          }

          tools.writeButton(&button);
          break;
        case 7:     // Delete
          choice = -1;
          holdChoice = 1;

          if (Data.wordInput.size() > 0) {
            Data.wordInput.pop();

            if (button.state.get(4 + button.listMod) == DEAD) {
              for (i = 4 + button.listMod; i < button.arrSize; i++) button.state.set(i, LIVE);
              tools.writeButton(&button);
            }

            GLCD.fillRoundRect(keys.searchBar.get(0), keys.searchBar.get(1), keys.searchBar.get(2), keys.searchBar.get(3));
            GLCD.setColor(VGA_BLACK);
            for (i = 0; i < Data.wordInput.size(); i++) GLCD.print(Data.wordInput.get(i), keys.searchBar.get(0) + 4 + 8 * i, keys.searchBar.get(1) + 4);
            GLCD.setColor(VGA_SCR_BACK);

            sd.search(&Data);
            for (i = 0; i < button.listMod; i++) button.text.set(i, String("-"));
            for (i = 0; i < Data.Result.size(); i++) button.text.set(i, Data.Result.get(i));
            tools.writeButton(&button);
          }
          if (Data.wordInput.size() == 0) {
            keys.shift = 1;
            for (i = button.listMod; i < button.arrSize; i++) button.text.set(i, keys.textHigh.get(i - button.listMod));
            tools.writeButton(&button);
          }
          if (Data.image) GLCD.fillRoundRect(keys.searchBar.get(2) + 6, button.posY.get(0) + button.listMod, keys.searchBar.get(2) + 76, button.posY.get(0) + button.listMod + 70);
          break;
      }
    }
    else if (sel >= button.listMod + 3) {

      if (Data.wordInput.size() < 11) {
        choice = -1;
        holdChoice = 1;

        Data.wordInput.add(String(button.text.get(sel)));

        if (keys.shift) {
          keys.shift = 0;
          for (i = button.listMod; i < button.arrSize; i++) button.text.set(i, keys.textLow.get(i - button.listMod));
        }

        GLCD.fillRoundRect(keys.searchBar.get(0), keys.searchBar.get(1), keys.searchBar.get(2), keys.searchBar.get(3));
        GLCD.setColor(VGA_BLACK);
        for (i = 0; i < Data.wordInput.size(); i++) GLCD.print(Data.wordInput.get(i), keys.searchBar.get(0) + 4 + 8 * i, keys.searchBar.get(1) + 4);
        GLCD.setColor(VGA_SCR_BACK);

        sd.search(&Data);
        for (i = 0; i < button.listMod; i++) button.text.set(i, String("-"));
        for (i = 0; i < Data.Result.size(); i++) button.text.set(i, Data.Result.get(i));
      }
      if (Data.wordInput.size() == 11) for (i = 4 + button.listMod; i < button.arrSize; i++) button.state.set(i, DEAD);

      tools.writeButton(&button);
      if (Data.image) GLCD.fillRoundRect(keys.searchBar.get(2) + 6, button.posY.get(0) + button.listMod, keys.searchBar.get(2) + 76, button.posY.get(0) + button.listMod + 70);
    }
    else {
      choice = sel;
      holdChoice = 1;
    }

    if (holdChoice) {
      holdChoice = 0;

      for (i = 0; i < button.listMod; i++) tools.writeButtonPress(button.posX.get(i), button.posY.get(i), button.sizeX.get(i), button.sizeY.get(i), NO_PRESS);

      if (choice >= 0 && choice < Data.Result.size()) {
        tools.writeButtonPress(button.posX.get(choice), button.posY.get(choice), button.sizeX.get(choice), button.sizeY.get(choice), PRESS);

        if (Data.image) {
          image.file = Data.imgFileLoc + Data.SearchID.get(choice) + String(".txt");
          tools.imageWrite(&image);
        }
      }
    }
  }
}


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

UTFT                    GLCD(ILI9325D_16, 38, 39, 40, 41);
UTouch                  myTouch(6, 5, 4, 3, 2);
SD_Interface            sd;
ToolBox                 tools;

File file;

uint8_t tempMin;
uint8_t hr;
int hold = 1;
int holdTime = 1;

DateTime Time;

void setup() {

  Serial.begin(9600);

  GLCD.InitLCD(PORTRAIT);
  myTouch.InitTouch(PORTRAIT);

  TFT_X = tools.getX();
  TFT_Y = tools.getY();

  Wire.begin();
  rtc.begin();
}

void loop() {

  /*database Data;

    Data.ID = APOKEDEX_ID;

    Data.wordInput.add(String("G"));

    sd.search(&Data);

    for (i=0;i<Data.SearchID.size();i++){
    Serial.print(Data.SearchID.get(i));
    Serial.print(" ");
    Serial.println(Data.Result.get(i));
    }*/

  initMenu();

  //search();

  while (1);
}

void search() {

  int sel;
  int touchX;
  int touchY;

  int choice = -1;
  int holdChoice = 1;

  keyboard keys;
  pushButton button;
  database Data;

  Data.ID = APOKEDEX_ID;

  GLCD.setBackColor(VGA_SCR_BACK);
  GLCD.setFont(SmallFont);

  tools.initKeyboard(&keys, &button);

  /*for(i=0;i<4;i++){
    output.text.add("-");
    output.sizeX.add(keys.searchBar.get(2)-keys.searchBar.get(0));
    output.sizeY.add(20);
    output.posX.add(keys.searchBar.get(0));
    output.posY.add(headerSize+outerBuff+innerBuff-1+i*output.sizeY.get(i));
    output.state.add(LIVE);
    output.font.add(SMALL);
    output.textSize.add(strlen(output.text.get(i)));
    output.textPos.add(MID_CENTER);
    }*/

  tools.footer(keys.scrOpen - 5, OPEN);
  tools.initMenuSetup(keys.scrOpen + 10);
  tools.writeButton(&button);

  GLCD.setColor(VGA_SCR_BACK);
  GLCD.fillRoundRect(keys.searchBar.get(0), keys.searchBar.get(1), keys.searchBar.get(2), keys.searchBar.get(3));
  for (i = 0; i < 4; i++) {
    GLCD.fillRoundRect(keys.searchBar.get(0), 44 + i * 25, keys.searchBar.get(2), 64 + i * 25);
    GLCD.setColor(VGA_BLACK);
    GLCD.print(String("-"), keys.searchBar.get(0) + 4, 48 + i * 25);
    GLCD.setColor(VGA_SCR_BACK);
  }
  //GLCD.fillRoundRect(keys.searchBar.get(2) + 6, 62, keys.searchBar.get(2) + 66, 122);

  while (1) {

    sel = readButton(&button);

    if (sel <= 3) {

      switch (sel) {

        case 0:     // Enter
          break;
        case 1:     // Clear
          choice = -1;
          holdChoice = 1;
          
          Data.wordInput.clear();

          if (button.state.get(4) == DEAD) {
            for (i = 4; i < button.arrSize; i++) button.state.set(i, LIVE);
            tools.writeButton(&button);
          }

          keys.shift = 1;
          for (i = 0; i < button.arrSize; i++) button.text.set(i, keys.textHigh.get(i));
          tools.writeButton(&button);

          GLCD.fillRoundRect(keys.searchBar.get(0), keys.searchBar.get(1), keys.searchBar.get(2), keys.searchBar.get(3));

          for (i = 0; i < 4; i++) {
            GLCD.fillRoundRect(keys.searchBar.get(0), 44 + i * 25, keys.searchBar.get(2), 64 + i * 25);
            GLCD.setColor(VGA_BLACK);
            GLCD.print(String("-"), keys.searchBar.get(0) + 4, 48 + i * 25);
            GLCD.setColor(VGA_SCR_BACK);
          }
          break;
        case 2:     // Shift
          keys.shift = !keys.shift;

          switch (keys.shift) {
            case 0:
              for (i = 0; i < button.arrSize; i++) button.text.set(i, keys.textLow.get(i));
              break;
            case 1:
              for (i = 0; i < button.arrSize; i++) button.text.set(i, keys.textHigh.get(i));
              break;
          }

          tools.writeButton(&button);
          break;
        case 3:     // Delete
          choice = -1;
          holdChoice = 1;
          
          if (Data.wordInput.size() > 0) {
            Data.wordInput.pop();

            if (button.state.get(4) == DEAD) {
              for (i = 4; i < button.arrSize; i++) button.state.set(i, LIVE);
              tools.writeButton(&button);
            }

            GLCD.fillRoundRect(keys.searchBar.get(0), keys.searchBar.get(1), keys.searchBar.get(2), keys.searchBar.get(3));
            GLCD.setColor(VGA_BLACK);
            for (i = 0; i < Data.wordInput.size(); i++) GLCD.print(Data.wordInput.get(i), keys.searchBar.get(0) + 4 + 8 * i, keys.searchBar.get(1) + 4);
            GLCD.setColor(VGA_SCR_BACK);

            sd.search(&Data);

            for (i = 0; i < 4; i++) {
              GLCD.fillRoundRect(keys.searchBar.get(0), 44 + i * 25, keys.searchBar.get(2), 64 + i * 25);
              GLCD.setColor(VGA_BLACK);
              if (i >= Data.Result.size()) GLCD.print(String("-"), keys.searchBar.get(0) + 4, 48 + i * 25);
              else GLCD.print(Data.Result.get(i), keys.searchBar.get(0) + 4, 48 + i * 25);
              GLCD.setColor(VGA_SCR_BACK);
            }
          }
          if (Data.wordInput.size() == 0) {
            keys.shift = 1;
            for (i = 0; i < button.arrSize; i++) button.text.set(i, keys.textHigh.get(i));
            tools.writeButton(&button);
          }
          break;
      }
    }
    else {

      if (Data.wordInput.size() < 11) {
        choice = -1;
        holdChoice = 1;

        Data.wordInput.add(String(button.text.get(sel)));

        if (keys.shift) {
          keys.shift = 0;
          for (i = 0; i < button.arrSize; i++) button.text.set(i, keys.textLow.get(i));
          tools.writeButton(&button);
        }

        GLCD.fillRoundRect(keys.searchBar.get(0), keys.searchBar.get(1), keys.searchBar.get(2), keys.searchBar.get(3));
        GLCD.setColor(VGA_BLACK);
        for (i = 0; i < Data.wordInput.size(); i++) GLCD.print(Data.wordInput.get(i), keys.searchBar.get(0) + 4 + 8 * i, keys.searchBar.get(1) + 4);
        GLCD.setColor(VGA_SCR_BACK);

        sd.search(&Data);

        for (i = 0; i < 4; i++) {
          GLCD.fillRoundRect(keys.searchBar.get(0), 44 + i * 25, keys.searchBar.get(2), 64 + i * 25);
          GLCD.setColor(VGA_BLACK);
          if (i >= Data.Result.size()) GLCD.print(String("-"), keys.searchBar.get(0) + 4, 48 + i * 25);
          else GLCD.print(Data.Result.get(i), keys.searchBar.get(0) + 4, 48 + i * 25);
          GLCD.setColor(VGA_SCR_BACK);
        }
      }
      if (Data.wordInput.size() == 11) {
        for (i = 4; i < button.arrSize; i++) button.state.set(i, DEAD);
        tools.writeButton(&button);
      }
    }

    if (myTouch.dataAvailable()) {

      myTouch.read();

      touchX = myTouch.getX();
      touchY = myTouch.getY();

      if (touchX >= keys.searchBar.get(0) && touchX <= keys.searchBar.get(2)) {
        if (touchY >= headerSize + outerBuff + innerBuff - 1 && touchY <= headerSize + outerBuff + innerBuff + 94) {

          for (i = 0; i < 4; i++) {
            GLCD.fillRoundRect(keys.searchBar.get(0), 44 + i * 25, keys.searchBar.get(2), 64 + i * 25);
            GLCD.setColor(VGA_BLACK);
            if (i >= Data.Result.size()) GLCD.print(String("-"), keys.searchBar.get(0) + 4, 48 + i * 25);
            else GLCD.print(Data.Result.get(i), keys.searchBar.get(0) + 4, 48 + i * 25);
            GLCD.setColor(VGA_SCR_BACK);
          }
          
          if (touchY <= headerSize + outerBuff + innerBuff + 19) {
            choice = 0;
            holdChoice = 1;
          }
          else if (touchY >= headerSize + outerBuff + innerBuff + 24 && touchY <= headerSize + outerBuff + innerBuff + 44) {
            choice = 1;
            holdChoice = 1;
          }
          else if (touchY >= headerSize + outerBuff + innerBuff + 49 && touchY <= headerSize + outerBuff + innerBuff + 69) {
            choice = 2;
            holdChoice = 1;
          }
          else if (touchY >= headerSize + outerBuff + innerBuff + 74) {
            choice = 3;
            holdChoice = 1;
          }
        }
      }
    }

    if (holdChoice){

      if (choice = -1) GLCD.fillRoundRect(keys.searchBar.get(2) + 6, 62, keys.searchBar.get(2) + 66, 122);
      else{
        
      }
    }
  }
}

void initBox() {


}

void initMenu() {

  tools.header();

  delay(500);

  //mainMenu();
  search();
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


  //    Z - Dead
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

#include "MenuManager.h"

MenuManage::MenuManage(){
}

void MenuManage::begin(int tft_x,int tft_y){
    
    TFT_X = tft_x;
    TFT_Y = tft_y;
}

void MenuManage::init_mainMenu(pushButton* button){
    
    button->text.add("Routines");
    button->text.add("Tools");
    button->text.add("Settings");
    
    button->arrSize = button->text.size();
    
    for (i = 0; i < button->arrSize; i++) {
        
        // General initializations
        
        button->sizeX.add(TFT_X - (2*screenBuff)-innerBuff);
        button->sizeY.add(30);
        button->posX.add(screenBuff - 1);
        button->posY.add(screenBuff + headerSize + (button->sizeY.get(i) + standBuff)*i - 1);
        button->state.add(LIVE);
        button->font.add(SMALL);
        button->textSize.add(button->text.get(i).length());
        button->textPos.add(MID_CENTER);
    }
    
    // Complex initializations
    
    button->sizeY.set(0, 60);
    button->posY.set(1, 109);
    button->posY.set(2, 144);
    button->font.set(0, LARGE);
    
    for (i=0;i<button->arrSize;i++) button->frame += button->sizeY.get(i) + standBuff;
    
    button->frame += screenBuff - standBuff;
}

// Routines

void MenuManage::init_routineMenu(){};

// Tools

void MenuManage::init_toolMenu(pushButton* button){
    
    button->text.add("Account Manager");
    button->text.add("SD Manager");
    button->text.add("Manual Control");
    button->text.add("Create Routine");
    
    button->arrSize = button->text.size();
    
    for (i = 0; i < button->arrSize; i++) {
        
        // General initializations
        
        button->sizeX.add(TFT_X - (2*screenBuff) - innerBuff);
        button->sizeY.add(30);
        button->posX.add(screenBuff - 1);
        button->posY.add(screenBuff + headerSize + (button->sizeY.get(i) + standBuff)*i - 1);
        button->state.add(DEAD);
        button->font.add(SMALL);
        button->textSize.add(button->text.get(i).length());
        button->textPos.add(MID_LEFT);
    }
    
    // Complex initializations
    
    button->state.set(0, LIVE);

    for (i=0;i<button->arrSize;i++) button->frame += button->sizeY.get(i) + standBuff;

    button->frame += screenBuff - standBuff;
}

// Settings

void MenuManage::init_settingMenu(pushButton* button){
    
    button->text.add("Game Manager");
    button->text.add("Routine Manager");
    button->text.add("Time-Date");
    button->text.add("Appearance Manager");
    
    button->arrSize = button->text.size();
    
    for (i = 0; i < button->arrSize; i++) {
        
        // General initializations
        
        button->sizeX.add(TFT_X - (2*screenBuff) - innerBuff);
        button->sizeY.add(30);
        button->posX.add(screenBuff - 1);
        button->posY.add(screenBuff + headerSize + (button->sizeY.get(i) + standBuff)*i - 1);
        button->state.add(LIVE);
        button->font.add(SMALL);
        button->textSize.add(button->text.get(i).length());
        button->textPos.add(MID_LEFT);
    }
    
    // Complex initializations
    
    button->state.set(2, DEAD);
    
    for (i=0;i<button->arrSize;i++) button->frame += button->sizeY.get(i) + standBuff;
    
    button->frame += screenBuff - standBuff;
}

void MenuManage::init_appearanceMenu(pushButton* button){
    
    int cnt = 0;
    
    for (i = 0; i < 20; i++) button->text.add("");
    button->text.add("Save");
    
    button->arrSize = button->text.size();
    
    for (i = 0; i < button->arrSize; i++) {
        
        // General initializations
        
        button->sizeX.add((TFT_X - (2 * screenBuff) - (3 * (standBuff-1))) / 4);
        button->sizeY.add(30);
        button->state.add(LIVE);
        button->font.add(SMALL);
        button->textSize.add(button->text.get(i).length());
        button->textPos.add(MID_CENTER);
    }
    
    // Complex initializations
    button->sizeX.set(button->arrSize - 1, TFT_X - (2 * screenBuff) - innerBuff);
    
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 4; j++) {
            
            button->posX.add(screenBuff + (j * (button->sizeX.get(cnt)) + standBuff-1) - 1);
            button->posY.add(headerSize + standBuff + (i * (button->sizeY.get(cnt) + standBuff-1)) - 1);
            
            cnt++;
        }
    }
    
    button->posX.add(screenBuff - 1);
    button->posY.add(headerSize + screenBuff + (5 * (button->sizeY.get(0) + standBuff-1)) - 1);
}
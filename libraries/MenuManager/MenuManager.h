#ifndef MenuManager_h
#define MenuManager_h

#include <stdio.h>

#include <ToolBox.h>

class MenuManage{
    
public:
    
    MenuManage();
    void begin(int tft_x,int tft_y);
    void init_mainMenu(pushButton* button);
    void init_gameMenu(int call);
    
    // Routines
    void init_routineMenu();
    
    // Tools
    void init_toolMenu(pushButton* button);
    
    // Settings
    void init_settingMenu(pushButton* button);
    void init_appearanceMenu(pushButton* button);
    
private:
    
    int i;
    int j;
    int k;
    
    int TFT_X;
    int TFT_Y;
};

#endif
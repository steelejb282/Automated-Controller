#ifndef ToolBox_h
#define ToolBox_h

#include <stdio.h>
#include "Arduino.h"

#include <UTFT.h>
#include <UTouch.h>
#include <UTouchCD.h>

#include <SD_Interface.h>

#include <LinkedList.h>

#include <memorysaver.h>
#include <avr/pgmspace.h>

#define RS              38
#define WR              39
#define CS              40
#define RST             41

#define NO_PRESS        0
#define PRESS           1
#define BLOCK_PRESS     2

#define TOP_LEFT        0
#define TOP_CENTER      1
#define TOP_RIGHT       2
#define MID_LEFT        3
#define MID_CENTER      4
#define MID_RIGHT       5
#define BOT_LEFT        6
#define BOT_CENTER      7
#define BOT_RIGHT       8

#define LIVE            1
#define DEAD            0

#define SMALL           0
#define LARGE           1

#define OPEN            0
#define CLOSE           1

#define headerSize      15
#define standBuff       5
#define innerBuff       10
#define outerBuff       10
#define screenBuff      innerBuff+outerBuff

#define VGA_BUTTON          0x39E7      // Color for active button
#define VGA_BUTTON_DEAD     0xCE59      // Color for inactive button
#define VGA_SCR_FRONT       0x8EF6      // Color for the display screen light
#define VGA_SCR_BACK        0x75B2      // Color for the display screen shade

#define VGA_SHELL_1         0xEB48      // Color for the Type 1 Shell
#define VGA_SHELL_2         0xFD00      // Color for the Type 2 Shell
#define VGA_SHELL_3         0xCB20      // Color for the Type 3 Shell
#define VGA_SHELL_4         0xF80F      // Color for the Type 4 Shell
#define VGA_SHELL_5         0x300C      // Color for the Type 5 Shell
#define VGA_SHELL_6         0x9CC0      // Color for the Type 6 Shell

typedef struct{
    
    LinkedList<char*>   text;
    LinkedList<int>     posX;
    LinkedList<int>     posY;
    LinkedList<int>     sizeX;
    LinkedList<int>     sizeY;
    LinkedList<int>     state;
    LinkedList<int>     font;
    LinkedList<int>     textSize;
    LinkedList<int>     textPos;
    
    byte                arrSize;
} pushButton;

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

class ToolBox{
    
public:
    
    ToolBox();
    int getX();
    int getY();
    void clearButtonList();
    
    // Menu Interfacing
    
    void initMenuSetup(int size);
    void displayColor(pushButton* info,int values[]);
    //void displayColor(int pos[][2],int size[][2],int values[]);
    
    // Header and Transition control
    void    header();
    void    footer(int opening,int direction=CLOSE);
    
    // Button Control
    void    writeButton(pushButton* info);
    void    writeButtonPress(int posX,int posY,int sizeX,int sizeY,int Press);
    
    // UTFT Control
    void setFont(int size);
    void setColor(int r,int g,int b);
    void setColor(word Color);
    void setBackColor(int r,int g,int b);
    void setBackColor(word Color);
    void fillScr(int r,int g,int b);
    void fillScr(word Color);
    void drawPixel(int x,int y);
    void drawLine(int x1,int y1,int x2,int y2);
    void drawRect(int x1,int y1,int x2,int y2);
    void drawRoundRect(int x1,int y1,int x2,int y2);
    void drawCircle(int x,int y,int r);
    void fillRect(int x1,int y1,int x2,int y2);
    void fillRoundRect(int x1,int y1,int x2,int y2);
    void fillCircle(int x,int y,int r);
    void print(char* str,int x,int y);
    void printInt(int num,int x,int y);
    
private:
    
    int TFT_X;
    int TFT_Y;
    
    int i;
    int j;
    int k;
    
    int ButtonStand[2]          = {20, 20};
    
    int lastFootPos;
    
    unsigned int ColorList[20] = {
        VGA_BLACK,        VGA_NAVY,           VGA_SHELL_5,          VGA_GRAY,
        VGA_MAROON,       VGA_GREEN,          VGA_BLUE,             VGA_PURPLE,
        VGA_RED,          VGA_SHELL_6,        VGA_LIME,             VGA_FUCHSIA ,
        VGA_SHELL_4,      VGA_SHELL_3,        VGA_YELLOW,           VGA_AQUA,
        VGA_SILVER,       VGA_SHELL_1,        VGA_SHELL_2,          VGA_WHITE
    };
};

#endif

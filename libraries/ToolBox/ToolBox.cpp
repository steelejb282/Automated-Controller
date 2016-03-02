#include "ToolBox.h"

UTFT            myGLCD(ILI9325D_16, RS, WR, CS, RST);
SD_Interface    mySD;

ToolBox::ToolBox(){
    
    TFT_X = myGLCD.getDisplayXSize();
    TFT_Y = myGLCD.getDisplayYSize();
}

int ToolBox::getX(){
    
    return TFT_X;
}
int ToolBox::getY(){
    
    return TFT_Y;
}

void ToolBox::clearButtonList(){
    
    pushButton button;
    
    button.text.clear();
    button.posX.clear();
    button.posY.clear();
    button.sizeX.clear();
    button.state.clear();
    button.font.clear();
    button.textSize.clear();
    button.textPos.clear();
}

keyboard ToolBox::initKeyboard(int anchorX,int anchorY){
    
    keyboard keys;
    
    keys.textLow.add(" Ent");
    keys.textLow.add(" Shft");
    keys.textLow.add(" Caps");
    keys.textLow.add("Del ");
    keys.textLow.add("1");
    keys.textLow.add("2");
    keys.textLow.add("3");
    keys.textLow.add("4");
    keys.textLow.add("5");
    keys.textLow.add("6");
    keys.textLow.add("7");
    keys.textLow.add("8");
    keys.textLow.add("9");
    keys.textLow.add("0");
    keys.textLow.add("a");
    keys.textLow.add("b");
    keys.textLow.add("c");
    keys.textLow.add("d");
    keys.textLow.add("e");
    keys.textLow.add("f");
    keys.textLow.add("g");
    keys.textLow.add("h");
    keys.textLow.add("i");
    keys.textLow.add("j");
    keys.textLow.add("k");
    keys.textLow.add("l");
    keys.textLow.add("m");
    keys.textLow.add("n");
    keys.textLow.add("o");
    keys.textLow.add("p");
    keys.textLow.add("q");
    keys.textLow.add("r");
    keys.textLow.add("s");
    keys.textLow.add("t");
    keys.textLow.add("u");
    keys.textLow.add("v");
    keys.textLow.add("w");
    keys.textLow.add("x");
    keys.textLow.add("y");
    keys.textLow.add("z");
    keys.textLow.add(".");
    keys.textLow.add("'");
    keys.textLow.add("-");
    keys.textLow.add(" ");
    
    keys.textHigh.add(" Ent");
    keys.textHigh.add(" Shft");
    keys.textHigh.add(" Caps");
    keys.textHigh.add("Del ");
    keys.textHigh.add("1");
    keys.textHigh.add("2");
    keys.textHigh.add("3");
    keys.textHigh.add("4");
    keys.textHigh.add("5");
    keys.textHigh.add("6");
    keys.textHigh.add("7");
    keys.textHigh.add("8");
    keys.textHigh.add("9");
    keys.textHigh.add("0");
    keys.textHigh.add("A");
    keys.textHigh.add("B");
    keys.textHigh.add("C");
    keys.textHigh.add("D");
    keys.textHigh.add("E");
    keys.textHigh.add("F");
    keys.textHigh.add("G");
    keys.textHigh.add("H");
    keys.textHigh.add("I");
    keys.textHigh.add("J");
    keys.textHigh.add("K");
    keys.textHigh.add("L");
    keys.textHigh.add("M");
    keys.textHigh.add("N");
    keys.textHigh.add("O");
    keys.textHigh.add("P");
    keys.textHigh.add("Q");
    keys.textHigh.add("R");
    keys.textHigh.add("S");
    keys.textHigh.add("T");
    keys.textHigh.add("U");
    keys.textHigh.add("V");
    keys.textHigh.add("W");
    keys.textHigh.add("X");
    keys.textHigh.add("Y");
    keys.textHigh.add("Z");
    keys.textHigh.add(",");
    keys.textHigh.add("?");
    keys.textHigh.add("+");
    keys.textHigh.add(" ");
    
    keys.arrSize    = keys.textLow.size();
    keys.shift      = 1;
    keys.caps       = 0;
    keys.anchorX    = anchorX;
    keys.anchorY    = anchorY;
    
    for (i = 0; i < keys.arrSize; i++) {
        
        keys.sizeX.add(20);
        keys.sizeY.add(20);
        keys.state.add(LIVE);
        keys.font.add(SMALL);
        keys.textSize.add(strlen(keys.textLow.get(i)));
        keys.textPos.add(MID_CENTER);
    }
    
    keys.sizeX.set(0, 51);
    keys.sizeX.set(1, 52);
    keys.sizeX.set(2, 51);
    keys.sizeX.set(3, 51);
    keys.posX.add(keys.anchorX+1);
    keys.posX.add(keys.posX.get(0)+keys.sizeX.get(0)+1);
    keys.posX.add(keys.posX.get(1)+keys.sizeX.get(1)+1);
    keys.posX.add(keys.posX.get(2)+keys.sizeX.get(2)+1);
    keys.posY.add(keys.anchorY);
    keys.posY.add(keys.anchorY);
    keys.posY.add(keys.anchorY);
    keys.posY.add(keys.anchorY);
    
    keys.textPos.set(0, MID_LEFT);
    keys.textPos.set(1, MID_LEFT);
    keys.textPos.set(2, MID_LEFT);
    keys.textPos.set(3, MID_RIGHT);
    
    for (i = 0; i < 4; i++) {
        
        for (j = 0; j < 10; j++) {
            
            keys.posX.add(anchorX + j * 21);
            keys.posY.add(anchorY + 21 + i * 21);
        }
    }
    
    return keys;
}

//
// MENU INTERFACING
//

void ToolBox::initMenuSetup(int size){
    
    myGLCD.setColor(VGA_SCR_FRONT);
    myGLCD.fillRoundRect(9,24,TFT_X-11,size);
}
void ToolBox::displayColor(pushButton* info,int values[]){
    
    for (i=values[0];i<values[1];i++){
        
        myGLCD.setColor(ColorList[i]);
        myGLCD.fillRect(info->posX.get(i)+2,info->posY.get(i)+2,info->posX.get(i)+info->sizeX.get(i)-3,info->posY.get(i)+info->sizeY.get(i)-3);
    }
}

//
// HEADER AND TRANSITIONING
//

void ToolBox::header(){
    
    int callShell;
    
    // header is used to create the header bar at the top of the screen.  The header will include
    // an icon to indicate successful connections to each the DS system and the SD card respectively,
    // and it will eventually include a clock.
    
    // Icon showing the connection to the 3DS
    
    const byte DSimg[8][12] = {
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}
    };
    
    // Icon showing the connection to the SD card
    
    const byte SDimg[8][5] = {
        {1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    };
    
    // Initiate the back screen
    
    callShell = mySD.ShellDisplay(READ);
    
    myGLCD.fillScr(ColorList[callShell]);
    
    // Create the header
    
    myGLCD.setColor(VGA_BUTTON);
    myGLCD.fillRect(0,0,TFT_X-1,9);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.fillRect(0,10,TFT_X-1,13);
    myGLCD.setColor(VGA_BUTTON_DEAD);
    myGLCD.drawLine(0,14,TFT_X-1,14);
    
    // Create the footer
    
    myGLCD.setColor(VGA_BLACK);
    myGLCD.fillRect(0,15,TFT_X-1,19);
    myGLCD.setColor(VGA_BUTTON);
    myGLCD.fillRect(0,20,TFT_X-1,28);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.drawLine(0,29,TFT_X-1,29);
    
    // Create and place the icons in the top-left hand corner
    
    myGLCD.setColor(VGA_WHITE);
    
    for (i = 0; i < 8; i++) {
        
        for (j = 0; j < 12; j++) {
            
            if (DSimg[i][j]) {
                
                myGLCD.drawPixel(j + 3, i + 1);
            }
        }
        for (k = 0; k < 5; k++) {
            
            if (SDimg[i][k]) {
                
                myGLCD.drawPixel(k + 24, i + 1);
            }
        }
    }
}

void ToolBox::footer(int opening, int direction){
    
    int claspOpen  = 14;
    int claspClose = 304;
    int callShell;
    
    callShell = mySD.ShellDisplay(READ);
    
    if (opening != CLOSE){
        
        lastFootPos = opening+claspOpen;
    }
    
    if (opening > 289){
        
        opening = 289;
    }
    if (direction == OPEN){
            
        for (i=3;i<=opening;i+=4){
                
            myGLCD.setColor(VGA_SCR_BACK);
            myGLCD.fillRect(0,claspOpen-1+i,TFT_X-1,claspOpen+3+i);
            myGLCD.setColor(VGA_BLACK);
            myGLCD.fillRect(0,claspOpen+4+i,TFT_X-1,claspOpen+8+i);
            myGLCD.setColor(VGA_BUTTON);
            myGLCD.fillRect(0,claspOpen+9+i,TFT_X-1,claspOpen+17+i);
            myGLCD.setColor(VGA_BLACK);
            myGLCD.drawLine(0,claspOpen+18+i,TFT_X-1,claspOpen+18+i);
        }
    }
        
    if (direction == CLOSE){
        
        for (i=lastFootPos+1;i>=14;i-=4){
                
            myGLCD.setColor(VGA_BLACK);
            myGLCD.fillRect(0,i,TFT_X-1,4+i);
            myGLCD.setColor(VGA_BUTTON);
            myGLCD.fillRect(0,5+i,TFT_X-1,13+i);
            myGLCD.setColor(VGA_BLACK);
            myGLCD.drawLine(0,14+i,TFT_X-1,14+i);
            myGLCD.setColor(ColorList[callShell]);
            myGLCD.fillRect(0,15+i,TFT_X-1,18+i);
        }
    }
}

void ToolBox::writeButton(pushButton* info) {
    
    int X;
    int Y;
    
    int posX;
    int posY;
    
    int sizeX;
    int sizeY;
    
    int TextSize;
    
    for (i = 0; i < info->arrSize; i++) {
        
        posX  = info->posX.get(i);
        posY  = info->posY.get(i);
        
        sizeX = info->sizeX.get(i);
        sizeY = info->sizeY.get(i);
        
        TextSize = info->textSize.get(i) * 8;
        
        if (info->font.get(i) == LARGE){
            
            TextSize *= 2;
        }
        
        // Prevent a button from being larger than the screen or from having a negative or minimal value. If so, reset values to the standard button size.
        
        if (sizeX < TextSize || sizeX >= TFT_X) {
            
            sizeX =  TextSize + ButtonStand[0];
        }
        if (sizeY < 20 || sizeY >= TFT_Y) {
            
            sizeY = ButtonStand[1];
        }
        
        // Prevent the button from being placed off screen.  If so, move button back within the bounds of the screen as well as the header and footer.
        
        if (posX < 0) {
            
            posX = 0;
        }
        else if (posX >= TFT_X) {
            
            posX = TFT_X - sizeX - 1;
        }
        if (posY <= 10) {
            
            posY = 11;
        }
        else if (posY >= TFT_Y - 12) {
            
            posY = TFT_Y - sizeY - 13;
        }
        
        // Compare the size and position to the original values.  If they have been altered, reset the corresponding original array to the new values.
        
        if (posX != info->posX.get(i) || posY != info->posY.get(i)) {
            
            info->posX.set(i,posX);
            info->posY.set(i,posY);
        }
        if (sizeX != info->sizeX.get(i) || sizeY != info->sizeY.get(i)) {
            
            info->sizeX.set(i,sizeX);
            info->sizeY.set(i,sizeY);
        }
        
        // Construct the button.
        
        if (info->state.get(i) == LIVE){
            
            myGLCD.setColor(VGA_BUTTON);            // Button is active
            myGLCD.setBackColor(VGA_BUTTON);
        }
        else{
            
            myGLCD.setColor(VGA_BUTTON_DEAD);       // Button is inactive
            myGLCD.setBackColor(VGA_BUTTON_DEAD);
        }
        
        myGLCD.fillRect(posX, posY + 1, posX + sizeX, posY + sizeY - 1);
        myGLCD.drawPixel(posX + sizeX - 1, posY);
        myGLCD.setColor(VGA_BLACK);
        myGLCD.drawLine(posX + 1, posY + sizeY, posX + sizeX - 1, posY + sizeY);
        myGLCD.drawPixel(posX, posY + sizeY - 1);
        
        if (info->font.get(i) == LARGE){
            
            myGLCD.setFont(BigFont);
        }
        else{
            
            myGLCD.setFont(SmallFont);
        }
        
        myGLCD.setColor(VGA_AQUA);
        
        // Orient the text within the button, according to provided positioning.
        
        switch (info->textPos.get(i)) {
                
            case TOP_LEFT:
                X = posX + 3;
                Y = posY + 1;
                break;
            case TOP_CENTER:
                X = posX + floor(sizeX / 2) - TextSize / 2;
                Y = posY + 1;
                break;
            case TOP_RIGHT:
                X = posX + sizeX - TextSize - 1;
                Y = posY + 1;
                break;
            case MID_LEFT:
                X = posX + 3;
                Y = posY + floor((sizeY - 1) / 2) - 6;
                break;
            case MID_CENTER:
                X = posX + floor(sizeX / 2) - TextSize / 2;
                Y = posY + floor((sizeY - 1) / 2) - 6;
                break;
            case MID_RIGHT:
                X = posX + sizeX - TextSize - 1;
                Y = posY + floor((sizeY - 1) / 2) - 6;
                break;
            case BOT_LEFT:
                X = posX + 3;
                Y = posY + sizeY - 14;
                break;
            case BOT_CENTER:
                X = posX + floor(sizeX / 2) - TextSize / 2;
                Y = posY + sizeY - 14;
                break;
            case BOT_RIGHT:
                X = posX + sizeX - TextSize - 1;
                Y = posY + sizeY - 14;
                break;
        }
        
        myGLCD.print(info->text.get(i), X, Y);
        
        // Signify the button image as ready.
        
        if (info->state.get(i) == LIVE){
            
            writeButtonPress(posX,posY,sizeX,sizeY,NO_PRESS);
        }
        else{
            
            writeButtonPress(posX,posY,sizeX,sizeY,BLOCK_PRESS);
        }
    }
}

void ToolBox::writeButtonPress(int posX,int posY,int sizeX,int sizeY,int Press) {
 
    switch (Press) {
 
        case NO_PRESS:
            myGLCD.setColor(VGA_WHITE);
            break;
        case PRESS:
            myGLCD.setColor(VGA_AQUA);
            break;
        case BLOCK_PRESS:
            myGLCD.setColor(VGA_RED);
            break;
    }
 
    myGLCD.drawLine(posX + 1, posY, posX + sizeX - 2, posY);
    myGLCD.drawLine(posX + 1, posY + sizeY - 1, posX + sizeX - 2, posY + sizeY - 1);
    myGLCD.drawLine(posX, posY + 1, posX, posY + sizeY - 2);
    myGLCD.drawLine(posX + sizeX - 1, posY + 1, posX + sizeX - 1, posY + sizeY - 2);
 
    myGLCD.drawRect(posX + 1, posY + 1, posX + sizeX - 2, posY + sizeY - 2);
}
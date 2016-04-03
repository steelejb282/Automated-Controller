#include "ToolBox.h"

UTFT            myGLCD(ILI9325D_16, RS, WR, CS, RST);
SD_Interface    mySD;
File            toolFile;

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

void ToolBox::clearButtonList(pushButton* button){
    
    button->text.clear();
    button->posX.clear();
    button->posY.clear();
    button->sizeX.clear();
    button->state.clear();
    button->font.clear();
    button->textSize.clear();
    button->textPos.clear();
    
    button->listMod = 0;
    button->frame = 0;
}

void ToolBox::initKeyboard(keyboard* keys,pushButton* button) {
    
    keys->textLow.add(String("Enter"));
    keys->textLow.add(String("Clear"));
    keys->textLow.add(String("Shift"));
    keys->textLow.add(String("Delete"));
    keys->textLow.add(String("1"));
    keys->textLow.add(String("2"));
    keys->textLow.add(String("3"));
    keys->textLow.add(String("4"));
    keys->textLow.add(String("5"));
    keys->textLow.add(String("6"));
    keys->textLow.add(String("7"));
    keys->textLow.add(String("8"));
    keys->textLow.add(String("9"));
    keys->textLow.add(String("0"));
    keys->textLow.add(String("a"));
    keys->textLow.add(String("b"));
    keys->textLow.add(String("c"));
    keys->textLow.add(String("d"));
    keys->textLow.add(String("e"));
    keys->textLow.add(String("f"));
    keys->textLow.add(String("g"));
    keys->textLow.add(String("h"));
    keys->textLow.add(String("i"));
    keys->textLow.add(String("j"));
    keys->textLow.add(String("k"));
    keys->textLow.add(String("l"));
    keys->textLow.add(String("m"));
    keys->textLow.add(String("n"));
    keys->textLow.add(String("o"));
    keys->textLow.add(String("p"));
    keys->textLow.add(String("q"));
    keys->textLow.add(String("r"));
    keys->textLow.add(String("s"));
    keys->textLow.add(String("t"));
    keys->textLow.add(String("u"));
    keys->textLow.add(String("v"));
    keys->textLow.add(String("w"));
    keys->textLow.add(String("x"));
    keys->textLow.add(String("y"));
    keys->textLow.add(String("z"));
    keys->textLow.add(String("."));
    keys->textLow.add(String("'"));
    keys->textLow.add(String("-"));
    keys->textLow.add(String(" "));
    
    keys->textHigh.add(String("Enter"));
    keys->textHigh.add(String("Clear"));
    keys->textHigh.add(String("Shift"));
    keys->textHigh.add(String("Delete"));
    keys->textHigh.add(String("1"));
    keys->textHigh.add(String("2"));
    keys->textHigh.add(String("3"));
    keys->textHigh.add(String("4"));
    keys->textHigh.add(String("5"));
    keys->textHigh.add(String("6"));
    keys->textHigh.add(String("7"));
    keys->textHigh.add(String("8"));
    keys->textHigh.add(String("9"));
    keys->textHigh.add(String("0"));
    keys->textHigh.add(String("A"));
    keys->textHigh.add(String("B"));
    keys->textHigh.add(String("C"));
    keys->textHigh.add(String("D"));
    keys->textHigh.add(String("E"));
    keys->textHigh.add(String("F"));
    keys->textHigh.add(String("G"));
    keys->textHigh.add(String("H"));
    keys->textHigh.add(String("I"));
    keys->textHigh.add(String("J"));
    keys->textHigh.add(String("K"));
    keys->textHigh.add(String("L"));
    keys->textHigh.add(String("M"));
    keys->textHigh.add(String("N"));
    keys->textHigh.add(String("O"));
    keys->textHigh.add(String("P"));
    keys->textHigh.add(String("Q"));
    keys->textHigh.add(String("R"));
    keys->textHigh.add(String("S"));
    keys->textHigh.add(String("T"));
    keys->textHigh.add(String("U"));
    keys->textHigh.add(String("V"));
    keys->textHigh.add(String("W"));
    keys->textHigh.add(String("X"));
    keys->textHigh.add(String("Y"));
    keys->textHigh.add(String("Z"));
    keys->textHigh.add(String(","));
    keys->textHigh.add(String("?"));
    keys->textHigh.add(String("+"));
    keys->textHigh.add(String(" "));
    
    button->arrSize = keys->textLow.size()+button->listMod; // Get the size of the keyboard
    keys->shift     = 1;                                    // Initiate with Shift on
    keys->holdShift = 0;                                    // Initiate with shift lock on
    keys->keyboardX = keys->anchorX;                        // Designate the x-position of the keyboard
    keys->keyboardY = keys->anchorY + 25;                   // Designate the y-position of the keyboard
    
    //for (i=0;i<keys->textLow.size();i++) button->text.add(keys->textHigh.get(i));
    
    for (i=0;i<keys->textLow.size();i++){//button->listMod; i < button->arrSize; i++) {
        
        button->text.add(keys->textHigh.get(i));
        button->sizeX.add(20);                                 // Standard size - 20p
        button->sizeY.add(20);                                 // Standard size - 20p
        button->state.add(LIVE);                               // Set button as active
        button->font.add(SMALL);                               // Set font as small
        button->textSize.add(keys->textLow.get(i).length());   // Sets placement
        button->textPos.add(MID_CENTER);                       // Sets all buttons to center
    }
    
    // Reset the size of the enter key
    button->sizeX.set(button->listMod, 69);
    // Reset the widths of the alterative buttons
    button->sizeX.set(1+button->listMod, 69);
    button->sizeX.set(2+button->listMod, 69);
    button->sizeX.set(3+button->listMod, 69);
    // Place the enter key
    button->posX.add(keys->searchBar.get(2)+5);
    button->posY.add(keys->searchBar.get(1));
    // Place the alterative buttons
    button->posX.add(keys->anchorX - 1);
    button->posX.add(button->posX.get(1+button->listMod) + button->sizeX.get(1+button->listMod) + 1);
    button->posX.add(button->posX.get(2+button->listMod) + button->sizeX.get(2+button->listMod) + 1);
    button->posY.add(keys->keyboardY);
    button->posY.add(keys->keyboardY);
    button->posY.add(keys->keyboardY);
    
    for (i = 0; i < 4; i++) {           // Rows of descriptive buttons
        for (j = 0; j < 10; j++) {      // Columns of descriptive buttons
            
            button->posX.add(keys->keyboardX + j * 21 - 1);
            button->posY.add(keys->keyboardY + 21 + i * 21);
        }
    }
    
    keys->scrOpen = 261;
}

//
// MENU INTERFACING
//

void ToolBox::initMenuSetup(int size){
    
    myGLCD.setColor(VGA_SCR_FRONT);
    myGLCD.fillRoundRect(outerBuff-1,headerSize+outerBuff-1,TFT_X-outerBuff-1,size+headerSize+outerBuff-1);
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


void ToolBox::initHeader(bool* startup){
    
    DateTime time = rtc.now();
    pushButton headBut;
    
    String timeString = String("");
    
    int timeX = TFT_X - 46;
    int timeY = 5;
    
    int Hour;
    int Minute;
    
    if (*startup == true){
        
        *startup = false;
        
        int headExit[11][11] = {
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,1,1,0,0,0,0,0,1,1,0},
            {0,1,1,1,0,0,0,1,1,1,0},
            {0,0,1,1,1,0,1,1,1,0,0},
            {0,0,0,1,1,1,1,1,0,0,0},
            {0,0,0,0,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,0,1,1,1,0,0},
            {0,1,1,1,0,0,0,1,1,1,0},
            {0,1,1,0,0,0,0,0,1,1,0},
            {0,0,0,0,0,0,0,0,0,0,0},
        };
        int headHome[11][11] = {
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,1,1,1,1,0,0},
            {0,1,1,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,1,1,0,0},
            {0,0,1,1,1,1,1,1,1,0,0},
            {0,0,1,1,1,1,1,1,1,0,0},
            {0,0,1,1,1,1,1,1,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
        };
        
        // Create the header
        
        myGLCD.setColor(VGA_BUTTON);
        myGLCD.fillRect(0,0,TFT_X-1,headerSize-5);
        myGLCD.setColor(VGA_BLACK);
        myGLCD.fillRect(0,headerSize-4,TFT_X-1,headerSize-2);
        myGLCD.setColor(VGA_BUTTON_DEAD);
        myGLCD.drawLine(0,headerSize-1,TFT_X-1,headerSize-1);
        
        // Initiate and create the exit buttons
        
        for (i=0;i<2;i++){
            headBut.text.add(String(""));
            headBut.sizeX.add(15);
            headBut.sizeY.add(15);
            headBut.posX.add(4+i*(headBut.sizeX.get(0)+5));
            headBut.posY.add(3);
            headBut.state.add(LIVE);
            headBut.font.add(SMALL);
            headBut.textSize.add(1);
            headBut.textPos.add(TOP_LEFT);
        }
        
        headBut.arrSize = 2;
        
        writeButton(&headBut);
        
        myGLCD.setColor(VGA_RED);
        
        for (i=0;i<2;i++) myGLCD.fillRect(headBut.posX.get(i)+2,headBut.posY.get(i)+2,headBut.posX.get(i)+headBut.sizeX.get(i)-3,headBut.posY.get(i)+headBut.sizeY.get(i)-3);
        
        myGLCD.setColor(VGA_WHITE);
        
        for (i=0;i<10;i++){
            for (j=0;j<10;j++){
                if (headExit[i][j]) myGLCD.drawPixel(headBut.posX.get(0)+j+2,headBut.posY.get(0)+i+2);
                if (headHome[i][j]) myGLCD.drawPixel(headBut.posX.get(1)+j+2,headBut.posY.get(1)+i+2);
            }
        }
    }
    
    // Initiate the back screen
    
    myGLCD.setColor(ColorList[mySD.ShellDisplay(READ)]);
    myGLCD.fillRect(0,headerSize,TFT_X-1,TFT_Y-1);
    
    // Create the footer
    
    myGLCD.setColor(VGA_BLACK);
    myGLCD.fillRect(0,headerSize,TFT_X-1,headerSize+3);
    myGLCD.setColor(VGA_BUTTON);
    myGLCD.fillRect(0,headerSize+4,TFT_X-1,headerSize+footerSize-3);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.drawLine(0,headerSize+footerSize-2,TFT_X-1,headerSize+footerSize-1);
    
    // Set the time
    
    myGLCD.setFont(SmallFont);
    myGLCD.setColor(VGA_WHITE);
    myGLCD.setBackColor(VGA_BUTTON);
    
    Hour = time.hour();
    Minute = time.minute();
    
    if (Hour > 12) Hour -= 12;
    else if (Hour == 0) Hour = 12;
    
    if (Hour < 10) timeString = String(" ") + String(Hour) + String(":");
    else timeString = String(Hour) + String(":");
    if (Minute < 10) timeString += String("0") + String(Minute);
    else timeString += String(Minute);
    
    myGLCD.print(timeString, timeX, timeY);
}

void ToolBox::footer(int opening, int direction){
    
    int claspOpen  = headerSize-1;
    int claspClose = 304;
    int scrOpen = opening + 2*outerBuff;
    unsigned int callShell;
    
    callShell = mySD.ShellDisplay(READ);
    
    if (opening != CLOSE) lastFootPos = scrOpen+claspOpen;
    if (scrOpen > 289) scrOpen = 289;
    
    if (direction == OPEN){
        
        for (i=3;i<=scrOpen;i+=4){
                
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
        
        for (i=lastFootPos+1;i>=claspOpen;i-=4){
                
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
        
        if (info->font.get(i) == LARGE) TextSize *= 2;
        
        // Prevent a button from being larger than the screen or from having a negative or minimal value. If so, reset values to the standard button size.
        
        //if (sizeX < TextSize || sizeX >= TFT_X) sizeX =  TextSize + ButtonStand[0];
        //if (sizeY < 20 || sizeY >= TFT_Y) sizeY = ButtonStand[1];
        
        // Prevent the button from being placed off screen.  If so, move button back within the bounds of the screen as well as the header and footer.
        
        if (posX < 0) posX = 0;
        else if (posX >= TFT_X) posX = TFT_X - sizeX - 1;

        if (posY < 0) posY = 0;
        else if (posY >= TFT_Y - 12) posY = TFT_Y - sizeY - 13;
        
        // Compare the size and position to the original values.  If they have been altered, reset the corresponding original array to the new values.
        
        if (posX != info->posX.get(i) || posY != info->posY.get(i)) {
            
            info->posX.set(i,posX);
            info->posY.set(i,posY);
        }
        //if (sizeX != info->sizeX.get(i) || sizeY != info->sizeY.get(i)) {
        //
        //    info->sizeX.set(i,sizeX);
        //    info->sizeY.set(i,sizeY);
        //}
        
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
        
        if (info->font.get(i) == LARGE) myGLCD.setFont(BigFont);
        else myGLCD.setFont(SmallFont);
        
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
                X = posX + 3 + 8;
                Y = posY + floor((sizeY - 1) / 2) - 6;
                break;
            case MID_CENTER:
                X = posX + floor(sizeX / 2) - TextSize / 2;
                Y = posY + floor((sizeY - 1) / 2) - 6;
                break;
            case MID_RIGHT:
                X = posX + sizeX - TextSize - 1 - 8;
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
        
        if (info->state.get(i) == LIVE) writeButtonPress(posX,posY,sizeX,sizeY,NO_PRESS);
        else writeButtonPress(posX,posY,sizeX,sizeY,NO_PRESS);
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
    
    myGLCD.drawRect(posX+1,posY,posX+sizeX-2,posY+sizeY-1);
    myGLCD.drawRect(posX,posY+1,posX+sizeX-1,posY+sizeY-2);
}

void ToolBox::imageWrite(graphic* icon) {
    
    char hexSize[4];
    char item[6];
    
    byte decSize[4];
    byte binXSize[8];
    byte binYSize[8];
    
    int pxlX  = 0;
    int pxlY  = 0;
    int sizeX = 0;
    int sizeY = 0;
    
    int magnitude;
    
    unsigned int pixCol;
    
    toolFile = SD.open(icon->file, FILE_READ);
    
    if (toolFile) {
        
        for (i = 0; i < 4; i++) {
            hexSize[i] = toolFile.read();
            
            if (hexSize[i] >= 'A') decSize[i] = hexSize[i] - 'A' + 10;
            else decSize[i] = hexSize[i] - '0';
        }
        toolFile.read();
        
        for (i=0;i<4;i++){
            binXSize[i] = hex2bin[decSize[0]][i];
            binXSize[i+4] = hex2bin[decSize[1]][i];
            binYSize[i] = hex2bin[decSize[2]][i];
            binYSize[i+4] = hex2bin[decSize[3]][i];
        }
        
        for (i=0;i<8;i++){
            sizeX += binXSize[i]*bin2dec[i];
            sizeY += binYSize[i]*bin2dec[i];
        }
        
        while (toolFile.available()) {
            
            pixCol      = 0;
            magnitude   = 0;
            
            for (i = 0; i < 6; i++) {
                item[i] = toolFile.read();
                if (item[i] == ',') i = 6;
                else magnitude++; item[i] -= '0';
            }
            toolFile.read();
            
            for (i = 0; i < magnitude; i++) pixCol = pixCol * 10 + item[i];
            
            if (pixCol == 0) myGLCD.setColor(icon->backColor);
            else myGLCD.setColor(pixCol);
            
            myGLCD.fillRect(icon->posX+pxlX, icon->posY+pxlY, icon->posX+pxlX+icon->scale-1, icon->posY+pxlY+icon->scale-1);
            
            pxlY += icon->scale;
            
            if (pxlY >= sizeY * icon->scale) {
                pxlY = 0;
                pxlX += icon->scale;
                
                if (pxlX >= sizeX * icon->scale) break;
            }
        }
        
        toolFile.close();
    }
    else {
        //Serial.println("File could not be opened");
    }
}
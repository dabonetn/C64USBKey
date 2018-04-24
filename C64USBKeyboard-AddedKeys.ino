// C64 USB Keyboard mod 
// Original by Pyofer 
// See original thread @ 
// http://www.lemon64.com/forum/viewtopic.php?t=55650 
// 
// Modified to Support restore key & US/EU keymaps by dabone. 
// Wiring is as follows 
// 64 Keyboard connector has 20 Pins with a key Pin @ Pin 2. 
// Arduino Pro Micro Pinout 
// https://cdn.sparkfun.com/assets/9/c/3/c/4/523a1765757b7f5c6e8b4567.png 

// Board type should be Arduino Leonardo (or the Pro mini clones) 


//KeyBoard Arduino Pro Micro 
//	Pin     Pin     Label

//   20       5       2 - SDA
//   19       6       3 - SCL
//   18       7       4 - A6
//   17       8       5
//   16       9       6 - A7
//   15       10      7 - 
//   14       11      8 - A8 
//   13       12      9 - A9 
//   12       13      10 - A10 
//   11       14      16 - MOSI 
//   10       15      14 - MISO 
//   9        16      15 - SCLK 
//   8        17      18 - A0 
//   7        18      19 - A1 
//   6        19      20 - A2 
//   5        20      21 - A3 
//   4        N/C
//   3        1       1 - TX
//   2        N/C
//   1        11      8 - A8 

//   Joysticks
//	Pins 1,2,3,4,6 Are Both Joysticks to the same input.
//	 1        5       2 - SDA
//	 2        6       3 - SCL
//	 3        7       4 - A6
//	 4        8       5
//	 6        9       6 - A7
//  Joy 1
//	 8        1       1 - TX
//  Joy 2
//	 8        2       2 - RX


// Keyboard Matrix Now Matches real C64 with one more column.
// Matrix can be found at 
// http://sta.c64.org/cbm64kbdlay.html

// For Standard Keys See
// http://www.asciitable.com/
// 
// Arduino Keyboard Modifier Keys
// Key                  Decimal value
// KEY_LEFT_CTRL            128
// KEY_LEFT_SHIFT           129
// KEY_LEFT_ALT             130
// KEY_LEFT_GUI             131
// KEY_RIGHT_CTRL           132
// KEY_RIGHT_SHIFT          133
// KEY_RIGHT_ALT            134
// KEY_RIGHT_GUI            135
// KEY_RETURN               176
// KEY_ESC                  177
// KEY_BACKSPACE            178
// KEY_TAB                  179
// KEY_CAPS_LOCK            193
// KEY_F1                   194
// KEY_F2                   195
// KEY_F3                   196
// KEY_F4                   197
// KEY_F5                   198
// KEY_F6                   199
// KEY_F7                   200
// KEY_F8                   201
// KEY_F9                   202
// KEY_F10                  203
// KEY_F11                  204
// KEY_F12                  205
// PrintScrn                206
// Scrlk                    207
// Pause                    208
// KEY_INSERT               209
// KEY_HOME                 210
// KEY_PAGE_UP              211
// KEY_END                  213
// KEY_DELETE               212
// KEY_PAGE_DOWN            214
// KEY_RIGHT_ARROW          215
// KEY_LEFT_ARROW           216
// KEY_DOWN_ARROW           217
// KEY_UP_ARROW             218
// Keypad /                 220
// Keypad *                 221
// Keypad -                 222
// Keypad +                 223
// Keypad ENTER             224
// Keypad 1 & End           225
// Keypad 2 & Down Arrow    226
// Keypad 3 & PageDn        227
// Keypad 4 & Left Arrow    228
// Keypad 5                 229
// Keypad 6 & Right Arrow   230
// Keypad 7 & Home          231
// Keypad 8 & Up Arrow      232
// Keypad 9 & PageUp        233
// Keypad 0 & Insert        234
// Keypad . & Delete        235


#include <HID.h> 
#include <Keyboard.h> 

int inChar=0;
int keyPos=0;
int digitalread=0;
int keyDown[80];
long lastDebounceTime[80];
int debounceDelay=50;
int shift=0;
int outPin=2;
int outPinSet=0;
int i;
int windowsShift;
int DefaultKBMode=0;                              // Select 0 For Windows Mode On startup or 1 for C64 Mode
int USKeyboard=1;                                 // Select 1 for US Keyboard or 0 For EU
int HybridKeyboard=1;                             // Select 0 for normal or 1 for the left shift key allowing all f keys and cursor keys in windows mode. (Also has a shifted restore key)



char keyMapUS[240]={

212,176,215,200,194,196,198,217,0,0,                //  Del Return LR F7 F1 F3 F5 UD Null Null
51,119,97,52,122,115,101,129,226,221,                   //  3 W A 4 Z S E LSHFT KP2 Kp*
53,114,100,54,99,102,116,120,228,222,                   //  5 R D 6 C F T X KP4 KP-
55,121,103,56,98,104,117,118,230,223,                   //  7 Y G 8 B H U V KP6 KP+
57,105,106,48,109,107,111,110,132,207,                  //  9 I J Zero M K O N Rcrtl ScrLk
43,112,108,45,46,58,64,44,0,0,                      //  + P L - . : @ , Null Null    (US Keyboard)
35,42,59,210,133,61,211,47,205,0,                   //  Pound * ; Home RSHFT = Pi / Restore Null
49,178,128,50,32,135,113,177,232,220,                   //  1 BS CTRL 2 SPC C= Q RunStop KP8 KP/

209,176,216,201,195,197,199,218,0,0,                //  Del Return LR F8 F2 F4 F6 UD Null Null
92,87,65,36,90,83,69,129,226,221,                       //  # W A $ Z S E LSHFT KP2 Kp*
37,82,68,38,67,70,84,88,228,222,                        //  % R D & C F T X KP4 KP-
39,89,71,40,66,72,85,86,230,223,                        //  ' Y G ( B H U V KP6 KP+
41,73,74,48,77,75,79,78,132,207,                        //  ) I J Zero M K O N Rcrtl ScrLk
43,80,76,95,62,91,96,60,0,0,                        //  + P L - > : @ < Null Null
35,42,93,210,133,61,214,63,205,0,                   //  Pound * ; Home RSHFT = Pi ? Restore Null
33,178,128,34,32,135,81,177,232,220,                    //  ! BS CTRL " SPC C= Q RS KP8 KP/     (US Keyboard)

178,176,215,200,194,196,198,217,0,0,                //  Del Return LR F7 F1 F3 F5 UD Null Null
51,119,97,52,122,115,101,129,226,221,                   //  3 W A 4 Z S E LSHFT KP2 Kp*
53,114,100,54,99,102,116,120,228,222,                   //  5 R D 6 C F T X KP4 KP-
55,121,103,56,98,104,117,118,230,223,                   //  7 Y G 8 B H U V KP6 KP+
57,105,106,48,109,107,111,110,132,207,                  //  9 I J Zero M K O N Rcrtl ScrLk
45,112,108,61,46,59,91,44,0,0,                      //  + P L - . : @ , Null Null
209,93,39,210,133,92,212,47,205,0,                  //  Pound * ; Home RSHFT = Pi / Restore Null
49,223,9,50,32,128,113,177,232,220,                     //  1 BS CTRL 2 SPC C= Q RunStop KP8 KP/

};

char keyMapEU[240]={
212,176,215,200,194,196,198,217,0,0,                //  Del Return LR F7 F1 F3 F5 UD Null Null
51,119,97,52,122,115,101,129,226,221,                   //  3 W A 4 Z S E LSHFT KP2 Kp*
53,114,100,54,99,102,116,120,228,222,                   //  5 R D 6 C F T X KP4 KP-
55,121,103,56,98,104,117,118,230,223,                   //  7 Y G 8 B H U V KP6 KP+
57,105,106,48,109,107,111,110,132,207,                  //  9 I J Zero M K O N Rcrtl ScrLk
43,112,108,45,46,58,34,44,0,0,                      //  + P L - . : @ , Null Null     (EU Keyboard)
35,42,59,210,133,61,211,47,205,0,                   //  Pound * ; Home RSHFT = Pi / Restore Null
49,178,128,50,32,135,113,177,232,220,                   //  1 BS CTRL 2 SPC C= Q RunStop KP8 KP/

209,176,216,201,195,197,199,218,0,0,                //  Del Return LR F8 F2 F4 F6 UD Null Null
92,87,65,36,90,83,69,129,226,221,                       //  # W A $ Z S E LSHFT KP2 Kp*
37,82,68,38,67,70,84,88,228,222,                        //  % R D & C F T X KP4 KP-
39,89,71,40,66,72,85,86,230,223,                        //  ' Y G ( B H U V KP6 KP+
41,73,74,48,77,75,79,78,132,207,                        //  ) I J Zero M K O N Rcrtl ScrLk
43,80,76,95,62,91,96,60,0,0,                        //  + P L - > : @ < Null Null
35,42,93,210,133,61,214,63,205,0,                   //  Pound * ; Home RSHFT = Pi ? Restore Null
33,178,128,64,32,135,81,177,232,220,                    //  ! BS CTRL " SPC C= Q RS KP8 KP/    (EU Keyboard)

178,176,215,200,194,196,198,217,0,0,                //  Del Return LR F7 F1 F3 F5 UD Null Null
51,119,97,52,122,115,101,129,226,221,                   //  3 W A 4 Z S E LSHFT KP2 Kp*
53,114,100,54,99,102,116,120,228,222,                   //  5 R D 6 C F T X KP4 KP-
55,121,103,56,98,104,117,118,230,223,                   //  7 Y G 8 B H U V KP6 KP+
57,105,106,48,109,107,111,110,132,207,                  //  9 I J Zero M K O N Rcrtl ScrLk
45,112,108,61,46,59,91,44,0,0,                      //  + P L - . : @ , Null Null
209,93,39,210,133,92,212,47,205,0,                  //  Pound * ; Home RSHFT = Pi / Restore Null
49,223,9,50,32,128,113,177,232,220,                     //  1 BS CTRL 2 SPC C= Q RunStop KP8 KP/

};

char Hybridkeys[7]{
                                                  // Hybrid Keys. These are the shifted values.  
216,201,195,197,199,218,205,                      // LR F8 F2 F4 F6 UD Restore
};
void setup() {
  Keyboard.begin();// initialize control over the keyboard:
  
  for (i=0; i<80; i++) keyDown[i]=0; // Set all keys as up
  
  pinMode(2,OUTPUT);  // configure inputs and outputs
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);

  pinMode(10,INPUT_PULLUP); // use internal pullups to hold pins high
  pinMode(16,INPUT_PULLUP);
  pinMode(15,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  pinMode(A0,INPUT_PULLUP);
  pinMode(A1,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);
  pinMode(A3,INPUT_PULLUP);
  pinMode(1,INPUT_PULLUP);
  pinMode(0,INPUT_PULLUP);
  
  digitalWrite(2,LOW);  // start with one active pin to detect '1'
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  
  if (DefaultKBMode==1)
  {  
  if (!digitalRead(10)) windowsShift=1; else windowsShift=0; // detect if '1' is held on power up to swap mode
  }
  if (DefaultKBMode==0)
  {
  if (!digitalRead(10)) windowsShift=0; else windowsShift=1; // detect if '1' is held on power up to swap mod
  }
 }

void loop() // main keyboard scanning loop
{
 
  for (outPin=2;outPin<10; outPin++) // scan through all rows
    {
    pinMode(2,INPUT);  // set unused (all) outputs to input to avoid ghosting
    pinMode(3,INPUT);
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    pinMode(6,INPUT);
    pinMode(7,INPUT);
    pinMode(8,INPUT);
    pinMode(9,INPUT);
    
    // pinMode(outPin,OUTPUT);  // select output to activate
    // digitalWrite(outPin,LOW); // set it as low, a pressed key will be pulled to ground
    // Changed order to match real c64 keyboard matrix layout.
    
    if (outPin==2) pinMode (9,OUTPUT);digitalWrite(9,LOW);outPinSet=9;
    if (outPin==3) pinMode (3,OUTPUT);digitalWrite(3,LOW);outPinSet=3;
    if (outPin==4) pinMode (4,OUTPUT);digitalWrite(4,LOW);outPinSet=4;
    if (outPin==5) pinMode (5,OUTPUT);digitalWrite(5,LOW);outPinSet=5;
    if (outPin==6) pinMode (6,OUTPUT);digitalWrite(6,LOW);outPinSet=6;
    if (outPin==7) pinMode (7,OUTPUT);digitalWrite(7,LOW);outPinSet=7;
    if (outPin==8) pinMode (8,OUTPUT);digitalWrite(8,LOW);outPinSet=8;
    if (outPin==9) pinMode (2,OUTPUT);digitalWrite(2,LOW);outPinSet=2;
    
    
    for (i=0; i<10; i++) // scan through columns
      {
      keyPos=i+((outPin-2)*10); // calculate character map position
      if (USKeyboard==1)
      {
      if (!windowsShift) inChar=keyMapUS[keyPos+shift]; // work out which key it is from the map and shift if needed
       else inChar=keyMapUS[keyPos+160];  // use "windows" keymap where shift is passed through
      }
      if (USKeyboard==0)
      {
      if (!windowsShift) inChar=keyMapEU[keyPos+shift]; // work out which key it is from the map and shift if needed
       else inChar=keyMapEU[keyPos+160];  // use "windows" keymap where shift is passed through
      }
      if (i==0) digitalread=1-digitalRead(10); // check the active input pin
      if (i==1) digitalread=1-digitalRead(16);
      if (i==2) digitalread=1-digitalRead(14);
      if (i==3) digitalread=1-digitalRead(A3);
      if (i==4) digitalread=1-digitalRead(A0);
      if (i==5) digitalread=1-digitalRead(A1);
      if (i==6) digitalread=1-digitalRead(A2);
      if (i==7) digitalread=1-digitalRead(15);
      if (i==8) digitalread=1-digitalRead(1);
      if (i==9) digitalread=1-digitalRead(0);
 
 if (HybridKeyboard==1)
 {
                  if ((millis()-lastDebounceTime[keyPos])>debounceDelay) // debounce for each key individually
                     {
                       if (digitalread==1 && keyDown[keyPos]==0) // if a key is pressed and wasn't already down
                       {
                        keyDown[keyPos]=inChar;        // put the right character in the keydown array
                        if (keyDown[17]&&keyDown[2])  {Keyboard.release (keyDown[17]);keyDown[keyPos]=Hybridkeys[0];}
                        if (keyDown[17]&&keyDown[3])  {Keyboard.release (keyDown[17]);keyDown[keyPos]=Hybridkeys[1];}
                        if (keyDown[17]&&keyDown[4])  {Keyboard.release (keyDown[17]);keyDown[keyPos]=Hybridkeys[2];}
                        if (keyDown[17]&&keyDown[5])  {Keyboard.release (keyDown[17]);keyDown[keyPos]=Hybridkeys[3];}
                        if (keyDown[17]&&keyDown[6])  {Keyboard.release (keyDown[17]);keyDown[keyPos]=Hybridkeys[4];}
                        if (keyDown[17]&&keyDown[7])  {Keyboard.release (keyDown[17]);keyDown[keyPos]=Hybridkeys[5];}
                        if (keyDown[17]&&keyDown[66]) {Keyboard.release (keyDown[17]);keyDown[keyPos]=Hybridkeys[6];}
        
        if ((keyPos!=17&&keyPos!=64)||windowsShift==1)// is it not-shift or in windows mode?
          {  // if so pass the key through
          lastDebounceTime[keyPos] = millis(); // reset the debounce delay
          Keyboard.press(keyDown[keyPos]);    // pass the keypress to windows
          }
        else { lastDebounceTime[keyPos]=millis(); shift=80; } // reset keybounce delay and mark as shift press
        }
      if (digitalread==0 && keyDown[keyPos]!=0) // key is up and a character is stored in the keydown position
        {
        if ((keyPos!=17&&keyPos!=64)||windowsShift==1) // not-shift or windows mode
          {
          lastDebounceTime[keyPos] = millis();  // reset keybounce delay
          Keyboard.release(keyDown[keyPos]);    // pass key release to windows
          }
          else { lastDebounceTime[keyPos]=millis(); shift=0; } // reset keybounce delay and mark as un-shifted
        keyDown[keyPos]=0; // set keydown array position as up
        }
   }
  }
  if (HybridKeyboard==0)
  {
    if ((millis()-lastDebounceTime[keyPos])>debounceDelay) // debounce for each key individually
   {
      if (digitalread==1 && keyDown[keyPos]==0) // if a key is pressed and wasn't already down
        {
        keyDown[keyPos]=inChar;        // put the right character in the keydown array
        if ((keyPos!=17&&keyPos!=64)||windowsShift==1)// is it not-shift or in windows mode?
          {  // if so pass the key through
          lastDebounceTime[keyPos] = millis(); // reset the debounce delay
          Keyboard.press(keyDown[keyPos]);    // pass the keypress to windows
          }
        else { lastDebounceTime[keyPos]=millis(); shift=80; } // reset keybounce delay and mark as shift press
        }
      if (digitalread==0 && keyDown[keyPos]!=0) // key is up and a character is stored in the keydown position
        {
        if ((keyPos!=17&&keyPos!=64)||windowsShift==1) // not-shift or windows mode
          {
          lastDebounceTime[keyPos] = millis();  // reset keybounce delay
          Keyboard.release(keyDown[keyPos]);    // pass key release to windows
          }
          else { lastDebounceTime[keyPos]=millis(); shift=0; } // reset keybounce delay and mark as un-shifted
        keyDown[keyPos]=0; // set keydown array position as up
        }
   }
  }
  
      }
  digitalWrite(outPinSet,HIGH); // set output back to high
 }

}

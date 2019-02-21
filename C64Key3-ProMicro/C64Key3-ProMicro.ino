// C64 USB Keyboard mod 
// Original by Pyofer 
// See original thread @ 
// http://www.lemon64.com/forum/viewtopic.php?t=55650 
// 
// Modified to Support restore key & US/EU keymaps by dabone. 
//
// Joystick support for keycodes added, and support for second fire button.
//
//
// Wiring is as follows 
// 64 Keyboard connector has 20 Pins with a key Pin @ Pin 2. 
// Arduino Pro Micro Pinout 
// https://cdn.sparkfun.com/assets/9/c/3/c/4/523a1765757b7f5c6e8b4567.png 
// Board type should be Arduino Leonardo (or the Pro mini clones) 

// Teensy LC Pinout
// https://www.pjrc.com/teensy/teensyLC.html

//KeyBoard Arduino Pro Micro                       Teensy LC
//  Pin     Pin     Label                             Pin 
//   20       5       2 - SDA                          5
//   19       6       3 - SCL                          6
//   18       7       4 - A6                           7
//   17       8       5                                8
//   16       9       6 - A7                           9
//   15       10      7 -                              10
//   14       11      8 - A8                           11
//   13       12      9 - A9                           12
//   12       13      10 - A10                         13
//   11       14      16 - MOSI                        14
//   10       15      14 - MISO                        15
//   9        16      15 - SCLK                        16
//   8        17      18 - A0                          17
//   7        18      19 - A1                          18
//   6        19      20 - A2                          19 
//   5        20      21 - A3                          20
//   4        N/C
//   3        1       1 - TX                           1
//   2        N/C
//   1        11      8 - A8                           8 




// The following pins are optional and not required for keyboard only usage.

//   Joysticks
//  Pins 1,2,3,4,6 Are Both Joysticks to the same input.
//   1        5       2 - SDA                          5 
//   2        6       3 - SCL                          6
//   3        7       4 - A6                           7
//   4        8       5                                8
//   6        9       6 - A7                           9
//   9        10      7 -                              10
//  Joy 1
//   8        1       1 - TX                           1
//  Joy 2
//   8        2       0 - RX                           2




#include <HID.h>                                    // Comment out for Teensey
#include <Keyboard.h>                               // Comment out for Teensey
#include "keydefs.h"                                // Comment out for Teensey

int inChar=0;
int keyPos=0;
int keyDown[80];
long lastDebounceTime[80];
int debounceDelay=50;
int shift=0;
int Row=0;
int i;
int HybridKeyboard=1;                                                         // Select 0 for normal or 1 for the left HybridKey allowing all f keys and cursor keys in windows mode. (Also has a shifted restore key)
int HybridKey=17;                                                             // Position of the Hybrid Select key in the keymap, Left Shift = 17, Right Shift = 64
int HybridKey2=72;                                                            // Position of the Hybrid2 Select key in the keymap, CTRL = 72
int RowPinMap[8] = {9, 3, 4, 5, 6, 7, 8, 2};                                // Convert Row number to associated output pin -- Arduino Pro Micro, comment out For Teensey LC
int ColPinMap[10] = {10, 16, 14, 21, 18, 19, 20, 15, 1, 0};                 // Convert Col number to associated input pin  -- Arduino Pro Micro, comment out For Teensey LC
//int RowPinMap[8] = {12, 6, 7, 8, 9, 10, 11, 5};                               // Convert Row number to associated output pin -- Uncomment For Teensey LC
//int ColPinMap[10] = {13, 14, 15, 20, 17, 18, 19, 16, 1, 2};                   // Convert Col number to associated input pin  -- Uncomment For Teensey LC




// Keyboard Matrix Now Matches real C64 with two more columns.
// Matrix can be found at 
// http://sta.c64.org/cbm64kbdlay.html


uint16_t keyMap[80]={
//Keymap                                              // Joystick Keys are the indicated by JoyNumber(Direction), i.e Joy1UP is up on joystick one.
                                                      // Firebuttons are Noted JoyF(Joystick number)- Fire Button number, i.e Joy2F1 Joystick 2, fire button 1, Joy2F2 Joystick 2, fire button 2.
                                                      // Current values for the Joysticks are located at the end of each line. KP stands for numeric keypad inputs.

KEY_BACKSPACE, KEY_ENTER,       KEY_RIGHT, KEY_F7,    KEY_F1,                  KEY_F3,           KEY_F5,         KEY_DOWN,          NULL,     NULL,            //  Del   Return LR   F7   F1    F3  F5  UD      Null      Null
KEY_3,         KEY_W,           KEY_A,     KEY_4,     KEY_Z,                   KEY_S,            KEY_E,          MODIFIERKEY_SHIFT, KEYPAD_2, KEYPAD_ASTERIX,  //  3     W      A    4    Z     S   E   LSHFT   Joy2Down  Joy1Down 
KEY_5,         KEY_R,           KEY_D,     KEY_6,     KEY_C,                   KEY_F,            KEY_T,          KEY_X,             KEYPAD_4, KEYPAD_MINUS,    //  5     R      D    6    C     F   T   X       Joy2Left  Joy1Left
KEY_7,         KEY_Y,           KEY_G,     KEY_8,     KEY_B,                   KEY_H,            KEY_U,          KEY_V,             KEYPAD_6, KEYPAD_PLUS,     //  7     Y      G    8    B     H   U   V       Joy2Right Joy1Right
KEY_9,         KEY_I,           KEY_J,     KEY_0,     KEY_M,                   KEY_K,            KEY_O,          KEY_N,             KEYPAD_5, KEY_SCROLL_LOCK, //  9     I      J    Zero M     K   O   N       Joy2F1    Joy1F1
KEY_MINUS,     KEY_P,           KEY_L,     KEY_EQUAL, KEY_PERIOD,              KEY_SEMICOLON,    KEY_LEFT_BRACE, KEY_COMMA,         KEYPAD_3, KEYPAD_1,        //  +     P      L    -    .     :   @   ,       Joy2F2    Joy1F2
KEY_INSERT,    KEY_RIGHT_BRACE, KEY_QUOTE, KEY_HOME,  MODIFIERKEY_RIGHT_SHIFT, KEY_BACKSLASH,    KEY_DELETE,     KEY_SLASH,         KEY_F10,  NULL,            //  Pound *      ;    Home RSHFT =   Pi  /       Restore   Null
KEY_1,         KEY_ESC,         KEY_TAB,   KEY_2,     KEY_SPACE,               MODIFIERKEY_CTRL, KEY_Q,          MODIFIERKEY_ALT,   KEYPAD_8, KEYPAD_SLASH,    //  1     BS     CTRL 2    SPC   C=  Q   RunStop Joy2Up    Joy1Up
};



uint16_t Hybridkeys[7]{
                                                                              // Hybrid Keys. These are the shifted values.  
                                                                              // These allow keys to be passed in windows mode that are not shifted values of the original key. 
                                                                              // This allows the windows mode to use the cursor keys and F1-F8. The cursor keys and F keys are default, The restore key is F10.
KEY_LEFT, KEY_F8, KEY_F2, KEY_F4, KEY_F6, KEY_UP, KEY_F12,                    // LR F8 F2 F4 F6 UD Restore
};

uint16_t Hybridkeys2[5]{
                                                                              // Hybrid Keys2. These are the shifted values.  Defaults to the CTRL Key
                                                                              // These allow keys to be passed in windows mode that are not shifted values of the original key. 
                                                                              // Page Down, F11, `, Page UP, F9 Mapped to F7,F1,F3,F8 and Restore
KEY_PAGE_DOWN, KEY_F11, KEY_TILDE, KEY_PAGE_UP, KEY_F9,
};

void setup() 
{
 
  Keyboard.begin();// initialize control over the keyboard

  for (i=0; i<80; i++) keyDown[i]=0; // Set all keys as up
  
  for (int Row=0; Row<8; Row++)
   pinMode(RowPinMap[Row], INPUT_PULLUP); 

  for (int Col=0; Col<10; Col++)
    pinMode(ColPinMap[Col],INPUT_PULLUP); // use internal pullups to hold pins high


}


void loop() // main keyboard scanning loop
{
  int InputValue; // Replaces digitalread, so that global variable can be removed
  int RowPin;     // Replaces rowPinSet, so that global variable can be removed

  for (int Row=0; Row<8; Row++) // scan through all rows
  {
    RowPin = RowPinMap[Row];  // Map logical row to output pin
    pinMode(RowPin,OUTPUT);   // Set output pin to OUTPUT
    digitalWrite(RowPin,LOW); // Set output pin to LOW
    
    for (int Col=0; Col<10; Col++) // scan through columns
    {
      keyPos=Col+(Row*10); // calculate character map position
      
      inChar=keyMap[keyPos];

      InputValue = digitalRead(ColPinMap[Col]); // LOW = Key pressed, HIGH = Key Not Pressed

      if ((millis()-lastDebounceTime[keyPos])>debounceDelay) // debounce for each key individually
      {
        if ((InputValue == LOW) && keyDown[keyPos]==0) // if a key is pressed and wasn't already down
        {
          keyDown[keyPos]=inChar;        // put the right character in the keydown array
          if (HybridKeyboard==1)
          {
            if (keyDown[HybridKey]&&keyDown[2])  {Keyboard.release (keyDown[HybridKey]);keyDown[keyPos]=Hybridkeys[0];}
            if (keyDown[HybridKey]&&keyDown[3])  {Keyboard.release (keyDown[HybridKey]);keyDown[keyPos]=Hybridkeys[1];}
            if (keyDown[HybridKey]&&keyDown[4])  {Keyboard.release (keyDown[HybridKey]);keyDown[keyPos]=Hybridkeys[2];}
            if (keyDown[HybridKey]&&keyDown[5])  {Keyboard.release (keyDown[HybridKey]);keyDown[keyPos]=Hybridkeys[3];}
            if (keyDown[HybridKey]&&keyDown[6])  {Keyboard.release (keyDown[HybridKey]);keyDown[keyPos]=Hybridkeys[4];}
            if (keyDown[HybridKey]&&keyDown[7])  {Keyboard.release (keyDown[HybridKey]);keyDown[keyPos]=Hybridkeys[5];}
            if (keyDown[HybridKey]&&keyDown[68]) {Keyboard.release (keyDown[HybridKey]);keyDown[keyPos]=Hybridkeys[6];}

            if (keyDown[HybridKey2]&&keyDown[3])  {Keyboard.release (keyDown[HybridKey2]);keyDown[keyPos]=Hybridkeys2[0];}
            if (keyDown[HybridKey2]&&keyDown[4])  {Keyboard.release (keyDown[HybridKey2]);keyDown[keyPos]=Hybridkeys2[1];}
            if (keyDown[HybridKey2]&&keyDown[5])  {Keyboard.release (keyDown[HybridKey2]);keyDown[keyPos]=Hybridkeys2[2];}
            if (keyDown[HybridKey2]&&keyDown[6])  {Keyboard.release (keyDown[HybridKey2]);keyDown[keyPos]=Hybridkeys2[3];}
            if (keyDown[HybridKey2]&&keyDown[68]) {Keyboard.release (keyDown[HybridKey2]);keyDown[keyPos]=Hybridkeys2[4];}
          }
          
            lastDebounceTime[keyPos] = millis(); // reset the debounce delay
            Keyboard.press(keyDown[keyPos]);    // pass the keypress to windows
        }
        if ((InputValue == HIGH) && keyDown[keyPos]!=0) // key is up and a character is stored in the keydown position
        {
            lastDebounceTime[keyPos] = millis();  // reset keybounce delay
            Keyboard.release(keyDown[keyPos]);    // pass key release to windows
  
          keyDown[keyPos]=0; // set keydown array position as up
        }
      }
    }
    digitalWrite(RowPin,HIGH);    // Set output pin to HIGH
    delay(1);                     // Delay to make sure it has time to go HIGH before switching to INPUT
    pinMode(RowPin,INPUT_PULLUP); // Set output pin back to INPUT with pullup to make sure it stays HIGH
  }
}

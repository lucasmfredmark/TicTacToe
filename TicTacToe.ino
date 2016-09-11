// include the LiquidCrystal library code
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

int ledStatus[] = { 3, 0, 0, 0, 0, 0, 0, 0, 0 }; // statuses of the LEDs
int ledCount = 9; // number of LEDs
int ledSelected = 0; // current selected LED
int playerTurn = 1; // which player's turn
int gameState = 1; // the game is ongoing or has ended
int playerWon = 0; // which player has won

const int buttonLeftPin = 11; // left button digital pin
const int buttonRightPin = 12; // right button digital pin
const int buttonSelectPin = 13; // select button digital pin

int buttonState = 0; // is a button being pressed
int buttonLeftState = 0; // is the left button being pressed
int buttonRightState = 0; // is the right button being pressed
int buttonSelectState = 0; // is the select button being pressed

void setup() {
  // set the digital pins 2-7 as output
  for (int i = 2; i < 8; i++) {
    pinMode(i, OUTPUT);
  }
  
  // set the analog pins A0-A5 as output
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  
  // set the digital pins 11-13 as input
  pinMode(buttonLeftPin, INPUT);
  pinMode(buttonRightPin, INPUT);
  pinMode(buttonSelectPin, INPUT);
  
  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  // print a message to the LCD
  lcdDisplay("Player " + String(playerTurn) + " turn", "Choose an LED ->");
}

void loop() {
  // clear the statuses of the LEDs
  clearLedStatus();
  
  // read the values from the button pins
  buttonLeftState = digitalRead(buttonLeftPin);
  buttonRightState = digitalRead(buttonRightPin);
  buttonSelectState = digitalRead(buttonSelectPin);
  
  if (buttonState == 0) { // if a button is not being pressed
    if (gameState == 1) { // if the game is ongoing
      if (buttonLeftState == HIGH) { // if the left button is being pressed
        selectPrevLed(); // select the previous LED
        buttonState = 1; // a button is being pressed
      } else if (buttonRightState == HIGH) { // if the right button is being pressed
        selectNextLed(); // select the next LED
        buttonState = 1; // a button is being pressed
      } else if (buttonSelectState == HIGH) { // if the select button is being pressed
        ledStatus[ledSelected] = playerTurn; // lock the selected LED with the current player color
        
        // check all possibilities for player 1 win
        if ((ledStatus[0] == 1 && ledStatus[1] == 1 && ledStatus[2] == 1) ||
            (ledStatus[3] == 1 && ledStatus[4] == 1 && ledStatus[5] == 1) ||
            (ledStatus[6] == 1 && ledStatus[7] == 1 && ledStatus[8] == 1) ||
            (ledStatus[0] == 1 && ledStatus[3] == 1 && ledStatus[6] == 1) ||
            (ledStatus[1] == 1 && ledStatus[4] == 1 && ledStatus[7] == 1) ||
            (ledStatus[2] == 1 && ledStatus[5] == 1 && ledStatus[8] == 1) ||
            (ledStatus[0] == 1 && ledStatus[4] == 1 && ledStatus[8] == 1) ||
            (ledStatus[2] == 1 && ledStatus[4] == 1 && ledStatus[6] == 1)) {
          playerWon = 1; // player 1 has won
          gameState = 0; // end the game
          lcdDisplay("Player 1 has won", "Play Again?"); // print a message to the LCD
        } else if (
        // check all possibilities for player 2 win
            (ledStatus[0] == 2 && ledStatus[1] == 2 && ledStatus[2] == 2) ||
            (ledStatus[3] == 2 && ledStatus[4] == 2 && ledStatus[5] == 2) ||
            (ledStatus[6] == 2 && ledStatus[7] == 2 && ledStatus[8] == 2) ||
            (ledStatus[0] == 2 && ledStatus[3] == 2 && ledStatus[6] == 2) ||
            (ledStatus[1] == 2 && ledStatus[4] == 2 && ledStatus[7] == 2) ||
            (ledStatus[2] == 2 && ledStatus[5] == 2 && ledStatus[8] == 2) ||
            (ledStatus[0] == 2 && ledStatus[4] == 2 && ledStatus[8] == 2) ||
            (ledStatus[2] == 2 && ledStatus[4] == 2 && ledStatus[6] == 2)) {
          playerWon = 2; // player 2 has won
          gameState = 0; // end the game
          lcdDisplay("Player 2 has won", "Play Again?"); // print a message to the LCD
        } else {
        // if no one has won
          // count remaining LEDs
          int count = 0;
          for (int i = 0; i < ledCount; i++) {
            if (ledStatus[i] == 0) {
              count++;
            }
          }
          
          if (count > 0) {
            // if there are remaining LEDs
            playerTurn = (playerTurn == 1) ? 2 : 1; // switch player turn
            lcdDisplay("Player " + String(playerTurn) + " turn", "Choose an LED ->"); // print a message to the LCD
          } else {
            // if there are no remaining LEDs
            gameState = 0; // end the game
            lcdDisplay("It's a draw!", "Play Again?"); // print a message to the LCD
          }
        }
        
        // if the game is ongoing
        if (gameState == 1) {
          // select the next possible LED
          selectNextLed();
        }
        
        buttonState = 1; // a button is being pressed
      }
    } else if (gameState == 0 && (buttonLeftState == HIGH || buttonRightState == HIGH || buttonSelectState == HIGH)) {
      // if the game has ended, and a button is being pressed
      
      // set every LED's status to 0
      for (int i = 0; i < ledCount; i++) {
        ledStatus[i] = 0;
      }
      
      ledStatus[0] = 3; // set the status of the first LED to 3
      ledSelected = 0; // select the first LED
      playerTurn = (playerTurn == 1) ? 2 : 1; // switch starting player
      gameState = 1; // start a new game
      playerWon = 0; // no one has won
      lcdDisplay("Player " + String(playerTurn) + " turn", "Choose an LED ->"); // print a message to the LCD
      
      buttonState = 1; // a button is being pressed
    }
  } else if (buttonState == 1 && buttonLeftState == LOW && buttonRightState == LOW && buttonSelectState == LOW) {
    // if a button is no longer being pressed
    buttonState = 0; // a button is not being pressed
  }
}

int i = 0;
int j = 0;
int k = 0;

void clearLedStatus() {
  // turn off all LEDs
  for (int i = 2; i < 8; i++) {
    digitalWrite(i, LOW);
  }
  
  // set all LED's with a status of 3 to 0, if they are not the selected LED
  for (int i = 0; i < ledCount; i++) {
    if (ledStatus[i] == 3 && ledSelected != i) {
      ledStatus[i] = 0;
    }
  }
  
  // turn on red/green/both colors
  if (ledStatus[i] == 1) { // if current LED has a status of 1
    digitalWrite(2, HIGH); // turn on the red color
  } else if (ledStatus[i] == 2) { // if current LED has a status of 2
    digitalWrite(3, HIGH); // turn on the green color
  } else if (ledStatus[i] == 3) { // if current LED has a status of 3
    // turn on both colors
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
  }
  
  // turn on the current LED
  if (i == 0) { // if the current LED is LED 1
    ledWrite(0, 0, 0, 0); // turn on LED 1
  } else if (i == 1) {
    ledWrite(1, 0, 0, 0);
  } else if (i == 2) {
    ledWrite(0, 1, 0, 0);
  } else if (i == 3) {
    ledWrite(1, 1, 0, 0);
  } else if (i == 4) {
    ledWrite(0, 0, 1, 0);
  } else if (i == 5) {
    ledWrite(1, 0, 1, 0);
  } else if (i == 6) {
    ledWrite(0, 1, 1, 0);
  } else if (i == 7) {
    ledWrite(1, 1, 1, 0);
  } else if (i == 8) {
    ledWrite(0, 0, 0, 1);
  }
  
  // set the current LED to the next LED
  if (i == (ledCount - 1)) {
    i = 0;
  } else {
    i++;
  }
  
  if (gameState == 1) {
    // if the game is ongoing
    
    if (j == 200) {
      j = 0;
      
      // turn selected LED on/off
      if (ledStatus[ledSelected] == 3) {
        ledStatus[ledSelected] = 0;
      } else {
        ledStatus[ledSelected] = 3;
      }
    } else {
      j++;
    }
  } else if (gameState == 0) {
    // if the game has ended
    
    if (k == 200) {
      k = 0;
      
      // if a player has won
      if (playerWon != 0) {
        // if the LEDs are turned on
        if (ledStatus[0] == playerWon) {
          // turn off all the LEDs
          for (int i = 0; i < ledCount; i++) {
            ledStatus[i] = 0;
          }
        } else {
          // turn on all the LEDs in winning color
          for (int i = 0; i < ledCount; i++) {
            ledStatus[i] = playerWon;
          }
        }
      }
    } else {
      k++;
    }
  }
  
  delay(1);
}

// select the next possible LED
void selectNextLed() {
  // set the current selected LED to the next LED
  if (ledSelected == (ledCount - 1)) {
    ledSelected = 0;
  } else {
    ledSelected++;
  }
  
  // while the new selected LED is locked, select the next LED
  while (ledStatus[ledSelected] == 1 || ledStatus[ledSelected] == 2) {
    if (ledSelected == (ledCount - 1)) {
      ledSelected = 0;
    } else {
      ledSelected++;
    }
  }
  
  ledStatus[ledSelected] = 3; // set the status for the selected LED to 3
}

// select the previous possible LED
void selectPrevLed() {
  if (ledSelected == 0) {
    ledSelected = (ledCount - 1);
  } else {
    ledSelected--;
  }
  
  while (ledStatus[ledSelected] == 1 || ledStatus[ledSelected] == 2) {
    if (ledSelected == 0) {
      ledSelected = (ledCount - 1);
    } else {
      ledSelected--;
    }
  }
  
  ledStatus[ledSelected] = 3;
}

// turn on an LED using the 4-to-16 decoder
void ledWrite(int a0, int a1, int a2, int a3) {
  digitalWrite(4, a0);
  digitalWrite(5, a1);
  digitalWrite(6, a2);
  digitalWrite(7, a3);
}

// print a message to the LCD
void lcdDisplay(String topText, String bottomText) {
  lcd.clear(); // clear the display
  lcd.print(topText); // print text on the first line
  lcd.setCursor(0, 1); // set cursor to the next line
  lcd.print(bottomText); // print text on the second line
}

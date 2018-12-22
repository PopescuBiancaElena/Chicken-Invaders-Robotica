#include "LedControl.h"
#include "LiquidCrystal.h"

#define joyY A1
#define button 2
#define V0_PIN 9
#define LED_PIN 13

LedControl lc = LedControl(12, 11, 10, 1);
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

int column = 0, row = 0;

//joystick
int valY;
int posY = 0, prevY = 0;
const int valMax = 600;
const int valMin = 400;
int conditionButton = 1;
byte buttonValue = false;

//glont
int bulletX = 5;
int bulletY = 0;
bool isBulletGenerated = false;

//conditiile folosite pentru millis-1
int bulletInterval = 200;
long long previousBulletTime = 0;

//conditiile folosite pentru millis-2
int eggsInterval = 150;
long long prevTime = 0;

//conditii folosite pentru millis-3
int navaInterval = 150;
long long prevTime2 = 0;

//conditiile pentru millis level 2
int eggsInterval2 = 100;
long long prevTime3 = 0;

//level3- trebuie sa se stinga rand cu rand ca sa poti termina nivelul
int row1= 0, row2 = 0, row3 = 0;

//viata si scorul
int life = 2;
int score = 0;

//conditie pentru a restarta jocul
int restart = 0;

//daca fall = 0 cad ouale, iar cand viata ajunge la 0 fall = 1, iar ouale nu mai cad pana jucatorul nu alege
int fall = 1;

//de la ce nivel incepe jocul
int level = 1;

//numarul gainilor care au fost impuscate- tinem cont de asta pentru a putea trece la nivelul urmator
int number = 0, number1 = 0, number2 = 0;

bool startGame = false;
bool state = false;

boolean matrix1[8][8]={ {1,0,1,0,1,0,1,0},
                        {0,1,0,1,0,1,0,1},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0}};
                        
boolean matrix2[8][8]={ {1,1,1,1,1,1,1,1},
                        {0,1,1,1,1,1,1,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0}};

boolean matrix3[8][8]={{0,0,0,1,0,0,0,0},
                       {0,0,1,1,1,0,0,0},
                       {0,1,1,1,1,1,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}};
boolean matrixCopie1[8][8];
boolean matrixCopie2[8][8];
boolean matrixCopie3[8][8];

void setup() {
  pinMode(button, INPUT_PULLUP);
  pinMode(V0_PIN, OUTPUT);
  analogWrite(V0_PIN, 90);
  lcd.begin(16, 2);
  lcd.clear();
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  randomSeed(analogRead(A2));
}
void DisplayBeginning() {
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  lcd.setCursor(0, 1);
  lcd.print("Press Button");
}
void DisplayRestart() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Left - Again");
  lcd.setCursor(0, 1);
  lcd.print("Right - Exit Game");
}
//afisarea celor 3 matrici in functie de nivel
void Display() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      if (level == 1) {
        if (matrix1[row][column] == 1) {
          lc.setLed(0, column, row, HIGH);
          matrixCopie1[row][column] = matrix1[row][column];
        }
      }
      if (level == 2) {
        if (matrix2[row][column] == 1) {
          lc.setLed(0, column, row, HIGH);
          matrixCopie2[row][column] = matrix2[row][column];
        }
      }
      if (level == 3) {
        if (matrix3[row][column] == 1) {
          lc.setLed(0, column, row, HIGH);
          matrixCopie3[row][column] = matrix3[row][column];
        }
      }
    }
  }
}
void Display2() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      if (level == 1) {
        if (matrixCopie1[row][column] == 1) {
          lc.setLed(0, column, row, HIGH);
          matrix1[row][column] = matrixCopie1[row][column];
        }
      }
      if (level == 2) {
        if (matrixCopie2[row][column] == 1) {
          lc.setLed(0, column, row, HIGH);
          matrix2[row][column] = matrixCopie2[row][column];
        }
      }
      if (level == 3) {
        if (matrixCopie3[row][column] == 1) {
          lc.setLed(0, column, row, HIGH);
          matrix3[row][column] = matrixCopie3[row][column];
        }
      }
    }
  }
}
void DisplayFinal() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Finish!");
  lc.shutdown(0, true);
}
//restartarea jocului, la alegere dupa ce s-a ajuns la viata egal 0
void Restart() { 
  fall = 0;
  DisplayRestart();
  valY = analogRead(joyY);
  //restart = 0
  if (valY > valMax) {
    lcd.clear();
    Display2();
    life = 2;
    score = 0;
    restart = 0;
    if (level == 1)
      number = 0;
    if (level == 2)
      number1 = 0;
    if (level == 3)
      number2 = 0;
    fall = 1;
  }
  //restart = 1
  if (valY < valMin) {
    level = 4;
    lcd.clear();
    restart = 1;
  }
}
void PressButton() {
  if (life != 0) {
    lcd.setCursor(0, 0);
    lcd.print((String)
      "Level: " + level);
    lcd.setCursor(0, 1);
    lcd.print((String)
      "Life:" + life);
    lcd.setCursor(8, 1);
    lcd.print((String)
      "Score:" + score);
  }
  //stingerea gainilor de la leve 1 dupa ce au fost impuscate
  if (isBulletGenerated == true) {
    lc.setLed(0, bulletY, bulletX, true);
    if (level == 1) {
      if (matrix1[bulletX][bulletY] == 1) {
        matrix1[bulletX][bulletY] = 0;
        score++;
        number++;
      }
    }
    //stingerea gainilor de la level 2 dupa ce au fost impuscate
    if (level == 2) {
      if (state == false)
        if (matrix2[bulletX][bulletY] == 1) {
          matrix2[bulletX][bulletY] = 0;
          number1++;
          score++;
          state = true;
        }
      if (bulletX == 5 && state == true)
        state = false;
    }
    //stingerea gainilor de pe level 3 dupa ce au fost impuscate
    if (level == 3) {
      if ((matrix3[2][bulletY] == 1 && matrix3[1][bulletY] == 1) || (matrix3[2][bulletY] == 1 && matrix3[1][bulletY] == 0)) {
        matrix3[2][bulletY] = 0;
        number2++;
        score++;
        row1++;
      }
      if (row1 == 5) {
        if (matrix3[1][bulletY] == 1) {
          matrix3[1][bulletY] = 0;
          number2++;
          score++;
          row2++;
        }
      }
      if (row2 == 3) {
        if (matrix3[0][bulletY] == 1) {
          matrix3[0][bulletY] == 0;
          number2++;
          score++;
          row3++;
        }
      }
      if (row3 == 1)
        DisplayFinal();
    }
    if (millis() - previousBulletTime > bulletInterval) {
      lc.setLed(0, bulletY, bulletX, false);
      bulletX--;
      previousBulletTime = millis();
    }
    if (bulletX <= -1) {
      isBulletGenerated = false;
      bulletX = 5;
    }
  }
  if (digitalRead(button) == LOW && isBulletGenerated == false) {
    isBulletGenerated = true;
    bulletY = posY;
    bulletX = 5;
  }
}
void EggsLevel() {
  if (fall = 1) {
    if (level == 1 || level == 3) {
      if (millis() - prevTime > eggsInterval) {
        lc.setLed(0, column, row, LOW);
        lc.setLed(0, column, ++row, HIGH);
        if (column == posY && row == 6) 
        //daca jucatorul este atins de un ou viata acestuia se va reduce
          life = life - 1;
        prevTime = millis();
      }
    }
    if (level == 2) {
      if (millis() - prevTime3 > eggsInterval2) {
        lc.setLed(0, column, row, LOW);
        lc.setLed(0, column, ++row, HIGH);
        if (column == posY && row == 6) 
          life = life - 1;
        prevTime3 = millis();
      }
    }
    //ouale vor aparea random, dar doar de la gainile care sunt inca in viata
    if (row > 7) {
      if (level == 1) {
        column = random(8);
        while (matrix1[1][column] == 0 && matrix1[0][column] == 0)
          column = random(8);
        row = 0;
      }
      if (level == 2) {
        column = random(8);
        while (matrix2[1][column] == 0 && matrix2[0][column] == 0)
          column = random(8);
        row = 0;
      }
    }
    if (row > 7) {
      if (level == 3) {
        column = random(8);
        while (matrix3[1][column] == 0 && matrix3[0][column] == 0)
          column = random(8);
        row = 1;
      }
    }
  }
}
void JoyStick() {
  if (millis() - prevTime2 > navaInterval) {
    valY = analogRead(joyY);
    if (valY < valMin) {
      prevY = posY;
      if (posY > 0)
        posY--;
    } else
    if (valY > valMax) {
      prevY = posY;
      if (posY < 7)
        posY++;
    }
    if (posY < 0)
      posY = 7;
    if (posY > 7)
      posY = 0;
    prevTime2 = millis();
  }
}
void loop() {
  //jocul porneste doar prin apasarea butonului
  if (conditionButton == 1)
    DisplayBeginning();
  buttonValue = true;
  buttonValue = digitalRead(button);
  if (buttonValue == LOW) {
    startGame = true;
    conditionButton = 0;
    lcd.clear();
  }
  if (restart == 0) {
    if (level == 1) {
      if (startGame == true) {
        JoyStick();
        //nava aprinsa
        lc.setLed(0, posY, 7, true);
        lc.setLed(0, posY, 6, true);
        Display();
        PressButton();
        if (fall == 1)
          EggsLevel();
        if (life == 0) {
          Restart();
        }
      }
      //stingerea navei
      lc.setLed(0, posY, 7, false);
      lc.setLed(0, posY, 6, false);
      if (number == 8) {
        level++;
        isBulletGenerated = true;
      }
    }
    if (level == 2) {
      if (startGame == true) {
        JoyStick();
        //nava aprinsa
        lc.setLed(0, posY, 7, true);
        lc.setLed(0, posY, 6, true);
        Display();
        PressButton();
        if (fall == 1)
          EggsLevel();
        if (life == 0)
          Restart();
      }
      //stingerea navei
      lc.setLed(0, posY, 7, false);
      lc.setLed(0, posY, 6, false);
      if (number1 == 14) {
        level++;
        lc.clearDisplay(0);
        isBulletGenerated = true;
      }
    }
    if (level == 3) {
      if (startGame == true) {
        JoyStick();
        lc.setLed(0, posY, 7, true);
        lc.setLed(0, posY, 6, true);
        Display();
        PressButton();
        if (fall == 1)
          EggsLevel();
        if (life == 0) {
          Restart();
        }
      }
      lc.setLed(0, posY, 7, false);
      lc.setLed(0, posY, 6, false);
      if (number2 == 9)
        level++;
    }
    if (level == 4) {
      lcd.setCursor(0, 0);
      lcd.print("Finish Game!");
    }
  }
}

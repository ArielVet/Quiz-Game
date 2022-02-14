#include <stdlib.h>
#include <time.h>

#define yInput A0
#define ButtonInput A1 //SW input

#define LEFT -1
#define RIGHT 1
#define NEUTRAL 0

#define BLUE 8
#define YELLOW 9
#define WHITE 10
#define CORRECT 11
#define INCORRECT 12

#define FOUND 1
#define NOT_FOUND 0

//Outputs
int yOut = 0;
int yPos = 0;
int ButtonOut = 0;

int currentButton = BLUE;
int rounds = 0;
int mark = 0;

int answers[3];
int questions[10];

int checkPosition(void);
void lightUp(int position);
void selectButton(int min, int max);
void checkButton(void);
void generateSequence(int arr[], int length);
int checkArray(int arr[], int num, int value);
int askQuestion(int question, int current_round);
void printQuestion(int question_num);

void setup() {
  Serial.begin(9600);

  //Setup Iput for Joy Stick
  pinMode(yInput, INPUT);
  pinMode(ButtonInput, INPUT_PULLUP);

  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(CORRECT, OUTPUT);
  pinMode(INCORRECT, OUTPUT);

  randomSeed(analogRead(13));
  generateSequence(questions, 10);
  lightUp(BLUE);
}

void loop() {
  // put your main code here, to run repeatedly:

  while (rounds < 10) {
    Serial.print("Question #"); Serial.print(rounds + 1); Serial.print(":\n");
    askQuestion(questions[rounds], rounds);
  }
  if (rounds == 10) {
    lightUp(NEUTRAL);
    Serial.print("Your Score is "); Serial.print(mark * 10); Serial.print("%\n");
    rounds++;
  }
}

/* Purpose: find position of switch
 */
int checkPosition(void) {

  yOut = analogRead(yInput);
  yPos = map(yOut, 0, 1023, 256, -256); //Remaps Max and Min of JoyStick so 0 is aprox center

  if (yPos > 200)
    return LEFT;
  else if (yPos < -200)
    return RIGHT;
  else if (abs(yPos) < 200)
    return NEUTRAL;
}

/* Purpose: turns on selected colours
 * Param: position - the position of the switch
 */
void lightUp(int position) {
  if (position == BLUE) {
    digitalWrite(BLUE, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(WHITE, LOW);
    digitalWrite(CORRECT, LOW);
    digitalWrite(INCORRECT, LOW);
  }
  else if (position == YELLOW) {
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(WHITE, LOW);
    digitalWrite(CORRECT, LOW);
    digitalWrite(INCORRECT, LOW);
  }
  else if (position == WHITE) {
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(WHITE, HIGH);
    digitalWrite(CORRECT, LOW);
    digitalWrite(INCORRECT, LOW);
  }
  else if (position == CORRECT) {
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(WHITE, LOW);
    digitalWrite(CORRECT, HIGH);
    digitalWrite(INCORRECT, LOW);
  }
  else if (position == INCORRECT) {
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(WHITE, LOW);
    digitalWrite(CORRECT, LOW);
    digitalWrite(INCORRECT, HIGH);
  }
  else if (position == NEUTRAL) {
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(WHITE, LOW);
    digitalWrite(CORRECT, LOW);
    digitalWrite(INCORRECT, LOW);
  }
}

/* Purpose: Changes selected colour 
 */
void selectButton() {
  int change = checkPosition();

  if (change != 0) {
    if (currentButton + change > WHITE)
      currentButton = BLUE;
    else if (currentButton + change < BLUE)
      currentButton = WHITE;
    else
      currentButton += change;

    lightUp(currentButton);
    delay(200);
  }
}

/* Purpose: Upon Button Click Checks if Correct Answer
 */
void checkButton(void) {
  int state;
  selectButton();

  ButtonOut = digitalRead(ButtonInput);

  if (ButtonOut == 0) {
    state = currentButton;
    if (answers[state - 8] == 0) {
      Serial.print("Correct!\n");
      mark++;
      lightUp(CORRECT);
    }
    else {
      Serial.print("Incorrect!\n");
      lightUp(INCORRECT);
    }

    rounds++;
    delay(2000);
  }
}

/* Purpose: to genrate a random sequence of colours
 * Param: arr[] - stores the colours
 * Param: length - the length of the array
 */
void generateSequence(int arr[], int length) {
  int i = 0;
  int j;

  for (int i = 0; i < length; i++) {
    arr[i] = 11;
  }

  while (i < length) {
    j = random(0, length);
    if (checkArray(arr, i, j) != FOUND) {
      arr[i] = j;
      i++;
    }
  }
}

/* Purpose: to check the array for exsiting numbers
 * Param: arr[] - stores the numbers
 * Param: num - the amoutn of elements to check
 * Param: Value - the vlaue to check for
 * Return: whether it finds or does not find the value in the array
 */
int checkArray(int arr[], int num, int value) {
  for (int i = 0; i <= num; i++) {
    if (value == arr[i])
      return FOUND;
  }

  return NOT_FOUND;
}

/* Purpose: Run the Code for a specifc round
 * Param: question - the current round #
 */
int askQuestion(int question, int current_round) {

  randomSeed(analogRead(13));
  generateSequence(answers, 3);

  printQuestion(question );

  currentButton = BLUE;
  lightUp(BLUE);

  while (current_round == rounds) {
    checkButton();
  }
}

/* Purpose: Print the question and the answer to the screen
 * Param: question_num - the question that needs to be printed
 */
void printQuestion(int question_num) {

  int Q0[3] = { 1909, 1878, 1942 };
  char Q1[3][12] = {"The Flash", "Green Arrow", "FIRESTORM"};
  char Q2[3][8] = { "Paris", "Milan", "Beijing" };
  int Q3[3] = {2010, 1996, 2004 };
  char Q4[3][12] = { "Jeff Bezos", "Elon Musk", "Bill Gates" };
  char Q5[3][16] = { "Dreams", "Cramps", "Litres of Water" };
  char Q6[3][5] = { "42", "3.14","2.72" }; 
  int Q7[3] = {2009, 2005, 2014 };
  int Q8[3] = {1969, 1963, 1972 };
  char Q9[3][15] = {"Toshiba", "HTC", "Motorola" };

  switch (question_num)
  {
  case 0:
    Serial.print("What year was UBC founded?\n");
    Serial.print("BLUE) "); Serial.print(Q0[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q0[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q0[answers[2]]); Serial.print("\n");
    break;
  case 1:
    Serial.print("Who is Barry Allen?\n");
    Serial.print("BLUE) "); Serial.print(Q1[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q1[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q1[answers[2]]); Serial.print("\n");
    break;
  case 2:
    Serial.print("Where are the 2024 Olympics Hosted?\n");
    Serial.print("BLUE) "); Serial.print(Q2[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q2[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q2[answers[2]]); Serial.print("\n");
    break;
  case 3:
    Serial.print("When were the Olympics in Vancouver?\n");
    Serial.print("BLUE) "); Serial.print(Q3[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q3[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q3[answers[2]]); Serial.print("\n");
    break;
  case 4:
    Serial.print("Who is the richest person in the World?\n");
    Serial.print("BLUE) "); Serial.print(Q4[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q4[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q4[answers[2]]); Serial.print("\n");
    break;
  case 5:
    Serial.print("The average person has over 1,460 of what each year?\n");
    Serial.print("BLUE) "); Serial.print(Q5[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q5[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q5[answers[2]]); Serial.print("\n");
    break;
  case 6:
    Serial.print("What is the meaning of Life?\n");
    Serial.print("BLUE) "); Serial.print(Q6[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q6[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q6[answers[2]]); Serial.print("\n");
    break;
  case 7:
    Serial.print("When was Minecraft Released?\n");
    Serial.print("BLUE) "); Serial.print(Q7[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q7[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q7[answers[2]]); Serial.print("\n");
    break;
  case 8:
    Serial.print("When did man land on the moon?\n");
    Serial.print("BLUE) "); Serial.print(Q8[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q8[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q8[answers[2]]); Serial.print("\n");
    break;
  case 9:
    Serial.print("Who was the first to release a SmartPhone with a fingreprint Sensor?\n");
    Serial.print("BLUE) "); Serial.print(Q9[answers[0]]); Serial.print("\n");
    Serial.print("YELLOW) "); Serial.print(Q9[answers[1]]); Serial.print("\n");
    Serial.print("WHITE) "); Serial.print(Q9[answers[2]]); Serial.print("\n");
    break;
  }
}

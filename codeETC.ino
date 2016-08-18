#include <pt.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define PT_DELAY(pt,ms,ts) \
  ts = millis(); \
  PT_WAIT_WHILE(pt,millis()-ts<(ms));

struct pt pt_taskLight;
struct pt pt_taskDoor;
struct pt pt_taskReportS;
struct pt pt_taskReportW;
struct pt pt_taskShelfOne;
struct pt pt_taskShelfTwo;
struct pt pt_taskDetect;
struct pt pt_taskEvent;
struct pt pt_taskCheck;
struct pt pt_taskChk;

struct steps
{
  int dirc = 0;
  struct steps *next = NULL;
};
struct steps *stepStart;
struct steps *pointing;
struct steps *pointing1;
struct steps *pointing2;

long duration = 0.0, dist1 = 0.0, dist2 = 0.0;
bool lightCon = false;
bool doorCon = true;
bool stand1 = false;
bool stand2 = false;
bool holdIn = false;
bool holdOut = false;
bool sentStatOD = false;
bool sentStatCD = false;
bool sentStatLF = false;
bool sentStatLN = false;
bool passOne = false;
bool passTwo = false;
bool walkInChk = false;
bool walkOutChk = false;
bool pressed = false;
bool freeOne = true;
bool freeTwo = true;
int motion1 = 0;
int motion2 = 0;
int num = 0;
int his = 0;
int i = 0;
int stepCount = 0;
int outLig1 = 0;
int outLig2 = 0;
String sInput = "";
String report = "";
Servo doorServo;


/*void blinkLight()
{
  digitalWrite(7, HIGH);
  delay(30);
  digitalWrite(7, LOW);
  delay(60);
  digitalWrite(7, HIGH);
  delay(30);
  digitalWrite(7, LOW);
  delay(60);
  if(lightCon == true)
  {
    digitalWrite(7, HIGH);
  }
  else
  {
    digitalWrite(7, LOW);
  }
}*/
/*void Beep()
{
  analogWrite(10, 75);
  delay(20);
  analogWrite(10, 100);
  delay(30);
  analogWrite(10, 0);
}*///NO BUZZER
void checkDirect(int direct)
{
  //Serial.print("Direction ");
  //Serial.println(direct);
  pointing = stepStart;
  struct steps *before1 = NULL;
  struct steps *before2 = NULL;
  struct steps *held = NULL;
  if (direct == -1)
  {
    while (pointing->dirc != 1 && pointing->next != NULL)
    {
      before1 = pointing;
      pointing = pointing->next;
    }
    if (pointing->dirc == 1)
    {
      held = pointing;
      while (pointing->dirc != -1 && pointing->next != NULL)
      {
        before2 = pointing;
        pointing = pointing->next;
      }
      if (pointing->dirc == -1)
      {
        //Found Walking IN
        if (before1 != NULL)
        {
          before1->next = held->next;
        }
        if (stepStart == held)
        {
          stepStart = stepStart->next;
        }
        free(held);
        before2->next = pointing->next;
        if (stepStart == pointing)
        {
          stepStart = stepStart->next;
        }
        free(pointing);
        //Pass Walk IN (1)
        //blinkLight();
        Serial1.println("5");
        Serial.print("In");
        //passenger = passenger + 1;
        stepCount = stepCount - 2;
      }
    }
  }
  if (direct == 1)
  {
    while (pointing->dirc != -1 && pointing->next != NULL)
    {
      before1 = pointing;
      pointing = pointing->next;
    }
    if (pointing->dirc == -1)
    {
      held = pointing;
      while (pointing->dirc != 1 && pointing->next != NULL)
      {
        before2 = pointing;
        pointing = pointing->next;
      }
      if (pointing->dirc == 1)
      {
        //Found Walking IN
        if (before1 != NULL)
        {
          before1->next = held->next;
        }
        if (stepStart == held)
        {
          stepStart = stepStart->next;
        }
        free(held);
        before2->next = pointing->next;
        if (stepStart == pointing)
        {
          stepStart = stepStart->next;
        }
        free(pointing);
        //Pass Walk IN (1)
        //Serial1.println("2");
        //Serial.print("Out");
        //passenger = passenger - 1;
        stepCount = stepCount - 2;
      }
    }
  }
}
void stepOn(int direct)
{
  //Serial.print("Found ");
  //Serial.println(direct);
  struct steps *construct = (struct steps *)malloc(sizeof(struct steps));
  if (stepStart == NULL || stepCount == 0)
  {
    //Serial.println("Constructing New");
    stepStart = construct;
    stepStart->dirc = direct;
    stepStart->next = NULL;
    pointing = stepStart;
  }
  else if (stepStart->dirc != direct)
  {
    //Serial.println("Constructing Resume");
    pointing = stepStart;
    while (pointing->next != NULL)
    {
      pointing = pointing->next;
    }
    if (pointing->next == NULL)
    {
      pointing->next = construct;
      pointing = pointing->next;
      pointing->dirc = direct;
    }
    Serial1.println("5");
    checkDirect(direct);
  }
  else
  {
    free(stepStart);
    stepCount = 0;
    //Serial.println("Constructing New");
    stepStart = construct;
    stepStart->dirc = direct;
    stepStart->next = NULL;
    pointing = stepStart;
  }
  stepCount = stepCount + 1;
}
PT_THREAD(taskChk(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    outLig1 = analogRead(A3);
    digitalWrite(8, LOW);
    delayMicroseconds(2);
    digitalWrite(8, HIGH);
    delayMicroseconds(10);
    digitalWrite(8, LOW);
    duration = pulseIn(9, HIGH);
    outLig2 = (duration/2.0) / 29.1;
    if (outLig1 > 200)
    {
      freeOne = true;
      //No Foot Detected on Sensor1(Outside sensor)
    }
    else if (outLig1 < 200 && freeOne)
    {
      freeOne = false;
      //Serial.println("StepOnOne");
      stepOn(1);
      //Foot Detected on Sensor1
    }
    if (outLig2 >= 😎
    {
      freeTwo = true;
      //No Foot Detected on Sensor2(Inside Sensor)
    }
    else if (outLig2 < 8 && freeTwo)
    {
      freeTwo = false;
      //Serial.println("StepOnTwo");
      stepOn(-1);
      //Foot Detected on Sensor2
    }
    PT_DELAY(pt, 200, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskDoor(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    if(doorCon == true&&i>30)
    //if(true)
    {
      //OPEN DOOR
      doorServo.write(30);
      for(i=155; i>30; i = i - 1)
      {
        doorServo.write(i);
        delay(5);
      }
      if(sentStatOD == false)
      {
        SerialEvent("3");
        sentStatOD = true;
        sentStatCD = false;
      }
      //Beep();
    }
    else if(doorCon == false&&i<155)
    {
      //CLOSE DOOR
      if(sentStatCD == false)
      {
        SerialEvent("4");
        sentStatCD = true;
        sentStatOD = false;
      }
      for(i=30; i<155; i = i + 1)
      {
        doorServo.write(i);
        delay(5);
      }
      //doorServo.write(155);
      //Beep();
    }
    PT_DELAY(pt, 500, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskReportW(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    report = ((String)(dist1)+','+(String)(motion1)+','+(String)(motion2));
    Serial.println(report);
    SerialEvent(report);
    PT_DELAY(pt, 2000, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskReportS(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    //Serial.print("Light:");
    //Serial.println(lightCon);
    //Serial.print("Door:");
    //Serial.println(doorCon);
    //Serial.print("Shelf1:");
    //Serial.println(motion1);
    //Serial.print("Shelf2:");
    //Serial.println(motion2);
    //Serial.print("DoorOut:");
    //Serial.println(outLig1);
    //Serial.print("DoorIn:");
    //Serial.println(outLig2);
    //Serial.println(freeOne);
    //Serial.println(freeTwo);
    PT_DELAY(pt, 1000, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskShelfOne(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    motion1 = analogRead(A4);
    if(motion1 > 200 && stand1 == false)
    {
      stand1 = true;
      //doorCon = true;
      Serial.println("ShelfOne");
      /*digitalWrite(7, HIGH);
      delay(30);
      digitalWrite(7, LOW);
      delay(10);
      digitalWrite(7, HIGH);
      */SerialEvent("6");
    }
    else if(motion1 < 200)
    {
      
      if(stand1 == true)
      {
        SerialEvent("8");
      }
      stand1 = false;
      //doorCon = false;
    }
    PT_DELAY(pt, 100, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskShelfTwo(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    motion2 = analogRead(A5);
    if(motion2 > 200 && stand2 == false)
    {
      stand2 = true;
      //doorCon = true;
      Serial.println("ShelfTwo");
      SerialEvent("7");
      /*digitalWrite(7, HIGH);
      delay(10);
      digitalWrite(7, LOW);
      delay(30);
      digitalWrite(7, HIGH);*/
    }
    else if(motion2 < 200)
    {
      
      if(stand2 == true)
      {
        SerialEvent("9");
      }
      stand2 = false;
      //doorCon = false;
    }
    PT_DELAY(pt, 100, ts);
  }

  PT_END(pt);
}

PT_THREAD(taskCheck(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    if(sInput == "1")//Light ON
    {
      lightCon = true;
    }
    if(sInput == "2")//Light OFF
    {
      lightCon = false;
    }
    if(sInput == "3")//Door OPEN
    {
      doorCon = true;
    }
    if(sInput == "4")//Door CLOSE
    {
      doorCon = false;
    }
    PT_DELAY(pt, 313, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskLight(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    if(lightCon == true)
    //if(true)
    {
      //LIGHT ON
      if(sentStatLN == false)
      {
        SerialEvent("1");
        sentStatLN = true;
        sentStatLF = false;
      }
      digitalWrite(7, HIGH);
      //lightCon = false;
    }
    else if(lightCon == false)
    {
      //LIGHT OFF
      if(sentStatLF == false)
      {
        SerialEvent("2");
        sentStatLF = true;
        sentStatLN = false;
      }
      digitalWrite(7, LOW);
      //lightCon = true;
    }
    PT_DELAY(pt, 500, ts);
  }

  PT_END(pt);
}

void setup() {
  doorServo.attach(10);
  doorServo.write(30);
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(7, OUTPUT);//LIGHT
  pinMode(8, OUTPUT);//TRIGGERED
  pinMode(9, INPUT);//ECHO
  pinMode(A3, INPUT);//INFRA
  pinMode(A4, INPUT);//SHELF1
  pinMode(A5, INPUT);//SHELF2
  PT_INIT(&pt_taskLight);
  PT_INIT(&pt_taskDoor);
  PT_INIT(&pt_taskShelfOne);
  PT_INIT(&pt_taskShelfTwo);
  PT_INIT(&pt_taskReportS);
  PT_INIT(&pt_taskReportW);
  //PT_INIT(&pt_taskDetect);
  PT_INIT(&pt_taskEvent);
  PT_INIT(&pt_taskChk);
  //PT_INIT(&pt_taskCheck);
  
}

void loop() {
  taskDoor(&pt_taskDoor);
  taskLight(&pt_taskLight);
  taskShelfOne(&pt_taskShelfOne);
  taskShelfTwo(&pt_taskShelfTwo);
  taskReportS(&pt_taskReportS);
  //taskReportW(&pt_taskReportW);
  //taskDetect(&pt_taskDetect);
  taskEvent(&pt_taskEvent);
  taskChk(&pt_taskChk);
  //taskCheck(&pt_taskCheck);
}
void SerialEvent(String str) {
    Serial.print("Out - ");
    Serial.println(str);
    Serial1.println(str);
    str.replace("\r", "");
}
PT_THREAD(taskEvent(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    if ((Serial1.available())) {
    sInput = (String)Serial1.readStringUntil('\r');
    Serial1.flush();
    sInput.replace("\r", "");
    Serial.print("In - ");
    Serial.println(sInput);
    if(sInput == "1")//Light ON
    {
      lightCon = true;
    }
    if(sInput == "2")//Light OFF
    {
      lightCon = false;
    }
    if(sInput == "3")//Door OPEN
    {
      doorCon = true;
    }
    if(sInput == "4")//Door CLOSE
    {
      doorCon = false;
    }
    sInput = 0;
  }
    PT_DELAY(pt, 500, ts);
  }

  PT_END(pt);
}#include <pt.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define PT_DELAY(pt,ms,ts) \
  ts = millis(); \
  PT_WAIT_WHILE(pt,millis()-ts<(ms));

struct pt pt_taskLight;
struct pt pt_taskDoor;
struct pt pt_taskReportS;
struct pt pt_taskReportW;
struct pt pt_taskShelfOne;
struct pt pt_taskShelfTwo;
struct pt pt_taskDetect;
struct pt pt_taskEvent;
struct pt pt_taskCheck;
struct pt pt_taskChk;

struct steps
{
  int dirc = 0;
  struct steps *next = NULL;
};
struct steps *stepStart;
struct steps *pointing;
struct steps *pointing1;
struct steps *pointing2;

long duration = 0.0, dist1 = 0.0, dist2 = 0.0;
bool lightCon = false;
bool doorCon = true;
bool stand1 = false;
bool stand2 = false;
bool holdIn = false;
bool holdOut = false;
bool sentStatOD = false;
bool sentStatCD = false;
bool sentStatLF = false;
bool sentStatLN = false;
bool passOne = false;
bool passTwo = false;
bool walkInChk = false;
bool walkOutChk = false;
bool pressed = false;
bool freeOne = true;
bool freeTwo = true;
int motion1 = 0;
int motion2 = 0;
int num = 0;
int his = 0;
int i = 0;
int stepCount = 0;
int outLig1 = 0;
int outLig2 = 0;
String sInput = "";
String report = "";
Servo doorServo;


/*void blinkLight()
{
  digitalWrite(7, HIGH);
  delay(30);
  digitalWrite(7, LOW);
  delay(60);
  digitalWrite(7, HIGH);
  delay(30);
  digitalWrite(7, LOW);
  delay(60);
  if(lightCon == true)
  {
    digitalWrite(7, HIGH);
  }
  else
  {
    digitalWrite(7, LOW);
  }
}*/
/*void Beep()
{
  analogWrite(10, 75);
  delay(20);
  analogWrite(10, 100);
  delay(30);
  analogWrite(10, 0);
}*///NO BUZZER
void checkDirect(int direct)
{
  //Serial.print("Direction ");
  //Serial.println(direct);
  pointing = stepStart;
  struct steps *before1 = NULL;
  struct steps *before2 = NULL;
  struct steps *held = NULL;
  if (direct == -1)
  {
    while (pointing->dirc != 1 && pointing->next != NULL)
    {
      before1 = pointing;
      pointing = pointing->next;
    }
    if (pointing->dirc == 1)
    {
      held = pointing;
      while (pointing->dirc != -1 && pointing->next != NULL)
      {
        before2 = pointing;
        pointing = pointing->next;
      }
      if (pointing->dirc == -1)
      {
        //Found Walking IN
        if (before1 != NULL)
        {
          before1->next = held->next;
        }
        if (stepStart == held)
        {
          stepStart = stepStart->next;
        }
        free(held);
        before2->next = pointing->next;
        if (stepStart == pointing)
        {
          stepStart = stepStart->next;
        }
        free(pointing);
        //Pass Walk IN (1)
        //blinkLight();
        Serial1.println("5");
        Serial.print("In");
        //passenger = passenger + 1;
        stepCount = stepCount - 2;
      }
    }
  }
  if (direct == 1)
  {
    while (pointing->dirc != -1 && pointing->next != NULL)
    {
      before1 = pointing;
      pointing = pointing->next;
    }
    if (pointing->dirc == -1)
    {
      held = pointing;
      while (pointing->dirc != 1 && pointing->next != NULL)
      {
        before2 = pointing;
        pointing = pointing->next;
      }
      if (pointing->dirc == 1)
      {
        //Found Walking IN
        if (before1 != NULL)
        {
          before1->next = held->next;
        }
        if (stepStart == held)
        {
          stepStart = stepStart->next;
        }
        free(held);
        before2->next = pointing->next;
        if (stepStart == pointing)
        {
          stepStart = stepStart->next;
        }
        free(pointing);
        //Pass Walk IN (1)
        //Serial1.println("2");
        //Serial.print("Out");
        //passenger = passenger - 1;
        stepCount = stepCount - 2;
      }
    }
  }
}
void stepOn(int direct)
{
  //Serial.print("Found ");
  //Serial.println(direct);
  struct steps *construct = (struct steps *)malloc(sizeof(struct steps));
  if (stepStart == NULL || stepCount == 0)
  {
    //Serial.println("Constructing New");
    stepStart = construct;
    stepStart->dirc = direct;
    stepStart->next = NULL;
    pointing = stepStart;
  }
  else if (stepStart->dirc != direct)
  {
    //Serial.println("Constructing Resume");
    pointing = stepStart;
    while (pointing->next != NULL)
    {
      pointing = pointing->next;
    }
    if (pointing->next == NULL)
    {
      pointing->next = construct;
      pointing = pointing->next;
      pointing->dirc = direct;
    }
    Serial1.println("5");
    checkDirect(direct);
  }
  else
  {
    free(stepStart);
    stepCount = 0;
    //Serial.println("Constructing New");
    stepStart = construct;
    stepStart->dirc = direct;
    stepStart->next = NULL;
    pointing = stepStart;
  }
  stepCount = stepCount + 1;
}
PT_THREAD(taskChk(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    outLig1 = analogRead(A3);
    digitalWrite(8, LOW);
    delayMicroseconds(2);
    digitalWrite(8, HIGH);
    delayMicroseconds(10);
    digitalWrite(8, LOW);
    duration = pulseIn(9, HIGH);
    outLig2 = (duration/2.0) / 29.1;
    if (outLig1 > 200)
    {
      freeOne = true;
      //No Foot Detected on Sensor1(Outside sensor)
    }
    else if (outLig1 < 200 && freeOne)
    {
      freeOne = false;
      //Serial.println("StepOnOne");
      stepOn(1);
      //Foot Detected on Sensor1
    }
    if (outLig2 >= 😎
    {
      freeTwo = true;
      //No Foot Detected on Sensor2(Inside Sensor)
    }
    else if (outLig2 < 8 && freeTwo)
    {
      freeTwo = false;
      //Serial.println("StepOnTwo");
      stepOn(-1);
      //Foot Detected on Sensor2
    }
    PT_DELAY(pt, 200, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskDoor(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    if(doorCon == true&&i>30)
    //if(true)
    {
      //OPEN DOOR
      doorServo.write(30);
      for(i=155; i>30; i = i - 1)
      {
        doorServo.write(i);
        delay(5);
      }
      if(sentStatOD == false)
      {
        SerialEvent("3");
        sentStatOD = true;
        sentStatCD = false;
      }
      //Beep();
    }
    else if(doorCon == false&&i<155)
    {
      //CLOSE DOOR
      if(sentStatCD == false)
      {
        SerialEvent("4");
        sentStatCD = true;
        sentStatOD = false;
      }
      for(i=30; i<155; i = i + 1)
      {
        doorServo.write(i);
        delay(5);
      }
      //doorServo.write(155);
      //Beep();
    }
    PT_DELAY(pt, 500, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskReportW(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    report = ((String)(dist1)+','+(String)(motion1)+','+(String)(motion2));
    Serial.println(report);
    SerialEvent(report);
    PT_DELAY(pt, 2000, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskReportS(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    //Serial.print("Light:");
    //Serial.println(lightCon);
    //Serial.print("Door:");
    //Serial.println(doorCon);
    //Serial.print("Shelf1:");
    //Serial.println(motion1);
    //Serial.print("Shelf2:");
    //Serial.println(motion2);
    //Serial.print("DoorOut:");
    //Serial.println(outLig1);
    //Serial.print("DoorIn:");
    //Serial.println(outLig2);
    //Serial.println(freeOne);
    //Serial.println(freeTwo);
    PT_DELAY(pt, 1000, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskShelfOne(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    motion1 = analogRead(A4);
    if(motion1 > 200 && stand1 == false)
    {
      stand1 = true;
      //doorCon = true;
      Serial.println("ShelfOne");
      /*digitalWrite(7, HIGH);
      delay(30);
      digitalWrite(7, LOW);
      delay(10);
      digitalWrite(7, HIGH);
      */SerialEvent("6");
    }
    else if(motion1 < 200)
    {
      
      if(stand1 == true)
      {
        SerialEvent("8");
      }
      stand1 = false;
      //doorCon = false;
    }
    PT_DELAY(pt, 100, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskShelfTwo(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    motion2 = analogRead(A5);
    if(motion2 > 200 && stand2 == false)
    {
      stand2 = true;
      //doorCon = true;
      Serial.println("ShelfTwo");
      SerialEvent("7");
      /*digitalWrite(7, HIGH);
      delay(10);
      digitalWrite(7, LOW);
      delay(30);
      digitalWrite(7, HIGH);*/
    }
    else if(motion2 < 200)
    {
      
      if(stand2 == true)
      {
        SerialEvent("9");
      }
      stand2 = false;
      //doorCon = false;
    }
    PT_DELAY(pt, 100, ts);
  }

  PT_END(pt);
}

PT_THREAD(taskCheck(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    if(sInput == "1")//Light ON
    {
      lightCon = true;
    }
    if(sInput == "2")//Light OFF
    {
      lightCon = false;
    }
    if(sInput == "3")//Door OPEN
    {
      doorCon = true;
    }
    if(sInput == "4")//Door CLOSE
    {
      doorCon = false;
    }
    PT_DELAY(pt, 313, ts);
  }

  PT_END(pt);
}
PT_THREAD(taskLight(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    if(lightCon == true)
    //if(true)
    {
      //LIGHT ON
      if(sentStatLN == false)
      {
        SerialEvent("1");
        sentStatLN = true;
        sentStatLF = false;
      }
      digitalWrite(7, HIGH);
      //lightCon = false;
    }
    else if(lightCon == false)
    {
      //LIGHT OFF
      if(sentStatLF == false)
      {
        SerialEvent("2");
        sentStatLF = true;
        sentStatLN = false;
      }
      digitalWrite(7, LOW);
      //lightCon = true;
    }
    PT_DELAY(pt, 500, ts);
  }

  PT_END(pt);
}

void setup() {
  doorServo.attach(10);
  doorServo.write(30);
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(7, OUTPUT);//LIGHT
  pinMode(8, OUTPUT);//TRIGGERED
  pinMode(9, INPUT);//ECHO
  pinMode(A3, INPUT);//INFRA
  pinMode(A4, INPUT);//SHELF1
  pinMode(A5, INPUT);//SHELF2
  PT_INIT(&pt_taskLight);
  PT_INIT(&pt_taskDoor);
  PT_INIT(&pt_taskShelfOne);
  PT_INIT(&pt_taskShelfTwo);
  PT_INIT(&pt_taskReportS);
  PT_INIT(&pt_taskReportW);
  //PT_INIT(&pt_taskDetect);
  PT_INIT(&pt_taskEvent);
  PT_INIT(&pt_taskChk);
  //PT_INIT(&pt_taskCheck);
  
}

void loop() {
  taskDoor(&pt_taskDoor);
  taskLight(&pt_taskLight);
  taskShelfOne(&pt_taskShelfOne);
  taskShelfTwo(&pt_taskShelfTwo);
  taskReportS(&pt_taskReportS);
  //taskReportW(&pt_taskReportW);
  //taskDetect(&pt_taskDetect);
  taskEvent(&pt_taskEvent);
  taskChk(&pt_taskChk);
  //taskCheck(&pt_taskCheck);
}
void SerialEvent(String str) {
    Serial.print("Out - ");
    Serial.println(str);
    Serial1.println(str);
    str.replace("\r", "");
}
PT_THREAD(taskEvent(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    if ((Serial1.available())) {
    sInput = (String)Serial1.readStringUntil('\r');
    Serial1.flush();
    sInput.replace("\r", "");
    Serial.print("In - ");
    Serial.println(sInput);
    if(sInput == "1")//Light ON
    {
      lightCon = true;
    }
    if(sInput == "2")//Light OFF
    {
      lightCon = false;
    }
    if(sInput == "3")//Door OPEN
    {
      doorCon = true;
    }
    if(sInput == "4")//Door CLOSE
    {
      doorCon = false;
    }
    sInput = 0;
  }
    PT_DELAY(pt, 500, ts);
  }

  PT_END(pt);
}

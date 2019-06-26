#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <SPI.h>
#include <Wire.h>

#define check         A0
#define magneticS3    A1
#define magneticS2    A2
#define magneticS1    A3

SoftwareSerial mySerial(8,9); //Rx, Tx

int a = 0, b = 0, c = 0;
int truc1, truc2, truc3;
int q = 0; //khoảng cách từ đầu gieo đến đầu tưới
byte k[4];
byte m[4];
byte n[7];
int x = 0, y = 0;
uint8_t val = 0, count = 0;
byte degree;

int humid;
int z = 0; //đếm số vị trí từ 1 đến 25
byte p;

// Khai bao chan cap xung cho driver servo
AccelStepper stepperA(1,3,2);// pin 3 PULSE, pin 2 dir, x= stepperA
AccelStepper stepperB(1,5,4);// pin 5 PULSE, pin 4 dir, y= stepperB
AccelStepper stepperC(1,6,7);// pin 6 PULSE, pin 7 dir, z= stepperC

//Trục C tọa độ âm
int DongCoA = 0;
int DongCoB = 0;
int DongCoC = 0;
int Xoay0 = 0;

//Giá trị động cơ chạy
int xungA = 4200;  //trục x gieo hạt
int xungA1 = 3200; //trục x cho các vị trí tiếp theo
int xungB = 2700;  //trục y gieo hạt
int xungB1 = 3800; //trục y cho các vị trí tiếp theo
int gieohat = -16000;
int tuoi = 4200; //16cm
int doam = -16000;
int dietco = -13500;
int tuoiSDA = 3850;
//Trục x, y đo độ ẩm và tưới
int kcdoam = 350;
int a1 = 16650;
int a2 = 13450;
int a3 = 10250;
int a4 = 7050;
int a5 = 3850;
int b1 = 17900;
int b2 = 14100;
int b3 = 10300;
int b4 = 6500;
int b5 = 2700;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(magneticS1, INPUT);
  digitalWrite(magneticS1, HIGH); // ko phát hiện từ
  pinMode(magneticS2, INPUT);
  digitalWrite(magneticS2, HIGH);
  pinMode(magneticS3, INPUT);
  digitalWrite(magneticS3, HIGH);
  // Choose SPI Enable, bit 0 for bit (MASTER) to choose Slave for Arduino
  SPCR |= bit (SPE);
  // Choose Master In Slave Out for MISO pin of Arduino
  pinMode(MISO, OUTPUT);
  // Check pin to know signal is x or y, input(check) = 0 is x, input(check) = 1 is y.
  pinMode(check, INPUT);
  // Global Interrupt
  SPI.attachInterrupt();  
  //Chinh toc do va gia toc
  stepperA.setMaxSpeed(5000);  //chỉnh tốc độ driver 1 
  stepperA.setAcceleration(5000); // chinh gia toc driver 1
  stepperB.setMaxSpeed(5000);  //chỉnh tốc độ driver 2 
  stepperB.setAcceleration(5000); // chinh gia toc driver 2
  stepperC.setMaxSpeed(5000);  //chỉnh tốc độ driver 3 
  stepperC.setAcceleration(5000); // chinh gia toc driver 3
}

void ReceiveEvent(int n)
{
  while (Wire.available())
  {
    p = Wire.read();   //receive byte as an integer
  }
}

void Check_x_and_y(void)
{
  switch (val)
  {
    case 0:
      x += degree;
      // Run x
      Serial.print("x = ");
      Serial.print(x);
      Serial.print("\n");
      break;
    case 1:
      y += degree;
      // Run y
      Serial.print("y = ");
      Serial.print(y);
      Serial.print("\n"); 
      break;
    default:
      Serial.print("ERROR x and y!\n");
      break;
  }
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  degree = SPDR;
  if (degree != NULL)
  {
     Check_x_and_y();
  }
}

void Return_Zero()
{
  while (digitalRead(magneticS1) == 1) // không ở vị trí bắt đầu
  {
  	a = a + 50;
  	DongCoA = 14;
    while(DongCoA == 14) //lùi về vị trí bắt đầu
    {
      truc1 = -a;
      stepperA.moveTo(truc1);
      stepperA.run();
      if (stepperA.distanceToGo() == 0)
      {
        DongCoA = 0;
        break;
      }
    }
  }
  while (digitalRead(magneticS2) == 1)
  {
  	b = b + 50;
  	DongCoB = 13;
    while(DongCoB == 13) //trở về vị trí bắt đầu
    {
      truc2 = -b;
      stepperB.moveTo(truc2);
      stepperB.run();
      if (stepperB.distanceToGo() == 0) 
      {
        DongCoB = 0;
        break;
      }
    }
  }
  while (digitalRead(magneticS3) == 1)
  {
  	c = c + 50;
  	DongCoC = 9;
    while(DongCoC == 9) //trở về vị trí ban đầu
    {
      truc3 = c;
      stepperC.moveTo(truc3);
      stepperC.run();
      if (stepperC.distanceToGo() == 0)
      {
        DongCoC = 0;
      }
    }
  }
}

void Check_Humility()
{
  Return_Zero();
  DongCoA = 7; //x1
  DongCoB = 7; //y1
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }	
  DongCoA = 9; //x2
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 10; //x3
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 11; //x4
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 12; //x5, thêm DongCoA = 13 để trở về vị trí x1
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoB = 9; //y2
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 9; //x2
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 10; //x3
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 11; //x4
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 12; //x5
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoB = 10; //y3
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 9; //x2
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 10; //x3
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 11; //x4
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 12; //x5
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoB = 11; //y4
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 9; //x2
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 10; //x3
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 11; //x4
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 12; //x5
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoB = 12; //y5
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 9; //x2
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 10; //x3
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 11; //x4
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  DongCoA = 12; //x5
  z += 1;
  DongCoC = 5;
  while (humid < 70)
  {
    DongCoA = 15;
  	break;
  }
  z = 0;
  Return_Zero();  
}

void loop()
{
  val = digitalRead(check);
  // Check step for y
  if (val == 1)
  {
    count++;
  }
  // Reset x, y for the next step
  if (count > 0 && val == 0)
  {
    count = 0;
    Serial.println(x);
    Serial.println(y);
    stepperA.moveTo((125/9)*x);
    stepperA.run();
    delay(4000);
    stepperB.moveTo((125/9)*y);
    stepperB.run();
    x = y = 0;
  }

  if (p == 1)
  {
  	Serial.print('e');
    Serial.print('1');
  	Serial.print('1'); //small
  }
  else if (p == 2)
  {
  	Serial.print('e');
    Serial.print('1');
  	Serial.print('2'); //normal
  }
  else if (p == 3)
  {
  	Serial.print('e');
    Serial.print('1');
  	Serial.print('3'); //big
  }
  
  if(Serial.available() > 2)
  {
    k[0] = Serial.read();
    if(k[0] == 'a')
    {
      k[1] = Serial.read();
      if(k[1] == '1')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 1;
          DongCoC = 1;
        }
        else if(k[2] == '2')
        {
          Return_Zero();
		      DongCoA = 1;
          DongCoB = 2;
          DongCoC = 1;
        }
        else if(k[2] == '3')
        {
          Return_Zero();
		      DongCoA = 1;
          DongCoB = 3;
          DongCoC = 1;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 4;
          DongCoC = 1;
        }
        else if(k[2] == '5')
        {
          Return_Zero();
		      DongCoA = 1;
          DongCoB = 5;
          DongCoC = 1;
        }
      }
      else if(k[1] == '2')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 1;
          DongCoC = 1;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 2;
          DongCoC = 1;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 3;
          DongCoC = 1;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 4;
          DongCoC = 1;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 5;
          DongCoC = 1;
        }
      }
      else if(k[1] == '3')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 1;
          DongCoC = 1;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 2;
          DongCoC = 1;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 3;
          DongCoC = 1;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 4;
          DongCoC = 1;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 5;
          DongCoC = 1;
        }
      }
      else if(k[1] == '4')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 1;
          DongCoC = 1;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 2;
          DongCoC = 1;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 3;
          DongCoC = 1;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 4;
          DongCoC = 1;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 5;
          DongCoC = 1;
        }
      }
      else if(k[1] == '5')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 1;
          DongCoC = 1;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 2;
          DongCoC = 1;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 3;
          DongCoC = 1;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 4;
          DongCoC = 1;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 5;
          DongCoC = 1;
        }
      }
    }
    else if(k[0] == 'b')
    {
      k[1] = Serial.read();
      if(k[1] == '1')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 1;
          DongCoC = 2;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 2;
          DongCoC = 2;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 3;
          DongCoC = 2;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 4;
          DongCoC = 2;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 5;
          DongCoC = 2;
        }
      }
      else if(k[1] == '2')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 1;
          DongCoC = 2;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 2;
          DongCoC = 2;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 3;
          DongCoC = 2;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 4;
          DongCoC = 2;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 5;
          DongCoC = 2;
        }
      }
      else if(k[1] == '3')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 1;
          DongCoC = 2;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 2;
          DongCoC = 2;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 3;
          DongCoC = 2;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 4;
          DongCoC = 2;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 5;
          DongCoC = 2;
        }
      }
      else if(k[1] == '4')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 1;
          DongCoC = 2;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 2;
          DongCoC = 2;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 3;
          DongCoC = 2;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 4;
          DongCoC = 2;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 5;
          DongCoC = 2;
        }
      }
      else if(k[1] == '5')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 1;
          DongCoC = 2;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 2;
          DongCoC = 2;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 3;
          DongCoC = 2;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 4;
          DongCoC = 2;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 5;
          DongCoC = 2;
        }
      }
    }
	  else if(k[0] == 'c')
    {
      k[1] = Serial.read();
      if(k[1] == '1')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 1;
          DongCoC = 3;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 2;
          DongCoC = 3;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 3;
          DongCoC = 3;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 4;
          DongCoC = 3;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 5;
          DongCoC = 3;
        }
      }
      else if(k[1] == '2')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 1;
          DongCoC = 3;
        }
        else if(k[2] == '2')
        {
		     Return_Zero();
          DongCoA = 2;
          DongCoB = 2;
          DongCoC = 3;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 3;
          DongCoC = 3;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 4;
          DongCoC = 3;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 5;
          DongCoC = 3;
        }
      }
      else if(k[1] == '3')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 1;
          DongCoC = 3;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 2;
          DongCoC = 3;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 3;
          DongCoC = 3;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 4;
          DongCoC = 3;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 5;
          DongCoC = 3;
        }
      }
      else if(k[1] == '4')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 1;
          DongCoC = 3;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 2;
          DongCoC = 3;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 3;
          DongCoC = 3;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 4;
          DongCoC = 3;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 5;
          DongCoC = 3;
        }
      }
      else if(k[1] == '5')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 1;
          DongCoC = 3;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 2;
          DongCoC = 3;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 3;
          DongCoC = 3;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 4;
          DongCoC = 3;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 5;
          DongCoC = 3;
        }
      }
    }
    else if(k[0] == 'd')
    {
      k[1] = Serial.read();
      if(k[1] == '1')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 1;
          DongCoC = 4;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 2;
          DongCoC = 4;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 3;
          DongCoC = 4;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 4;
          DongCoC = 4;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 1;
          DongCoB = 5;
          DongCoC = 4;
        }
      }
      else if(k[1] == '2')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 1;
          DongCoC = 4;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 2;
          DongCoC = 4;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 3;
          DongCoC = 4;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 4;
          DongCoC = 4;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 2;
          DongCoB = 5;
          DongCoC = 4;
        }
      }
      else if(k[1] == '3')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 1;
          DongCoC = 4;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 2;
          DongCoC = 4;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 3;
          DongCoC = 4;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 4;
          DongCoC = 4;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 3;
          DongCoB = 5;
          DongCoC = 4;
        }
      }
      else if(k[1] == '4')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 1;
          DongCoC = 4;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 2;
          DongCoC = 4;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 3;
          DongCoC = 4;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 4;
          DongCoC = 4;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 4;
          DongCoB = 5;
          DongCoC = 4;
        }
      }
      else if(k[1] == '5')
      {
        k[2] = Serial.read();
        if(k[2] == '1')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 1;
          DongCoC = 4;
        }
        else if(k[2] == '2')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 2;
          DongCoC = 4;
        }
        else if(k[2] == '3')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 3;
          DongCoC = 4;
        }
        else if(k[2] == '4')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 4;
          DongCoC = 4;
        }
        else if(k[2] == '5')
        {
		      Return_Zero();
          DongCoA = 5;
          DongCoB = 5;
          DongCoC = 4;
        }
      }
    }
    if(k[0] == 'f')
    {
      k[1] = Serial.read() - 48;
      k[2] = Serial.read() - 48;
      humid = k[1]*10 + k[2];
    }
  }

  if(mySerial.available() > 1)
  {
    m[0] = mySerial.read();
    if(m[0] == 't')
    {
      m[1] = mySerial.read();
      if(m[1] == '4')
      {
        Check_Humility();
      }
      if(m[1] == '5')
      {
        Return_Zero();  
      }
    }  
  }
  //Dong co A, truc x
  while(DongCoA == 1) 
  {
    q = xungA + xungA1*4 + truc1;
    stepperA.moveTo(q);
    stepperA.run();
    if (stepperA.distanceToGo() == 0) 
    {
      DongCoA = 0;
    }
  }
  while(DongCoA == 2) 
  {
    stepperA.moveTo(xungA + xungA1*3 + truc1);
    stepperA.run();
    q = xungA + xungA1*3 + truc1;
    if (stepperA.distanceToGo() == 0) 
    {
      DongCoA = 0;
    }
  }
  while(DongCoA == 3) 
  {
    stepperA.moveTo(xungA + xungA1*2 + truc1);
    stepperA.run();
    q = xungA + xungA1*2 + truc1;
    if (stepperA.distanceToGo() == 0) 
    {
      DongCoA = 0;
    }
  }
  while(DongCoA == 4) 
  {
    stepperA.moveTo(xungA + xungA1 + truc1);
    stepperA.run();
    q = xungA + xungA1 + truc1;
    if (stepperA.distanceToGo() == 0) 
    {
      DongCoA = 0;
    }
  }
  while(DongCoA == 5) 
  {
    stepperA.moveTo(xungA + truc1);
    q = xungA + truc1;
    stepperA.run();
    if (stepperA.distanceToGo() == 0) 
    {
      DongCoA = 0;
    }
  }
  while(DongCoA == 6) //gieo xong roi tuoi, toa do X de tuoi
  {
  	stepperA.moveTo(q - tuoi);
  	stepperA.run();
  	if (stepperA.distanceToGo() == 0)
  	{
      q = 0;
	    mySerial.print(7);
      DongCoA = 0;
      DongCoC = 9;
  	}
  }
  while(DongCoA == 7) //tọa độ x1
  {
    q = a1 + truc1;
  	stepperA.moveTo(q);
  	stepperA.run();
  	if (stepperA.distanceToGo() == 0)
  	{
  	  DongCoA = 0;
  	}
  }
  while(DongCoA == 9) //tọa độ x2
  {
    q = a2 + truc1;
  	stepperA.moveTo(q);
  	stepperA.run();
  	if (stepperA.distanceToGo() == 0)
  	{
  	  DongCoA = 0;
  	}
  }
  while(DongCoA == 10) //tọa độ x3
  {
    q = a3 + truc1;
  	stepperA.moveTo(q);
  	stepperA.run();
  	if (stepperA.distanceToGo() == 0)
  	{
  	  DongCoA = 0;
  	}
  }
  while(DongCoA == 11) //tọa độ x4
  {
    q = a4 + truc1;
  	stepperA.moveTo(q);
  	stepperA.run();
  	if (stepperA.distanceToGo() == 0)
  	{
  	  DongCoA = 0;
  	}
  }
  while(DongCoA == 12) //tọa độ x5
  {
    q = a5 + truc1;
  	stepperA.moveTo(q);
  	stepperA.run();
  	if (stepperA.distanceToGo() == 0)
  	{
  	  DongCoA = 13;
  	}
  }
  while(DongCoA == 13) //tọa độ x1
  {
  	stepperA.moveTo(a5 - a1);
  	stepperA.run();
  	if (stepperA.distanceToGo() == 0)
  	{
  	  DongCoA = 0;
  	}
  }
  while(DongCoA == 15) //tưới sau khi đo
  {
    stepperA.moveTo(q - tuoiSDA);
    stepperA.run();
    if (stepperA.distanceToGo() == 0)
    {
      DongCoA = 0;
      mySerial.print(5);
      DongCoC = 9;
    }
  }
  while(DongCoA == 16) //về lại vị trí ban nãy
  {
    stepperA.moveTo(q);
    stepperA.run();
    q = 0;
    if (stepperA.distanceToGo() == 0)
    {
      DongCoA = 0;
    }
  }
  
    
  //Dong co B, truc y
  while(DongCoB == 1) 
  {
    stepperB.moveTo(xungB + xungB1*4 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0)
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 2) 
  {
    stepperB.moveTo(xungB + xungB1*3 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0)
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 3) 
  {
    stepperB.moveTo(xungB + xungB1*2 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0) 
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 4) 
  {
    stepperB.moveTo(xungB + xungB1 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0)
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 5) 
  {
    stepperB.moveTo(xungB + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0) 
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 7) //tọa độ y1
  {
    stepperB.moveTo(b1 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0) 
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 9) //tọa độ y2
  {
    stepperB.moveTo(b2 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0) 
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 10) //tọa độ y3
  {
    stepperB.moveTo(b3 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0) 
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 11) //tọa độ y4
  {
    stepperB.moveTo(b4 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0) 
    {
      DongCoB = 0;
    }
  }
  while(DongCoB == 12) //tọa độ y5
  {
    stepperB.moveTo(b5 + truc2);
    stepperB.run();
    if (stepperB.distanceToGo() == 0) 
    {
      DongCoB = 0;
    }
  }
  
  //Dong Co C, truc z
  while(DongCoC == 1) 
  {
    stepperC.moveTo(gieohat + truc3);
    stepperC.run();
    if (stepperC.distanceToGo() == 0) 
    {
      mySerial.print(1);
      DongCoC = 9;
    }
  }
  while(DongCoC == 2) 
  {
    stepperC.moveTo(gieohat + truc3);
    stepperC.run();
    if (stepperC.distanceToGo() == 0) 
    {
      mySerial.print(2);
      DongCoC = 9;
    }
  }
  while(DongCoC == 3) 
  {
    stepperC.moveTo(gieohat + truc3);
    stepperC.run();
    if (stepperC.distanceToGo() == 0) 
    {
      mySerial.print(3);
      DongCoC = 9;
    }
  }
  while(DongCoC == 4) 
  {
    stepperC.moveTo(gieohat + truc3);
    stepperC.run();
    if (stepperC.distanceToGo() == 0) 
    {
      mySerial.print(4);
      DongCoC = 9;
    }
  }
  while(DongCoC == 5) 
  {
    stepperC.moveTo(doam + truc3);
    stepperC.run();
    if (stepperC.distanceToGo() == 0) 
    {
	    Serial.print('h');
      Serial.print(z);
      delay(5000);
    }
  }
  while(DongCoC == 6) 
  {
    stepperC.moveTo(dietco + truc3);
    stepperC.run();
    if (stepperC.distanceToGo() == 0)
    {
      DongCoC = 8;
    }
  }
  while(DongCoC == 7) 
  {
    stepperC.moveTo(truc3);
    stepperC.run();
    if (stepperC.distanceToGo() == 0)
    {
	    DongCoA = 6;
      DongCoC = 0;
    }
  }
  while(DongCoC == 8) 
  {
    stepperC.moveTo(truc3);
    stepperC.run();
    if (stepperC.distanceToGo() == 0)
    {
      DongCoC = 0;
    }
  }
  while(DongCoC == 9)
  {
    if(mySerial.available() > 1)
    {
      m[0] = mySerial.read();
      if (m[0] == 't')
      {
        m[1] = mySerial.read();
        if (m[1] == '1')
        {
          DongCoC = 7;
        }
        if (m[1] == '2')
        {
          DongCoC = 0;
          Return_Zero();
        }
        if (m[1] == '3')
        {
          DongCoA = 16;
          DongCoC = 0;
        }
      }
    }
  }      
}

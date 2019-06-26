#include <Servo.h>
#include <Wire.h>

/* Địa chỉ của DS1307 */
const byte DS1307 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;
 
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;

#define S1  	3
#define S2  	5
#define S3  	6
#define S4  	9
#define Reset   12
#define van     11

Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;

int pos1, pos2, pos3, pos4;
int Receive;

void readDS1307()
{
  Wire.beginTransmission(DS1307);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom(DS1307, NumberOfFields);
  second = bcd2dec(Wire.read() & 0x7f);
  minute = bcd2dec(Wire.read() );
  hour   = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
  wday   = bcd2dec(Wire.read() );
  day    = bcd2dec(Wire.read() );
  month  = bcd2dec(Wire.read() );
  year   = bcd2dec(Wire.read() );
  year += 2000;    
}
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num)
{
  return ((num/16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num)
{
  return ((num/10 * 16) + (num % 10));
}

void Hen_Gio()
{
  if(((hour == 3) && (minute == 0)) || ((hour == 7) && (minute == 0)) || ((hour == 11) && (minute == 0)) || ((hour == 15) && (minute == 0)) || ((hour == 19) && (minute == 0)) || ((hour == 23) && (minute == 0)))  
  {
    Serial.print('t');
    Serial.print('4');
  }
}
/* cài đặt thời gian cho DS1307 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0x00)); // đặt lại pointer
  Wire.write(dec2bcd(sec));
  Wire.write(dec2bcd(min));
  Wire.write(dec2bcd(hr));
  Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
  Wire.write(dec2bcd(d)); 
  Wire.write(dec2bcd(mth));
  Wire.write(dec2bcd(yr));
  Wire.endTransmission();
}

void setup() 
{
  Wire.begin();
  setTime(9, 0, 0, 5, 27, 6, 19); // 09:00:00 T5 26-06-2019
  Serial.begin(9600);
  pinMode(Reset, INPUT);
  digitalWrite(Reset, LOW);
  pinMode(van, OUTPUT);
  digitalWrite(van, LOW);
  setTime(12, 30, 45, 1, 8, 2, 15); // 12:30:45 CN 08-02-2015
  Servo1.attach(S1);
  Servo2.attach(S2);
  Servo3.attach(S3);
  Servo4.attach(S4);
}

void loop() 
{
   /* Đọc dữ liệu của DS1307 */
  readDS1307();
  Hen_Gio();
  
  if (digitalRead(Reset) == HIGH)
  {
	  Serial.print('t');
    Serial.print('5');
  }
  
  if (Serial.available() > 0)
  { 
    Receive = Serial.read() - 48;
    if (Receive == 1)
    {
      for (pos1 = 0; pos1 <= 180; pos1 += 1) 
      { 
        Servo1.write(pos1);              
        delay(15);                       
      }
		  Serial.print('t');
      Serial.print('1');
    }
    else if (Receive == 2)
    {
      for (pos2 = 0; pos2 <= 180; pos2 += 1) 
      { 
        Servo2.write(pos2);              
        delay(15);                       
      }
      Serial.print('t');
      Serial.print('1');
    }
    else if (Receive == 3)
    {
      for (pos3 = 0; pos3 <= 180; pos3 += 1) 
      { 
        Servo3.write(pos3);              
        delay(15);                       
      }
    	Serial.print('t');
    	Serial.print('1');
    }
    else if (Receive == 4)
    {
      for (pos4 = 0; pos4 <= 180; pos4 += 1) 
      { 
        Servo4.write(pos4);              
        delay(15);                       
      }
    	Serial.print('t');
      Serial.print('1');
    }
	  else if (Receive == 5)
	  {
  		digitalWrite(van, HIGH);
  		delay(2000);
  		digitalWrite(van, LOW);
      delay(1500);
  		Serial.print('t');
  		Serial.print('3');
	  }
	  else if (Receive == 7)
	  {
  		digitalWrite(van, HIGH);
  		delay(2000);
  		digitalWrite(van, LOW);
      delay(1500);
  		Serial.print('t');
  		Serial.print('2');
	  }
  }
}

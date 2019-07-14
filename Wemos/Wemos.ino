#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define Humid     A0

byte m[4];
String growth, size;
int humi;
int giatriAnalog;
unsigned char id = 0; //số vị trí
int pos;

// SSID VÀ PASSWORD KẾT NỐI WIFI
const char* ssid     = "ATPco";
const char* password = "@welcome2*you*";

// HOST - DOMAIN NAME CỦA SERVER
const char* host = "1513025.000webhostapp.com";

WiFiServer server(80); // Creates a server that listens for incoming connections on the specified port, here in this case port is 80.
WiFiClient client;

//HÀM CHUYỂN ĐỔI ĐỘ ẨM TỪ ĐIỆN ẤP
void Humility()
{
  giatriAnalog = analogRead(Humid);
  delay(500);
  humi = map(giatriAnalog, 0, 1023,99,0);
  Serial.print(id);
  Serial.print(humi);
  Serial.println('%');  
}

void WiFiCon() 
{
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  server.begin();
  
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//HÀM ĐẨY ĐỘ ẨM ĐẤT LÊN WEBSERVER
void UpHumility_ToServer(int id, int humi, double time_late_ms)
{
  delay(10);

  // Use WiFiClient class to create TCP connections
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    return;
  }

  // We now create a URI for the request
  String url = "/hienthi/index.php?";
  url += "id=";
  url += id;
  url += "&humi=";
  url += humi;

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 5000) 
	{
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) 
  {
    String line = client.readStringUntil('\r');
  }

  delay(time_late_ms);
}

//HÀM ĐẨY ĐỘ TĂNG TRƯỞNG LÊN WEBSERVER
void UpGrowth_ToServer(int pos, String growth, double time_late_ms)
{
  delay(10);

  // Use WiFiClient class to create TCP connections
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    return;
  }

  // We now create a URI for the request
  String url = "/hienthi/growth_index.php?";
  url += "pos=";
  url += pos;
  url += "&size=";
  url += size;

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 5000) 
	{
      //Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) 
  {
    String line = client.readStringUntil('\r');
  }
  delay(time_late_ms);
}

//HÀM ĐIỀU KHIỂN ĐỘNG CƠ TRỤC X,Y,Z VÀ SERVO GIEO HẠT
void Servo_Controller()
{
  HTTPClient http;    //Declare object of class HTTPClient
 
  String Link;
  Link = "http://1513025.000webhostapp.com/vitri/show.php";
  
  http.begin(Link);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  if(httpCode > 0)
  {
    Serial.printf("[HTTP] GET… code: %d\n", httpCode);
    if(httpCode == HTTP_CODE_OK)
    {
      String req = http.getString();    //Get the response payload
      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(req);    //Print request response payload
      if (req.indexOf("26=a") != -1)
	    {
  	    if (req.indexOf("1=on") != -1) 
        {
          Serial.print('a');
          Serial.print('1');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("2=on") != -1) 
        {
          Serial.print('a');
          Serial.print('1');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("3=on") != -1) 
        {
          Serial.print('a');
          Serial.print('1');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("4=on") != -1) 
        {
          Serial.print('a');
          Serial.print('1');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("5=on") != -1) 
        {
          Serial.print('a');
          Serial.print('1');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("6=on") != -1) 
        {
          Serial.print('a');
          Serial.print('2');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("7=on") != -1) 
        {
          Serial.print('a');
          Serial.print('2');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("8=on") != -1) 
        {
          Serial.print('a');
          Serial.print('2');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("9=on") != -1) 
        {
          Serial.print('a');
          Serial.print('2');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("10=on") != -1) 
        {
          Serial.print('a');
          Serial.print('2');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("11=run") != -1) 
        {
          Serial.print('a');
          Serial.print('3');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("12=run") != -1) 
        {
          Serial.print('a');
          Serial.print('3');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("13=run") != -1) 
        {
          Serial.print('a');
          Serial.print('3');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("14=run") != -1) 
        {
          Serial.print('a');
          Serial.print('3');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("15=run") != -1) 
        {
          Serial.print('a');
          Serial.print('3');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("16=run") != -1) 
        {
          Serial.print('a');
          Serial.print('4');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("17=run") != -1) 
        {
          Serial.print('a');
          Serial.print('4');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("18=run") != -1) 
        {
          Serial.print('a');
          Serial.print('4');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("19=run") != -1) 
        {
          Serial.print('a');
          Serial.print('4');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("20=act") != -1) 
        {
          Serial.print('a');
          Serial.print('4');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("21=act") != -1) 
        {
          Serial.print('a');
          Serial.print('5');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("22=act") != -1) 
        {
          Serial.print('a');
          Serial.print('5');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("23=act") != -1) 
        {
          Serial.print('a');
          Serial.print('5');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("24=act") != -1) 
        {
          Serial.print('a');
          Serial.print('5');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("25=act") != -1) 
        {
          Serial.print('a');
          Serial.print('5');
          Serial.print('5');
          delay(150);
        }
	  }
      else if (req.indexOf("27=b") != -1)
      {
        if (req.indexOf("1=on") != -1) 
        {
          Serial.print('b');
          Serial.print('1');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("2=on") != -1) 
        {
          Serial.print('b');
          Serial.print('1');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("3=on") != -1) 
        {
          Serial.print('b');
          Serial.print('1');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("4=on") != -1) 
        {
          Serial.print('b');
          Serial.print('1');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("5=on") != -1) 
        {
          Serial.print('b');
          Serial.print('1');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("6=on") != -1) 
        {
          Serial.print('b');
          Serial.print('2');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("7=on") != -1) 
        {
          Serial.print('b');
          Serial.print('2');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("8=on") != -1) 
        {
          Serial.print('b');
          Serial.print('2');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("9=on") != -1) 
        {
          Serial.print('b');
          Serial.print('2');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("10=on") != -1) 
        {
          Serial.print('b');
          Serial.print('2');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("11=run") != -1) 
        {
          Serial.print('b');
          Serial.print('3');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("12=run") != -1) 
        {
          Serial.print('b');
          Serial.print('3');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("13=run") != -1) 
        {
          Serial.print('b');
          Serial.print('3');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("14=run") != -1) 
        {
          Serial.print('b');
          Serial.print('3');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("15=run") != -1) 
        {
          Serial.print('b');
          Serial.print('3');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("16=run") != -1) 
        {
          Serial.print('b');
          Serial.print('4');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("17=run") != -1) 
        {
          Serial.print('b');
          Serial.print('4');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("18=run") != -1) 
        {
          Serial.print('b');
          Serial.print('4');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("19=run") != -1) 
        {
          Serial.print('b');
          Serial.print('4');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("20=act") != -1) 
        {
          Serial.print('b');
          Serial.print('4');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("21=act") != -1) 
        {
          Serial.print('b');
          Serial.print('5');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("22=act") != -1) 
        {
          Serial.print('b');
          Serial.print('5');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("23=act") != -1) 
        {
          Serial.print('b');
          Serial.print('5');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("24=act") != -1) 
        {
          Serial.print('b');
          Serial.print('5');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("25=act") != -1) 
        {
          Serial.print('b');
          Serial.print('5');
          Serial.print('5');
          delay(150);
        }
      }
      else if (req.indexOf("28=c") != -1)
      {
        if (req.indexOf("1=on") != -1) 
        {
          Serial.print('c');
          Serial.print('1');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("2=on") != -1) 
        {
          Serial.print('c');
          Serial.print('1');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("3=on") != -1) 
        {
          Serial.print('c');
          Serial.print('1');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("4=on") != -1) 
        {
          Serial.print('c');
          Serial.print('1');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("5=on") != -1) 
        {
          Serial.print('c');
          Serial.print('1');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("6=on") != -1) 
        {
          Serial.print('c');
          Serial.print('2');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("7=on") != -1) 
        {
          Serial.print('c');
          Serial.print('2');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("8=on") != -1) 
        {
          Serial.print('c');
          Serial.print('2');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("9=on") != -1) 
        {
          Serial.print('c');
          Serial.print('2');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("10=on") != -1) 
        {
          Serial.print('c');
          Serial.print('2');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("11=run") != -1) 
        {
          Serial.print('c');
          Serial.print('3');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("12=run") != -1) 
        {
          Serial.print('c');
          Serial.print('3');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("13=run") != -1) 
        {
          Serial.print('c');
          Serial.print('3');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("14=run") != -1) 
        {
          Serial.print('c');
          Serial.print('3');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("15=run") != -1) 
        {
          Serial.print('c');
          Serial.print('3');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("16=run") != -1) 
        {
          Serial.print('c');
          Serial.print('4');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("17=run") != -1) 
        {
          Serial.print('c');
          Serial.print('4');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("18=run") != -1) 
        {
          Serial.print('c');
          Serial.print('4');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("19=run") != -1) 
        {
          Serial.print('c');
          Serial.print('4');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("20=act") != -1) 
        {
          Serial.print('c');
          Serial.print('4');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("21=act") != -1) 
        {
          Serial.print('c');
          Serial.print('5');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("22=act") != -1) 
        {
          Serial.print('c');
          Serial.print('5');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("23=act") != -1) 
        {
          Serial.print('c');
          Serial.print('5');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("24=act") != -1) 
        {
          Serial.print('c');
          Serial.print('5');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("25=act") != -1) 
        {
          Serial.print('c');
          Serial.print('5');
          Serial.print('5');
          delay(150);
        }
      }
      else if (req.indexOf("29=d") != -1)
      {
        if (req.indexOf("1=on") != -1) 
        {
          Serial.print('d');
          Serial.print('1');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("2=on") != -1) 
        {
          Serial.print('d');
          Serial.print('1');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("3=on") != -1) 
        {
          Serial.print('d');
          Serial.print('1');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("4=on") != -1) 
        {
          Serial.print('d');
          Serial.print('1');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("5=on") != -1) 
        {
          Serial.print('d');
          Serial.print('1');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("6=on") != -1) 
        {
          Serial.print('d');
          Serial.print('2');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("7=on") != -1) 
        {
          Serial.print('d');
          Serial.print('2');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("8=on") != -1) 
        {
          Serial.print('d');
          Serial.print('2');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("9=on") != -1) 
        {
          Serial.print('d');
          Serial.print('2');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("10=on") != -1) 
        {
          Serial.print('d');
          Serial.print('2');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("11=run") != -1) 
        {
          Serial.print('d');
          Serial.print('3');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("12=run") != -1) 
        {
          Serial.print('d');
          Serial.print('3');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("13=run") != -1) 
        {
          Serial.print('d');
          Serial.print('3');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("14=run") != -1) 
        {
          Serial.print('d');
          Serial.print('3');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("15=run") != -1) 
        {
          Serial.print('d');
          Serial.print('3');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("16=run") != -1) 
        {
          Serial.print('d');
          Serial.print('4');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("17=run") != -1) 
        {
          Serial.print('d');
          Serial.print('4');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("18=run") != -1) 
        {
          Serial.print('d');
          Serial.print('4');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("19=run") != -1) 
        {
          Serial.print('d');
          Serial.print('4');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("20=act") != -1) 
        {
          Serial.print('d');
          Serial.print('4');
          Serial.print('5');
          delay(150);
        }
        if (req.indexOf("21=act") != -1) 
        {
          Serial.print('d');
          Serial.print('5');
          Serial.print('1');
          delay(150);
        }
        if (req.indexOf("22=act") != -1) 
        {
          Serial.print('d');
          Serial.print('5');
          Serial.print('2');
          delay(150);
        }
        if (req.indexOf("23=act") != -1) 
        {
          Serial.print('d');
          Serial.print('5');
          Serial.print('3');
          delay(150);
        }
        if (req.indexOf("24=act") != -1) 
        {
          Serial.print('d');
          Serial.print('5');
          Serial.print('4');
          delay(150);
        }
        if (req.indexOf("25=act") != -1) 
        {
          Serial.print('d');
          Serial.print('5');
          Serial.print('5');
          delay(150);
        }
      }    
    }
  } 
  else 
  {
    Serial.printf("[HTTP] GET… failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();  //Close connection
  delay(500);
}

void setup()
{
  Serial.begin(9600);
  delay(10);
  WiFiCon();
}

void loop()
{
  Servo_Controller();
  if(Serial.available() > 2)
  {
  	m[0] = Serial.read();
  	if (m[0] == 'h')
  	{
  	  m[1] = Serial.read() - 48;
      m[2] = Serial.read() - 48;
      id = m[2]*10 + m[1];
      Humility();
  	  UpHumility_ToServer((int) id, (int) humi, 100);
      Serial.print('f');
      Serial.println(humi);
    
  	}
  	if (m[0] == 'e')
  	{
  		m[1] = Serial.read();
      if(m[1] == '1')
      {
        m[2] = Serial.read();
        for(pos = 1; pos < 26; pos++)
        {
          if(m[2] == '1')
          {
            size = "small";
            UpGrowth_ToServer((int) pos, (String) growth, 100);
          }
          if(m[2] == '2')
          {
            size = "normal";
            UpGrowth_ToServer((int) pos, (String) growth, 100);
          }
          if(m[2] == '3')
          {
            size = "big";
            UpGrowth_ToServer((int) pos, (String) growth, 100);
          }
        }
      }
  	}
  }
} 

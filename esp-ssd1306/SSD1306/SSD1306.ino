#include <DHTesp.h>
#include <ESP8266WiFi.h>

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "images.h"

SSD1306Wire  display(0x3c, D6, D5);
// SH1106 display(0x3c, D3, D5);

#define DEMO_DURATION 3000
typedef void (*Demo)(void);

const char* ssid = "VOffice";
const char* password = "KhaneyeM@38-2";
DHTesp DHT;

#define DHT11_PIN 2
float maxT, minT, maxH, minH;
float temp;
float hum;
int d = 0;
String ip ; 
int ledPin = 15; // 8
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  maxT = -99999;
  minT = 99999;
  minH = 100;
  maxH = 0;

  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, ssid);

  display.display();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
 }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  ip =  WiFi.localIP().toString();
  Serial.print(ip);
  Serial.println("/");

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0,  WiFi.localIP().toString());
  display.display();
  delay(2000);
  DHT.setup(DHT11_PIN);
}



void loop() {
  // clear the display
  display.clear();

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0,  WiFi.localIP().toString());
    //display.drawString(0, 0, "Hello");

  int L = 0;
  float  rTemp;
  float  rHum;
  String s = String(64);
  delay(DHT.getMinimumSamplingPeriod());
    rTemp = DHT.getTemperature();
    rHum = DHT.getHumidity();
    if(!isnan(rTemp))
      temp = rTemp;
    if(!isnan(rHum))     
      hum = rHum;
      
    if (temp > maxT)
      maxT = temp;
    if (temp < minT)
      minT = temp;

    if (hum > maxH)
      maxH = hum;
    if (hum < minH)
      minH = hum;

    s = (String)temp /* + (char)223 */ + "C  " + (String)hum + "%";
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0,15, (String)temp  + (char)223  + "C  " + (String)hum + "%");
    
    if (d<10)
      {display.drawString(0, 40, "Max Temp:"+(String)maxT);}
    else if (d<20)
      {display.drawString(0, 40, "Min Temp:"+(String)minT);}
    else if (d<30)
      {display.drawString(0, 40, "Max Hum: "+(String)maxH);}
    else if (d<40)
      {display.drawString(0, 40, "Min Hum: "+(String)minH);}
    else if (d>40)
      {d=0;}
      
    d = d+1;    
    Serial.println("temp data:"+s);

    display.display();
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }

  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<h1> Here is the ....(location) </h1>");
 client.println("<br><br>");
   client.print("<h2>Led pin is now: ");

  if (value != HIGH) {
    client.print("Off </h2>");
  } else {
    client.print("ON </h2>");
  }
  client.println("<br><br>");
  client.println("<h2> Ambient temprature and humidirty: " + s + " </h2>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

    display.display();
    delay(1);
}

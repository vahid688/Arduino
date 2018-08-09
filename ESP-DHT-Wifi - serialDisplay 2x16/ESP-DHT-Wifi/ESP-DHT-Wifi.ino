#include <DHTesp.h>
#include <ESP8266WiFi.h>
//#include <dht.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2); 


//const char* ssid = "Vandod";
//const char* password = "KhaneyeM@38";

const char* ssid = "wilspot";
const char* password = "ABCabc123";

DHTesp DHT;

#define DHT11_PIN 2
float maxT, minT, maxH, minH;
float temp;
float hum;
int d = 0;
 
int ledPin = 12; // GPIO13
WiFiServer server(80);

void setup() {
  lcd.init();
  lcd.backlight();
  
  maxT = -99999;
  minT = 99999;
  minH = 100;
  maxH = 0;

  Serial.begin(9600);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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
  Serial.print(WiFi.localIP());
  Serial.println("/");
  DHT.setup(DHT11_PIN);
}

void loop() {
  //int chk = DHT.read11(DHT11_PIN);
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
    lcd.setCursor(0,0); //col, row  zero based
    lcd.print((String)temp  + (char)223  + "C  " + (String)hum + "%");

    lcd.setCursor(0,1);
    if (d<10)
      {lcd.print("Max Temp:"+(String)maxT);}
    else if (d<20)
      {lcd.print("Min Temp:"+(String)minT);}
    else if (d<30)
      {lcd.print("Max Hum: "+(String)maxH);}
    else if (d<40)
      {lcd.print("Min Hum: "+(String)minH);}
    else if (d>40)
      {d=0;}
      
    d = d+1;    
    Serial.println("temp data:"+s);
    
  // Check if a client has connected
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

}


#include <DHTesp.h>
#include <ESP8266WiFi.h>
//#include <dht.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);



const char* ssid = "VOffice";
const char* password = "KhaneyeM@38-2";

DHTesp DHT;

#define DHT11_PIN 12 //esp mini 06 digital
float maxT, minT, maxH, minH;
float temp;
float hum;

int ledPin = 13; // GPIO13  esp 08 digital
WiFiServer server(80);

void setup() {
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
  sensors.begin();
}

void loop() {
    // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));  

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

  client.print("Led pin is now: ");

  if (value == HIGH) {
    client.print("Off");
  } else {
    client.print("ON");
  }
  client.println("<br><br>");
  client.println("<h2> Ambient temprature and humidirty" + s + " </h2>");
  client.println("<h1> Here is the first Arduino server </h1>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn Off </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>

const char* ssid = "";
const char* password = ""; 

WiFiServer server(80);

#define screenWidth 128
#define screenHeight 64

#define DHTTYPE DHT11
#define DHTPIN 5

#define buttonPin 13

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, -1);

float temperature;
float humidity;

unsigned long long currentTime = millis();
unsigned long long previousTime = 0;
const int timeoutTime = 200000;
String header;

//bool lastButtonState = LOW;
bool buttonState = LOW;

void readHumidityAndTemperature () {
  temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("failed to read temperature");
    delay(1000);

    display.println("N/A");
    display.display();

    return;
  }

  humidity = dht.readHumidity();

  if (isnan(humidity)) {
    Serial.println("failed to read humidity");
    delay(1000);

    display.println("N/A");
    display.display();

    return;
  }
}

void printTemperature () {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("TEMPERATURE:");

  display.setTextSize(3);
  display.setCursor(0, 20);
  display.print(temperature);
  display.println("C");

  display.display();
}

void printHumidity () {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("HUMIDITY:");

  display.setTextSize(3);
  display.setCursor(0, 20);
  display.print(humidity);
  display.println("%");

  display.display();
}

void buttonPressed () {
  buttonState = !buttonState;
  Serial.println("button pressed");
}

void setup() {
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    while(true);
  }

  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, RISING);

  pinMode(DHTPIN, INPUT);
  pinMode(buttonPin, INPUT);

  //connecting to the the wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //starting the server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

}

void loop() {
  readHumidityAndTemperature();
  delay(500);

  if (buttonState == HIGH) {
    printHumidity();
  } else {
    printTemperature();
  }

  //server wait for a request
  WiFiClient client = server.available();

  if (client) { // if a new client connect
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = ""; // String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // while the client is connected
      currentTime = millis();
      if (client.available()) { // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c); // print it out the serial monitor
        header += c;
        if (c == '\n') { // if the byte is a newline character
          if (currentLine.length() == 0) { // if the current line is blank, you got two newline characters in a row. that's the end of the client HTTP request, so send a response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!doctype html><html>");
            client.println("<head lang=en_GB> <meta charset=utf-8> <meta name=viewport content=width=device-width/> <meta http-equiv=refresh content=10/>");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // Web Page Heading
            client.println("<body><h1>Temperature & Humidity with DHT11</h1>");

            client.println("<div class=container> <div id=temp>");
            client.println("<h2>Temperature</h2> <p class=text> <br>" + String(temperature) + "°C</p> </div>");

            client.println("<div id=hum>");
            client.println("<h2>Humidity</h2> <p class=text> <br>" + String(humidity) + "%</p> </div>");

            client.println("</div></body></html>");

            client.println("<style>html {background-color: #292929; color: white; font-family: Courier-new, monospace;}");
            client.println("h1 {text-align: center; text-shadow: 3px 3px 1px grey;}");
            client.println(".container {display: flex; justify-content: space-between; margin: 20px;}");
            client.println("#temp, #hum {border: 2px solid white; padding: 20px; width: 800px; height: auto; border-radius: 10px; display: flex; flex-direction: column; align-items: center;}");
            client.println("#temp {background-color: #4f757d;}");
            client.println("#hum {background-color: #4f7d55;}");
            client.println(".text {font-size: 24px; margin-top: 10px;}");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

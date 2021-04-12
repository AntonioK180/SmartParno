#include <ESP8266WiFi.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
const char auth[] = "";

// Network settings
const char ssid[] = "";
const char pass[] = "";

// Blynk cloud server
const char* host = "blynk-cloud.com";
unsigned int port = 8080;

int motorControl = 0;
int motorPosition = 0;

int MotorIn1 = 15;
int MotorIn2 = 13;
int enable = D1;

WiFiClient client;

// Start the WiFi connection
void connectNetwork()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

bool httpRequest(const String& method,
                 const String& request,
                 String&       response)
{
  Serial.print(F("Connecting to "));
  Serial.print(host);
  Serial.print(":");
  Serial.print(port);
  Serial.print("... ");
  if (client.connect(host, port)) {
    Serial.println("OK");
  } else {
    Serial.println("failed");
    return false;
  }

  client.print(method); client.println(F(" HTTP/1.1"));
  client.print(F("Host: ")); client.println(host);
  client.println(F("Connection: close"));
  if (request.length()) {
    client.println(F("Content-Type: application/json"));
    client.print(F("Content-Length: ")); client.println(request.length());
    client.println();
    client.print(request);
  } else {
    client.println();
  }

  //Serial.println("Waiting response");
  int timeout = millis() + 3000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return false;
    }
  }

  //Serial.println("Reading response");
  int contentLength = -1;
  while (client.available()) {
    String line = client.readStringUntil('\n');
    line.trim();
    line.toLowerCase();
    if (line.startsWith("content-length:")) {
      contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
    } else if (line.length() == 0) {
      break;
    }
  }

  //Serial.println("Reading response body");
  response = "";
  response.reserve(contentLength + 1);
  while (response.length() < contentLength && client.connected()) {
    while (client.available()) {
      char c = client.read();
      response += c;
    }
  }
  client.stop();
  return true;
}

void brake(){
  digitalWrite(MotorIn1, LOW);
  digitalWrite(MotorIn2, LOW);
}

void runClockwise(){
  analogWrite(enable, 900);
  digitalWrite(MotorIn1, HIGH);
  digitalWrite(MotorIn2, LOW);
}

void runCounterClockwise(){
  analogWrite(enable, 900);
  digitalWrite(MotorIn1, LOW);
  digitalWrite(MotorIn2, HIGH);
}


void setup(){
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
  pinMode(enable, OUTPUT);
  
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println();

  connectNetwork();
}

void loop() {
  brake();
  String response;

  if (httpRequest(String("GET /") + auth + "/get/V3", "", response)) {
    if(response != ""){
      motorControl = (response.charAt(2) - 48);
      Serial.println("Motor is at:");
      Serial.println(motorControl);
      if(motorControl == 1){
        runClockwise(); 
        Serial.println("Running clockwise.");
      } else if(motorControl == 2){
        brake();
        Serial.println("NOT RUNNING");
      } else if (motorControl == 3){
        runCounterClockwise();
        Serial.println("Running counter clockwise.");
      }
    }
  }

  if(motorControl == 1 || motorControl == 3){
    delay(3000);
  }
  
  delay(20);
}

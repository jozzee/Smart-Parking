#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <time.h>

//Defines pins numbers
#define LED_NODE 16 //D0, GIPO16 (D0 Hegiht = Low, Low = Height)

//NO.1
#define TRIG_NO_1 5  //D1, GIPO5
#define ECHO_NO_1 4  //D2, GPIO4
#define LED_NO_1 0  //D3, GPIO0

//No.2
#define TRIG_NO_2 2  //D4, GPIO2
#define ECHO_NO_2 14 //D5, GIPO14
#define LED_NO_2 12 //D6, GIPO12

#define TRIG_NO_3 13 //D7, GIPO13
#define ECHO_NO_3 15 //D8, GPIO15
#define LED_NO_3 10 //SDd3, GIPO10

#define TX 1
#define RX 3

#define POINT_1 "no1"
#define POINT_2 "no2"
#define POINT_3 "no3"

#define WIFI_SSID       "ROOM309"
#define WIFI_PASSWORD   "password"

#define FIREBASE_HOST "project-cb8fd.firebaseio.com" //https://project-cb8fd.firebaseio.com
#define FIREBASE_KEY "vAPQvwMb190kRe1Fc9pwAtBUmxDz1FoCg5XweFfo"

bool isBlankNo1 = true;
bool isBlankNo2 = true;
bool isBlankNo3 = true;

void toggleLedNode() {
  digitalWrite(LED_NODE, !digitalRead(LED_NODE));
}

int getDistance(String point) {
  // defines variables
  long duration;
  int trigPin = 0 ;
  int echoPin = 0;

  if (point == POINT_1) {
    trigPin = TRIG_NO_1;
    echoPin = ECHO_NO_1;
  } else if (point == POINT_2) {
    trigPin = TRIG_NO_2;
    echoPin = ECHO_NO_2;
  } else if (point == POINT_3) {
    trigPin = TRIG_NO_3;
    echoPin = ECHO_NO_3;
  }

  Serial.print(F("Get Distance From Point: "));
  Serial.println(point);

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  Serial.print(F("Duration: "));
  Serial.print(duration);
  Serial.println(F(" microseconds"));

  // Calculating the distance
  int distance = (duration * 0.034 / 2);

  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.println(F(" .cm"));
  //Serial.println(F("-----------------------------"));

  return distance;
}

void connectWiFi() {
  Serial.print(F("Connecting to: ")); Serial.println(WIFI_SSID);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(F("."));
    toggleLedNode();
    delay(500);
  }
  //digitalWrite(LED_NODE, HIGH);
  Serial.println(F(""));
  Serial.println(F("WiFi connected"));
  Serial.print(F("IP address: ")); Serial.println(WiFi.localIP());
}

void initialTime() {
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println(F("Initializing time"));
  while (!time(nullptr)) {
    Serial.print(F("."));
    toggleLedNode();
    delay(500);
  }
  Serial.println(F(""));
}

String getCurrentTime() {
  time_t now = time(nullptr);
  //Serial.println(ctime(&now));
  return (String(time(&now)));
}

String createHistory(String act) {
  //Example to user json
  //char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  //StaticJsonBuffer<200> jsonBuffer;
  //JsonObject& root = jsonBuffer.parseObject(json);

  return "{\"time\":" + getCurrentTime() + ",\"action\":" + act + "}";
}

void pushData(String point, bool parckingStatus, String action) {
  Serial.println("Push Data to Firebase...");
  //Update status
  Firebase.setString("status/" + String(point), parckingStatus ? "true" : "false");

  //Add history
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& historyObject = jsonBuffer.createObject();
  historyObject["time"] = getCurrentTime();
  historyObject["action"] = action;

  Firebase.push("history/" + String(point), historyObject);
  //...

  delay(1000);
}

void setup() {
  pinMode(LED_NODE, OUTPUT);

  pinMode(TRIG_NO_1, OUTPUT);
  pinMode(ECHO_NO_1, INPUT);
  pinMode(LED_NO_1, OUTPUT);

  pinMode(TRIG_NO_2, OUTPUT);
  pinMode(ECHO_NO_2, INPUT);
  pinMode(LED_NO_2, OUTPUT);

  pinMode(TRIG_NO_3, OUTPUT);
  pinMode(ECHO_NO_3, INPUT);
  pinMode(LED_NO_3, OUTPUT);

  //Starts the serial communication
  Serial.begin(115200);

  //Connecting to WiFi and initial time
  connectWiFi();
  initialTime();

  //Setup firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);

  delay(3000);
  Serial.println(F("Start..Program!!"));


  digitalWrite(LED_NO_1, ((isBlankNo1)  ? HIGH : LOW));
  digitalWrite(LED_NO_2, ((isBlankNo2)  ? HIGH : LOW));
  digitalWrite(LED_NO_3, ((isBlankNo3)  ? HIGH : LOW));

  Firebase.setString("status/" + String(POINT_1), isBlankNo1 ? "true" : "false");
  Firebase.setString("status/" + String(POINT_2), isBlankNo2 ? "true" : "false");
  Firebase.setString("status/" + String(POINT_3), isBlankNo3 ? "true" : "false");
}

void loop() {


  int minDistance = 100;
  int distance;

  Serial.print(F("At Time: "));
  Serial.println(getCurrentTime());
  Serial.println(F(""));

  //------------------------------------------------------
  distance = getDistance(POINT_1);
  if (distance < minDistance && isBlankNo1) {
    //Have a car parked
    Serial.println(F("Have a car parked at No.1"));

    digitalWrite(LED_NO_1, LOW);
    isBlankNo1 = false;

    //Send data to firebase.
    pushData(POINT_1, isBlankNo1, "In");

  } else if (distance > minDistance && !isBlankNo1) {
    //Free parking
    Serial.println(F("Free parking at No.1"));

    digitalWrite(LED_NO_1, HIGH);
    isBlankNo1 = true;

    //Send data to firebase.
    pushData(POINT_1, isBlankNo1, "Out");

  }
  Serial.println(F("--------------------------------------------"));

  //Delay 100 milisecond.
  delay(100);
  distance = 0;
  distance = getDistance(POINT_2);
  if (distance < minDistance && isBlankNo2) {
    //Have a car parked
    Serial.println(F("Have a car parked at No.2"));

    digitalWrite(LED_NO_2, LOW);
    isBlankNo2 = false;

    //Send data to firebase.
    pushData(POINT_2, isBlankNo2, "In");

  } else if (distance > minDistance && !isBlankNo2) {
    //Free parking
    Serial.println(F("Free parking at No.2"));

    digitalWrite(LED_NO_2, HIGH);
    isBlankNo2 = true;

    //Send data to firebase.
    pushData(POINT_2, isBlankNo2, "Out");
  }
  Serial.println(F("--------------------------------------------"));

  //Delay 100 milisecond.
  delay(100);
  distance = 0;
  distance = getDistance(POINT_3);
  if (distance < minDistance && isBlankNo3) {
    //Have a car parked
    Serial.println(F("Have a car parked at No.3"));

    digitalWrite(LED_NO_3, LOW);
    isBlankNo3 = false;

    //Send data to firebase.
    pushData(POINT_3, isBlankNo3, "In");

  } else if (distance > minDistance && !isBlankNo3) {
    //Free parking
    Serial.println(F("Free parking at No.3"));

    digitalWrite(LED_NO_3, HIGH);
    isBlankNo3 = true;

    //Send data to firebase.
    pushData(POINT_3, isBlankNo3, "Out");
  }
  Serial.println(F("--------------------------------------------"));

  delay(10000);
}

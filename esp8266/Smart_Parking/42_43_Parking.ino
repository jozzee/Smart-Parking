#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <time.h>

//Defines pins numbers
#define LED_NODE 16 //D0, GIPO16 (D0 Hegiht = Low, Low = Height)

//NO.01
#define TRIG_NO_42 5  //D1, GIPO5
#define ECHO_NO_42 4  //D2, GPIO4
#define LED_NO_42 0  //D3, GPIO0

//No.02
#define TRIG_NO_43 2  //D4, GPIO2
#define ECHO_NO_43 14 //D5, GIPO14
#define LED_NO_43 12 //D6, GIPO12

#define TX 1
#define RX 3

#define POINT_42 "no42"
#define POINT_43 "no43"

#define WIFI_SSID       "SmartParking"
#define WIFI_PASSWORD   "0972207328"

#define FIREBASE_HOST "project-cb8fd.firebaseio.com" //https://project-cb8fd.firebaseio.com
#define FIREBASE_KEY "vAPQvwMb190kRe1Fc9pwAtBUmxDz1FoCg5XweFfo"

bool isBlankNo42 = true;
bool isBlankNo43 = true;


void toggleLedNode() {
  digitalWrite(LED_NODE, !digitalRead(LED_NODE));
}

int getDistance(String point) {
  // defines variables
  long duration;
  int trigPin = 0 ;
  int echoPin = 0;

  if (point == POINT_42) {
    trigPin = TRIG_NO_42;
    echoPin = ECHO_NO_42;
  } else if (point == POINT_43) {
    trigPin = TRIG_NO_43;
    echoPin = ECHO_NO_43;
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
  Serial.print(F("IP address: ")); 
  Serial.println(WiFi.localIP());
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
  return (String(ctime(&now)));
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
    Serial.print(F("Set status is success: "));
    Serial.println(Firebase.success());
    if (Firebase.success() != 1) {
        Serial.print(F("Error: "));
        Serial.println(Firebase.error());
        Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
    }

    //Add history
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& historyObject = jsonBuffer.createObject();
    historyObject["time"] = getCurrentTime();
    historyObject["action"] = action;

    Firebase.push("history/" + String(point), historyObject);
    Serial.print(F("Add history is success: "));
    Serial.println(Firebase.success());
    if (Firebase.success() != 1) {
        Serial.print(F("Error: "));
        Serial.println(Firebase.error());
        Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
    }

  delay(1000);
}

void setup() {
  pinMode(LED_NODE, OUTPUT);

  pinMode(TRIG_NO_42, OUTPUT);
  pinMode(ECHO_NO_42, INPUT);
  pinMode(LED_NO_42, OUTPUT);

  pinMode(TRIG_NO_43, OUTPUT);
  pinMode(ECHO_NO_43, INPUT);
  pinMode(LED_NO_43, OUTPUT);


  //Starts the serial communication
  Serial.begin(115200);

  //Connecting to WiFi and initial time
  connectWiFi();
  initialTime();

  //Setup firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);

  delay(3000);
  Serial.println(F("Start..Program!!"));


  digitalWrite(LED_NO_42, ((isBlankNo42)  ? HIGH : LOW));
  digitalWrite(LED_NO_43, ((isBlankNo43)  ? HIGH : LOW));

  Firebase.setString("status/" + String(POINT_42), isBlankNo42 ? "true" : "false");
  Firebase.setString("status/" + String(POINT_43), isBlankNo43 ? "true" : "false");
}

void loop() {


  int minDistance = 190;
  int distance;

  Serial.print(F("At Time: "));
  Serial.println(getCurrentTime());
  Serial.println(F(""));

  //------------------------------------------------------
  distance = getDistance(POINT_42);
  if (distance < minDistance && isBlankNo42) {
    //Have a car parked
    Serial.println(F("Have a car parked at No.42"));

    digitalWrite(LED_NO_42, LOW);
    isBlankNo42 = false;

    //Send data to firebase.
    pushData(POINT_42, isBlankNo42, "In");

  } else if (distance > minDistance && !isBlankNo42) {
    //Free parking
    Serial.println(F("Free parking at No.42"));

    digitalWrite(LED_NO_42, HIGH);
    isBlankNo42 = true;

    //Send data to firebase.
    pushData(POINT_42, isBlankNo42, "Out");

  }
  Serial.println(F("--------------------------------------------"));

  //Delay 100 milisecond.
  delay(100);
  distance = 0;
  distance = getDistance(POINT_43);
  if (distance < minDistance && isBlankNo43) {
    //Have a car parked
    Serial.println(F("Have a car parked at No.43"));

    digitalWrite(LED_NO_43, LOW);
    isBlankNo43 = false;

    //Send data to firebase.
    pushData(POINT_43, isBlankNo43, "In");

  } else if (distance > minDistance && !isBlankNo43) {
    //Free parking
    Serial.println(F("Free parking at No.43"));

    digitalWrite(LED_NO_43, HIGH);
    isBlankNo43 = true;

    //Send data to firebase.
    pushData(POINT_43, isBlankNo43, "Out");
  }
  Serial.println(F("--------------------------------------------"));

  //Delay 1000 milisecond.
  delay(10000);
}

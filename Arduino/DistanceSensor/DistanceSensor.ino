#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
//#include <time.h>

#include <ArduinoJson.h>
#include <FirebaseArduino.h>

//Defines pins numbers
#define LED_NODE 16  //D0, GIPO16 (D0 Hegiht = Low, Low = Height)

//NO.1
#define TRIG_NO_1 5  //D1, GIPO5
#define ECHO_NO_1 4  //D2, GPIO4
#define LED_NO_1 0   //D3, GPIO0

//No.2
#define TRIG_NO_2 2   //D4, GPIO2
#define ECHO_NO_2 14  //D5, GIPO14
#define LED_NO_2 12   //D6, GIPO12

#define TRIG_NO_3 13  //D7, GIPO13
#define ECHO_NO_3 15  //D8, GPIO15
#define LED_NO_3 10   //SDd3, GIPO10

#define TX 1
#define RX 3

#define POINT_1 "no01"
#define POINT_2 "no02"
#define POINT_3 "no03"

#define WIFI_SSID "Your Wifi"
#define WIFI_PASSWORD "Your Password"

#define FIREBASE_HOST "project-cb8fd.firebaseio.com"  //https://project-cb8fd.firebaseio.com
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
    int trigPin = 0;
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

void connectWiFiRmuti() {
    char ssid[] = "@RMUTI-WiFi";
    char pass[] = "";

    Serial.print(F("Connecting to: "));
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    HTTPClient http;
    http.begin("http://afw-kkc.rmuti.ac.th/login.php?r004335174&url=http://www.msftconnecttest.com/redirect");
    Serial.print("[HTTP] POST...\n");
    int httpCode = http.POST("_u: narongchai.ph_p: 0944366656a: aweb_host: afw-kkc.rmuti.ac.thweb_host4: afw4-kkc.rmuti.ac.th_ip4: 172.25.170.171web_host6: _ip6: ");
    if (httpCode > 0) {
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
            Serial.println("Yes....You can do it");
        }
    } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}

void connectWiFi() {
    Serial.print(F("Connecting to: "));
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    //digitalWrite(LED_NODE, HIGH);
    Serial.println(F(""));
    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}

// void initialTime() {
//     configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
//     Serial.println(F("Initializing time"));
//     while (!time(nullptr)) {
//         Serial.print(F("."));
//         toggleLedNode();
//         delay(500);
//     }
//     Serial.println(F(""));
// }

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
    }

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
    //connectWiFiRmuti();
    //initialTime();

    //Setup firebase
    Serial.println(F("Setup Fireebase..."));
    Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
    if (Firebase.success() != 1) {
        Serial.print(F("Error: "));
        Serial.println(Firebase.error());
    }

    delay(3000);
    Serial.println(F("Start..Program!!"));

    digitalWrite(LED_NO_1, ((isBlankNo1) ? HIGH : LOW));
    digitalWrite(LED_NO_2, ((isBlankNo2) ? HIGH : LOW));
    digitalWrite(LED_NO_3, ((isBlankNo3) ? HIGH : LOW));

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
        Serial.print(F("Have a car parked at "));
        Serial.println(POINT_1);

        digitalWrite(LED_NO_1, LOW);
        isBlankNo1 = false;

        //Send data to firebase.
        pushData(POINT_1, isBlankNo1, "In");

    } else if (distance > minDistance && !isBlankNo1) {
        //Free parking
        Serial.print(F("Free parking at "));
        Serial.println(POINT_1);

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
        Serial.print(F("Have a car parked at No.2"));
        Serial.println(POINT_2);

        digitalWrite(LED_NO_2, LOW);
        isBlankNo2 = false;

        //Send data to firebase.
        pushData(POINT_2, isBlankNo2, "In");

    } else if (distance > minDistance && !isBlankNo2) {
        //Free parking
        Serial.print(F("Free parking at No.2"));
        Serial.println(POINT_2);

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
        Serial.print(F("Have a car parked at "));
        Serial.println(POINT_3);

        digitalWrite(LED_NO_3, LOW);
        isBlankNo3 = false;

        //Send data to firebase.
        pushData(POINT_3, isBlankNo3, "In");

    } else if (distance > minDistance && !isBlankNo3) {
        //Free parking
        Serial.print(F("Free parking at "));
        Serial.println(POINT_1);

        digitalWrite(LED_NO_3, HIGH);
        isBlankNo3 = true;

        //Send data to firebase.
        pushData(POINT_3, isBlankNo3, "Out");
    }
    Serial.println(F("--------------------------------------------"));

    delay(10000);
}

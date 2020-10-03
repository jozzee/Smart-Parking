#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SPI.h>
#include <Ticker.h>

#include "CharactersData.h"
#include "PxMatrix.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define FIREBASE_HOST "project-cb8fd.firebaseio.com"  //https://project-cb8fd.firebaseio.com
#define FIREBASE_KEY "vAPQvwMb190kRe1Fc9pwAtBUmxDz1FoCg5XweFfo"

#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2
Ticker display_ticker;

// Pins for LED MATRIX
uint8_t display_draw_time = 0;

//PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
PxMATRIX display(64, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D);
//PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t myCOLORS[8] = {myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myBLACK};
uint16_t TD_color = myRED;

int TD_max_row = 32;
int TD_max_col = 64;
int TD_max_char_row1 = 10;
int TD_max_char_row2 = 12;
const int TD_max_char_col = 16;
int TD_normal_row = 3;
const int TD_start_not_thai_char = 33;
const int TD_start_thai_char = 161;
const int TD_gap_pixel = 1;
int TD_led_delay = 15;

int g_pcman_char = 0;
int g_pcman_n = 0;

// thai over
const int char_char_x1 = 13;
int char_x1[char_char_x1] = {209, 212, 213, 214, 215, 231, 232, 233, 234, 235, 236, 237, 238};

// thai under
const int char_char_x2 = 2;
int char_x2[char_char_x2] = {216, 217};

char *TD_IntToBin(unsigned int p_ui) {
    static char ret[TD_max_char_col];  // need to change if expand font width

    for (int i = 0; i < TD_max_char_col; i++) ret[TD_max_char_col - 1 - i] = '0' + ((p_ui & (1 << i)) > 0);
    ret[TD_max_char_col] = '\0';

    return ret;
}

void TD_SerialShowBinChar2(int p_idx) {
    String c;

    Serial.println(p_idx);
    for (int i = 0; i < TD_max_char_row2; i++) {
        c = TD_IntToBin(char_data2[p_idx][i]);
        c.replace("1", "X");
        c.replace("0", " ");
        Serial.println(c);
    }
    Serial.println("");
}

int TD_IsX1(int idx) {
    int ret = 0;

    for (int i = 0; i < char_char_x1; i++) {
        if (char_x1[i] == idx) {
            ret = 1;
            break;
        }
    }

    return ret;
}

int TD_IsX2(int idx) {
    int ret = 0;

    for (int i = 0; i < char_char_x2; i++) {
        if (char_x2[i] == idx) {
            ret = 1;
            break;
        }
    }

    return ret;
}

void TD_WriteChar(int p_r, int p_c, int p_idx) {
    char *c;

    if (p_idx == 219) {
        if (g_pcman_n == 0) {
            if (g_pcman_char == 220) {
                g_pcman_char = 219;
            } else {
                g_pcman_char = 220;
            }
        }
        p_idx = g_pcman_char;
        g_pcman_n = g_pcman_n + 1;
        if (g_pcman_n == 8) g_pcman_n = 0;
    }

    // not thai
    if (p_idx < TD_start_thai_char) {
        p_idx = p_idx - TD_start_not_thai_char;
        //TD_SerialShowBinChar2(p_idx);

        for (int i = 0; i < TD_max_char_row1; i++) {
            c = TD_IntToBin(char_data1[p_idx][i]);
            for (int j = 0; j < TD_max_char_col; j++)
                if (c[j] == '1') {
                    //TD_setPoint(p_r + i, p_c + j, true);
                    display.drawPixel(p_c + j, p_r + i, TD_color);
                }
        }
        // thai
    } else {
        p_idx = p_idx - TD_start_thai_char;
        //TD_SerialShowBinChar2(p_idx);

        for (int i = 0; i < TD_max_char_row2; i++) {
            c = TD_IntToBin(char_data2[p_idx][i]);
            for (int j = 0; j < TD_max_char_col; j++)
                if (c[j] == '1') {
                    //TD_setPoint(p_r + i, p_c + j, true);
                    display.drawPixel(p_c + j, p_r + i, TD_color);
                }
        }
    }
}

int TD_CharWidth(int p_idx) {
    char *c;
    int w = 0;

    if (p_idx < TD_start_thai_char) {
        p_idx = p_idx - TD_start_not_thai_char;

        for (int i = 0; i < TD_max_char_row1; i++) {
            c = TD_IntToBin(char_data1[p_idx][i]);
            for (int j = 0; j < TD_max_char_col; j++)
                if (c[j] == '1') {
                    if (j >= w) w = j;
                }
        }

        // thai
    } else {
        p_idx = p_idx - TD_start_thai_char;

        for (int i = 0; i < TD_max_char_row2; i++) {
            c = TD_IntToBin(char_data2[p_idx][i]);
            for (int j = 0; j < TD_max_char_col; j++)
                if (c[j] == '1') {
                    if (j >= w) w = j;
                }
        }
    }

    return w + 1;
}

int TD_Char2Width(int p_idx) {
    char *c;
    int w = 0;

    for (int i = 0; i < TD_max_char_row2; i++) {
        c = TD_IntToBin(char_data2[p_idx][i]);
        for (int j = 0; j < TD_max_char_col; j++)
            if (c[j] == '1') {
                if (j >= w) w = j;
            }
    }

    return w + 1;
}

void TD_LEDWriteText(int p_r, int p_c, String p_text) {
    char c1;
    char c2;
    int w = 0;
    int idx = 0;
    int normal;
    String myText;

    myText = p_text;

    //mx.clear();
    display.clearDisplay();
    display_update_enable(true);

    for (int i = 0; i < myText.length(); i++) {
        c1 = myText[i];
        // E0 224 Thai
        if ((int)c1 == 224) {
            c1 = myText[i + 1];
            c2 = myText[i + 2];
            // B8,81(129)  ก - ฮ
            if ((int)c1 == 184 && ((int)c2 + 32) >= 161 && ((int)c2 + 32) <= 207) {
                // ก 81 = 129 --> +32 --> 161
                idx = (int)c2 + 32;
            }
            // B8,B0(176)  สระ
            if ((int)c1 == 184 && ((int)c2 + 32) >= 208 && ((int)c2 + 32) <= 218) {
                // ะ B0 = 176 --> +32 --> 208
                idx = (int)c2 + 32;
            }
            // B9,80(128)  สระ
            if ((int)c1 == 185 && ((int)c2 + 96) >= 224 && ((int)c2 + 96) <= 231) {
                // เ 80 = 176 --> +96 --> 224
                idx = (int)c2 + 96;
            }
            // B9,B0(136)  วรรณยุก
            if ((int)c1 == 185 && ((int)c2 + 96) >= 232 && ((int)c2 + 96) <= 238) {
                // ่ 88 = 136 --> +96 --> 208
                idx = (int)c2 + 96;
            }
            i = i + 2;
        } else {
            idx = (int)c1;
        }

        normal = 1;
        // thai over
        if (TD_IsX1(idx) == 1) {
            w = TD_CharWidth(idx);
            TD_WriteChar(p_r - 2, p_c - (w + TD_gap_pixel), idx);
            normal = 0;
        }
        // thai under
        if (TD_IsX2(idx) == 1) {
            w = TD_CharWidth(idx);
            TD_WriteChar(p_r + 10, p_c - (w + TD_gap_pixel), idx);
            normal = 0;
        }
        if (normal == 1) {
            switch (idx) {
                // space
                case 32:
                    p_c = p_c + 4;
                    break;

                // comma
                case 44:
                    TD_WriteChar(p_r + 1, p_c, idx);
                    w = TD_CharWidth(idx);
                    p_c = p_c + w + TD_gap_pixel;
                    break;

                // g j p q y
                case 103:
                case 106:
                case 112:
                case 113:
                case 121:
                    TD_WriteChar(p_r + 3, p_c, idx);
                    w = TD_CharWidth(idx);
                    p_c = p_c + w + TD_gap_pixel;
                    break;

                // ำ
                case 211:
                    w = TD_CharWidth(idx);
                    TD_WriteChar(p_r, p_c - (3 + TD_gap_pixel), idx);
                    p_c = p_c - 3 + w + TD_gap_pixel;
                    break;

                default:
                    TD_WriteChar(p_r, p_c, idx);
                    w = TD_CharWidth(idx);
                    p_c = p_c + w + TD_gap_pixel;
                    break;
            }
        }
    }
    //mx.MyflushBufferAll();
    display.flushDisplay();
    delay(TD_led_delay);
}

int TD_LEDTextPixel(String p_text) {
    char c1;
    char c2;
    int w = 0;
    int idx = 0;
    int normal;
    String myText;
    int p_c;

    p_c = 0;
    myText = p_text;
    for (int i = 0; i < myText.length(); i++) {
        c1 = myText[i];
        // E0 224 Thai
        if ((int)c1 == 224) {
            c1 = myText[i + 1];
            c2 = myText[i + 2];
            // B8,81(129)  ก - ฮ
            if ((int)c1 == 184 && ((int)c2 + 32) >= 161 && ((int)c2 + 32) <= 207) {
                // ก 81 = 129 --> +32 --> 161
                idx = (int)c2 + 32;
            }
            // B8,B0(176)  สระ
            if ((int)c1 == 184 && ((int)c2 + 32) >= 208 && ((int)c2 + 32) <= 218) {
                // ะ B0 = 176 --> +32 --> 208
                idx = (int)c2 + 32;
            }
            // B9,80(128)  สระ
            if ((int)c1 == 185 && ((int)c2 + 96) >= 224 && ((int)c2 + 96) <= 231) {
                // เ 80 = 176 --> +96 --> 224
                idx = (int)c2 + 96;
            }
            // B9,B0(136)  วรรณยุก
            if ((int)c1 == 185 && ((int)c2 + 96) >= 232 && ((int)c2 + 96) <= 238) {
                // ่ 88 = 136 --> +96 --> 208
                idx = (int)c2 + 96;
            }
            i = i + 2;
        } else {
            idx = (int)c1;
        }

        normal = 1;
        // thai over
        if (TD_IsX1(idx) == 1) {
        }
        // thai under
        if (TD_IsX2(idx) == 1) {
            w = TD_CharWidth(idx);
            normal = 0;
        }
        if (normal == 1) {
            switch (idx) {
                case 32:  // space
                    p_c = p_c + 4;
                    break;
                case 211:  // ำ
                    w = TD_CharWidth(idx);
                    p_c = p_c - 3 + w + TD_gap_pixel;
                    break;
                default:
                    w = TD_CharWidth(idx);
                    p_c = p_c + w + TD_gap_pixel;
                    break;
            }
        }
    }

    return p_c;
}

void TD_LEDText(String p_text) {
    TD_LEDWriteText(TD_normal_row, 1, p_text);
}

void TD_LEDScrollText(String p_text) {
    int n;

    n = TD_LEDTextPixel(p_text);
    for (int i = TD_max_col; i >= n * -1; i--) {
        TD_LEDWriteText(TD_normal_row, i, p_text);
    }
}

void TD_LEDScrollUpText(String p_text, int p_direction) {
    int cs;
    int n;
    int xdelay;

    xdelay = TD_led_delay;
    TD_led_delay = 30;
    n = TD_LEDTextPixel(p_text);
    cs = (TD_max_col / 2) - (n / 2);
    // up
    if (p_direction == 1)
        for (int i = TD_max_row; i >= -TD_max_row; i--) {
            TD_LEDWriteText(i, cs, p_text);
            if (i == TD_normal_row) delay(1000);
        }
    // down
    else
        for (int i = -TD_max_row; i <= TD_max_row + 1; i++) {
            TD_LEDWriteText(i, cs, p_text);
            if (i == TD_normal_row) delay(1000);
        }
    TD_led_delay = xdelay;
}

// ISR for display refresh
void display_updater() {
    display.display(display_draw_time);
}

void display_update_enable(bool is_enable) {
    if (is_enable) {
        display_ticker.attach(0.002, display_updater);
    } else {
        display_ticker.detach();
    }
}

void pixel_time_test(uint8_t draw_time) {
    Serial.print("Pixel draw latency in us: ");
    unsigned long start_timer = micros();
    display.drawPixel(1, 1, 0);
    unsigned long delta_timer = micros() - start_timer;
    Serial.println(delta_timer);

    Serial.print("Display update latency in us: ");
    start_timer = micros();
    display.display(draw_time);
    delta_timer = micros() - start_timer;
    Serial.println(delta_timer);

    display.clearDisplay();
    display.setTextColor(myCYAN);
    display.setCursor(2, 0);
    display.print("Pixel");
    display.setTextColor(myMAGENTA);
    display.setCursor(2, 8);
    display.print("Time");

    display_update_enable(true);

    yield();
    delay(3000);

    display_update_enable(false);
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
        Serial.println("[HTTP] POST... code: %d\n " + httpCode);
        if (httpCode == HTTP_CODE_OK) {
            Serial.println("Yes....You can do it");
        }
    } else {
        Serial.println("[HTTP] POST... failed, error: %s\n");
        Serial.println(http.errorToString(httpCode).c_str());
    }
    http.end();
}

void connectWiFi() {
    printLedConnectingWifi(WIFI_SSID);
    Serial.print(F("Connecting to: "));
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    //digitalWrite(LED_NODE, HIGH);
    printLedIpAddress();
    Serial.println(F(""));
    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
    
}

void printLedConnectingWifi(String ssid) {
    TD_normal_row = 11;
    TD_color = myBLUE;
    TD_LEDScrollText("Connecting to: " + ssid + "...");
}

void printLedIpAddress() {
    TD_LEDScrollText("WiFi connected, IP: " + WiFi.localIP().toString());
}

void setup() {
    Serial.begin(115200);

    // Define your display layout here, e.g. 1/8 step
    display.begin(16);

    // Compare draw latency at similar display brightness for standard and fast drawing
    //display.setFastUpdate(false);
    display.setFastUpdate(true);

    //  display_update_enable(false);

    //Connecting to WiFi and initial time
    connectWiFi();
    //connectWiFiRmuti();
}
union single_double {
    uint8_t two[2];
    uint16_t one;
} this_single_double;

void m_1() {
    display.clearDisplay();
    display.setTextColor(myCYAN);
    display.setCursor(2, 0);
    display.print("Hello");
    display.setTextColor(myMAGENTA);
    display.setCursor(2, 8);
    display.print("How r u?");

    display_update_enable(true);

    yield();
    delay(1500);

    display.clearDisplay();
    display.setTextColor(myRED);
    display.setCursor(2, 0);
    display.print("Hello");
    display.setTextColor(myGREEN);
    display.setCursor(2, 8);
    display.print("How r u?");

    display_update_enable(true);

    yield();
    delay(1500);
}

void m_2() {
    int i, j;

    display.clearDisplay();
    display_update_enable(true);
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 64; j++) {
            if ((i % 3) == 0) display.drawPixel(j, i, myRED);
            if ((i % 3) == 1) display.drawPixel(j, i, myGREEN);
            if ((i % 3) == 2) display.drawPixel(j, i, myBLUE);
            display.flushDisplay();
            //yield();
            delay(2);
        }
    }
}

void m_3() {
    TD_normal_row = 3;
    TD_color = myRED;
    TD_LEDScrollText("ทดสอบ LED P3 ภาษาไทย");

    TD_normal_row = 3 + 8;
    TD_color = myGREEN;
    TD_LEDScrollText("มันจะเขียวๆหน่อยนะครับ");

    TD_normal_row = 3 + 8 + 8;
    TD_color = myBLUE;
    TD_LEDScrollText("This is third line in BLUE");

    TD_normal_row = 3 + 8;
    TD_color = myYELLOW;
    TD_LEDScrollText((String) char(219) + "  Thanks for watching  " + (String) char(219));
}

void testFullText() {
    TD_color = myRED;
    TD_LEDText("Full");
}

void loop() {

}

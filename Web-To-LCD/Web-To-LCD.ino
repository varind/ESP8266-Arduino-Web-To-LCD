/*
*** THIS PROJECT IS NOW SUPERSEDED BY: https://github.com/varind/ESP8266-Web-To-LCD***
   Web to LCD by Ben Lipsey www.varind.com 2016. This code is public domain, enjoy!
   Compiles with Arduino 1.6.7
   With help in the form of code from:
   Dave http://blog.huntgang.com/2015/01/20/arduino-esp8266-tutorial-web-server-monitor-example/
   Abhinaba Basu https://blogs.msdn.microsoft.com/abhinaba/2016/01/23/esp8266-wifi-with-arduino-uno-and-nano/
   Martyn Currey http://www.martyncurrey.com/arduino-to-esp8266-serial-commincation/

   ESP8266 Flashed with circuit from:
   http://www.allaboutcircuits.com/projects/update-the-firmware-in-your-esp8266-wi-fi-module/

   with instructions and software (esp8266flasher.exe & v0.9.2.2 AT Firmware.bin) from:
   http://www.xess.com/blog/esp8266-reflash/

   ESP8266 Command Refrence:
   https://cdn.sparkfun.com/assets/learn_tutorials/4/0/3/4A-ESP8266__AT_Instruction_Set__EN_v0.30.pdf
   http://www.pridopia.co.uk/pi-doc/ESP8266ATCommandsSet.pdf
   http://www.microtechnica.tv/support/manual/ESP8266_WiFi_Module_Quick_Start_Guide_v_1.0.4.pdf

   *******************   YOU MUST INCREASE YOUR SOFTWARE SERIAL RX BUFFER TO 256 **********************
     (On a mac, navigate to your applications folder then command-click on the Arduino app
     select "Show Package Contents")
     OPEN /Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SoftwareSerial/SoftwareSerial.h
     Chage the line:  #define _SS_MAX_RX_BUFF 64 // RX buffer size
     To:              #define _SS_MAX_RX_BUFF 256 // RX buffer size
     Then Command-S or File->Save
   ****************************************************************************************************

   A few extra commands that may come in handy:
   ESPserial.println("AT+CWLAP"); // List Available Access Points
   ESPserial.println("ATE0");  // Don't echo AT Command
   ESPserial.println("ATE1");  // Echo AT Command

   LCD 1 VSS -> 200 ohm -> +5V
   LCD 2 GND -> GND
   LCD 3 VO Pin -> center pin of 10k Potentiometer (ends to +5V & GND)
   LCD 4 RS pin -> Arduino digital pin 4
   LCD 5 R/W pin -> GND
   LCD 6 Enable pin -> digital pin 5
   LCD 11 D4 pin -> Arduino digital pin 6
   LCD 12 D5 pin -> Arduino digital pin 7
   LCD 13 D6 pin -> Arduino digital pin 8
   LCD 14 D7 pin -> Arduino digital pin 9
   LCD 15 A (backlight) -> 200 ohm -> +5V
   LCD 16 K (backlight) -> GND

        V G
        S N V R R           D D D D
        S D O S W E         4 5 6 7 A K
     _______________________________________________
    |O  o o o o o o o o o o o o o o o o           O |
    | _____________________________________________ |
    ||                                             ||
    ||                                             ||
    ||                                             ||
    ||                                             ||
    ||_____________________________________________||
    |                                               |
    |O                                            O |
     -----------------------------------------------

   ESP01 TX -> Arduino digital pin 10
   ESP01 RX -> 1k ohm -> Arduino digital pin 11
   ESP01 RX -> 2k ohm -> GND
   ESP01 CH_PD -> 10k ohm -> +3.3V (3.3V voltage regulator from +5V, NOT Arduino 3.3v)
   ESP01 VCC -> +3.3v (3.3V voltage regulator from +5V, NOT Arduino 3.3v)
   ESP01 GND -> GND
                _____________________
   TX    GND   |o o   ----      ____  |
   CH_PD GPIO2 |o o  |    |     ____| |
   RST   GPIO1 |o o  ------    |____  |
   VCC   RX    |o o |      |        | |
                ----------------------

*/


#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
SoftwareSerial ESPserial(10, 11); // RX | TX
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);
bool debug = true;

byte lcdRows = 4;
byte lcdCols = 20;

char AP[] = "YOUR WIFI NETWORK HERE";
char password[] = "YOUR WIFI PASSWORD HERE";


#define BBCNEWS
//#define NYTNEWS
//#define ABCNEWS
//#define HAM
//#define HAM2
//#define WEATHER
//#define BIGWEATHER
//#define STOCK
//#define BIGSTOCK


//----------------------------------------------------------------------------------|
//                                  BBC NEWS
//----------------------------------------------------------------------------------|
#ifdef BBCNEWS // http://feeds.bbci.co.uk/news/rss.xml?edition=us
char dataServer[] = "feeds.bbci.co.uk";         // base URL of site to connect to
char dataPage[] = "/news/rss.xml?edition=us";   // specific paget to conenct to
const char *elements[]  = {"<title>"};          // list of elements to search for
char *elementPreLabels[]  = {"BBC NEWS: "};     // labels to display before data
char *elementPostLabels[]  = {""};              // labels to display after data
const char dataEnd = '/';                       // character that marks the end of data
const byte normalStartBias = 0;                 // characters after element start that data begins
const byte normalEndBias = -1;                  // characters before end charater that data ends
const char elementCols[] = {1};                 // formatting for display
#define LONGDATA                                // use "longdata" formatting (useful for news)
#define INSTANCENUMBER 3                        // find this instance of the element on the page
#define SPECIALREPLACE                          // replace special characters
const byte maxDataLength = 80;                  // longest possible length of element data
char charBuf[150];                              // total length of elements plus extra for +IPD reprints
const long reloadDelay = 120000;                // time between page reloads
//#define CUSTOMNEWLINECHAR '\r'                // always looks for \n but we may want to find onother character
//#define BIGTEXT                               // use big font for data display
//#define BIGLABELS                             // use big font for data labels
//#define GRID                                  // align to grid
//#define CUSTOMBIAS                            // if certain data elements need custom start/end postitions
#ifdef CUSTOMBIAS
const char *customBiasElements[] = {""};        // the elements that need custom start/end positions
const byte customStartBias = 0;                 // characters after element start that data begins
const byte customEndBias = 0;                   // characters before end charater that data ends
#endif
#endif
//----------------------------------------------------------------------------------|
//                                  NEW YORK TIMES NEWS
//----------------------------------------------------------------------------------|
#ifdef NYTNEWS // http://rss.nytimes.com/services/xml/rss/nyt/HomePage.xml
char dataServer[] = "rss.nytimes.com";
char dataPage[] = "/services/xml/rss/nyt/HomePage.xml";
const char *elements[]  = {"<title>"};
char *elementPreLabels[]  = {"BREAKING: "};
char *elementPostLabels[]  = {""};
const char dataEnd = '/';
const byte normalStartBias = 0;
const byte normalEndBias = -1;
const char elementCols[] = {1};

#define LONGDATA
#define INSTANCENUMBER 3
#define SPECIALREPLACE
const byte maxDataLength = 100;
char charBuf[200];
const long reloadDelay = 120000;
#endif

//----------------------------------------------------------------------------------|
//                                  ABC NEWS
//----------------------------------------------------------------------------------|
#ifdef ABCNEWS // http://www.newslookup.com/rss/wires/newslookup-wires.rss
//hsttp://feeds.abcnews.com/abcnews/topstories
char dataServer[] = "feeds.abcnews.com";
char dataPage[] = "/abcnews/topstories";
const char *elements[]  = {"<title><![CDATA["};
char *elementPreLabels[]  = {"BREAKING: "};
char *elementPostLabels[]  = {""};
const char dataEnd = ']';
const byte normalStartBias = 0;
const byte normalEndBias = 0;
const char elementCols[] = {1};

#define LONGDATA
#define INSTANCENUMBER 1
#define SPECIALREPLACE
const byte maxDataLength = 80;
char charBuf[80];
const long reloadDelay = 120000;
#endif
//----------------------------------------------------------------------------------|
//                     SOLAR ELEMENTS FOR AMATEUR RADIO
//----------------------------------------------------------------------------------|
#ifdef HAM // http://www.hamqsl.com/solarxml.php
char dataServer[] = "www.hamqsl.com";
char dataPage[] = "/solarxml.php";

const char *elements[]  = {"<solarflux>", "<sunspots>", "<aindex>",
                           "<kindex>", "<xray>", "<geomagfield>",
                           "<aurora>", "<signalnoise>", "<muf>"
                          };
char *elementPreLabels[]  = {"SFI:", "SN:", "A:",
                             "K:",  "XRY:", "Mag:",
                             "Aur:", "S/N:", "MUF:"
                            };

char *elementPostLabels[]  = {" ", " ", " ", " ", " ", " ", " ", " ", " "};
const char dataEnd = '/';
const byte normalStartBias = 0;
const byte normalEndBias = -1;
const char elementCols[] = {3, 2, 2, 2};
const byte maxDataLength = 10;
char charBuf[100];
long reloadDelay = 60000;
#endif
//----------------------------------------------------------------------------------|
//                     AMATEUR RADIO BAND INFORMATION
//----------------------------------------------------------------------------------|
#ifdef HAM2 // http://www.hamqsl.com/solarxml.php
char dataServer[] = "www.hamqsl.com";
char dataPage[] = "/solarxml.php";

const char *elements[]  = {"40m\" time=\"day\">", "40m\" time=\"night\">",
                           "20m\" time=\"day\">", "20m\" time=\"night\">",
                           "15m\" time=\"day\">", "15m\" time=\"night\">",
                           "10m\" time=\"day\">", "10m\" time=\"night\">"
                          };
char *elementPreLabels[]  = {"80-40m D ", "N ",
                             "30-20m D ", "N ",
                             "17-15m D ", "N ",
                             "12-10m D ", "N "
                            };

char *elementPostLabels[]  = {" ", "", " ", "", " ", "", " ", ""};
const char dataEnd = '/';
const byte normalStartBias = 0;
const byte normalEndBias = -1;
const char elementCols[] = {2, 2, 2, 2};
const byte maxDataLength = 10;
char charBuf[50];
long reloadDelay = 120000;
#endif
//----------------------------------------------------------------------------------|
//                                  STOCKS
//----------------------------------------------------------------------------------|
#ifdef STOCK  // http://finance.google.com/finance/info?client=ig&q=MCHP
char dataServer[] = "finance.google.com";
char dataPage[] = "/finance/info?client=ig&q=MCHP";
const char *elements[]  = {"\"lt\"", "\"t\"", "\"l_cur\"", "\"c\"", "\"cp\""};
char *elementPreLabels[]  = {"", "", "$", "", ""};
char *elementPostLabels[]  = {"", ":", "", " ", "%"};
const char dataEnd = '\0';
const byte normalStartBias = 4;
const byte normalEndBias = -2;
const char elementCols[] = {1, 2, 2, 1};
const byte maxDataLength = 25;
char charBuf[50];
long reloadDelay = 60000;
#endif
//----------------------------------------------------------------------------------|
//                                  STOCKS (BIG FONT)
//----------------------------------------------------------------------------------|
#ifdef BIGSTOCK  // http://finance.google.com/finance/info?client=ig&q=MCHP
char dataServer[] = "finance.google.com";
char dataPage[] = "/finance/info?client=ig&q=MCHP";
const char *elements[]  = {"\"t\"", "\"l_cur\""};
char *elementPreLabels[]  = {"", ""};
char *elementPostLabels[]  = {"", " "};
const char dataEnd = '\0';
const byte normalStartBias = 4;
const byte normalEndBias = -2;
const char elementCols[] = {1, 2};
#define BIGTEXT
const byte maxDataLength = 8;
char charBuf[50];
long reloadDelay = 60000;
#endif
//----------------------------------------------------------------------------------|
//                                  WEATHER
//----------------------------------------------------------------------------------|
#ifdef WEATHER // http://w1.weather.gov/xml/current_obs/KPDX.xml"
char dataServer[] = "w1.weather.gov";
char dataPage[] = "/xml/current_obs/KPDX.xml";
const char *elements[]  = {"temp_f", "relative_humidity", "pressure_in", "wind_dir", "wind_mph", "<weather"};
char *elementPreLabels[]  = {"", " ", "Pressure: ", "", " @ ", ""};
char *elementPostLabels[]  = {"F ", "% Humidity", "in", "", "MPH", ""};
const char dataEnd = '/';
const byte normalStartBias = 1;
const byte normalEndBias = -1;
const char elementCols[] = {2, 1, 2, 1};
const byte maxDataLength = 21;
char charBuf[50];
long reloadDelay = 60000;
#endif
//----------------------------------------------------------------------------------|
//                                  WEATHER  (BIG FONT)
//----------------------------------------------------------------------------------|
#ifdef BIGWEATHER // http://w1.weather.gov/xml/current_obs/KPDX.xml"
char dataServer[] = "w1.weather.gov";
char dataPage[] = "/xml/current_obs/KPDX.xml";
const char *elements[]  = {"temp_f", "relative_humidity", "wind_mph"};
char *elementPreLabels[]  = {"", "", ""};
char *elementPostLabels[]  = {"F ", "%", "MPH"};
const char dataEnd = '/';
const byte normalStartBias = 1;
const byte normalEndBias = -1;
const char elementCols[] = {2, 1};
#define BIGTEXT
//#define BIGLABELS
const byte maxDataLength = 5;
char charBuf[20];
long reloadDelay = 60000;
#endif
//----------------------------------------------------------------------------------
byte GETdelay = 0;
byte maxLineLength = 100;
int lcdWriteDelay = 2000;
long dataTimer;
long lcdTimer;
long bufferTimer;
bool getDataTest = true;
bool startupTest = true;
bool printLCDTest = false;
byte memPos = 0; // Memory offset
const byte elementsArrayLength = sizeof(elements) / sizeof(*elements);
const byte colArrayLength = sizeof(elementCols) / sizeof(*elementCols);
char *elementValues[elementsArrayLength + 1];
byte char_x = 0, char_y = 0;
String serialData;
String lastData;

#ifdef CUSTOMBIAS
const byte customElementsArrayLength = sizeof(customBiasElements) / sizeof(*customBiasElements);
#endif
#ifdef INSTANCENUMBER
byte dataInstance = 0;
#endif

bool IPDflag = false;
byte IPDcheck = 0;

void setup() {
  lcd.begin(lcdCols, lcdRows);
  lcd.clear();
#ifdef BIGTEXT
  createChars();                            // for large font
#endif
  startupMessages(1);
  Serial.begin(115200);                     // fast output helps reduce softserial input overflow
  ESPserial.begin(9600);                    // slow serial keeps things behaving well
  delay(1000);
  sendData("AT+RST\r\n", 2000, true);       // reset the ESP8266
  Serial.println("");
  sendData("AT+CWMODE=1\r\n", 1000, true);  // Set to mode 1 (Station Mode)
  char ATcmd[44];
  ATcmd[0] = 0;
  strcat(ATcmd, "AT+CWJAP=\"");              // make command: "AT+CWJAP=\"access point\",\"password\""
  strcat(ATcmd, AP);
  strcat(ATcmd, "\",\"");
  strcat(ATcmd, password);
  strcat(ATcmd, "\"\r\n");
  Serial.print(F("Set ATcmd[] to: "));
  Serial.println(strlen (ATcmd) + 1);
  startupMessages(2);
  sendData(ATcmd, 10000, true);             // Join Wifi network
  startupMessages(3);
  sendData("AT+CIFSR\r\n", 1000, true);     // Print IP address
  startupMessages(4);
}



void loop() {
  if (getDataTest) {
    getData();
    if (debug) Serial.print(F("Available RAM: ")); Serial.println(freeRam());
  }

  if (printLCDTest && startupTest) {
    startupTest = false;
    lcd.clear();
  }

  if (!startupTest && printLCDTest) {
    if (debug) Serial.print(F("Available RAM: ")); Serial.println(freeRam());
    printDataToLCD();
  }

  serialBuffer();                   // alwasy look for incoming data
  
  lcdDelayTimer(lcdWriteDelay);     // update the LCD
  pageDelayTimer(reloadDelay);      // Re-Load the webpage
}




void getData() {
  lcdTimer = millis();
  char ATcmd[200];
  ATcmd[0] = 0;
  strcat(ATcmd, "AT+CIPSTART=\"TCP\",\"");        // make command: AT+CPISTART="TCP","www.anyurl.com",80
  strcat(ATcmd, dataServer);
  strcat(ATcmd, "\",80\r\n");
  sendData(ATcmd, 1000, true);
  ATcmd[0] = 0;
  strcat(ATcmd, "GET ");                          // make command: GET /anypage.html HTTP/1.0\r\nHost: www.anyurl.com\r\n\r\n
  strcat(ATcmd, dataPage);
  strcat(ATcmd, " HTTP/1.1\r\nHost: ");
  strcat(ATcmd, dataServer);
  strcat(ATcmd, "\r\nUser-Agent: Mozilla/4.0");   // some servers i.e. weather.gov require this
  strcat(ATcmd, "\r\nConnection: close");
  strcat(ATcmd, "\r\n\r\n");
  sendData("AT+CIPSEND=", 0, true);               // the ESP8266 needs to know the size of the GET request
  ESPserial.print(strlen (ATcmd));
  printSerial();
  sendData("\r\n", 1000, true);                   // wait for the > prompt from the esp8266
  sendData(ATcmd, GETdelay, false);
  ATcmd[0] = 0;
  memPos = 0;
  getDataTest = false;
#ifdef INSTANCENUMBER
  dataInstance = 0;
#endif
}


void serialBuffer() {
  byte instanceElement;

  while (ESPserial.available() > 0) {
    lcdTimer = millis();                  // don't refresh the LCD if we are downloading data

#ifdef INSTANCENUMBER
    if (dataInstance == INSTANCENUMBER) {
      ESPserial.read();
      break;
    }
#endif

    char received = ESPserial.read();

    if (serialData.length() < maxLineLength || received == '\n') {
      serialData += received;
    }

#ifdef CUSTNEWLINECHAR
    if (received == '\n' || received == CUSTNEWLINECHAR)    {        // Process message when new line character is received
      if (received == CUSTNEWLINECHAR && debug)Serial.println("");
#else
    if (received == '\n') {
#endif

#ifdef SPECIALREPLACE
      serialData.replace("&#x2019;", "\'");                        //replace special characters
      serialData.replace("&#39;", "\'");
      serialData.replace("&apos;", "\'");
      serialData.replace("’", "\'");
      serialData.replace("‘", "\'");
      serialData.replace("&amp;", "&");
      serialData.replace("&quot;", "\"");
      serialData.replace("&gt;", ">");
      serialData.replace("&lt;", "<");

#endif

      if (serialData.indexOf("OK") == 0) {                         //  remove +IPD messages
        IPDflag = true;
        IPDcheck = 3;
        if (debug) Serial.println(F("--FOUND OK"));
      }
      if (IPDcheck > 0) {
        IPDcheck--;
      }
      if (IPDcheck == 0)IPDflag = false;                           // Found "OK" but not "+IPD" so reset for next one
      if (serialData.indexOf("+IPD,") == 0) {
        byte ipdEnd = serialData.indexOf(':');
        byte newLine = lastData.indexOf('\n');
        serialData = serialData.substring(ipdEnd + 1);
        lastData = lastData.substring(0, newLine - 1);
        serialData = lastData + serialData;
        IPDflag = false;
        if (debug) Serial.println(F("--FOUND +IPD: Reprinting last line "));
      }

      if (!IPDflag) {
        lastData = serialData;                // save this line of data in case of IPD+ Event
      }

      if (debug)Serial.print(F("Arduino Received "));               // print webpage to serial monitor
      if (debug)Serial.print(serialData);

      int elementPos;
      byte elementLength;
      byte endElement;
      char startBias, endBias;

      for (byte i = 0; i < elementsArrayLength; i++) {
        elementLength = strlen (elements[i]);
        elementPos = serialData.indexOf(elements[i]);
        endElement = serialData.indexOf(dataEnd);

        if (elementPos > -1)  {                                       // Found the element!!!
          ////!!!!!!! TODO: if don't find end element

#ifdef INSTANCENUMBER
          dataInstance++;
#endif

#ifdef CUSTOMBIAS                                  // Check for custom bias
          bool customFlag = false;
          for (byte k = 0; k < customElementsArrayLength; k++) {
            if (elements[i] == customBiasElements[k]) {
              startBias = customStartBias;
              endBias = customEndBias;
              customFlag = true;
              break;
            }
          }
          if (!customFlag) {
            startBias = normalStartBias;
            endBias = normalEndBias;
          }
#else
          startBias = normalStartBias;
          endBias = normalEndBias;
#endif

          serialData = serialData.substring(elementPos + elementLength + startBias, endElement + endBias);
          if (serialData == "")serialData = "-";        // if no value, replace with a dash
          serialData.toCharArray(charBuf + memPos, maxDataLength);
          elementValues[i] = charBuf + memPos;
          // if (!memPosIPDflag)memPos = memPos + serialData.length() + 1;  //!!! TODO if IPD flag don't increment memPos
          memPos = memPos + serialData.length() + 1;

          //  memPosIPDflag = false;
          if (debug) {

#ifdef CUSTNEWLINECHAR
            if (received == CUSTNEWLINECHAR) Serial.println("");
#endif
            Serial.print(F("FOUND "));
            Serial.print(elements[i]);
            Serial.print(F("  VALUE="));
            Serial.print(elementValues[i]);
            Serial.print(F("  MEMORY BUF From "));
            Serial.print(memPos - serialData.length() - 1);
            Serial.print(F(" To "));
            Serial.println(memPos);
          }
          break;  // no need to keep searching for a match
        }
      }
      serialData = ""; // to flush the value.
      if (ESPserial.overflow()) Serial.println(F("<----! SoftwareSerial overflow !---->"));
    }
  }
}

void printDataToLCD() {
  if (debug) Serial.println(F("<---------PRINT DATA TO LCD--------->"));
  byte x = 0;
  byte y = 0;
  byte cols = 0;
  byte dataLength = 0;

#ifdef LONGDATA
  byte longDataLength = 0;
  char text[maxDataLength + 20];   // +20 for labels
  text[0] = 0;
#endif

  for (byte el = 0; el < elementsArrayLength; el++) {
    dataLength = 0;
    lcd.setCursor(x, y);
    for (byte col = 0; col < elementCols[cols]; col++) {
      if (y == lcdRows) break;     // we are off the screen, just stop

      dataLength = dataLength + strlen (elementPreLabels[el]) + strlen (elementValues[el]) + strlen (elementPostLabels[el]);

#ifdef GRID
      x = col * ((lcdCols / elementCols[cols]) + 1);
      lcd.setCursor(x, y);
#endif


#ifdef LONGDATA
      longDataLength = longDataLength + dataLength;
      byte offset;
      byte charNum;
      strcat(text, elementPreLabels[el]);
      strcat(text, elementValues[el]);
      strcat(text, elementPostLabels[el]);

      if (el == elementsArrayLength - 1) {
        if (longDataLength > lcdCols * lcdRows) longDataLength = lcdCols * lcdRows + 3;
        offset = 0;
        for (byte rowNum = 0; rowNum < lcdRows; rowNum++) {
          lcd.setCursor(x, rowNum);
          charNum = rowNum * lcdCols + offset;
          while (charNum < ((rowNum + 1) *lcdCols) + offset) {
            if (charNum - offset == rowNum * lcdCols && charNum < longDataLength && text[charNum] == ' ') {
              charNum++;
              offset++;
              longDataLength--;
            }
            if (charNum - offset >= longDataLength) lcd.write(254);
            // if (charNum-offset >= longDataLength) lcd.print("x");
            else if (charNum - offset < longDataLength)lcd.print(text[charNum]);
            charNum++;
          }
        }
      }
#endif


#ifdef BIGTEXT
      char_x = x; char_y = y;
#ifdef BIGLABELS
      printBigCharacters(elementPreLabels[el], char_x, char_y);
#else
      for (byte i = 0; i < strlen (elementPreLabels[el]); i++) {
        lcd.setCursor(char_x + i, char_y);
        lcd.write(254);
      }
#endif
      lcd.setCursor(char_x, char_y + 1);
      lcd.print(elementPreLabels[el]);
      char_x = char_x + strlen (elementPreLabels[el]);
      printBigCharacters(elementValues[el], char_x, char_y);
#ifdef BIGLABELS
      printBigCharacters(elementPostLabels[el], char_x, char_y);
#else
      for (byte i = 0; i < strlen (elementPostLabels[el]); i++) {
        lcd.setCursor(char_x + i, char_y);
        lcd.write(254);
      }

      lcd.setCursor(char_x, char_y + 1);
      lcd.print(elementPostLabels[el]);
      char_x = char_x + strlen (elementPostLabels[el]);
#endif
      x = x + char_x;
#endif


#if !defined BIGTEXT && !defined LONGDATA
      lcd.print(elementPreLabels[el]); lcd.print(elementValues[el]); lcd.print(elementPostLabels[el]);
#endif

      if (el == elementsArrayLength - 1 || col == elementCols[cols] - 1) {


#if !defined GRID && !defined BIGTEXT
        while (dataLength < lcdCols) {
          lcd.write(254);   // comment this to debug
          //lcd.print("x"); // uncomment this debug
          dataLength++;
        }
#endif

#if !defined GRID && defined BIGTEXT
        while (char_x < lcdCols) {
          lcd.setCursor(char_x, char_y);
          lcd.write(254);
          lcd.setCursor(char_x, char_y + 1);
          lcd.write(254);
          char_x++;
        }
#endif
        x = 0;
        break;
      }
      el++;
    }
    cols++;
    y++;
#ifdef BIGTEXT
    y++;
#endif
  }
  printLCDTest = false;
  lcdTimer = millis();
}

void sendData(char command[], int wait, bool printSer) {
  ESPserial.print(command);
  delay(wait);
  if (printSer)printSerial();
}

void printSerial() {
  while (ESPserial.available() > 0)
  {
    char a = ESPserial.read();
    if (a == '\0')
      continue;
    if (a != '\r' && a != '\n' && (a < 32))
      continue;
    Serial.print(a);
    if (ESPserial.overflow()) Serial.println(F("<----! SoftwareSerial overflow !---->"));
  }
}

void pageDelayTimer(long delayTime) {
  if (dataTimer + delayTime < millis()) {
    dataTimer = millis();
    getDataTest = true;
  }
}

void lcdDelayTimer(long delayTime) {
  if (lcdTimer + delayTime < millis()) {
    lcdTimer = millis();
    printLCDTest = true;
  }
}

int freeRam ()  // from: https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void startupMessages(byte msgNum) {
  lcd.setCursor(0, 1);
  if (msgNum == 1) {
    lcd.print(F("     STARTING UP    "));
    lcd.setCursor(0, 2);
    lcd.print(F("  [->            ]  "));
  }
  if (msgNum == 2) {
    lcd.print(F("    JOINING WIFI    "));
    lcd.setCursor(0, 2);
    lcd.print(F("  [--->          ]  "));
  }
  if (msgNum == 3) {
    lcd.print(F("      CONNECTED     "));
    lcd.setCursor(0, 2);
    lcd.print(F("  [------->      ]  "));
  }
  if (msgNum == 4) {
    lcd.print(F("    GETTING DATA    "));
    lcd.setCursor(0, 2);
    lcd.print(F("  [---------->   ]  "));
  }
}


//---------------------- Large LCD Characters ------------------------//  Originally by Michael Pilcher, modified by Ben Lipsey
//  http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1265696343


byte UB[8] = //1
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte RT[8] = //2
{
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte LL[8] = //3
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111
};
byte LB[8] = //4
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte LR[8] = //5
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
};
byte UMB[8] =  //6
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte LMB[8] =  //7
{
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte LT[8] =  //8
{
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void createChars() {
  lcd.createChar(1, UB);
  lcd.createChar(2, RT);
  lcd.createChar(3, LL);
  lcd.createChar(4, LB);
  lcd.createChar(5, LR);
  lcd.createChar(6, UMB);
  lcd.createChar(7, LMB);
  lcd.createChar(8, LT);
}


void printBigCharacters(char character[], byte x, byte y) {
  bool foundChar;
  byte charWidth = 0;
  for (byte i = 0; i < strlen(character); i++) {

    charWidth = 0;
    foundChar = false;

    if (character[i] == '0' || character[i] == 'O' || character[i] == 'o')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(1);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
    }

    if (character[i] == '1')
    {
      foundChar = true;
      charWidth = 2;
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(255);
    }

    if (character[i] == '2')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(7);
    }

    if (character[i] == '3')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
    }

    if (character[i] == '4')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(3);
      lcd.write(4);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(254);
      lcd.write(255);
    }

    if (character[i] == '5')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
    }

    if (character[i] == '6')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(5);
    }

    if (character[i] == '7')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(1);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(8);
      lcd.write(254);
    }

    if (character[i] == '8')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(5);
    }

    if (character[i] == '9')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(254);
      lcd.write(255);
    }

    if (character[i] == 'A' || character[i] == 'a')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(254);
      lcd.write(255);
    }

    if (character[i] == 'B' || character[i] == 'b')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(6);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(7);
      lcd.write(2);
    }

    if (character[i] == 'C' || character[i] == 'c')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(1);
      lcd.write(1);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(4);
    }

    if (character[i] == 'D' || character[i] == 'd')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(1);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(4);
      lcd.write(5);
    }

    if (character[i] == 'E' || character[i] == 'e')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(7);
      lcd.write(7);
    }

    if (character[i] == 'F' || character[i] == 'f')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(254);
      lcd.write(254);
    }

    if (character[i] == 'G' || character[i] == 'g')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(1);
      lcd.write(1);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(2);
    }

    if (character[i] == 'H' || character[i] == 'h')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(4);
      lcd.write(255);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(254);
      lcd.write(255);
    }

    if (character[i] == 'I' || character[i] == 'i')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(255);
      lcd.write(1);
      lcd.setCursor(x, y + 1);
      lcd.write(4);
      lcd.write(255);
      lcd.write(4);
    }

    if (character[i] == 'J' || character[i] == 'j')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(254);
      lcd.write(254);
      lcd.write(255);
      lcd.setCursor(x, y + 1);
      lcd.write(4);
      lcd.write(4);
      lcd.write(5);
    }

    if (character[i] == 'K' || character[i] == 'k')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(4);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(254);
      lcd.write(2);
    }

    if (character[i] == 'L' || character[i] == 'l')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(254);
      lcd.write(254);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(4);
      lcd.write(4);
    }

    if (character[i] == 'M' || character[i] == 'm')
    {
      foundChar = true;
      charWidth = 4;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(3);
      lcd.write(5);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(254);
      lcd.write(254);
      lcd.write(255);
    }

    if (character[i] == 'N' || character[i] == 'n')
    {
      foundChar = true;
      charWidth = 4;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(2);
      lcd.write(254);
      lcd.write(255);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(254);
      lcd.write(3);
      lcd.write(5);
    }

    if (character[i] == 'P' || character[i] == 'p')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(254);
      lcd.write(254);
    }

    if (character[i] == 'Q' || character[i] == 'q')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(1);
      lcd.write(2);
      lcd.write(254);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(255);
      lcd.write(4);
    }

    if (character[i] == 'R' || character[i] == 'r')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(254);
      lcd.write(2);
    }

    if (character[i] == 'S' || character[i] == 's')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(8);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
    }

    if (character[i] == 'T' || character[i] == 't')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(255);
      lcd.write(1);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(255);
      lcd.write(254);
    }

    if (character[i] == 'U' || character[i] == 'u')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(254);
      lcd.write(255);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
    }

    if (character[i] == 'V' || character[i] == 'v')
    {
      foundChar = true;
      charWidth = 4;
      lcd.setCursor(x, y);
      lcd.write(3);
      lcd.write(254);
      lcd.write(254);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(2);
      lcd.write(8);
      lcd.write(254);
    }

    if (character[i] == 'W' || character[i] == 'w')
    {
      foundChar = true;
      charWidth = 4;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(254);
      lcd.write(254);
      lcd.write(255);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(8);
      lcd.write(2);
      lcd.write(5);
    }

    if (character[i] == 'X' || character[i] == 'x')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(8);
      lcd.write(254);
      lcd.write(2);
    }

    if (character[i] == 'Y' || character[i] == 'y')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(255);
      lcd.write(254);
    }

    if (character[i] == 'Z' || character[i] == 'z')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(6);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(8);
      lcd.write(7);
      lcd.write(4);
    }

    if (character[i] == '?')
    {
      foundChar = true;
      charWidth = 3;
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(7);
      lcd.write(254);
    }

    if (character[i] == '!')
    {
      foundChar = true;
      charWidth = 1;
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
    }
    if (character[i] == ':')
    {
      foundChar = true;
      charWidth = 1;
      lcd.setCursor(x, y);
      lcd.print(".");
      lcd.setCursor(x, y + 1);
      lcd.print(".");
    }
    if (character[i] == ' ')
    {
      foundChar = true;
      charWidth = 2;
      lcd.setCursor(x, y);
      lcd.write(254);
      lcd.write(254);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(254);
    }

    if (character[i] == '.')
    {
      foundChar = true;
      charWidth = 1;
      lcd.setCursor(x, y);
      lcd.write(254);
      lcd.setCursor(x, y + 1);
      lcd.write(4);
    }
    if (character[i] == '-')
    {
      foundChar = true;
      charWidth = 2;
      lcd.setCursor(x, y);
      lcd.write(4);
      lcd.write(4);
      lcd.setCursor(x, y + 1);
      lcd.write(254);
      lcd.write(254);
    }
    if (character[i] == '\"')
    {
      foundChar = true;
      charWidth = 1;
      lcd.setCursor(x, y);
      lcd.write("\"");
      lcd.setCursor(x, y + 1);
      lcd.write(254);
    }
    if (character[i] == '\'')
    {
      foundChar = true;
      charWidth = 1;
      lcd.setCursor(x, y);
      lcd.write("\'");
      lcd.setCursor(x, y + 1);
      lcd.write(254);
    }
    if (character[i] == '|')
    {
      foundChar = true;
      charWidth = 1;
      lcd.setCursor(x, y);
      lcd.write("|");
      lcd.setCursor(x, y + 1);
      lcd.write("|");
    }
    if (character[i] == '/')
    {
      foundChar = true;
      charWidth = 2;
      lcd.setCursor(x, y);
      lcd.write(254);
      lcd.write("/");
      lcd.setCursor(x, y + 1);
      lcd.write("/");
      lcd.write(254);
    }

    if (foundChar) {
      x = x + charWidth;
      char_x = char_x + charWidth;
    } else {
      lcd.setCursor(x, y);
      lcd.write(254);
      lcd.setCursor(x, y + 1);
      lcd.print(character[i]);
      x = x + 1;
      char_x = char_x + 1;
    }
  }
}




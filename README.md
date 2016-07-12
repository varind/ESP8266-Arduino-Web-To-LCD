# Web-To-LCD
*** THIS PROJECT IS NOW SUPERSEDED BY: https://github.com/varind/ESP8266-Web-To-LCD***

Use Arduino & ESP8266 to display XML/RSS/HTML on an LCD
Project website: http://www.variableindustries.com/web-to-lcd/

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

   ESP01 TX -> Arduino digital pin 10  
   ESP01 RX -> 1k ohm -> Arduino digital pin 11  
   ESP01 RX -> 2k ohm -> GND  
   ESP01 CH_PD -> 10k ohm -> +3.3V (3.3V voltage regulator from +5V, NOT Arduino 3.3v)  
   ESP01 VCC -> +3.3v (3.3V voltage regulator from +5V, NOT Arduino 3.3v)  
   ESP01 GND -> GND  


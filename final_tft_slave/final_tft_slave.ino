#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h> // Core graphics library

#include <Wire.h>
 
// Define Slave I2C Address
#define SLAVE_ADDR 9
 
// Define Slave answer size
#define ANSWERSIZE 10
 
// Define string with response to Master
 
#define LCD_RESET 1 // Can alternately just connect to Arduino's reset pin
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
 
// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// resolution is around 300 * 280
 
MCUFRIEND_kbv tft;

String answer;
String last_answer;
int line_index = 1;

void receiveEvent() {
  // Read while data received
  char x;
  
  if(line_index == 4) {
    line_index = 1;
    tft.reset();
    tft.fillScreen(BLACK);
 
    int16_t box_x = 0;
    int16_t box_y = 0;
    int16_t box_width = 300;
    int16_t box_height = 60;
    tft.fillRect(box_x, box_y, box_width, box_height, WHITE); // Draw a white rectangle
    tft.setTextColor(BLACK);
    tft.setTextSize(3);
    tft.setCursor(20, 20);
    tft.print("DETECTING..");
  }
  

  Serial.println("line_index is "+String(line_index));
  Serial.print("Receive event : ");
  while (0 < Wire.available()) {
    x = Wire.read();
    Serial.print(char(x));
    answer += char(x);
  }
  Serial.println();
  Serial.println("Answer is : "+answer);
  // Print to Serial Monitor
  if(answer != last_answer){
    
    if(line_index == 1){
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.setCursor(10, 80);
    }
    if(line_index == 2){
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.setCursor(10, 100);
    }
    if(line_index == 3){
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.setCursor(10, 120);
    }
    tft.print(answer);
    Serial.println("-TFT Print Success-");
    line_index += 1;
  }
  last_answer = answer;
  answer = " ";
  
}

void setup(void) {
  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);
  Serial.begin(115200);
  Wire.onReceive(receiveEvent);

  Serial.println(F("TFT LCD test"));
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.fillScreen(BLACK);
  tft.setRotation(2);

  int16_t box_x = 0;
  int16_t box_y = 0;
  int16_t box_width = 300;
  int16_t box_height = 60;
  tft.fillRect(box_x, box_y, box_width, box_height, WHITE); // Draw a white rectangle
  tft.setTextColor(BLACK);
  tft.setTextSize(3);
  tft.setCursor(20, 20);
  tft.print("DETECTING..");

}

void loop(void) {

}
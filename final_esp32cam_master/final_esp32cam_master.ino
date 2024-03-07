#include <Wire.h>
#include <DIWZ-project-1_inferencing.h>
#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/edgeimpulse/fomo.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9
 
// Define Slave answer size
#define ANSWERSIZE 10


#define SDA 15
#define SCL 14

using eloq::camera;
using eloq::ei::fomo;

void write_i2c(const String& value){
  // Write each character of the string to the Slave
  Wire.beginTransmission(SLAVE_ADDR);
  for (int i = 0; i < value.length(); i++) {
    Wire.write(value.charAt(i));
  }
  Wire.endTransmission();
}


void setup() {
    
    Serial.begin(115200);
    Wire.begin(SDA, SCL);
    Serial.println("__EDGE IMPULSE FOMO (NO-PSRAM)__");

    // camera settings
    // replace with your own model!
    camera.pinout.aithinker();
    camera.brownout.disable();
    // NON-PSRAM FOMO only works on 96x96 (yolo) RGB565 images
    camera.resolution.yolo();
    camera.pixformat.rgb565();

    // init camera
    while (!camera.begin().isOk())
        Serial.println(camera.exception.toString());
    
    Serial.println("Camera OK");
    Serial.println("Put object in front of camera");
}


void loop() {

  //write_i2c("Finding...");
   //Serial.println("<<---------- write i2c complete ---------->>");
  
  // capture picture
  if (!camera.capture().isOk()) {
      Serial.println(camera.exception.toString());
      return;
  }

  // run FOMO
  if (!fomo.run().isOk()) {
    Serial.println(fomo.exception.toString());
    return;
  }

  // how many objects were found?
  Serial.printf(
    "Found %d object(s) in %dms\n", 
    fomo.count(),
    fomo.benchmark.millis()
  );

  // if no object is detected, return
  if (!fomo.foundAnyObject())
    return;

  // if you expect to find a single object, use fomo.first
  Serial.printf(
    "Found %s at (x = %d, y = %d) (size %d x %d). "
    "Proba is %.2f\n",
    fomo.first.label,
    fomo.first.x,
    fomo.first.y,
    fomo.first.width,
    fomo.first.height,
    fomo.first.proba
  );

  String location = ("at x = " ) + String(fomo.first.x) + (", y = ") + String(fomo.first.y);
  String prob = "Proba: " + String(int(fomo.first.proba * 100)) + "%";

  write_i2c(String(fomo.first.label));
  delay(500);
  Serial.println("<<---------- write i2c complete ---------->>");
  write_i2c(location);
  delay(500);
  Serial.println("<<---------- write i2c complete ---------->>");
  write_i2c(prob);
  delay(500);
  Serial.println("<<---------- write i2c complete ---------->>");
    
    
     
//  if you expect to find many objects, use fomo.forEach
//    if (fomo.count() > 1) {
//      fomo.forEach([](int i, bbox_t bbox) {
//        Serial.printf(
//          "#%d) Found %s at (x = %d, y = %d) (size %d x %d). "
//          "Proba is %.2f\n",
//          i + 1,
//          bbox.label,
//          bbox.x,
//          bbox.y,
//          bbox.width,
//          bbox.height,
//          bbox.proba
//        );
//      });
//    }
delay(5000);
}
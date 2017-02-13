#include <Adafruit_TSL2561_U.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <CapPin.h>


#define PIN 9
#define crossLed 1
#define toneC 1911
boolean dark = false;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

CapPin cPin10 = CapPin(10);    
CapPin cPin0  = CapPin(0);

int thresholdCapPin = 500;
int thresholdLightSensor = 35;
int thresholdTime = 5000;

void setup() {

   Serial.begin(115200);
   pinMode(crossLed, OUTPUT);
   strip.begin();
   strip.show();

   /* Setup the sensor gain and integration time */
   configureSensor();
}

void loop() {

  int sensorValueLeft = cPin0.readPin(2000);
  int sensorValueRight = cPin10.readPin(2000);
  
  
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);


  
  //bright outside
   if(event.light > thresholdLightSensor){
    ledOff();
    if ( sensorValueLeft > thresholdCapPin){   
      blinkLeft();
    } 
    if ( sensorValueRight > thresholdCapPin) {
      blinkRight();
    }
  }
  //dark outside
   if(event.light < thresholdLightSensor ){
    backLight();
    if ( sensorValueLeft > thresholdCapPin){   
      ledOff();
      digitalWrite(crossLed,HIGH);
      blinkLeft();
    } 
    if ( sensorValueRight > thresholdCapPin) {
      ledOff();
      digitalWrite(crossLed,HIGH);
      blinkRight();
    }
  }

}

void blinkRight() {
   for(int j=0; j<3; j++){
      strip.setBrightness(255);
    for(int i=0; i<255;i++){
      strip.setPixelColor(0, i,i,0); 
      strip.setPixelColor(6, i,i,0);
      strip.setPixelColor(7, i,i,0);
      strip.setPixelColor(9, i,i,0);
      strip.show();
    }  
    tone(9, 350,50); 
    for(int i=0; i<255;i++){
      strip.setPixelColor(1, i,i,0); 
      strip.setPixelColor(5, i,i,0);
      strip.setPixelColor(8, i,i,0);
      strip.show();
    }
    for(int i=0; i<255;i++){
      strip.setPixelColor(2, i,i,0); 
      strip.setPixelColor(4, i,i,0);
      strip.show();
    }   
    for(int i=0; i<255;i++){
      strip.setPixelColor(3, i,i,0); 
      strip.show();
    }
    for(int i=255; i>0; i--){
        strip.setBrightness(i);
        strip.show();
    }
    delay(300);
   }
}
void blinkLeft(){
  for(int j=0; j<3; j++){
   strip.setBrightness(255);
   for(int i=0; i<255;i++){
      strip.setPixelColor(19, i,i,0); 
      strip.setPixelColor(17, i,i,0);
      strip.setPixelColor(16, i,i,0);
      strip.setPixelColor(10, i,i,0);
      strip.show();
   }
   tone(9, 350,50); 
    for(int i=0; i<255;i++){
      strip.setPixelColor(18, i,i,0); 
      strip.setPixelColor(15, i,i,0);
      strip.setPixelColor(11, i,i,0);
      strip.show();
    }
    for(int i=0; i<255;i++){
      strip.setPixelColor(14, i,i,0); 
      strip.setPixelColor(12, i,i,0);
      strip.show();
    }   
    for(int i=0; i<255;i++){
      strip.setPixelColor(13, i,i,0); 
      strip.show();
    }
    for(int i=255; i>0; i--){
      strip.setBrightness(i);
      strip.show();
    }
    delay(300);
  }
}
void backLight(){
   digitalWrite(crossLed,HIGH);
   strip.setBrightness(100);
  //right
   strip.setPixelColor(6, 255,0,0); 
   strip.setPixelColor(2, 255,0,0);
   strip.setPixelColor(5, 255,0,0);
   strip.setPixelColor(7, 255,0,0);
   strip.setPixelColor(4, 255,0,0);
  //left
   strip.setPixelColor(14, 255,0,0); 
   strip.setPixelColor(17, 255,0,0);
   strip.setPixelColor(15, 255,0,0);
   strip.setPixelColor(12, 255,0,0);
   strip.setPixelColor(16, 255,0,0);
   strip.show();
}

void ledOff(){
  for(int i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i,0,0,0);
  }
  digitalWrite(crossLed,LOW);
  strip.show();
}
void configureSensor(void)
{
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
}


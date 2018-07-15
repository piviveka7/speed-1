/*
 *    FILE: MM01
 *  AUTHOR: Rob van den Tillaart; modified Ralph Martin
 *    DATE: 2012 06 10 
 *     ORIGINAL URL: http://playground.arduino.cc/Code/HallEffect
 *
 * PURPOSE: use an A1301 or A1302 as magnetometer   
 *
 * Pin Layout LH Package
 * =====================
 *  1     VCC 5V
 *  2     signal    connected to Analog 2    
 *  3     GND
 *
 * Pin Layout UA Package
 * =====================
 *  1     VCC 5V
 *  2     GND
 *  3     signal    connected to Analog 2    
 *
 */

int sensorPin = 4;
int counter = 0;
boolean sensorState = false;
const unsigned long sampleTime=1000;
const int maxRPM = 10200; 
float mph;

/*
const float TireDiameter = 5.0;  // Tire diameter in inches
const float TireCircumference = TireDiameter * PI;
const int FeetPerMile = 5280;
const int InchesPerFoot = 12;
const long InchesPerMile = (long)FeetPerMile * (long)InchesPerFoot;
const int MinutesPerHour = 60;

float RPM = 1000;  // test example
 */


#define NOFIELD 505L    // Analog output with no applied field, calibrate this

// Uncomment one of the lines below according to device in use A1301 or A1302
// This is used to convert the analog voltage reading to milliGauss
#define TOMILLIGAUSS 1953L  // For A1301: 2.5mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 1953mG
// #define TOMILLIGAUSS 3756L  // For A1302: 1.3mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 3756mG

void setup() 
{
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

}

void DoMeasurement()
{
// measure magnetic field
  int raw = analogRead(2);   // Range : 0..1024

//  Uncomment this to get a raw reading for calibration of no-field point
//  Serial.print("Raw reading: ");
//  Serial.println(raw);

  long compensated = raw - NOFIELD;                 // adjust relative to no applied field 
  long gauss = compensated * TOMILLIGAUSS / 1000;   // adjust scale to Gauss

  Serial.print(gauss);
  Serial.print(" Gauss ");

  if (gauss > 0)     Serial.println("(South pole)");
  else if(gauss < 0) Serial.println("(North pole)");
  else               Serial.println();
}

void checkMagnet() 
{ 
  // check if the magnet is there 
  if(magnetPresent(sensorPin)&& !sensorState)
  {
    sensorState = true;
    printMessage("Magnet is there");
  }
  else if ( !magnetPresent(sensorPin)&& sensorState) 
  { 
    sensorState =false;
    printMessage(" No Magnet");
  }
}

boolean magnetPresent(int pin){
  return digitalRead(pin) == LOW;
}
int getRPM()
{
  // sample for sampleTime in millisecs
  int kount=0;
  boolean kflag=LOW;
  unsigned long currentTime=0;
  unsigned long startTime=millis(); 
  /*millis :Returns the number of milliseconds since the Arduino board began running the current program. 
   * This number will overflow (go back to zero), after approximately 50 days.
   */
  while (currentTime<=sampleTime)
  {
    if (digitalRead(sensorPin)==HIGH)
    {
      kflag=HIGH;
    }
    if (digitalRead(sensorPin)==LOW && kflag==HIGH)
    {
      kount++;
      kflag=LOW;
    }
    currentTime=millis()-startTime;
  }
  int kount2rpm = int(60000./float(sampleTime))*kount;

  Serial.print(" count of the rpm is  ");
  Serial.print(kount2rpm);
  Serial.print(" ");
  mph =kount2rpm  * 3.14159 * 5.0 /  63360.0;
  /* 1 mile =  63360  inch so you have:
pi * pi*  d(inch) * 1 mile/30960 inch.
   */
   Serial.print("mph is ");
   Serial.print(mph);
  return kount2rpm;
}
void printMessage(String message){
  counter++;

  Serial.print(counter);
  Serial.print(" ");
  Serial.println(message);
//  delay(1000);
}
 


void loop() 
{
    delay(1000);
    checkMagnet();
    DoMeasurement();
    getRPM();
    /*float InchesPerMinute = rpm * TireCircumference;
    float InchesPerHour = InchesPerMinute * MinutesPerHour;
    float MPH = InchesPerHour / InchesPerMile;
    Serial.println(MPH);
    */
 }

//#include "ArduFlowMtrConstants.h"

const char METER_1_DEVICE_ID[ ] = "WTR_PUN_00004_2016";
//const char METER_2_DEVICE_ID[ ] = "WTR_PUN_00002_2016";
//const char METER_3_DEVICE_ID[ ] = "WTR_PUN_00003_2016";
const float arduinoRefVoltage = 4.75;//this is indeed Arduino reference voltage since analogRead will read voltage from target circuit as how many parts of Arduino voltage (out of 1023 parts)
const float targetMaxVoltage = 5.06;//Maximum voltage of the circuit which is to be measured
//float totalCircuitResistance = 0; //add up all the attached resistances in circuit in Ohms
const int mAmVConversionFactor = 1000; //factor to convert from Amp to mA
const int endResisterValue = 22; //fixed value of last resister (in Ohms) in circuit
const int PIPE_FLOWRATE_CONV_FACTOR = 2; // this is the factor using which flow rate value is uniformly upgraded. Use across all sensors if flow rate dropping very low to be measured.
const int MAX_FLOWRATE_IGNORE_LIMIT = 18;//below this limit if flow is flowing through supply line, Green LED will not light and user will not be informed.
const int PIPE_LENGTH = 10;//10 meters of pipe length (represented by 1 meters of actual length
const long READING_INTERVAL = 300000; //Take readings every 5 minutes. 1 min = 60000 milliseconds
const int checkInterval = 10;//check if time interval reached and blink LED every checkInterval milliseconds

//const int lastSensorPin = A0;
const int meter1Pin1 = A0;
const int meter1Pin2 = A1;
const int meter2Pin1 = A2;
const int meter2Pin2 = A3;
const int meter3Pin1 = A4;
const int meter3Pin2 = A5;
//const int valveM1Pin = 8;
const int meter1LEDg = 4;
const int meter1LEDr = 5;
const int meter2LEDg = 6;
const int meter2LEDr = 7;
const int meter3LEDg = 8;
const int meter3LEDr = 9;
const int NUM_SAMPLES = 100;//number of samples to be read from a pin to average the output voltage. Maintain it at 100 as otherwise it gives inconsistant results.

unsigned long timer;
boolean firstTime;


void setup() {
  pinMode(meter1LEDg, OUTPUT);
  pinMode(meter1LEDr, OUTPUT);
  pinMode(meter2LEDg, OUTPUT);
  pinMode(meter2LEDr, OUTPUT);
  timer = millis();
  firstTime = true;
  Serial.begin(57600);
   
}

void loop() {

  //get the flow meter readings
  

    float totalFlowRate = 0.00;
    float meter1FlowRate = 0.00;
    float meter2FlowRate = 0.00;
    float meter3FlowRate = 0.00;
    totalFlowRate = getTotalFlowRate(targetMaxVoltage, mAmVConversionFactor, PIPE_FLOWRATE_CONV_FACTOR);
    meter1FlowRate = getMeterReading(meter1Pin1, meter1Pin2);
    meter2FlowRate = getMeterReading(meter2Pin1, meter2Pin2);
   // meter3FlowRate = getMeterReading(meter3Pin1, meter3Pin2);
    
    
    //check if there is indeed flow flowing through supply line and if so calculate reading and also start LED blinking
    if(meter1FlowRate > MAX_FLOWRATE_IGNORE_LIMIT) {
      //tap on, start green LED blinking sequence
      stopLED(meter1LEDr);
      blinkLED(meter1LEDg);
      
    } else {
      stopLED(meter1LEDg);
      startLED(meter1LEDr);
    }
  
    if(meter2FlowRate > MAX_FLOWRATE_IGNORE_LIMIT) {
      //tap on, start green LED blinking sequence
      stopLED(meter2LEDr);
      blinkLED(meter2LEDg);
    } else {
      stopLED(meter2LEDg);
      startLED(meter2LEDr);
    }

    /*if(meter3FlowRate > MAX_FLOWRATE_IGNORE_LIMIT) {
      //tap on, start green LED blinking sequence
      stopLED(meter3LEDr);
      blinkLED(meter3LEDg);
    } else {
      stopLED(meter3LEDg);
      startLED(meter3LEDr);
    }*/

  //Print the output the serial (since we could not send in cloud in time of hackathon
  Serial.println("-----------------------------");
  Serial.println("---------");
  Serial.print("Meter no. 4, Meter/consumer no. ");
  Serial.print(METER_1_DEVICE_ID);
  Serial.print(" Reading: ");
  Serial.print(meter1FlowRate);
  Serial.println(" L/Min");
  Serial.print("Total Main Pipeline flow rate at end : ");
  Serial.print(meter2FlowRate);
  Serial.println(" L/Min");

   //sleep for an interval
  delay(checkInterval);
}



void startLED(int LEDPin) {
  digitalWrite(LEDPin, HIGH);
}
void stopLED(int LEDPin) {
  digitalWrite(LEDPin, LOW);
}
void blinkLED(int LEDPin) {
  digitalWrite(LEDPin, HIGH);
  delay(1);
  digitalWrite(LEDPin, LOW);
  delay(1);
  digitalWrite(LEDPin, HIGH);
}


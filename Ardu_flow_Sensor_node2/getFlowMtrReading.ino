/*
 * library to read, calibrate and return Flow meter reading
 */


 float getMeterReading(int meterPinStart, int meterPinEnd) {
  float totalFlowRate = 0.00;
  unsigned long timeInterval = 0;
  
  //check if reading interval reached
  if(firstTime) {
    timeInterval = millis();
  } else {
    timeInterval = timer + READING_INTERVAL;
  }
  if(millis() >= timeInterval) {
    //do calibration
    int sample_count = 0;
    unsigned long meterPin1ReadTot = 0;
    unsigned long meterPin2ReadTot = 0;
    
    //read average analog value from meter pin (range is 0 to 1023)
    meterPin1ReadTot = readMeterPin(meterPinStart);
    meterPin2ReadTot = readMeterPin(meterPinEnd);
    
    float meterPin1Volt = ((float)meterPin1ReadTot/(float)NUM_SAMPLES)* targetMaxVoltage/1023;
    float meterPin2Volt = ((float)meterPin2ReadTot/(float)NUM_SAMPLES)* targetMaxVoltage/1023;
    
    //calculate supply meter flow Rate
    float meterFlowRate = getFlowRatePerMin(meterPin1Volt, meterPin2Volt, mAmVConversionFactor, PIPE_FLOWRATE_CONV_FACTOR);
    return meterFlowRate;
  }
 }

 int readMeterPin(int pinNo) {
  delay(300);
  int currSensorValue1 = 0;
  unsigned long sensorValSum = 0;
  for(int i = 0; i < NUM_SAMPLES; i++) {
    int readVal = analogRead(pinNo);
    sensorValSum = sensorValSum + readVal;
  }
  currSensorValue1 = sensorValSum/NUM_SAMPLES;
  sensorValSum = 0;
  return currSensorValue1;
 }

 float getFlowRatePerMin(float startVoltage, float endVoltage, int conversionFactor, int pipeFlowFactor) {
  float flowRate = 0.00;
  if((startVoltage-endVoltage) <= startVoltage) {
    flowRate = convertToFlowRatePerMin(startVoltage, endVoltage, conversionFactor, pipeFlowFactor);
  } else if(((startVoltage-endVoltage) > startVoltage)) {
    if(startVoltage > 0) {
      flowRate = convertToFlowRatePerMin(startVoltage, 0, conversionFactor, pipeFlowFactor);
    } else {
      flowRate = 0.00;
    }
  }  else {
      flowRate = 0.00;
    }

    return flowRate; 
}

float convertToFlowRatePerMin(float beginVolt, float endVolt, int conversionFactor, int pipeFlowFactor) {
  /*
   * Heigan-Posielle equation is : Volumetric flow rate = (pie * r4 * (P-P0))/ 8*viscocity*length of pipe
   * assumption is r = 1 meter
   * Viscocity at 30 degree celcius = 0.798
   * 
   */
   float flowRateVolt = (beginVolt-endVolt) * (float)conversionFactor * (float)pipeFlowFactor;
   float waterFlowRate = (3.14 * 1 * flowRateVolt * 60)/(8 * 0.798 * 1);
   return waterFlowRate;
}

float getTotalFlowRate(float targetMaxVoltage, int conversionFactor, int pipeFlowFactor) {
 float flowRate = convertToFlowRatePerMin(targetMaxVoltage, 0.00, conversionFactor, pipeFlowFactor);
}


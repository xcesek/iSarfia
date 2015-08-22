/* ==================================================================================
 *  iSarfia - Weather station project
 *
 *  Tempature sensor helper funcions
 *
 *  ================================================================================== */


void initThermometer() {
  temperature[index] = readTemperature();
  
  Serial.println(" - temperature sensor initialized");
}

double thermistorCalc(int RawADC) {
  double Temp;  // Working variable
  
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;
  Temp = Temp - 20;
  
  if (debug) {
    Serial.print("    Measured temperature: "); Serial.println(Temp);
  }

  return Temp;
}

byte readTemperature() {
  int rawAnalogValue;
  int correctedAnalogValue;

  rawAnalogValue = analogRead(THERMOMETER_ANALOG_PIN);  
  correctedAnalogValue = map(rawAnalogValue, 0, 1023, 1023, 0);

  return (byte) thermistorCalc(correctedAnalogValue);
}

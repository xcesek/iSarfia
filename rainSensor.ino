/* ==================================================================================
 *  iSarfia - Weather station project
 *
 *  Rain sensor helper funcions
 *
 *  ================================================================================== */
 
 
void initRainSensor() {
  pinMode(RAIN_DIGITAL_PIN, INPUT);
  
  rainingState[index] = checkRaininig();
  
  Serial.println(" - rain sensor initialized");
}

int checkRaininig() {
  int isRaining; 
  
  isRaining = !digitalRead(RAIN_DIGITAL_PIN);  // pin is reverse logic, threrefore negation
  
  if (debug) {
    Serial.print("    Is Raining? : "); Serial.println(isRaining);
  }
  
  return isRaining;
}

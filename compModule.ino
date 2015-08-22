/* ==================================================================================
 *  iSarfia - Weather station project
 *
 *  Computer module helper funcions
 *
 *  ================================================================================== */

void sendDataToComputer() {
  int i;
  
  Serial.print("#TEMPERATURE_DATA_START:");
  for (i = 0; i < MAX_INDEX; i++) {
    Serial.print(temperature[i]);    Serial.print(",");
  }
  Serial.print("#TEMPERATURE_DATA_END:");
  
  Serial.print("#RAINING_DATA_START:");
  for (i = 0; i < MAX_INDEX; i++) {
    Serial.print(rainingState[i]);  Serial.print(",");
  }
  Serial.print("#RAINING_DATA_END:");
  
}

/* ==================================================================================
 *  iSarfia - Weather station project
 *
 *  Simple client that measures temperature and rain occurence and periodically sends data to server
 *  Server project address: http://isarfia.cloudapp.net/
 *
 *
 *  Note: if you change timer, do not forget to change arr size ind max index, not to overflow arrays!
 *
 *  August 2015
 *  By Pavol Cesek
 *  ================================================================================== */

#include <math.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <SoftwareSerial.h>
#include <String.h>


#define DEFAUL_LOOP_DEALY 1000
#define DEFAUL_USER_INPUT_TIMEOUT 20000
#define G_TIMER_H 0
#define G_TIMER_M 0
#define G_TIMER_S 5
#define S_TIMER_H 0
#define S_TIMER_M 3
#define S_TIMER_S 0
#define ARR_SIZE 36
#define MAX_INDEX 35

#define THERMOMETER_ANALOG_PIN 0
#define RAIN_ANALOG_PIN 1
#define RAIN_DIGITAL_PIN 2
#define GPRS_CONTROL_PIN 9

#define debug 1

/* ========================================================== Structures ========================================================== */
enum RecieverType {
  GPRS_MODULE,
  COMPYTER_VIA_SERIAL
};


/* ========================================================== Global variables ========================================================== */
SoftwareSerial mySerial(7, 8);
byte temperature[ARR_SIZE];
byte rainingState[ARR_SIZE];
byte index;
byte timeCorreclySet;
RecieverType recieverType;


/* ========================================================== Program setup ========================================================== */
void setup() {
  Serial.begin(19200);
  delay(500);

  Serial.println("\n\n\n====   Setup procedure started    ====\n");

  recieverType = COMPYTER_VIA_SERIAL;
  initIndices();
  initTimer();
  initThermometer();
  initRainSensor();
  
  switch(recieverType) {
    case GPRS_MODULE:
      initGprsModule();
      break;
    
    case COMPYTER_VIA_SERIAL:
      break;
  }
  
  
  Alarm.timerRepeat(G_TIMER_H, G_TIMER_M, G_TIMER_S, doGatherDataJob);
  Alarm.timerRepeat(S_TIMER_H, S_TIMER_M, S_TIMER_S, doSendDataJob);

  Serial.println("\n====   Setup procedure finished    ====\n\n\n");
}


/* ========================================================== Program main loop ========================================================== */
void loop() {

  if (debug) {
      ShowSerialData();
      digitalClockDisplay();
  }

  Alarm.delay(DEFAUL_LOOP_DEALY);
}



/* ========================================================== Functions ========================================================== */
void initIndices() {
  for (int i = 0; i < MAX_INDEX; i++) {
    temperature[i] = 0;
    rainingState[i] = 0;
  }
  index = 0;
}

int incrementIndex() {
  if(index <= MAX_INDEX) {
    index++;
    return 1;
    
  } else {
    return 0;
  }
}

void resetIndex() {
  index = 0;
}


void collectData() {
  rainingState[index] = checkRaininig();
  temperature[index] = readTemperature();
  
  if (incrementIndex() == 0) {
    sendData();
  }
}


void sendData() {  
  switch (recieverType) {
  case GPRS_MODULE:
      powerUp();
      submitHttpRequest();
      powerDown();
      break;
    
    case COMPYTER_VIA_SERIAL:
      sendDataToComputer();
      break;
  }
  
  resetIndex();
}


/* ========================================================== Callback Functions ========================================================== */

void doGatherDataJob() {
  if (debug) Serial.println("\n\n == doGatherDataJob callback function started ==\n");
  if (debug) logTime();

  collectData();

  if (debug) Serial.println("\n == doGatherDataJob callback function finished ==\n\n");
}


void doSendDataJob() {
  if (debug) Serial.println("\n\n == doSendDataJob callback function started ==\n");
  if (debug) logTime();

  sendData();

  if (debug) Serial.println("\n == doSendDataJob callback function finished ==\n\n");
}

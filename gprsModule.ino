/* ==================================================================================
 *  iSarfia - Weather station project
 *
 *  GPRS module helper funcions
 *
 *  ================================================================================== */
 
 
 void initGprsModule() {
   mySerial.begin(19200); 
 
  Serial.println(" - GPRS module sensor initialized");
}

void powerUp() {
 if (debug) Serial.println("GPRS powering up ..."); 
 
 pinMode(GPRS_CONTROL_PIN, OUTPUT); 
 digitalWrite(GPRS_CONTROL_PIN,LOW);
 delay(1000);
 digitalWrite(GPRS_CONTROL_PIN,HIGH);
 delay(2000);
 digitalWrite(GPRS_CONTROL_PIN,LOW);
 delay(8000);
 
 if (debug) Serial.println("GPRS device UP");
}

void powerDown() {
  if (debug) {  
    while (mySerial.available())  // print if sth in buffer
      Serial.write(mySerial.read());
  }
  
 if (debug) Serial.println("    GPRS powering down ..."); 
 
 pinMode(GPRS_CONTROL_PIN, OUTPUT); 
 digitalWrite(GPRS_CONTROL_PIN,LOW);
 delay(1000);
 digitalWrite(GPRS_CONTROL_PIN,HIGH);
 delay(2000);
 digitalWrite(GPRS_CONTROL_PIN,LOW);
 delay(7000);
 
 if (debug) Serial.println("    GPRS device DOWN");
}

void sendTextMessage() {
  mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
  delay(100);
  mySerial.println("AT + CMGS = \"+421949348429\"");//send sms message, be careful need to add a country code before the cellphone number
  delay(100);
  mySerial.println("Greetings from iSarfia arduino client!");//the content of the message
  delay(100);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
  mySerial.println();
  
  delay(15000);
}


void dialVoiceCall() {
  mySerial.println("ATD + +421949348429;"); //dial the number
  delay(100);
  mySerial.println();
  
  delay(15000);
}


void submitHttpRequest() {
  String queryString = prepareQueryString(now(), temperature, rainingState, index);
  
  mySerial.println("AT+CSQ");  // Request: Check the Cellphone signal quality 
  delay(300);
  ShowSerialData();
  
  mySerial.println("AT+CGATT=1");  // Perform a GPRS Attach. The device should be attached to the GPRS network before a PDP context can be established
  delay(300);
  ShowSerialData();
  
  mySerial.println("AT+CGATT?");  // Request: Check for GPRS network and how many there are 
  delay(300);
  ShowSerialData();
  
  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  // Set up profile 1, connection type to internet. 
  delay(1000);
  ShowSerialData();
  
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"o2internet\"");  // Connect to a perdiculare APN. This is the APN for o2
  delay(4000);
  ShowSerialData();
  
  mySerial.println("AT+SAPBR=1,1");  //setting the SAPBR, for detail you can refer to the AT command mamual
  delay(2000);
  ShowSerialData();
  
  mySerial.println("AT+HTTPINIT"); //init the HTTP request
  delay(2000);
  ShowSerialData();
  
  mySerial.println(queryString);  // setting the http request
  delay(1000);
  ShowSerialData();
  
  mySerial.println("AT+HTTPACTION=0");  //submit the request
  delay(4000);  //the delay is very important, the delay time is base on the return from the website
  ShowSerialData();
  
  mySerial.println("AT+HTTPREAD");  // read the data from the website you access
  delay(1000);
  ShowSerialData();
  
  mySerial.println("");
  delay(4000);
  ShowSerialData();
}

String prepareQueryString(time_t _date, byte *_temperature, byte *_rainingState, byte _index) {
  String dateStr = String(_date, DEC);
  String tempStr = String(_temperature[0], 2);
  String rainingStr = String(_rainingState[0], DEC);
  
  String queryString  = "AT+HTTPPARA=\"URL\",\"";
  queryString.concat("http://isarfia.cloudapp.net/weather-station/index.jsp?");
  queryString.concat("d=");  queryString.concat(dateStr);  queryString.concat("&");
  queryString.concat("t=");  queryString.concat(tempStr);  queryString.concat("&");
  queryString.concat("r=");  queryString.concat(rainingStr);
  queryString.concat("\"");

  return queryString;
}


void ShowSerialData(){
  if (debug) {
    while (mySerial.available() != 0)
      Serial.write(mySerial.read());
  }
}


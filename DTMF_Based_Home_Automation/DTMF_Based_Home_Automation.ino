//Prateek

#include "SoftwareSerial.h"
#define SIM800L_Tx 9
#define SIM800L_Rx 10
SoftwareSerial SIM800L(SIM800L_Tx, SIM800L_Rx);

#define LED1 4
#define LED2 5
#define LED3 6
#define LED4 7

char dtmf_cmd;
bool call_flag = false;
bool LED1_STATE = false;
bool LED2_STATE = false;
bool LED3_STATE = false;
bool LED4_STATE = false;
void init_gsm();
void update_led();

void setup() {
  SIM800L.begin(9600);
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  digitalWrite(LED1, LED1_STATE);
  digitalWrite(LED2, LED2_STATE);
  digitalWrite(LED3, LED3_STATE);
  digitalWrite(LED4, LED4_STATE);
  init_gsm();
}

void loop() {
  String gsm_data;
  int x = -1;
  while (SIM800L.available()) {
    char c = SIM800L.read();
    gsm_data += c;
    delay(10);
  }

  if (call_flag) {
    x = gsm_data.indexOf("DTMF");
    if (x > -1) {
      dtmf_cmd = gsm_data[x + 6];
      Serial.println(dtmf_cmd);
      update_led();
    }

    x = gsm_data.indexOf("NO CARRIER");
    if (x > -1) {
      SIM800L.println("ATH");
      call_flag = false;
    }
  } else {
    x = gsm_data.indexOf("RING");
    if (x > -1) {
      delay(5000);
      SIM800L.println("ATA");
      call_flag = true;
    }
  }
}

void init_gsm() {
  boolean gsm_Ready = 1;
  Serial.println("initializing GSM module");
  while (gsm_Ready > 0) {
    SIM800L.println("AT");
    Serial.println("AT");
    while (SIM800L.available()) {
      if (SIM800L.find("OK") > 0)
        gsm_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("AT READY");

  boolean ntw_Ready = 1;
  Serial.println("finding network");
  while (ntw_Ready > 0) {
    SIM800L.println("AT+CPIN?");
    Serial.println("AT+CPIN?");
    while (SIM800L.available()) {
      if (SIM800L.find("+CPIN: READY") > 0)
        ntw_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("NTW READY");

  boolean DTMF_Ready = 1;
  Serial.println("turning DTMF ON");
  while (DTMF_Ready > 0) {
    SIM800L.println("AT+DDET=1");
    Serial.println("AT+DDET=1");
    while (SIM800L.available()) {
      if (SIM800L.find("OK") > 0)
        DTMF_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("DTMF READY");
}

void update_led() {
  if (dtmf_cmd == '1') {
    if (LED1_STATE) {
      LED1_STATE = false;
      digitalWrite(LED1, LED1_STATE);
      Serial.println("RELAY 1 OFF");
    } else {
      LED1_STATE = true;
      digitalWrite(LED1, LED1_STATE);
      Serial.println("RELAY 1 ON");
    }
  }

  if (dtmf_cmd == '2') {
    if (LED2_STATE) {
      LED2_STATE = false;
      digitalWrite(LED2, LED2_STATE);
      Serial.println("RELAY 2 OFF");
    } else {
      LED2_STATE = true;
      digitalWrite(LED2, LED2_STATE);
      Serial.println("RELAY 2 ON");
    }
  }

  if (dtmf_cmd == '3') {
    if (LED3_STATE) {
      LED3_STATE = false;
      digitalWrite(LED3, LED3_STATE);
      Serial.println("RELAY 3 OFF");
    } else {
      LED3_STATE = true;
      digitalWrite(LED3, LED3_STATE);
      Serial.println("RELAY 3 ON");
    }
  }

  if (dtmf_cmd == '4') {
    if (LED4_STATE) {
      LED4_STATE = false;
      digitalWrite(LED4, LED4_STATE);
      Serial.println("RELAY 4 OFF");
    } else {
      LED4_STATE = true;
      digitalWrite(LED4, LED4_STATE);
      Serial.println("RELAY 4 ON");
    }
  }
}
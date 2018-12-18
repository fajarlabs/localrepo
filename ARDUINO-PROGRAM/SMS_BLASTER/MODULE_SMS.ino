/* define io signal input */
#define SIGNAL_NUC_IN_3 A2
#define SIGNAL_NUC_IN_2 A1
#define SIGNAL_NUC_IN_1 A0

/* define led power button */
#define LED_POWER 5
#define LED_POWER_2 6

/* define relay output */
#define RELAY1 7 // A2 sensor IN
#define RELAY2 8 // A1 sensor IN
#define RELAY3 9 // A0 sensor IN

#define SENSOR_IN_S1 13
#define SENSOR_IN_S2 12
#define SENSOR_IN_S3 11
#define SENSOR_IN_S4 2
#define SENSOR_IN_S5 3

/* waktu lama relay ditekan */
int time_wait = 2000;

int data_remote = 0;


/* define io signal power NUC input */
/* module lama pakai pin A6 */
/* #define SIGNAL_POWER_BUTTON A6 */

/* module baru pakai pin A7 */
/*#define SIGNAL_POWER_BUTTON A7*/

#define SIGNAL_POWER_BUTTON A6 // module promini 5v

/* flag power */
int flag_power = 0; // 0 is off
int flag_continue = 0;

/* the setup function runs once when you press reset or power the board */
void setup() {
  /* serial set */
  Serial.begin(9600);
  
  /* initialize digital pin LED_BUILTIN as an output. */
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  
  pinMode(SIGNAL_POWER_BUTTON, INPUT);
  
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_POWER_2, OUTPUT);
  
  digitalWrite(LED_POWER,HIGH);
  digitalWrite(LED_POWER_2,HIGH);
}

void _relay_all_on() {
  digitalWrite(RELAY1,HIGH);
  digitalWrite(RELAY2,HIGH);
  digitalWrite(RELAY3,HIGH);
}

void _relay_all_off() {
  digitalWrite(RELAY1,LOW);
  digitalWrite(RELAY2,LOW);
  digitalWrite(RELAY3,LOW);
}

void _checking_nuc_status_on() {
  int j = 0;

  while(true) {
    int SEN_A3 = analogRead(SIGNAL_NUC_IN_3);
    int SEN_A2 = analogRead(SIGNAL_NUC_IN_2);
    int SEN_A1 = analogRead(SIGNAL_NUC_IN_1);
    
    if((SEN_A3 < 900) && (SEN_A2 < 900) && (SEN_A1 < 900)) {
      digitalWrite(LED_POWER,HIGH);
      digitalWrite(LED_POWER_2,HIGH);
      break;
    }

    if(SEN_A3 > 900) {
      Serial.println("On A3");
      digitalWrite(RELAY1,HIGH);
      delay(400);
      digitalWrite(RELAY1,LOW);
    }

    if(SEN_A2 > 900) {
      Serial.println("On A2");
      digitalWrite(RELAY2,HIGH);
      delay(400);
      digitalWrite(RELAY2,LOW);
    }

    if(SEN_A1 > 900) {
      Serial.println("On A1");
      digitalWrite(RELAY3,HIGH);
      delay(400);
      digitalWrite(RELAY3,LOW);
    }

    if(j == 0) {
      digitalWrite(LED_POWER,LOW);
      digitalWrite(LED_POWER_2,LOW);
      j = 1;
    } else {
      digitalWrite(LED_POWER,HIGH);
      digitalWrite(LED_POWER_2,HIGH);
      j = 0;
    }

    delay (400);
  }
}

// the loop function runs over and over again forever
void loop() {
  int in_read = analogRead(SIGNAL_POWER_BUTTON);
  if(in_read > 900){
    data_remote = 1; // dari tombol flag
  }
  if(digitalRead(SENSOR_IN_S4) == HIGH) {
    data_remote = 2; // dari remote flag
    if(flag_power < 1) {
      in_read = 1024;
    }
  } else {
    if(flag_power > 0) {
      if(data_remote == 2) {
        if(flag_continue > 0) {
          flag_continue = 2;
        }
        in_read = 1024;
        Serial.println("LOW");
      }
    }
  }
  if(in_read > 900) {
    // agar saat ditekan akan menunggu event onkeyup
    if(flag_continue != flag_power) {
      // show blink led
      for(int i = 0; i < 5; i++) {
        if((i % 2) == 1) {
          digitalWrite(LED_POWER,LOW);
          digitalWrite(LED_POWER_2,LOW);
        } else {
          digitalWrite(LED_POWER,HIGH);
          digitalWrite(LED_POWER_2,HIGH);
        }
        delay(500);
      }
      // turn Power ON
      if(flag_power < 1) {
        flag_power = 1;
        flag_continue = 1;
        digitalWrite(LED_POWER,LOW);
        digitalWrite(LED_POWER_2,LOW);
        _relay_all_on();
        delay(time_wait);
        _relay_all_off();
      } else {
        flag_power = 0;
        flag_continue = 0;
        digitalWrite(LED_POWER,HIGH);
        digitalWrite(LED_POWER_2,HIGH);
        _relay_all_on();
        delay(time_wait);
        _relay_all_off();
        _checking_nuc_status_on();
      }
    }
  } else {
    // reset flag
    flag_continue = 2;
  }

  delay(100);        
}

/* define io signal power NUC input */
#define SIGNAL_POWER_BUTTON A7

/* define led power */
#define LED_POWER_1 8 
#define LED_POWER_2 9 

/* define relay output*/
#define RELAY 10

/* flag power */
int flag_power = 0; // 0 is off
int flag_continue = 0;
/* waktu lama tombol ditekan relay */
int time_wait = 4000;

/* the setup function runs once when you press reset or power the board */
void setup() {
  /* serial set */
  Serial.begin(9600);
  
  /* initialize digital pin LED_BUILTIN as an output. */
  pinMode(LED_POWER_1, OUTPUT);
  pinMode(LED_POWER_2, OUTPUT);
  pinMode(SIGNAL_POWER_BUTTON, INPUT);
  pinMode(RELAY, OUTPUT);

  digitalWrite(LED_POWER_1,HIGH);
  digitalWrite(LED_POWER_2,HIGH);
}

void _relay_all_on() {
  digitalWrite(RELAY,HIGH);
}

void _relay_all_off() {
  digitalWrite(RELAY,LOW);
}

// the loop function runs over and over again forever
void loop() {
  int in_read = analogRead(SIGNAL_POWER_BUTTON);
  Serial.println(in_read);
  if(in_read > 900) {
    // agar saat ditekan akan menunggu event onkeyup
    if(flag_continue != flag_power) {
      // show blink led
      for(int i = 0; i < 5; i++) {
        if((i % 2) == 1) {
          digitalWrite(LED_POWER_1,LOW);
          digitalWrite(LED_POWER_2,LOW);
        } else {
          digitalWrite(LED_POWER_1,HIGH);
          digitalWrite(LED_POWER_2,HIGH);
        }
        delay(500);
      }
      // turn Power ON
      if(flag_power < 1) {
        flag_power = 1;
        flag_continue = 1;
        digitalWrite(LED_POWER_1,LOW);
        digitalWrite(LED_POWER_2,LOW);
        _relay_all_on();
        delay(time_wait); // tunggu selama 4 detik
        _relay_all_off();
      } else {
        flag_power = 0;
        flag_continue = 0;
        digitalWrite(LED_POWER_1,HIGH);
        digitalWrite(LED_POWER_2,HIGH);
        _relay_all_on();
        delay(time_wait);
        _relay_all_off();
      }
    }
  } else {
    // reset flag
    flag_continue = 2;
  }

  delay(100);        
}

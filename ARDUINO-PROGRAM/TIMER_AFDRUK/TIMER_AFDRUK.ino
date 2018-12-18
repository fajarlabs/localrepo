#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
#include <TimedAction.h>

#define BUZZER 11
#define LAMP_CENTER 12
#define INPUT_LAMP_CENTER 3
#define LAMP_AFDRUK 13
#define INPUT_LAMP_AFDRUK 2
#define INPUT_RESET_AFDRUK 10

// konfigurasi rotary encoder
int val;
int encoder0PinA = 8;
int encoder0PinB = 7;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

// data untuk hitung mundur timer afdruk
int afdruk_counter = 0; 
bool start_time_afdruk_flag = false;

// konfigurasi lampu tengah
bool lamp_center_flag = false;
bool lamp_center_switch = false;

// konfigurasi buzzer
bool buzzer_active_flag = false;

// konfigurasi LCD blink
bool blink_flag = false;

// flag reset system
// dibutuhkan untuk reset TimedAction / Thread
bool resetSystemFlag = false;

// current message LCD
bool currentStateFlag = false;
typedef struct
 {
     int current_page;
     char change;
 }  record_lcd;

record_lcd record_page;

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(LAMP_CENTER, OUTPUT);
  pinMode(LAMP_AFDRUK, OUTPUT);
  pinMode(INPUT_LAMP_CENTER, INPUT);
  pinMode(INPUT_LAMP_AFDRUK, INPUT);
  pinMode(INPUT_RESET_AFDRUK, INPUT);
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  Serial.begin (9600);
  lcd.begin();
  lcd.setCursor(2,0);
  String str1 = "FLABS PRODUCT";
  lcd.print(str1);
  lcd.setCursor(0,1);
  String str2 = "Timer For Sablon";
  lcd.print(str2);
  delay(3000);
  idle_screen();
}

void statePage(int currPage, int changePage) {
  record_page.current_page = currPage;
  record_page.change = changePage;
  // reset protothreading to 3 seconds
  // to show text screen LCD
  currentStateFlag = true;
}

void idle_screen() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">IDLE STATUS");
  lcd.setCursor(0,1);
  lcd.print(" waiting order");
}

void prepare_screen(int t) {
  lcd.clear();
  lcd.setCursor(0,1);
  if(t > 0) {
    lcd.print(" set in ");
    lcd.print(t);
    lcd.print(" M");
  } else {
    lcd.print("-");
  }
  lcd.setCursor(0,0);
  lcd.print(">COUNTDOWN");
}

void standby_screen() {
  String strA = ">STANDBY LAMP";
  String strB = " glass light";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(strA);
  lcd.setCursor(0,1);
  lcd.print(strB);
}

void info_screen(String s) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">INFO");
  lcd.setCursor(0,1);
  lcd.print(" ");
  lcd.print(s);
}


void afdruk_screen(int t) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">AFDRUK LAMP");
  lcd.setCursor(0,1);
  lcd.print(" countdown ");
  lcd.print(t);
  lcd.print(" M");
}

void _reset() {
    // lamp center reset
    lamp_center_switch = false;
    lamp_center_flag = false;
    buzzer_active_flag = false;
    // time afdruk
    afdruk_counter = 0;
    // flag key time afdruk
    start_time_afdruk_flag = false;
    idle_screen();
    // on backlight
    lcd.backlight();
}

void reset_timer_afdruk() {
  if(digitalRead(INPUT_RESET_AFDRUK) == HIGH) {
    _reset();
  }
}

void start_timer_afdruk() {
  if(digitalRead(INPUT_LAMP_AFDRUK) == HIGH) {
    if(lamp_center_switch == false) {
      if(afdruk_counter > 1) {
        if(start_time_afdruk_flag == false) {
          resetSystemFlag = true;
          start_time_afdruk_flag = true;
          info_screen("countdown "+String(afdruk_counter)+" M");
        }
      } else {
        tone(BUZZER, 3000, 500);
        info_screen("is time set?");
        statePage(1,'Y');
      }
    } else {
      tone(BUZZER, 3000, 500);
      info_screen("is standby?");
      statePage(2,'Y');
    }
  }
}

void run_timer_afdruk() {
  if(start_time_afdruk_flag == true) {
    if(afdruk_counter > 1) {
      afdruk_counter --;
      afdruk_screen(afdruk_counter);
    } else {
      _reset();
    }
  }
}

void active_buzzer() {
  if(start_time_afdruk_flag == true || buzzer_active_flag == true) {
    if(afdruk_counter > 0 && afdruk_counter < 2) {
      tone(BUZZER, 3000, 500);
      if(blink_flag == false) {
        blink_flag = true;
        lcd.backlight();
      } else {
        blink_flag = false;
        lcd.noBacklight();
      }
    }
  }
}

void lamp_center() {
    // jika tombol ditekan terus maka tidak terjadi bouncing
    // atau tombol putus - sambung jika tombol ditahan / ditekan
    if(digitalRead(INPUT_LAMP_CENTER) == HIGH) {
      if(start_time_afdruk_flag == false) {
        if(lamp_center_flag == false) {
          if(lamp_center_switch == false) {
            lamp_center_switch = true;
            digitalWrite(LAMP_CENTER,HIGH);
            standby_screen();
          } else {
            lamp_center_switch = false;
            digitalWrite(LAMP_CENTER,LOW);
            idle_screen();
          }
          lamp_center_flag = true;
        }
      } else {
        tone(BUZZER, 3000, 500);
        info_screen("is afdruk?");
        statePage(3,'Y');
      }
    } else {
      // jika tombol di lepas maka terjadi reset
      // pada kunci tombol / flag lampu center
      lamp_center_flag = false;
    }
}

void rotary_encoder_listening() {
  // blocking rotary encoder access
  if(start_time_afdruk_flag == true) {
    if(digitalRead(encoder0PinA) == LOW || digitalRead(encoder0PinB) == LOW) {
      info_screen("in afdruk mode");
      statePage(3,'Y');
    }
    return;
  }
  
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      if(encoder0Pos > 1) {
        encoder0Pos--;
        afdruk_counter = encoder0Pos;
        prepare_screen(afdruk_counter);
      }
    } else {
      encoder0Pos++;
      afdruk_counter = encoder0Pos;
      prepare_screen(afdruk_counter);
    }
  }
  encoder0PinALast = n;
}

void load_relay() {
    if(start_time_afdruk_flag == true) {
      digitalWrite(LAMP_CENTER, HIGH);
      digitalWrite(LAMP_AFDRUK, HIGH);
    } else {
      if(lamp_center_switch == true){
        digitalWrite(LAMP_CENTER, HIGH);
      } else {
        digitalWrite(LAMP_CENTER, LOW);
      }
      digitalWrite(LAMP_AFDRUK, LOW);
    }
}

void router_state() {
  if(record_page.change == 'Y') {
    switch(record_page.current_page) {
      case 1 :
        idle_screen();
      break;
      case 2 :
        standby_screen();
      break;
      case 3 :
        afdruk_screen(afdruk_counter);
      break;
      default :
      idle_screen();
      break;
    }
    // reset page
    record_page.change = 'N';
  }
}
TimedAction switchLampCenter = TimedAction(50, lamp_center);
TimedAction rotaryEncListThread = TimedAction(0,rotary_encoder_listening);
TimedAction loadRelay = TimedAction(0,load_relay);
TimedAction runTimerAfdruk = TimedAction(60000,run_timer_afdruk);
TimedAction startTimeAfdruk = TimedAction(50,start_timer_afdruk);
TimedAction resetTimeAdruk = TimedAction(50,reset_timer_afdruk);
TimedAction activeBuzzer = TimedAction(1000,active_buzzer);
TimedAction routerState = TimedAction(3000,router_state);

void loop() {

  if(resetSystemFlag == true) {
    resetSystemFlag = false;
    runTimerAfdruk.reset();
  }

  if(currentStateFlag == true) {
    currentStateFlag = false;
    routerState.reset();
  }
  
  rotaryEncListThread.check();
  loadRelay.check();
  switchLampCenter.check();
  runTimerAfdruk.check();
  startTimeAfdruk.check();
  resetTimeAdruk.check();
  activeBuzzer.check();
  routerState.check();
}

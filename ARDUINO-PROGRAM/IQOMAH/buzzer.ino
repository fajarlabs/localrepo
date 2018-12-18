/* 
 *  KONFIGURASI PIN BUZZER
 */
#define PIN_BUZZER 9

void buzzerOn() {
  tone(PIN_BUZZER, 1300, 252);
}

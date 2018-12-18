/**
 * PROGRAM MICRO RESKA TAHUN 2018
 * UJI PROGRAM, MICRO DI PARKIR STASIUN JUANDA
 */

#include <TimedAction.h>

// Input signal
#define IN1 A0 // loop1
#define IN2 A1 // tombol tiket
#define IN3 A2 // loop2
#define IN4 A3 // not use
#define IN5 A4 // not use
#define IN6 A5 // not use

// Output
#define OUT1 8
#define OUT2 7
#define OUT3 6
#define OUT4 5
#define OUT5 4

// Wiegan Card 125KHz
#define WD0 2 // not use
#define WD1 3 // not use

// LED POWER BLINK
#define LED0 9

int led_flag = false;

void setup()
{
    // Serial
    Serial.begin(9600);

    // Set output
    pinMode(OUT1, OUTPUT);
    pinMode(OUT2, OUTPUT);
    pinMode(OUT3, OUTPUT);
    pinMode(OUT4, OUTPUT);
    pinMode(OUT5, OUTPUT);

    // Set analog to digital
    pinMode(IN1, INPUT);
    pinMode(IN2, INPUT);
    pinMode(IN3, INPUT);
    pinMode(IN4, INPUT); // not use
    pinMode(IN5, INPUT); // not use
    pinMode(IN6, INPUT); // not use

    // Led Signal
    pinMode(LED0, OUTPUT);

    // Set default to HIGH input
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    digitalWrite(IN5, HIGH);
    digitalWrite(IN6, HIGH);
}

void start(char a)
{
    // Check String is Exist
    if (a == 'Q') {
        digitalWrite(OUT1, HIGH);
        digitalWrite(OUT2, HIGH);
        digitalWrite(OUT3, HIGH);
        return;
        // If sending data from serial is "P"
        // Send status controller
    }
    else if (a == 'P') {
        Serial.print("X");
        // CARA II
        // Ini adalah flow untuk release relay dengan perintah P
        // lebih cocok untuk flow dari bawaan alat sebelumnya cuma
        // ini belum di coba untuk digunakan, jika ini digunakan
        // buka tag komentarnya 3 baris dibawah ini, agar kode bisa berjalan
        // dan beri komentar comment // script pada CARA I
        // digitalWrite(OUT1,LOW);
        // digitalWrite(OUT2,LOW);
        // digitalWrite(OUT3,LOW);
        return;
    }

    // CARA I
    // Ini menyesuikan flow yang sekarang bisa dilapangan
    // Ini hanya menyalakan relay sekali saja
    // walaupun dari aplikasi tester di hentikan selama 3 detik
    // Jika menggunakan cara pada CARA II matikan program
    // dibawah ini dengan mengguankan garis miring // didepan scriptnya
    // pada 3 baris dibawah ini
    digitalWrite(OUT1, LOW);
    digitalWrite(OUT2, LOW);
    digitalWrite(OUT3, LOW);

    bool An1 = false;
    bool An2 = false;
    bool An3 = false;
    bool An4 = false; // not use
    bool An5 = false; // not use
    bool An6 = false; // not use

    // Init signal digital to LOGICAL
    if (digitalRead(IN1) == LOW) {
        An1 = true;
    }
    if (digitalRead(IN2) == LOW) {
        An2 = true;
    }
    if (digitalRead(IN3) == LOW) {
        An3 = true;
    }

    // Not Use
    if (digitalRead(IN4) == LOW) {
        An4 = true;
    }
    // Not Use
    if (digitalRead(IN5) == LOW) {
        An5 = true;
    }
    // Not Use
    if (digitalRead(IN6) == LOW) {
        An6 = true;
    }

    // loop1(0),tombol(1),loop2(1)
    if ((An1 == true) && (An2 == false) && (An3 == false)) {
        Serial.print("A");
        return;
    }

    // loop1(1),tombol(0),loop2(1)
    if ((An1 == false) && (An2 == true) && (An3 == false)) {
        Serial.print("C");
        return;
    }

    // loop1(1),tombol(1),loop2(0)
    if ((An1 == false) && (An2 == false) && (An3 == true)) {
        Serial.print("E");
        return;
    }

    // loop1(0),tombol(0),loop2(1)
    if ((An1 == true) && (An2 == true) && (An3 == false)) {
        Serial.print("B");
        return;
    }

    // loop1(1),tombol(0),loop2(0)
    if ((An1 == false) && (An2 == true) && (An3 == true)) {
        Serial.print("D");
        return;
    }

    // loop1(0),tombol(1),loop2(0)
    if ((An1 == true) && (An2 == false) && (An3 == true)) {
        Serial.print("H");
        return;
    }

    // loop1(0),tombol(0),loop2(0)
    if ((An1 == true) && (An2 == true) && (An3 == true)) {
        Serial.print("G");
        return;
    }

    // loop1(1),tombol(1),loop2(1)
    if ((An1 == false) && (An2 == false) && (An3 == false)) {
        Serial.print("R");
        return;
    }
}

void receiveSerial()
{
    char a = 'N';
    while (Serial.available()) {
        a = Serial.read(); // read the incoming data as string
        if (a == 'Q')
            break;
        if (a == 'P')
            break;
    }

    // start flow
    start(a);
}

void ledCtrl()
{
    if (!led_flag) {
        digitalWrite(LED0, HIGH);
        led_flag = true;
    }
    else {
        digitalWrite(LED0, LOW);
        led_flag = false;
    }
}

TimedAction t_ReceiveSerial = TimedAction(50, receiveSerial);
TimedAction t_LedCtrl = TimedAction(500, ledCtrl);

void loop()
{
    t_ReceiveSerial.check();
    t_LedCtrl.check();
}

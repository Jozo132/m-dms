#pragma once
#include <IWatchdog.h>
#include <Vovk.h>

bool& LED = AutoPins.attach(PC13, OUTPUT, INVERTED);

// EXPANSION
#include <LSR32IO.h>

#define cs_pin PB5

#define sck_pin PB13
#define miso_pin PB14
#define mosi_pin PB15

#define latch_pin PB4
#define en_pin PB3
#define rst_pin PA15

LSR32IO expansion(cs_pin, latch_pin, en_pin, rst_pin);

#define BUTTON_DEBOUNCE_CYCLES 30

// // EXPANSION VHODI
// bool& START_TIPKA = expansion.attachInputBit(26, BUTTON_DEBOUNCE_CYCLES); // I-26
// LongPress START_HOLD(1000); // I-26

// bool& STOP_TIPKA = expansion.attachInputBit(30, BUTTON_DEBOUNCE_CYCLES, INVERTED); // I-30
// LongPress STOP_HOLD(1000); // I-30

#define LETEV_IN_1   16
#define LETEV_IN_2   17
#define LETEV_IN_3   18
#define LETEV_IN_4   19
#define LETEV_IN_5   20
#define LETEV_IN_6   21
#define LETEV_IN_7   22
#define LETEV_IN_8   23
#define LETEV_IN_9   24
#define LETEV_IN_10  25
#define LETEV_IN_11  26
#define LETEV_IN_12  27
#define LETEV_IN_13  28
#define LETEV_IN_14  29
#define LETEV_IN_15  30
#define LETEV_IN_16  31

#define LETEV_OUT_1  16
#define LETEV_OUT_2  17
#define LETEV_OUT_3  18
#define LETEV_OUT_4  19
#define LETEV_OUT_5  20
#define LETEV_OUT_6  21
#define LETEV_OUT_7  22
#define LETEV_OUT_8  23
#define LETEV_OUT_9  24
#define LETEV_OUT_10 25
#define LETEV_OUT_11 26
#define LETEV_OUT_12 27
#define LETEV_OUT_13 28
#define LETEV_OUT_14 29
#define LETEV_OUT_15 30
#define LETEV_OUT_16 31

// INPUTS
bool& S1 = expansion.attachInputBit(LETEV_IN_1, BUTTON_DEBOUNCE_CYCLES);          // LSR32IO input LETEV_IN_1 (16)
bool& S2 = expansion.attachInputBit(LETEV_IN_2, BUTTON_DEBOUNCE_CYCLES);          // LSR32IO input LETEV_IN_2 (17)
bool& S3 = expansion.attachInputBit(LETEV_IN_3, BUTTON_DEBOUNCE_CYCLES);          // LSR32IO input LETEV_IN_3 (18)

bool& SW_A_STOP = expansion.attachInputBit(0, BUTTON_DEBOUNCE_CYCLES, INVERTED);   // LSR32IO input 0
bool& SW_A_START = expansion.attachInputBit(1, BUTTON_DEBOUNCE_CYCLES);  // LSR32IO input 1
bool& SW_M_AUTO = expansion.attachInputBit(2, BUTTON_DEBOUNCE_CYCLES);   // LSR32IO input 2
bool& SW_M_ROCNO = expansion.attachInputBit(3, BUTTON_DEBOUNCE_CYCLES);  // LSR32IO input 3
bool& SW_1_M1 = expansion.attachInputBit(4, BUTTON_DEBOUNCE_CYCLES);     // LSR32IO input 4
bool& SW_2_C = expansion.attachInputBit(5, BUTTON_DEBOUNCE_CYCLES);      // LSR32IO input 5
bool& SW_3_M2 = expansion.attachInputBit(6, BUTTON_DEBOUNCE_CYCLES);     // LSR32IO input 6
bool& SW_4_M3 = expansion.attachInputBit(7, BUTTON_DEBOUNCE_CYCLES);     // LSR32IO input 7

// OUTPUTS
bool& OUT_HOLD = expansion.attachOutputBit(0);        // LSR32IO output 0
bool& MOTOR_1 = expansion.attachOutputBit(1);         // LSR32IO output 1
bool& MOTOR_2 = expansion.attachOutputBit(2);         // LSR32IO output 2
bool& MOTOR_3 = expansion.attachOutputBit(3);         // LSR32IO output 3
 
bool& LED_AUTO = expansion.attachOutputBit(8);        // LSR32IO output 8
bool& LED_1_M1 = expansion.attachOutputBit(9);        // LSR32IO output 9
bool& LED_2_C = expansion.attachOutputBit(10);        // LSR32IO output 10
bool& LED_3_M2 = expansion.attachOutputBit(11);       // LSR32IO output 11
bool& LED_4_M3 = expansion.attachOutputBit(12);       // LSR32IO output 12

bool& SEMAFOR_RDECA = expansion.attachOutputBit(4);   // LSR32IO output 4
bool& SEMAFOR_ZELENA = expansion.attachOutputBit(5);  // LSR32IO output 5

bool& CILINDER_1Y2 = expansion.attachOutputBit(LETEV_OUT_1);   // LSR32IO output LETEV_OUT_1 (16)
bool& CILINDER_2Y2 = expansion.attachOutputBit(LETEV_OUT_2);   // LSR32IO output LETEV_OUT_2 (17)
bool& CILINDER_1Y1 = expansion.attachOutputBit(LETEV_OUT_3);   // LSR32IO output LETEV_OUT_3 (18)
bool& CILINDER_2Y1 = expansion.attachOutputBit(LETEV_OUT_4);   // LSR32IO output LETEV_OUT_4 (19)
bool& IZPIH = expansion.attachOutputBit(LETEV_OUT_5);          // LSR32IO output LETEV_OUT_5 (20)

bool& RI_WORKING = expansion.attachInputBit(LETEV_IN_16, BUTTON_DEBOUNCE_CYCLES, INVERTED);         // Kuka digital signal - Robot je v delu
bool& RO_D1 = expansion.attachOutputBit(LETEV_OUT_11);         // Kuka digital signal 1 - Pozicija deske
bool& RO_D2 = expansion.attachOutputBit(LETEV_OUT_12);         // Kuka digital signal 2 - Pozicija deske
bool& RO_D3 = expansion.attachOutputBit(LETEV_OUT_13);         // Kuka digital signal 3 - Pozicija deske
bool& RO_D4 = expansion.attachOutputBit(LETEV_OUT_14);         // Kuka digital signal 4 - Pozicija deske
bool& RO_D5 = expansion.attachOutputBit(LETEV_OUT_15);         // Kuka digital signal 5 - Pozicija deske
bool& RO_AVAILABLE = expansion.attachOutputBit(LETEV_OUT_16);         // Kuka digital signal - Robot ima desko na voljo

bool P1 = false;  // Deska na vhodnem senzorju za merjenje razdalje
bool P2 = true;   // Deska na mestu za premik na trak
bool P3 = true;   // Deska na vhodu traka
bool P4 = true;   // Deska na spodnjem traku
bool P5 = false;  // Deska na izhodnem senzorju za merjenje razdalje
bool P6 = false;  // Deska pripravljena za robota

bool AUTO = false;   // Mode AUTO
bool ROCNO = false;  // Mode ROCNO
bool SERVIS = false; // Mode SERVIS
bool STOP = true;

bool SERVIS_M1 = false; // SW_1_M1 toggles this state       
bool SERVIS_C = false;  // SW_2_C  toggles this state     
bool SERVIS_M2 = false; // SW_3_M2 toggles this state       
bool SERVIS_M3 = false; // SW_4_M3 toggles this state       

#define STEVILO_CIKLOV_ZA_IZPIH 30

bool deske_ni = false;
bool deska_kratka = false;

#define DVOJNI_ZAGON  true

#define DD_TOLERANCA 30 // DOdatni pogresek pri kratkih deskah
#define DD_MIN 750
#define DD_DOLGA 1200

#define DD_KOREKCIJA_DOLZINE_2 -5.0

uint32_t cycle_count = 0;

double cycle_time = 0;
double last_idle_time = 0;
double idle_sec = 0;

double spool_hold_time = 1.0;

struct StanjeZalogovnika {
    bool pozicija[3] = { 1 };
    int st_desk = 3;
    void reset() {
        pozicija[0] = 1;
        pozicija[1] = 1;
        pozicija[2] = 1;
        st_desk = 3;
        P3 = true;
    }
    void push() {
        P3 = true;
        pozicija[0] = 1;
        st_desk = pozicija[0] + pozicija[1] + pozicija[2];
    }
    bool move() {
        bool output = pozicija[2];
        pozicija[2] = pozicija[1];
        pozicija[1] = pozicija[0];
        pozicija[0] = 0;
        P3 = false;
        st_desk = pozicija[0] + pozicija[1] + pozicija[2];
        return output;
    }
    bool safeToMoveOne() {
        return pozicija[0] == 1 && pozicija[2] == 0;
    }
} stanjeZalogovnika;

bool C1_zadaj = true;
bool C2_zadaj = true;

void C1(bool state) {
    CILINDER_1Y1 = state;
    CILINDER_1Y2 = !state;
    C1_zadaj = !state;
}
void C2(bool state) {
    CILINDER_2Y1 = state;
    CILINDER_2Y2 = !state;
    C2_zadaj = !state;
}

float timeToDistance_OUT(float time) {
    return time * 1352.0;
}

// Plank positions (index) and standard plank sizes (length):
// 1 = 1720
// 2 = 1970
// 3 = /

// 4 = 770
// 5 = 820
// 6 = 870

// 7 = 920
// 8 = 970
// 9 = 1020

// 10 = 1070
// 11 = 1120
// 12 = 1170

// 13 = 1220
// 14 = 1270
// 15 = 1320

// 16 = 1370
// 17 = 1420
// 18 = 1520

// 19 = 620
// 20 = 670
// 21 = 720

// 22 = 2120
// 23 = 2220
// 24 = 2320

// 25 = 2420
// 26 = 2620
// 27 = 3000


struct plank_t {
    int index = 0;
    float length = 0;
};

plank_t plank_sizes [] = {
    // { 19, 620 },
    // { 20, 670 },
    // { 21, 720 },
    { 4, 770 },
    { 5, 820 },
    { 6, 870 },
    { 7, 920 },
    { 8, 970 },
    { 9, 1020 },
    { 10, 1070 },
    { 11, 1120 },
    { 12, 1170 },
    { 13, 1220 },
    { 14, 1270 },
    { 15, 1320 },
    { 16, 1370 },
    { 17, 1420 },
    { 18, 1520 },
    { 1, 1720 },
    { 2, 1970 },
    { 22, 2120 },
    { 23, 2220 },
    { 24, 2320 },
    { 25, 2420 },
    { 26, 2620 },
    { 27, 3000 }
};

int num_of_planks = sizeof(plank_sizes) / sizeof(plank_sizes[0]);

// Get plank position based on plank size.
// Take into account the tolerance to determine which plank position is the closest based on the sorted plank size list.

int plankPositionFromLength(float length) {
    for (int i = 0; i < num_of_planks - 1; i++) {
        int index = plank_sizes[i].index;
        float L1 = plank_sizes[i].length;
        float L2 = plank_sizes[i + 1].length;
        float LM = L1 + (L2 - L1) * 0.5;
        if (length < LM) return index;
    }
    return plank_sizes[num_of_planks - 1].index;
}

void setPlankPosition(int plank_position) {
    RO_D1 = (plank_position & 0b00001) > 0;
    RO_D2 = (plank_position & 0b00010) > 0;
    RO_D3 = (plank_position & 0b00100) > 0;
    RO_D4 = (plank_position & 0b01000) > 0;
    RO_D5 = (plank_position & 0b10000) > 0;
}

int podajRobotuDesko(float length) {
    int plank_position = plankPositionFromLength(length);
    setPlankPosition(plank_position);
    RO_AVAILABLE = true;
    return plank_position;
}

void resetRobotStanja() {
    P6 = false;
    RO_D1 = false;
    RO_D2 = false;
    RO_D3 = false;
    RO_D4 = false;
    RO_D5 = false;
    RO_AVAILABLE = false;
}
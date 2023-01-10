#pragma once
#include "../config.h"

struct merjenje_vhodno_t: _vovk_plc_block_t {
    double time_zacetek = 0;
    double time_konec = 0;
    double dolzina = 0;

    void izhodisce() {
        if (running) Serial.printf("[merjenje_vhodno] Konec\n");
        running = false;
        finished = false;
        safe = true;
        MOTOR_1 = false;
        IZPIH = false;
        flow.reset();
    }

    void setup() {
        enabled = 1;
        izhodisce();
    }

    void run() {
        // if (!running && AUTO) {
        if (!running) {
            Serial.printf("[merjenje_vhodno] Zagon\n");
            safe = false;
            running = true;
            finished = false;
            flow.reset();
        } else {
            if (ROCNO) P2 = true;
        }
    }
    void stop() {
        if (running) {
            izhodisce();
        }
    }

    enum {
        FAZA_0_PRICAKAJ_POGOJE = 0,
        FAZA_1_M1_ON,
        FAZA_2_AWAIT_P1_OFF_IF_P1_ON,
        FAZA_3_AWAIT_P1_ON,
        FAZA_4_AWAIT_P1_OFF,
        FAZA_5_AWAIT_POMIK_DO_POZICIJE,
        FAZA_6_M1_OFF,
        FAZA_7_STATUS_UPDATE,
    };

    void loop() {
        if (!enabled && running) {  // ignore disabled blocks
            izhodisce();
            finished = true;
            safe = true;
        }
        if (!enabled) return;

        bool on = AUTO || ROCNO;
        bool work = running && on;
        P1 = S1;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    if (on && C1_zadaj && C2_zadaj && !P2) {
                        last_idle_time = t;
                        safe = false;
                        flow.next();
                    }
                    break;
                }
                case FAZA_1_M1_ON: {
                    MOTOR_1 = true;
                    if (P1) flow.goTo(FAZA_2_AWAIT_P1_OFF_IF_P1_ON);
                    else flow.goTo(FAZA_3_AWAIT_P1_ON);
                    break;
                }
                case FAZA_2_AWAIT_P1_OFF_IF_P1_ON: {
                    MOTOR_1 = true;
                    if (!P1) flow.next();
                    break;
                }

                case FAZA_3_AWAIT_P1_ON: {
                    MOTOR_1 = true;
                    if (P1) {
                        time_zacetek = t;
                        Serial.printf("[merjenje_vhodno] zacetek merjenja...\n");
                        flow.next();
                    }
                    break;
                }
                case FAZA_4_AWAIT_P1_OFF: {
                    MOTOR_1 = true;
                    if (flow.phaseSetup()) timer.set(3000);
                    if (!P1) {
                        time_konec = t;
                        dolzina = timeToDistance_OUT(time_konec - time_zacetek);
                        const char* tip_deske = dolzina < (DD_MIN - DD_TOLERANCA) ? "IZMET" : (dolzina >= DD_DOLGA ? "DOLGA" : "KRATKA");
                        Serial.printf("[merjenje_vhodno] vhodna dolzina: %2.3fs %s\n", dolzina, tip_deske);
                        if (dolzina >= DD_MIN) flow.next();
                        else flow.goTo(FAZA_3_AWAIT_P1_ON);
                    } else if (timer.finished()) flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                    break;
                }
                case FAZA_5_AWAIT_POMIK_DO_POZICIJE: {
                    MOTOR_1 = true;
                    // if (flow.phaseSetup()) timer.set(40);
                    // if (timer.finished()) flow.next();
                    flow.next();
                    break;
                }
                case FAZA_6_M1_OFF: {
                    if (flow.phaseSetup()) {
                        if (cycle_count % STEVILO_CIKLOV_ZA_IZPIH == 0) IZPIH = true;
                        timer.set(200);
                    }
                    MOTOR_1 = false;
                    if (timer.finished()) {
                        IZPIH = false;
                        flow.next();
                    }
                    break;
                }

                case FAZA_7_STATUS_UPDATE: {
                    // last_idle_time = t;
                    IZPIH = false;
                    deska_kratka = dolzina < DD_DOLGA;
                    P2 = true;
                    flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                    break;
                }
                default: {
                    izhodisce();
                    break;
                }
            }
        } else {
            izhodisce();
        }
    }
};

merjenje_vhodno_t merjenje_vhodno;
#pragma once
#include "../config.h"

struct zalogovnik_t : _vovk_plc_block_t {
    // // EXPANSION IZHODI
    // bool& ZASUK = expansion.attachOutputBit(6);         // Q-6
    // bool& BREMZA = expansion.attachOutputBit(7);        // Q-7

    // // EXPANSION VHODI
    // bool& NA_POZICIJI = expansion.attachInputBit(0);    // I-0

    void izhodisce() {
        if (running) Serial.printf("[zalogovnik] Konec\n");
        running = false;
        finished = false;
        stopping = false;
        safe = true;

        stanjeZalogovnika.reset();
        MOTOR_2 = false;
        flow.reset();
    }

    void setup() {
        enabled = 1;
        izhodisce();
    }

    Timeout t_zakasnitev;
    Timeout t_timeout;

    void run() {
        if (!running) {
            Serial.printf("[zalogovnik] Zagon\n");
            running = true;
            finished = false;
            flow.reset();
        }
    }
    void stop_soft() {
        if (running) {
            stopping = true;
        }
    }
    void stop_hard() {
        if (running) {
            izhodisce();
        }
    }

    enum {
        FAZA_0_PRICAKAJ_POGOJE = 0,
        FAZA_1_M2_ON,
        FAZA_2_AWAIT_S2_OFF,
        FAZA_3_AWAIT_S2_ON,
        FAZA_4_CHECK,
        FAZA_5_M2_OFF_AWAIT_DA_PADE,
    };

    void loop() {
        if (!enabled && running) {  // ignore disabled blocks
            izhodisce();
            finished = true;
            safe = true;
        }
        if (!enabled) return;

        bool on = AUTO || ROCNO || stopping;
        bool work = running && on;
        bool push_is_safe = (!P4 && stanjeZalogovnika.st_desk > 0) || stanjeZalogovnika.safeToMoveOne();
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    if (on && !stopping && !P1 && !MOTOR_3 && push_is_safe && C1_zadaj && C2_zadaj) {
                        Serial.printf("[zalogovnik] Pomik\n");
                        safe = false;
                        finished = false;
                        flow.next();
                    }
                    break;
                }
                case FAZA_1_M2_ON: {
                    MOTOR_2 = true;
                    if (!S2) flow.goTo(FAZA_3_AWAIT_S2_ON);
                    else flow.next();
                    break;
                }
                case FAZA_2_AWAIT_S2_OFF: {
                    if (flow.phaseSetup()) {
                        t_zakasnitev.set(20);
                        t_timeout.set(1000);
                    }
                    if (t_zakasnitev.finished()) {
                        if (!S2) {
                            errors_count[flow.phase] = 0;
                            flow.next();
                        } else if (t_timeout.finished()) {
                            errors_count[flow.phase]++;
                            flow.next();
                        }
                    }
                    break;
                }
                case FAZA_3_AWAIT_S2_ON: {
                    if (flow.phaseSetup()) {
                        t_zakasnitev.set(20);
                        t_timeout.set(1000);
                    }
                    if (t_zakasnitev.finished()) {
                        if (S2) {
                            errors_count[flow.phase] = 0;
                            flow.next();
                        } else if (t_timeout.finished()) {
                            errors_count[flow.phase]++;
                            flow.next();
                        }
                    }
                    break;
                }
                case FAZA_4_CHECK: {
                    bool single_move = P4 && stanjeZalogovnika.safeToMoveOne();
                    bool prisotna_deska = stanjeZalogovnika.move();
                    if (prisotna_deska) {
                        P4 = true;
                        // if (P2) cilindri.run();
                        flow.next();
                    } else {
                        if (single_move) {
                            MOTOR_2 = false;
                            safe = true;
                            finished = true;
                        }
                        flow.goTo(FAZA_0_PRICAKAJ_POGOJE); // Podaj naslednjo desko
                    }
                    break;
                }
                case FAZA_5_M2_OFF_AWAIT_DA_PADE: {
                    MOTOR_2 = false;
                    if (flow.phaseSetup()) timer.set(800);
                    if (timer.finished()) {
                        safe = true;
                        finished = true;
                        // if (AUTO)
                        flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                        // else izhodisce();
                    }
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

zalogovnik_t zalogovnik;
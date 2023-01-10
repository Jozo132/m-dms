#pragma once
#include "../config.h"
#include "zalogovnik.h"

struct merjenje_izhodno_t: _vovk_plc_block_t {
    double time_zacetek = 0;
    double time_konec = 0;
    double trajanje = 0;
    double dolzina = 0;

    Timeout t_delay;
    void izhodisce() {
        if (running) Serial.printf("[merjenje_izhodno] Konec\n");
        running = false;
        finished = false;
        safe = true;
        MOTOR_3 = false;
        flow.reset();
    }

    void setup() {
        enabled = 1;
        izhodisce();
    }

    void run() {
        if (!running) {
            Serial.printf("[merjenje_izhodno] Zagon\n");
            safe = false;
            running = true;
            finished = false;
            flow.reset();
        }
    }
    void stop() {
        if (running) {
            izhodisce();
        }
    }

    enum {
        FAZA_0_PRICAKAJ_POGOJE = 0,
        FAZA_1_M3_ON_1,
        FAZA_1_M3_OFF_2,
        FAZA_1_M3_ON_3,
        FAZA_2_AWAIT_P5_ON,
        FAZA_3_AWAIT_P5_OFF,
        FAZA_4_AWAIT_POMIK_DO_POZICIJE,
        FAZA_5_M3_OFF,
        FAZA_6_STATUS_UPDATE,
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
        P5 = S3;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    if (on && zalogovnik.safe && P4 && !P5 && !P6) {
                        last_idle_time = t;
                        safe = false;
                        if (AUTO) flow.next();
                        if (ROCNO) {
                            safe = true;
                            P4 = false;
                            flow.goTo(FAZA_5_M3_OFF);
                        }
                    }
                    break;
                }
                case FAZA_1_M3_ON_1: {
                    if (flow.phaseSetup()) timer.set(80);
                    MOTOR_3 = true;
                    if (!DVOJNI_ZAGON) flow.goTo(FAZA_1_M3_ON_3);
                    if (timer.finished()) flow.next();
                    break;
                }
                case FAZA_1_M3_OFF_2: {
                    if (flow.phaseSetup()) timer.set(160);
                    MOTOR_3 = false;
                    if (timer.finished()) flow.next();
                    break;
                }
                case FAZA_1_M3_ON_3: {
                    MOTOR_3 = true;
                    flow.next();
                    break;
                }
                case FAZA_2_AWAIT_P5_ON: {
                    if (flow.phaseSetup()) timer.set(500);
                    bool failed = !P5 && timer.finished();
                    if (P5 || failed) {
                        safe = true;
                        P4 = false;
                        if (failed) {
                            errors_count[flow.phase]++;
                            MOTOR_3 = false;
                            if (errors_count[flow.phase] < 5) zalogovnik.run();
                            else errors_count[flow.phase] = 0;
                            flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                        } else {
                            errors_count[flow.phase] = 0;
                            flow.next();
                            time_zacetek = t;
                            Serial.printf("[merjenje_izhodno] zacetek merjenja...\n");
                        }
                    }
                    break;
                }
                case FAZA_3_AWAIT_P5_OFF: {
                    if (flow.phaseSetup()) {
                        timer.set(5000);
                        t_delay.set(100);
                    }
                    if (t_delay.finished()) {
                        if (!P5) {
                            time_konec = t;
                            trajanje = time_konec - time_zacetek;
                            dolzina = timeToDistance_OUT(trajanje) + DD_KOREKCIJA_DOLZINE_2;
                            if (dolzina < 200) {
                                errors_count[flow.phase]++;
                                MOTOR_3 = false;
                                safe = true;
                                flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                                break;
                            }
                            Serial.printf("[merjenje_izhodno] koncna dolzina: %2.0fmm [%5.3fsec]\n", dolzina, trajanje);
                            P6 = true;
                            errors_count[flow.phase] = 0;
                            flow.next();
                        } else if (timer.finished()) {
                            errors_count[flow.phase]++;
                            MOTOR_3 = false;
                            safe = true;
                            flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                        }
                    }
                    break;
                }
                case FAZA_4_AWAIT_POMIK_DO_POZICIJE: {
                    if (flow.phaseSetup()) timer.set(5000);
                    if (timer.finished() || time_zacetek > t || (t >= (time_zacetek + 2.7))) flow.next();
                    // flow.next();
                    break;
                }
                case FAZA_5_M3_OFF: {
                    MOTOR_3 = false;
                    flow.next();
                    break;
                }

                case FAZA_6_STATUS_UPDATE: {
                    // last_idle_time = t;
                    safe = true;
                    finished = true;
                    // if (AUTO)
                    flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                    // else izhodisce();
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

merjenje_izhodno_t merjenje_izhodno;
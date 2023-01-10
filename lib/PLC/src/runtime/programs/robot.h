#pragma once
#include "../config.h"
#include "merjenje_izhodno.h"

struct robot_t: _vovk_plc_block_t {
    void izhodisce() {
        if (running) Serial.printf("[robot] Konec\n");
        resetRobotStanja();
        running = false;
        finished = false;
        safe = true;
        flow.reset();
    }

    void setup() {
        enabled = 1;
        izhodisce();
    }

    void run() {
        if (!running) {
            Serial.printf("[robot] Zagon\n");
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
        FAZA_1_OBVESTI_ROBOTA,
        FAZA_2_POCAKAJ_ROBOTA
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
        if (RI_WORKING) RO_AVAILABLE = false;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    if (on && AUTO && P6 && !MOTOR_3 && !RI_WORKING) {
                        last_idle_time = t;
                        safe = false;
                        flow.next();
                    }
                    break;
                }
                case FAZA_1_OBVESTI_ROBOTA: {
                    if (flow.phaseSetup()) {
                        double dolzina = merjenje_izhodno.dolzina;
                        int pozicija = podajRobotuDesko(dolzina);
                        Serial.printf("[robot] Robot pobira na '%d' desko dolzine %3.1f\n", pozicija, dolzina);
                    }
                    if (RI_WORKING) flow.next();
                    break;
                }
                case FAZA_2_POCAKAJ_ROBOTA: {
                    if (!RI_WORKING) {
                        // last_idle_time = t;
                        P6 = false;
                        resetRobotStanja();
                        Serial.printf("[robot] Robot pobral\n");
                        // if (AUTO) {
                            finished = true;
                            safe = true;
                            flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                        // } else izhodisce();
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

robot_t robot;
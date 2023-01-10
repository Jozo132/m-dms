#pragma once
#include "../config.h"
#include "zalogovnik.h"

struct cilindri_t : _vovk_plc_block_t {
    double c1_cas_pomika = 0;
    double c2_cas_pomika = 0;

    void naprej(int count = 1) {
        c1_cas_pomika = t;
        c2_cas_pomika = t;
        bool a = count >= 1;
        bool b = count >= 2;
        if (a || !C1_zadaj) C1(a);
        if (b || !C2_zadaj) C2(b);
    }

    void nazaj(bool force = false) {
        c1_cas_pomika = t;
        c2_cas_pomika = t;
        if (force || !C1_zadaj) C1(0);
        if (force || !C2_zadaj) C2(0);
    }


    void izhodisce() {
        if (running) {
            Serial.printf("[cilindri] Konec\n");
            nazaj();
        }
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
            Serial.printf("[cilindri] Zagon\n");
            safe = false;
            running = true;
            finished = false;
            flow.reset();
        }
    }
    void stop() {
        if (running) {
            izhodisce();
            nazaj(true);
        }
    }

    enum {
        FAZA_0_PRICAKAJ_POGOJE = 0,
        FAZA_1_AWAIT_POMIK_NAPREJ,
        FAZA_2_AWAIT_POMIK_NAZAJ,
        FAZA_3_STATUS_UPDATE,
    };

    void loop() {
        if (t > (c1_cas_pomika + spool_hold_time)) {
            CILINDER_1Y1 = 0;
            CILINDER_1Y2 = 0;
        }
        if (t > (c2_cas_pomika + spool_hold_time)) {
            CILINDER_2Y1 = 0;
            CILINDER_2Y2 = 0;
        }
        if (!enabled && running) {  // ignore disabled blocks
            izhodisce();
            finished = true;
            safe = true;
        }
        if (!enabled) return;

        bool on = AUTO || ROCNO;
        bool work = running && on;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    if (on && !MOTOR_1 && (P2 || ROCNO) && !P3) {
                        last_idle_time = t;
                        safe = false;
                        if (deska_kratka)
                            naprej(1);
                        else
                            naprej(2);
                        flow.next();
                    }
                    break;
                }
                case FAZA_1_AWAIT_POMIK_NAPREJ: {
                    if (flow.phaseSetup()) timer.set(800);
                    if (timer.finished()) {
                        nazaj();
                        flow.next();
                    }
                    break;
                }
                case FAZA_2_AWAIT_POMIK_NAZAJ: {
                    if (flow.phaseSetup()) timer.set(500);
                    if (timer.finished()) flow.next();
                    break;
                }
                case FAZA_3_STATUS_UPDATE: {
                    P2 = false;
                    stanjeZalogovnika.push();
                    finished = true;
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

cilindri_t cilindri;
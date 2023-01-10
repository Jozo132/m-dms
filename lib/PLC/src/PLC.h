#pragma once

#include "runtime/runtime.h"


Interval P_1s_timer(1000);
Interval P_500ms_timer(500);
Interval P_100ms_timer(100);
bool P_1s = false;
bool P_500ms = false;
bool P_100ms = false;

struct plc_t {
    void izhodisce() {
        cilindri.izhodisce();
        merjenje_izhodno.izhodisce();
        merjenje_vhodno.izhodisce();
        robot.izhodisce();
        zalogovnik.izhodisce();
    }
    void setup() {
        cilindri.setup();
        merjenje_izhodno.setup();
        merjenje_vhodno.setup();
        robot.setup();
        zalogovnik.setup();
        izhodisce();
    }

    void start() {
        cilindri.run();
        merjenje_izhodno.run();
        merjenje_vhodno.run();
        robot.run();
        zalogovnik.run();
    }
    void stop_soft() {
        cilindri.stop();
        merjenje_izhodno.stop();
        merjenje_vhodno.stop();
        robot.stop();
        zalogovnik.stop_soft();
    }
    void stop_hard() {
        stop_soft();
        zalogovnik.stop_hard();
    }

    bool _old_stop = false;
    bool _old_auto = false;
    bool _old_rocno = false;
    bool _old_servis = false;

    bool _old_SW_1_M1 = false;
    bool _old_SW_2_C = false;
    bool _old_SW_3_M2 = false;
    bool _old_SW_4_M3 = false;

    void loop() {
        STOP = SW_A_STOP;
        AUTO = !STOP && SW_M_AUTO && !SW_M_ROCNO;
        ROCNO = !STOP && !SW_M_AUTO && !SW_M_ROCNO;
        SERVIS = !STOP && !SW_M_AUTO && SW_M_ROCNO;

        if (AUTO && _old_auto != AUTO) {
            STOP = true;
            Serial.println("ACTIVATED AUTO");
        }
        if (ROCNO && _old_rocno != ROCNO) {
            STOP = true;
            Serial.println("ACTIVATED ROCNO");
        }
        if (SERVIS && _old_servis != SERVIS) Serial.println("ACTIVATED SERVIS");
        if (STOP && _old_stop != STOP) Serial.println("ACTIVATED STOP");
        if (_old_stop && !STOP) LED_AUTO = OFF;
        _old_stop = STOP;
        _old_auto = AUTO;
        _old_rocno = ROCNO;
        _old_servis = SERVIS;

        if (!SERVIS || STOP) {
            SERVIS_M1 = false;
            SERVIS_C = false;
            SERVIS_M2 = false;
            SERVIS_M3 = false;
        }

        bool all_stopped = !cilindri.running && !merjenje_izhodno.running && !merjenje_vhodno.running && !robot.running && !zalogovnik.running;

        if (all_stopped) LED_AUTO = P_1s ? !LED_AUTO : LED_AUTO;
        else LED_AUTO = ON;

        if (STOP) {
            SEMAFOR_RDECA = true;
            SEMAFOR_ZELENA = false;
            if (SW_A_STOP)
                stop_hard();
            else
                stop_soft();
        } else {  // READY

            if (AUTO || ROCNO) {
                if (SW_A_START) {
                    LED_AUTO = true;
                    SEMAFOR_RDECA = false;
                    SEMAFOR_ZELENA = true;
                    start();
                }
                if (!all_stopped) {
                    idle_sec = t - last_idle_time;
                    if (idle_sec > 5) {
                        SEMAFOR_RDECA = P_500ms ? !SEMAFOR_RDECA : SEMAFOR_RDECA;
                        SEMAFOR_ZELENA = false;
                    } else {
                        SEMAFOR_RDECA = false;
                        SEMAFOR_ZELENA = true;
                    }
                } else {
                    SEMAFOR_RDECA = true;
                    SEMAFOR_ZELENA = false;
                }
            } else  if (SERVIS) {
                SEMAFOR_RDECA = P_1s ? !SEMAFOR_RDECA : SEMAFOR_RDECA;
                SEMAFOR_ZELENA = !SEMAFOR_RDECA;
                if (SW_1_M1 && !_old_SW_1_M1) {
                    SERVIS_M1 = !SERVIS_M1;
                    MOTOR_1 = SERVIS_M1;
                }
                if (SW_2_C && !_old_SW_2_C) {
                    SERVIS_C = !SERVIS_C;
                    if (SERVIS_C) {
                        cilindri.naprej(2);
                    } else {
                        cilindri.nazaj(2);
                    }
                }
                if (SW_3_M2 && !_old_SW_3_M2) {
                    SERVIS_M2 = !SERVIS_M2;
                    if (SERVIS_M2) {
                        zalogovnik.run();
                    }
                }
                if (SW_4_M3 && !_old_SW_4_M3) {
                    SERVIS_M3 = !SERVIS_M3;
                    MOTOR_3 = SERVIS_M3;
                }
            }
            // if (ROCNO && SW_R_CYCLE) start();
        }

        LED_1_M1 = MOTOR_1;
        LED_2_C = !C1_zadaj;
        LED_3_M2 = MOTOR_2;
        LED_4_M3 = MOTOR_3;

        OUT_HOLD = !MOTOR_1;

        _old_SW_1_M1 = SW_1_M1;
        _old_SW_2_C = SW_2_C;
        _old_SW_3_M2 = SW_3_M2;
        _old_SW_4_M3 = SW_4_M3;

        if (!SERVIS) {
            cilindri.loop();
            merjenje_izhodno.loop();
            merjenje_vhodno.loop();
            robot.loop();
        }
        zalogovnik.loop();
    }
};

plc_t plc;
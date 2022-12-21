#pragma once

#include "runtime/runtime.h"


Interval P_1s_timer(1000);
Interval P_100ms_timer(100);
bool P_1s = false;
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

    void loop() {
        STOP = (SW_M_AUTO == SW_M_ROCNO) || SW_A_STOP;
        AUTO = !STOP && SW_M_AUTO && !SW_M_ROCNO;
        ROCNO = !STOP && SW_M_ROCNO && !SW_M_AUTO;

        if (STOP && _old_stop != STOP) Serial.println("ACTIVATED STOP");
        if (AUTO && _old_auto != AUTO) Serial.println("ACTIVATED AUTO");
        if (ROCNO && _old_rocno != ROCNO) Serial.println("ACTIVATED ROCNO");
        if (_old_stop && !STOP) LED_AUTO = OFF;
        _old_stop = STOP;
        _old_auto = AUTO;
        _old_rocno = ROCNO;

        bool all_stopped = !cilindri.running && !merjenje_izhodno.running && !merjenje_vhodno.running && !robot.running && !zalogovnik.running;

        if (all_stopped) LED_AUTO = P_1s ? !LED_AUTO : LED_AUTO;
        else LED_AUTO = ON;

        if (STOP) {
            if (SW_A_STOP)
                stop_hard();
            else
                stop_soft();
        } else {  // READY
            if ((AUTO || ROCNO) && SW_A_START) {
                LED_AUTO = true;
                start();
            }
            // if (ROCNO && SW_R_CYCLE) start();
        }

        LED_1_M1 = MOTOR_1;
        LED_2_C = !C1_zadaj;
        LED_3_M2 = MOTOR_2;
        LED_4_M3 = MOTOR_3;

        OUT_HOLD = !MOTOR_1;

        cilindri.loop();
        merjenje_izhodno.loop();
        merjenje_vhodno.loop();
        robot.loop();
        zalogovnik.loop();
    }
};

plc_t plc;
#include <PLC.h>

void setup() {
    IWatchdog.begin(10000);
    IWatchdog.reload();
    Serial.begin(115200);
    LED = OFF;
    AutoPins.loop();
    for (int i = 0; i < 5; i++) {
        LED = ON;
        AutoPins.loop();
        smartDelay(50);
        LED = OFF;
        AutoPins.loop();
        smartDelay(50);
    }
    expansion.begin();
    // thread_setup(1000);
    smartDelay(1000);
    IWatchdog.reload();
    smartDelay(1000);
    plc.setup();
    validateTime();
    P_1s_timer.reset();
    P_100ms_timer.reset();
}


void log_bytes(const char* name, const byte* bytes, int len, bool reverse = false) {
    Serial.printf("[%d] %s: ", cycle_count, name);
    for (int i = 0; i < len; i++) {
        if (reverse) for (int b = 7; b >= 0; b--) Serial.print((bytes[i] >> b) & 1);
        else for (int b = 0; b < 8; b++) Serial.print((bytes[i] >> b) & 1);
        Serial.print(" ");
    }
    Serial.println();
}
int sec_count = 0;
void loop() {
    cycle_count++;
    validateTime();
    P_1s = P_1s_timer.check();
    P_100ms = P_100ms_timer.check();
    AutoPins.loop();
    expansion.loop();
    if (P_1s) sec_count++;
    if (P_100ms) LED = !LED;
    if (P_1s && sec_count %10 == 0) {
        // Serial.print('\n');
        // byte positions = 0;
        // positions = (positions << 1) | P1;
        // positions = (positions << 1) | P2;
        // positions = (positions << 1) | P3;
        // positions = (positions << 1) | P4;
        // positions = (positions << 1) | P5;
        // positions = (positions << 1) | P6;
        // positions = (positions << 1) | 0;
        // positions = (positions << 1) | 0;
        // log_bytes("positions", &positions, 1, true);





        Serial.print("positions: ");
        Serial.print(P1 ? "P1 " : "   ");
        Serial.print(P2 ? "P2 " : "   ");
        Serial.print(P3 ? "P3 " : "   ");
        Serial.print(P4 ? "P4 " : "   ");
        Serial.print(P5 ? "P5 " : "   ");
        Serial.print(P6 ? "P6 " : "   ");
        Serial.println();

        Serial.print("states: ");
        // Serial.print(merjenje_vhodno.running ? "merjenje_vhodno " : "             ");
        Serial.printf("%s[%d] ", merjenje_vhodno.running ? "merjenje_vhodno " : "             ", merjenje_vhodno.flow.phase);
        // Serial.print(cilindri.running ? "cilindri " : "             ");
        Serial.printf("%s[%d] ", cilindri.running ? "cilindri " : "             ", cilindri.flow.phase);
        // Serial.print(zalogovnik.running ? "zalogovnik " : "             ");
        Serial.printf("%s[%d] ", zalogovnik.running ? "zalogovnik " : "             ", zalogovnik.flow.phase);
        // Serial.print(merjenje_izhodno.running ? "merjenje_izhodno " : "             ");
        Serial.printf("%s[%d] ", merjenje_izhodno.running ? "merjenje_izhodno " : "             ", merjenje_izhodno.flow.phase);
        // Serial.print(robot.running ? "robot " : "             ");
        Serial.printf("%s[%d] ", robot.running ? "robot " : "             ", robot.flow.phase);
        Serial.println();




        // int size = expansion.availableBytes();
        // byte* inputs = expansion.readBytes();
        // byte* outputs = expansion.readOutputBytes();
        // log_bytes("LSR32IO inputs", inputs, size);
        // log_bytes("LSR32IO outputs", outputs, size);

    }
    plc.loop();
    AutoPins.loop();
    expansion.loop();
    IWatchdog.reload();
}

// 1kHz Parallel running code
void thread(void) {
}

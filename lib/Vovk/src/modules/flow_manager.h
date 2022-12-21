#pragma once

class FlowManager {
public:
    bool initialized = false;
    int phase = 0;
    FlowManager() {}
    void reset() {
        phase = 0;
        initialized = false;
    }
    void next() {
        phase++;
        initialized = false;
    }
    void goTo(int p) {
        phase = p;
        initialized = false;
    }
    bool phaseSetup() {
        if (initialized) return false;
        initialized = true;
        return true;
    }
};
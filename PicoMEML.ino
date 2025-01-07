#include "src/PicoDefs.hpp"
#include "src/audio/AudioDriver.hpp"
#include "src/audio/AudioApp.hpp"
#include "src/interface/ButtonsPots.hpp"

#include <cstdint>
#include "pico/util/queue.h"

static queue_t queue_audioparam;
static bool flag_init_0 = false;
static bool flag_init_1 = false;
static bool flag_init_serial = false;

void setup() {
    while (!flag_init_serial) {
        // Wait for serial interface to be inited
    };

    // AUDIO routine setup
    if (!AudioDriver_Output::Setup()) {
        Serial.println("setup - I2S init failed!");
    }
    AudioAppSetup();
    // I2S callback is last
    AudioDriver_Output::SetCallback(&AudioAppProcess);
    // Wait for init sync
    flag_init_0 = true;
    while (!flag_init_1) {
        // Wait for other core
    };
}

void AUDIO_FUNC(loop)() {
    uint32_t param;
    queue_remove_blocking(&queue_audioparam, &param);
    Serial.print("loop - Param: ");
    Serial.println(param);
}

void setup1() {
    // Init serial and signal other core
    Serial.begin();
    flag_init_serial = true;

    // Core INTERFACE routine setup
    queue_init(&queue_audioparam, sizeof(uint32_t), 32);
    // GPIO/ADC setup
    ButtonsPots::Setup();

    // Wait for init sync
    flag_init_1 = true;
    while (!flag_init_0) {
        // Wait for other core
    };
}

void loop1() {
    static uint32_t counter = 0;
    if (!queue_try_add(&queue_audioparam, &counter)) {
        Serial.println("ERROR - Queue not ready.");
    }
    counter++;

    // Read ADC
    ButtonsPots::Process();
    delay(1000);
}

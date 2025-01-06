#include "PicoDefs.hpp"
#include "AudioDriver.hpp"

#include <cstdint>
#include "pico/util/queue.h"

static queue_t queue_audioparam;
static bool flag_init_0 = false;
static bool flag_init_1 = false;

void setup() {
    // AUDIO routine setup
    if (!AudioDriver_Output::Setup()) {
        Serial.println("setup - I2S init failed!");
    }
    
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
    Serial.begin();

    // INTERFACE routine setup
    queue_init(&queue_audioparam, sizeof(uint32_t), 32);

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
    delay(1000);
}

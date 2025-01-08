#include "src/PicoDefs.hpp"
#include "src/audio/AudioDriver.hpp"
#include "src/audio/AudioApp.hpp"
#include "src/interface/ButtonsPots.hpp"
#include "src/common/common_defs.h"
#include "src/synth/FMSynth.hpp"
#include "src/interface/MEMLInterface.hpp"
#include "src/interface/mlp_task.hpp"

#include <cstdint>
#include "pico/util/queue.h"

static queue_t queue_audioparam;
static queue_t queue_interface_pulse;
static queue_t queue_interface_midi;
static volatile bool flag_init_0 = false;
static volatile bool flag_init_1 = false;

const bool waitForSerialOnStart = true;

// Global app state
ts_app_state gAppState = {
    0,
    0,
    0,
    app_id_fmsynth,
    0,
    0,
    mode_inference,
    expl_mode_nnweights,
};

// Global objects
MEMLInterface meml_interface(
    &queue_audioparam,
    &queue_interface_pulse,
    &queue_interface_midi,
    &FMSynth::GenParams,
    kN_synthparams
);


void setup() {
    //wait for serial
    if (waitForSerialOnStart){
        while(!Serial) {;}
    }
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

    {
        float audio_params[kN_synthparams];
        if (queue_try_remove(&queue_audioparam, audio_params)) {
            Serial.println("A- Audio params received.");
        }
    }

    {
        float pulse;
        if (queue_try_remove(&queue_interface_pulse, &pulse)) {
            Serial.println("A- Pulse received.");
        }
    }

    {
        ts_midi_note midi;
        if (queue_try_remove(&queue_interface_midi, &midi)) {
            Serial.println("A- MIDI received.");
        }
    }
}


void setup1() {
    // Init serial and signal other core
    if (waitForSerialOnStart){
        while(!Serial) {;}
    }

    // Core INTERFACE routine setup
    queue_init(&queue_audioparam, sizeof(float)*kN_synthparams, 1);
    queue_init(&queue_interface_pulse, sizeof(float), 1);
    queue_init(&queue_interface_midi, sizeof(ts_midi_note), 1);
    // GPIO/ADC setup
    ButtonsPots::Setup(true);
    // MLP setup
    mlp_init(&queue_audioparam, kN_synthparams);

    // Wait for init sync
    flag_init_1 = true;
    while (!flag_init_0) {
        // Wait for other core
    };
}

void loop1() {

    // Read ADC
    ButtonsPots::Process();

    // Pulse
#if 0
    static constexpr uint32_t period_ms = 1;
    static constexpr float pulse_every_s = 1;
    static constexpr float count_wraparound = (1000.f * pulse_every_s)
            / static_cast<float>(period_ms);
    static volatile float counter = 0;
    counter++;
    if (counter >= count_wraparound) {
        counter = 0;
        Serial.println(".");
    }
    delay(period_ms);
#endif
}

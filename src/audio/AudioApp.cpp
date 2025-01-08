#include <Arduino.h>
#include "AudioApp.hpp"
#include "../synth/FMSynth.hpp"
#include "../synth/SineTone.hpp"
#include <vector>


static FMSynth fm_synth_(kSampleRate);
static SineTone sine_tone_(kSampleRate, 200.f);

std::vector<float> fmSynthParams;



void AudioAppSetup(void)
{
    fmSynthParams.resize(kN_synthparams);
    fm_synth_.EnableMIDI(false);
    // FMSynth::GenParams(fmSynthParams);
    // fm_synth_.mapParameters(fmSynthParams);
}

size_t count=0;

stereosample_t AudioAppProcess(stereosample_t y)
{
    // Serial.println("process");
    y.L = fm_synth_.process();
    // y.L = sine_tone_.process();
    y.R = y.L;
    if (count++ % 5000==0) {
        // Serial.println(y.L);
    }

    return y;
}

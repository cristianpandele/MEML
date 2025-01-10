#include <Arduino.h>
#include "AudioApp.hpp"
#include "../PicoDefs.hpp"
#if FM_SYNTH
#include "../synth/FMSynth.hpp"
#elif FX_PROCESSOR
#include "../synth/matrixMix.hpp"
#endif
#include <vector>

#include <cmath>

#if FM_SYNTH
static FMSynth fm_synth_(kSampleRate);
#elif FX_PROCESSOR
static MaxtrixMixApp multi_fx_app_(kSampleRate);
#endif  // FM_SYNTH


void AudioAppSetup(void)
{
#if FM_SYNTH
    fm_synth_.EnableMIDI(false);
#endif
}

size_t count=0;

stereosample_t AudioAppProcess(stereosample_t y)
{
#if FM_SYNTH
    y.L = fm_synth_.process();
    y.R = y.L;
#elif FX_PROCESSOR
    static int counter = 0;
    if (counter >= 48000) {
        Serial.print(y.L);
        Serial.print(" -> ");
    }
    y.L = multi_fx_app_.play(y.L);
    y.R = y.L;
    if (counter++ >= 48000) {
        Serial.println(y.L);
        counter = 0;
    }
#endif // FM_SYNTH

    return y;
}

void AudioAppSetParams(std::vector<float> &params)
{
#if FM_SYNTH
    fm_synth_.mapParameters(params);
#elif FX_PROCESSOR
    multi_fx_app_.mapParameters(params);
#endif
}

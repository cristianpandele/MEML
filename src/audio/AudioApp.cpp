#include <Arduino.h>
#include "AudioApp.hpp"
#include "../synth/FMSynth.hpp"
#include "../synth/SineTone.hpp"
#include <vector>
#include "../PicoDefs.hpp"

#include <cmath>

#if FM_SYNTH
static FMSynth fm_synth_(kSampleRate);
#elif FX_PROCESSOR
static MatrixMixApp multi_fx_(kSampleRate);
#endif  // FM_SYNTH


void AudioAppSetup(void)
{
    fm_synth_.EnableMIDI(false);
}

size_t count=0;

stereosample_t AudioAppProcess(stereosample_t y)
{
#if FM_SYNTH
    y.L = fm_synth_.process();
    y.R = y.L;
#elif FX_PROCESSOR
    y.L = multi_fx_app_.play(y.L);
    y.R = y.L;
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

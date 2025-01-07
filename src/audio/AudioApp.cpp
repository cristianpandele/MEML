#include "AudioApp.hpp"

//#include "../synth/FMSynth.hpp"
#include "../synth/SineTone.hpp"


//static FMSynth fm_synth_(kSampleRate);
static SineTone sine_tone_(kSampleRate, 440.f);


void AudioAppSetup(void)
{
    //fm_synth_.EnableMIDI(false);
}


stereosample_t AudioAppProcess(stereosample_t y)
{
    //y.L = fm_synth_.process();
    y.L = sine_tone_.process();
    y.R = y.L;

    return y;
}

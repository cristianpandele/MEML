#include "AudioApp.hpp"

#include "AudioDriver.hpp"
#include "synth/FMSynth.hpp"


static FMSynth fm_synth_(kSampleRate);


void AudioAppSetup(void)
{
    fm_synth_.EnableMIDI(false);
}

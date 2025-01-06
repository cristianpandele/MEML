#include "AudioDriver.hpp"
#include "PicoDefs.hpp"

#include "control_sgtl5000.h"
#include <I2S.h>

// Create the I2S port using a PIO state machine
I2S AUDIO_MEM i2s(OUTPUT);

// GPIO pin numbers
#define pDOUT 19
#define pBCLK 20
#define pWS (pBCLK+1)
#define pMCLK 22  // optional MCLK pin

const int AUDIO_MEM frequency = 200; // frequency of square wave in Hz
const int AUDIO_MEM sampleRate = kSampleRate; // minimum for many i2s DACs
const int AUDIO_MEM bitsPerSample = 32;
const float AUDIO_MEM amplitude = (1 << (bitsPerSample - 2)) - 1; // amplitude of square wave = 1/2 of maximum
const float AUDIO_MEM neg_amplitude = -(amplitude);

#define MCLK_MUL  256     // depends on audio hardware. Suits common hardware.


AudioControlSGTL5000 codecCtl;

audiocallback_fptr_t audio_callback_;

float _scale_and_saturate(float x) {
    x *= amplitude;
    if (x >= amplitude) {
        x = amplitude;
    } else if (x <= neg_amplitude) {
        x = neg_amplitude;
    }
    return x;
}

void __isr AudioDriver_Output::i2sOutputCallback() {
    // Serial.println(i2s.availableForWrite());
    //512 bytes to fill - where does this get set?

    // Timing start
    auto ts = micros();

    for(size_t i=0;  i < kBufferSize; i++) {

        stereosample_t y { 0 };
        y = audio_callback_(y);

        stereosample_t y_scaled {
            _scale_and_saturate(y.L),
            _scale_and_saturate(y.R),
        };
        i2s.write32(static_cast<int32_t>(y_scaled.L), static_cast<int32_t>(y_scaled.R));
    }

    // Timing end
    auto elapsed = micros() - ts;
    static constexpr float quantumLength = 1.f/
            ((static_cast<float>(kBufferSize)/static_cast<float>(kSampleRate))
            * 1000000.f);
    float dspload = elapsed * quantumLength;
    // Report DSP overload if needed
    static volatile bool dsp_overload = false;
    if (dspload > 0.95 and !dsp_overload) {
        dsp_overload = true;
    } else if (dspload < 0.9) {
        dsp_overload = false;
    }
}

bool AudioDriver_Output::Setup() {

    audio_callback_ = &silence_;

    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, 1);
    // Serial.begin(115200);
    // while (!Serial) {
    //     delay(10);
    // }
    // Serial.println("I2S with MCLK - square wave");

    // set the system clock to a compatible value to the samplerate
    i2s.setSysClk(sampleRate); // best to do this before starting anything clock-dependent



    i2s.setBCLK(pBCLK);
    i2s.setDATA(pDOUT);
    i2s.setMCLK(pMCLK); // must be run before setFrequency() and i2s.begin()
    i2s.setMCLKmult(MCLK_MUL); // also before i2s.begin()
    i2s.setBitsPerSample(bitsPerSample);


    i2s.setBuffers(2, 8); //what does this actually do? doesn't seem to affect buffer size of the callback?

    i2s.onTransmit(i2sOutputCallback);


    // start I2S at the sample rate with 16-bits per sample
    if (!i2s.begin(sampleRate)) {
        return false;
    }


    codecCtl.enable();
    codecCtl.volume(0.5);

    return true;
}


stereosample_t AudioDriver_Output::silence_(stereosample_t x) {
    x.L = 0;
    x.R = 0;

    return x;
}

#include "AudioDriver.hpp"
#include "../PicoDefs.hpp"
#include <Arduino.h> 

#include "control_sgtl5000.h"
#include "i2s_pio/i2s.h"
#include "../synth/maximilian.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"


#define FAST_MEM __not_in_flash("mydata")

const int FAST_MEM sampleRate = 48000; // minimum for many i2s DACs
const int FAST_MEM bitsPerSample = 32;

const float FAST_MEM amplitude = 1 << (bitsPerSample - 2); // amplitude of square wave = 1/2 of maximum
const float FAST_MEM neg_amplitude = -amplitude; // amplitude of square wave = 1/2 of maximum

int32_t FAST_MEM sample = amplitude; // current sample value

AudioControlSGTL5000 codecCtl;

audiocallback_fptr_t audio_callback_;

static __attribute__((aligned(8))) pio_i2s i2s;

// maxiOsc osc, osc2;

// float f1=20, f2=2000;

inline float AUDIO_FUNC(_scale_and_saturate)(float x) {
    x *= amplitude;
    if (x > amplitude) {
        x = amplitude;
    } else if (x < neg_amplitude) {
        x = neg_amplitude;
    }
    return x;
}


static void AUDIO_FUNC(process_audio)(const int32_t* input, int32_t* output, size_t num_frames) {
    // Timing start
    auto ts = micros();

    stereosample_t y { 0 };
    for (size_t i = 0; i < num_frames; i++) {

        y = audio_callback_(y);

        stereosample_t y_scaled {
            _scale_and_saturate(y.L),
            _scale_and_saturate(y.R),
        };
        output[i*2] = static_cast<int32_t>(y_scaled.L);
        output[(i*2) + 1] = static_cast<int32_t>(y_scaled.R);

        // output[i] = input[i];

        // output[i*2] = input[i*2];
        // output[(i*2) + 1] = input[(i*2) + 1];

        // output[i*2] = 0;
        // output[(i*2) + 1] = 0;

        // if (count % halfWavelength == 0) {
        //   // invert the sample every half wavelength count multiple to generate square wave
        //   sample = -1 * sample;
        // }

        // output[i*2] = osc.sinewave(f1) * sample;
        // output[(i*2) + 1] = osc2.sinewave(f2) * sample;
        // // count++;
        // f1 *= 1.00001;
        // f2 *= 1.00001;
        // if (f1 > 15000) {
        //   f1 = 20.0;
        // }
        // if (f2 > 15000) {
        //   f2 = 20.0;
        // }



    }
    // Timing end
    auto elapsed = micros() - ts;
    static constexpr float quantumLength = 1.f/
            ((static_cast<float>(kBufferSize)/static_cast<float>(kSampleRate))
            * 1000000.f);
    float dspload = elapsed * quantumLength;
    // Serial.println(dspload);
    // Report DSP overload if needed
    static volatile bool dsp_overload = false;
    if (dspload > 0.95 and !dsp_overload) {
        dsp_overload = true;
    } else if (dspload < 0.9) {
        dsp_overload = false;
    }
}

static void __isr dma_i2s_in_handler(void) {
    /* We're double buffering using chained TCBs. By checking which buffer the
     * DMA is currently reading from, we can identify which buffer it has just
     * finished reading (the completion of which has triggered this interrupt).
     */
    if (*(int32_t**)dma_hw->ch[i2s.dma_ch_in_ctrl].read_addr == i2s.input_buffer) {
        // It is inputting to the second buffer so we can overwrite the first
        process_audio(i2s.input_buffer, i2s.output_buffer, AUDIO_BUFFER_FRAMES);
    } else {
        // It is currently inputting the first buffer, so we write to the second
        process_audio(&i2s.input_buffer[STEREO_BUFFER_SIZE], &i2s.output_buffer[STEREO_BUFFER_SIZE], AUDIO_BUFFER_FRAMES);
    }
    dma_hw->ints0 = 1u << i2s.dma_ch_in_data;  // clear the IRQ
}


void __isr AudioDriver_Output::i2sOutputCallback() {


    // for(size_t i=0;  i < kBufferSize; i++) {

    //     stereosample_t y { 0 };
    //     y = audio_callback_(y);

    //     stereosample_t y_scaled {
    //         _scale_and_saturate(y.L),
    //         _scale_and_saturate(y.R),
    //     };
    //     i2s.write32(static_cast<int32_t>(y_scaled.L), static_cast<int32_t>(y_scaled.R));
    // }

    // Timing end
    // auto elapsed = micros() - ts;
    // static constexpr float quantumLength = 1.f/
    //         ((static_cast<float>(kBufferSize)/static_cast<float>(kSampleRate))
    //         * 1000000.f);
    // float dspload = elapsed * quantumLength;
    // // Report DSP overload if needed
    // static volatile bool dsp_overload = false;
    // if (dspload > 0.95 and !dsp_overload) {
    //     dsp_overload = true;
    // } else if (dspload < 0.9) {
    //     dsp_overload = false;
    // }
}

bool AudioDriver_Output::Setup() {

    audio_callback_ = &silence_;

    maxiSettings::setup(48000, 2, 64);


    set_sys_clock_khz(132000, true);
    // set_sys_clock_khz(129600, true);
    Serial.printf("System Clock: %lu\n", clock_get_hz(clk_sys));

    i2s_config picoI2SConfig = i2s_config_default;
    picoI2SConfig.fs = sampleRate;
    picoI2SConfig.bit_depth = bitsPerSample;
    picoI2SConfig.sck_mult=256;
    i2s_program_start_synched(pio0, &picoI2SConfig, dma_i2s_in_handler, &i2s);

    // init i2c
    codecCtl.enable();
    codecCtl.volume(0.99);
    codecCtl.inputSelect(AUDIO_INPUT_LINEIN);
    codecCtl.lineInLevel(7);


    return true;
}


stereosample_t AudioDriver_Output::silence_(stereosample_t x) {
    x.L = 0;
    x.R = 0;

    return x;
}

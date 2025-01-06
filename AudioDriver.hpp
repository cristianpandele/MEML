#ifndef __AUDIO_DRIVER_HPP__
#define __AUDIO_DRIVER_HPP__

#include <cstddef>

using audiocallback_fptr_t = float (*)(float);

const size_t kBufferSize = 64;
const size_t kSampleRate = 48000;

class AudioDriver_Output {

 public:
    static bool Setup();
    static void SetCallback(audiocallback_fptr_t callback);

 protected:
    AudioDriver_Output() = delete;

    static audiocallback_fptr_t audio_callback_;
    static void i2sOutputCallback(void);
};


#endif  // __AUDIO_DRIVER_HPP__

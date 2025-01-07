#ifndef __AUDIO_DRIVER_HPP__
#define __AUDIO_DRIVER_HPP__

#include <cstddef>

extern "C" {

const size_t kBufferSize = 64;
const size_t kSampleRate = 48000;

typedef struct {
    float L;
    float R;
} stereosample_t;

using audiocallback_fptr_t = stereosample_t (*)(stereosample_t);

}

extern audiocallback_fptr_t audio_callback_;

class AudioDriver_Output {

 public:

    static bool Setup();
    static inline void SetCallback(audiocallback_fptr_t callback) {
        audio_callback_ = callback;
    }

    AudioDriver_Output() = delete;

    static void i2sOutputCallback(void);
    static stereosample_t silence_(stereosample_t);
};


#endif  // __AUDIO_DRIVER_HPP__

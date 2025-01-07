#ifndef _SINE_TONE_HPP_
#define _SINE_TONE_HPP_

#include <cstdint>


class SineTone {
 public:
    SineTone(float sample_rate, float freq);
    float process();
    int32_t processInt();

 private:
    float sample_rate_;
    float freq_;
    float phase_;
    float w_;

};

#endif  // _SINE_TONE_HPP_
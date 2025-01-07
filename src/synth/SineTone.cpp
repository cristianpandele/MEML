#include "SineTone.hpp"
#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


SineTone::SineTone(float sample_rate, float freq) :
    sample_rate_(sample_rate),
    freq_(freq)
{
    w_ = 2.f * M_PI * freq_ / sample_rate_;
    phase_ = 0;
}

float SineTone::process()
{
    float y = std::sin(phase_);
    phase_ += w_;
    if (phase_ > 2 * M_PI) {
        phase_ -= 2 * M_PI;
    }
    return y;
}

int32_t SineTone::processInt()
{
    static const float scaling = std::pow(2.f, 31.f) - 1000.f;

    return static_cast<int32_t>(process() * scaling);
}

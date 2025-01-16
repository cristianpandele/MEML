#ifndef _COLLAB_HPP
#define _COLLAB_HPP
#include <Arduino.h>

#include <cstdint>

#include "maximilian.h"

#include <vector>
#include <array>
#include <cmath>

#include "OnePoleSmoother.hpp"

#include "../common/common_defs.h"

const size_t kN_synthparams = 5;
using synthparams_array = std::array<float, kN_synthparams>;
static constexpr size_t kN_notes = 4;

class CollabMixer {
public:
    static void GenParams(std::vector<float> &param_vector);
    CollabMixer(float sample_rate);
    float process();
    int32_t processInt();
    void mapParameters(std::vector<float> &params);
    void UpdateParams();

private:
    synthparams_array synthparams;
    synthparams_array synthparams_smoothed;
    OnePoleSmoother<kN_synthparams> smoother_;
    OnePoleSmoother<1> envelope_smoother_;
    float note_freq_;
    float note_amplitude_;
    bool play_note_;
    bool midi_enabled_;
};

#endif  // _COLLAB_HPP
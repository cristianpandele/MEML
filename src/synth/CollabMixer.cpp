#include "CollabMixer.hpp"
#include <cmath>
#include <random>
#include <cstdlib>
#include <vector>

void CollabMixer::GenParams(std::vector<float> &param_vector)
{
#if 0
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> dis(0.f, 1.0f);
#else
    float rand_scale = 1.f / static_cast<float>(RAND_MAX);
#endif
    //printf("Calling CollabMixer::GenParams\n");

    for(size_t i=0; i < kN_synthparams; i++) {
        param_vector[i] = std::rand() * rand_scale;
        //printf(".");
    }
    //printf("\n");
}

void CollabMixer::UpdateParams() {

    
}

CollabMixer::CollabMixer(float sample_rate) :
    smoother_(100.f, sample_rate),
    envelope_smoother_(10.f, sample_rate),
    note_freq_(0),
    note_amplitude_(0),
    play_note_(false),
    midi_enabled_(true)
{
    // std::srand(0);
    maxiSettings::setup(sample_rate, 1, 16);
    UpdateParams();
    std::vector<float> randParams(kN_synthparams);
    GenParams(randParams);

    mapParameters(randParams);
    
}

void CollabMixer::mapParameters(std::vector<float> &params) {
    float *params_ptr = params.data();
    float *dest_ptr = synthparams.data();

    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = (*(params_ptr++) * 100);

    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = (*(params_ptr++) * 100);

    *dest_ptr++ = (*(params_ptr++) * 100);

    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ =  (params[9] * 100);

    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ =  (*(params_ptr++) * 100);

    *dest_ptr++ = (*(params_ptr++) * 100);


}

inline float midiNoteToFrequency(int midiNote) {


#if 1
    return 0.0f;
#else
    return op1.play(carrier_1, 0, 0) * envelope;
#endif
}

int32_t CollabMixer::processInt()
{
    static const float scaling = std::pow(2.f, 31.f) - 1000.f;

    return static_cast<int32_t>(process() * scaling);
}

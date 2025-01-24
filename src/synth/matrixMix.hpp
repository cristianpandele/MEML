#pragma once

//#define NDEBUG
#include <cassert>
#include <vector>
#include <array>
#include <cmath>
#include "maximilian.h"
#include "OnePoleSmoother.hpp"
#include "AudioSample.h"
#include "../audio/AnalysisParams.hpp"
#include "../interface/mlp_task.hpp"


#define PERIODIC_DEBUG(COUNT, FUNC) \
        static size_t ct=0; \
        if (ct++ % COUNT == 0) { \
            FUNC         \
        }

const size_t kN_synthparams = 5;

inline float vox_fasttanh_ultra( const float x )
{
	const float ax = fabs( x );
	const float x2 = x * x;
	const float z = x * ( 0.773062670268356 + ax +
		( 0.757118539838817 + 0.0139332362248817 * x2 * x2 ) *
		x2 * ax );

	return( z / ( 0.795956503022967 + fabs( z )));
}

template<size_t N>
class matrixMixer {
public:
    matrixMixer() {
        //clear matrix
        for (auto &v: mixingMatrix) {
            v = 0.f;
        }
        scale = 1.f/N;
    }

    void randomise_linear(float low, float high) {
        float randMaxInv = 1.f / RAND_MAX;
        for(auto &v: mixingMatrix) {
            v = ((rand() * randMaxInv) * (high-low)) + low;
            v *= scale;
        }
    }

    void set(std::vector<float> &newValues) {
        // assert(newValues.size() < mixingMatrix.size());
        for(size_t i=0; i < mixingMatrix.size(); i++) {
            mixingMatrix[i] = ((newValues[i] * 2.f)-1.f) * scale;
        }
        // PERIODIC_DEBUG(6000,
        //     for(size_t i=0; i < mixingMatrix.size(); i++) {
        //         printf("%f\t", newValues[i]);
        //     }
        //     printf("\n");
        // )
    }

    float calculateMix(const std::array<float,N> &inputs, const size_t outputIndex) {
        assert(inputs.size() == N);
        float sum=0;
        size_t offset = N * outputIndex;
        for(size_t i=0; i < N; i++) {
            float mixValue = inputs[i] * mixingMatrix[offset+i];
            if (i == outputIndex) {
                mixValue *= directFeedbackScale;
            }
            sum += mixValue;

        }
        // PERIODIC_DEBUG(3001,
        //     printf("%u\t %u\t %f\t", outputIndex, mixingMatrix.size(),  sum);
        //     for(size_t i=0; i < N; i++) {
        //         printf("%f\t", mixingMatrix[offset+i]);
        //     };
        //     printf("\n");
        // )
        return sum;
    }

    void setDirectFeedbackScaling(const float scale) {
        directFeedbackScale = scale;
    }

    void scaleWithEigenValues(float alpha) {
        //is this needed?
    }

private:
    std::array<float, N*N> mixingMatrix;
    float directFeedbackScale = 0.1f;
    float scale=1;
    
};

#define NFX 4


class MaxtrixMixApp {
public:
    std::array <maxiSample, 5> sample;
    std::vector<float> mixerOutput;
    static const size_t kNFilters = 5;
    const std::vector<std::vector<float>> desiredGains{
        {1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};
    maxiBiquad filters_[kNFilters];
    maxiRMS rms_[kNFilters];
    static constexpr float filter_cutoffs_[kNFilters] { 70.f, 100.f, 130.f, 180.f, 250.f };

    MaxtrixMixApp(size_t sample_rate) : smoother_(100.f, sample_rate)
    {
        mmix.setDirectFeedbackScaling(0.0);
        unsmoothParams.resize(kN_synthparams);
        params.resize(kN_synthparams);
        // Set sample playback buffers and gains
        for (int i = 0; i < audioSample.size(); i++) {
            //sample.push_back(maxiSample());
            sample[i].setSample(audioSample[i]);
        }
        // Init filters
        for (size_t n = 0; n < kNFilters; n++) {
            filters_[n].set(maxiBiquad::BANDPASS, filter_cutoffs_[n], 0.71f, 1.f);
            rms_[n].setup(60.0f, 50.0f);
        }
    }

    static void GenParams(std::vector<float> &param_vector)
    {
        float rand_scale = 1.f / static_cast<float>(RAND_MAX);

        for(size_t i=0; i < kN_synthparams; i++) {
            param_vector[i] = std::rand() * rand_scale;
        }
        //printf("\n");
    }

    float play(float x) {
        smoother_.Process(unsmoothParams.data(), params.data());

        static constexpr enum PinConfig trainingButtons[]{
            trainClass_0,
            trainClass_1,
            trainClass_2,
            trainClass_3,
            trainClass_4};

        bool inTraining = digitalRead(toggle_Training);

        if(digitalRead(button_Randomise))
        {
          mlp_draw(1.0f);
        }

        // Process filterbank
        std::vector<float> filterbank_out(kNFilters);
        std::vector<float> rms_out(kNFilters);
        for (size_t n = 0; n < kNFilters; n++) {
          filterbank_out[n] = filters_[n].play(x);
          rms_out[n] = rms_[n].play(filterbank_out[n]);
        }

        // if (gAppState.current_nn_mode == mode_training)
        // {
        //     mlp_train();
        //     mlp_inference({joystick_current_.as_struct.potX,
        //                     joystick_current_.as_struct.potY,
        //                     joystick_current_.as_struct.potRotate});
        // }
        // gAppState.current_nn_mode = static_cast<te_nn_mode>(inTraining);
        // digitalWrite(led_Training, gAppState.current_nn_mode);

        if (inTraining)
        {
            // Set the LED
            int8_t trainingIx = -1;
            for (size_t n = 0; n < kNFilters; n++)
            {
                if (digitalRead(trainingButtons[n]))
                {
                    trainingIx = n;
                    break;
                }
            }
            if (trainingIx != -1)
            {
                mlp_add_data_point(rms_out, desiredGains[trainingIx]);
            }
        }

        AnalysisParamsWrite(rms_out);

        x = 0;
        for (int i = 0; i < audioSample.size(); i++) {
            x += sample[i].play()*params[i];
        }
        PERIODIC_DEBUG(96000, for (auto &v : params) { Serial.printf("%f\t", v); } Serial.println();)
        return x;
    }

    void mapParameters(std::vector<float> &newparams) {
        for(size_t i=0; i < params.size(); i++) {
            unsmoothParams[i] = newparams[i];
        }
        filt.set(maxiBiquad::filterTypes::HIGHPASS, unsmoothParams[17] * 5000.f, 2, 1);


        // PERIODIC_DEBUG(6000,
            // printf("1-----\n");
            // union {float f; uint32_t i;} pu;
            // for(size_t i=0; i < newparams.size(); i++) {
            //     pu.f = newparams[i];
                
            //     printf("%x\t", pu.i);
            // }
            // printf("\n");
            // printf("2-----\n");
            // for(size_t i=0; i < params.size(); i++) {
            //     pu.f = params[i];
                
            //     printf("%x\t", pu.i);
            // }
            // printf("\n");
        // )
    }

private:
    maxiNonlinearity distortion;
    maxiDelayline<15000> dl;
    maxiFlanger<15000> flanger;
    maxiOsc sinosc;
    maxiBiquad filt;

    matrixMixer<NFX> mmix;

    std::array<float, NFX> fxInputs;
    std::array<float, NFX> fxOutputs;

    std::vector<float> unsmoothParams, params;
    OnePoleSmoother<kN_synthparams> smoother_;

};


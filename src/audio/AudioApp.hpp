#ifndef __AUDIO_APP_HPP__
#define __AUDIO_APP_HPP__


#include "AudioDriver.hpp"
#include "../PicoDefs.hpp"

#include <vector>

const size_t kAudioApp_NAnalysisParams = 5;

/**
 * @brief App setup routine, called on board power-up.
 * Insert your audio initialisation code here.
 */
void AudioAppSetup(void);
/**
 * @brief Audio app processing routine called by the I2S
 * handler.
 * @param y Input from the codec
 * @return Output to the codec
 */
stereosample_t AUDIO_FUNC(AudioAppProcess)(stereosample_t);
/**
 * @brief Parameter update function controlled by the
 * inter-core queue_audioparam queue. Usually generated by the
 * MEML neural network, unless manually passed elsewhere.
 * @param params Vector of params (of length kN_synthparams)
 */
void AudioAppSetParams(std::vector<float> &params);

#endif  // __AUDIO_APP_HPP__
#ifndef __AUDIO_APP_HPP__
#define __AUDIO_APP_HPP__


#include "AudioDriver.hpp"
#include "../PicoDefs.hpp"

#include <vector>

void AudioAppSetup(void);
stereosample_t AUDIO_FUNC(AudioAppProcess)(stereosample_t);
void AUDIO_FUNC(AudioAppSetParams)(std::vector<float> &params);

#endif  // __AUDIO_APP_HPP__
#ifndef __AUDIO_APP_HPP__
#define __AUDIO_APP_HPP__


#include "AudioDriver.hpp"

#include <vector>

void AudioAppSetup(void);
stereosample_t AudioAppProcess(stereosample_t);
void AudioAppSetParams(std::vector<float> &params);

#endif  // __AUDIO_APP_HPP__
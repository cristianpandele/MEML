#ifndef __PICO_DEFS_HPP__
#define __PICO_DEFS_HPP__

#include "common/common_defs.h"

#define AUDIO_FUNC(x)    __not_in_flash_func(x)
#define AUDIO_MEM    __not_in_flash("audio")

enum PinConfig {
    toggle_SaveData = 13,
    button_Randomise = 14,
    toggle_Training = 15,
    button_ClearData = 16,
    i2s_pDOUT = 19,
    i2s_pBCLK = 20,
    i2s_pWS = 21,
    i2s_pMCLK = 22,
    pot_JoystickX = 26,
    pot_JoystickY = 27,
    pot_JoystickZ = 28
};


extern ts_app_state gAppState;


#endif  // __PICO_DEFS_HPP__

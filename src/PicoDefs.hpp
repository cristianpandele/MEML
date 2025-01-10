#ifndef __PICO_DEFS_HPP__
#define __PICO_DEFS_HPP__

#include "common/common_defs.h"
#include "interface/MEMLInterface.hpp"


// Select which example app to run
#define FM_SYNTH         0
#define FX_PROCESSOR     1


#define AUDIO_FUNC(x)    __not_in_flash_func(x)
#define AUDIO_MEM    __not_in_flash("audio")

enum PinConfig {
    i2c_sgt5000Data = 0,
    i2c_sgt5000Clk = 1,
    led_Training = 2,
    uart_MidiTX = 4,
    uart_MidiRX = 5,
    i2s_pDIN = 6,
    i2s_pDOUT = 7,
    i2s_pBCLK = 8,
    i2s_pWS = 9,
    i2s_pMCLK = 10,
    toggle_SaveData = 13,
    button_Randomise = 14,
    toggle_Training = 15,
    button_ClearData = 16,
    led_MIDI = 21,
    button_ZoomOut = 18,
    button_ZoomIn = 19,
    pot_JoystickX = 26,
    pot_JoystickY = 27,
    pot_JoystickZ = 28
};


// Global objects
extern ts_app_state gAppState;
extern MEMLInterface meml_interface;


#endif  // __PICO_DEFS_HPP__

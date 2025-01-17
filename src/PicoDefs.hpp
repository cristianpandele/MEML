#ifndef __PICO_DEFS_HPP__
#define __PICO_DEFS_HPP__

#include "common/common_defs.h"
#include "interface/MEMLInterface.hpp"

// Select which example app to run
#define FM_SYNTH         0  ///< FM Synth (new macro)
#define FX_PROCESSOR     1  ///< FX Processor (new macro)
#define EUCLIDEAN        0
#define COLLAB_MIXER     0


#define AUDIO_FUNC(x)    __not_in_flash_func(x)  ///< Macro to make audio function load from mem
#define AUDIO_MEM    __not_in_flash("audio")  ///< Macro to make variable load from mem


/**
 * @brief Pin configuration on the Pi Pico 2
 */
enum PinConfig {
    i2c_sgt5000Data = 0,
    i2c_sgt5000Clk = 1,
    led_Training = 2,
#if FMSYNTH
    uart_MidiTX = 4,
    uart_MidiRX = 5,
#endif
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
    pot_JoystickZ = 28,
    button_zoomModeSwitch = 17

#ifdef EUCLIDEAN
    ,
    pulse0=20,
    pulse1=21,
    pulse2=12,
    pulse3=11,
    pulse4=3,
    pulse5=4,
    pulse6=5
#endif    
};


// Global objects
/** Global app state container (define only once). */
extern ts_app_state gAppState;
/** Global MEML interface (define only once). */
extern MEMLInterface meml_interface;

#endif  // __PICO_DEFS_HPP__

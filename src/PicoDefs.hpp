#ifndef __PICO_DEFS_HPP__
#define __PICO_DEFS_HPP__

#include "common/common_defs.h"
#include "interface/MEMLInterface.hpp"


// Select which example app to run
#define FM_SYNTH         1  ///< FM Synth (new macro)
#define FX_PROCESSOR     0  ///< FX Processor (new macro)


#define AUDIO_FUNC(x)    __not_in_flash_func(x)  ///< Macro to make audio function load from mem
#define AUDIO_MEM    __not_in_flash("audio")  ///< Macro to make variable load from mem


/**
 * @brief Pin configuration on the Pi Pico 2
 */
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
/** Global app state container (define only once). */
extern ts_app_state gAppState;
/** Global MEML interface (define only once). */
extern MEMLInterface meml_interface;

uint32_t pico_get_random_bits(int num_bits) {
    uint32_t random_value = 0;

    // Read RANDOMBIT num_bits times
    for (int i = 0; i < num_bits; i++) {
        // Shift the random value left by 1 and add the RANDOMBIT
        random_value = (random_value << 1) | (rosc_hw->randombit & 0x1);
    }

    return random_value;
}

#endif  // __PICO_DEFS_HPP__

#ifndef __BUTTONS_POTS_HPP__
#define __BUTTONS_POTS_HPP__

#include "../PicoDefs.hpp"
#include "../common/common_defs.h"

#include <cstdint>
#include <cstring>

class ButtonsPots {

 public:

    static void Setup(bool button_isr = false);
    static void Process(void);

    static constexpr enum PinConfig Buttons[] {
        toggle_SaveData,
        button_Randomise,
        toggle_Training,
        button_ClearData,
    };
    static constexpr size_t kNButtons = sizeof(Buttons)/sizeof(PinConfig);
    static constexpr enum PinConfig Pots[] {
        pot_JoystickX,
        pot_JoystickY,
        pot_JoystickZ
    };
    static constexpr size_t kNPots = sizeof(Pots)/sizeof(PinConfig);

 protected:

    static void ButtonISR(void);
    static void ProcessButton_(const PinConfig pin_n,
                               const size_t n,
                               bool button_current_state);
};


#endif  // __BUTTONS_POTS_HPP__

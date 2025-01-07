#ifndef __MEML_INTERFACE_HPP__
#define __MEML_INTERFACE_HPP__


#include "../common/common_defs.h"
#include <cstdint>
#include <vector>
#include "pico/util/queue.h"

class MEMLInterface {
 public:

    using GenParamsFn_ptr_t = void (*)(std::vector<float>&);

    MEMLInterface(queue_t interface_fmsynth,
                  queue_t interface_midi,
                  queue_t interface_pulse,
                  GenParamsFn_ptr_t gen_params_fn_ptr,
                  size_t nn_output_size);
    void SetPot(te_joystick_pot pot_n, num_t value);
    void SetToggleButton(te_button_idx button_n, int8_t state);
    void SetSlider(te_slider_idx idx, num_t value);
    void SendMIDI(ts_midi_note midi_note);
    void EnableMIDI(bool midi_on = true);
    void EnablePulse(bool pulse_on = true);
    void SetPulse(int32_t pulse);

 protected:

    // States
    bool joystick_inference_;
    union {
        ts_joystick_read as_struct;
        num_t as_array[sizeof(ts_joystick_read)/sizeof(num_t)];
    } joystick_current_;
    std::vector<float> current_fmsynth_params_;
    // Channels for outside comms
    queue_t interface_fmsynth_;
    queue_t interface_midi_;
    queue_t interface_pulse_;

    GenParamsFn_ptr_t gen_params_fn_ptr_;

    const size_t nn_output_size_;
    num_t draw_speed_;
    bool midi_on_;
    bool pulse_on_;
};


#endif  // __MEML_INTERFACE_HPP__

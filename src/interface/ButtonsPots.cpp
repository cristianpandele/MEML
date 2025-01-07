#include "ButtonsPots.hpp"
#include "../utils/MedianFilter.h"

#include <Arduino.h>


static bool button_current_[ButtonsPots::kNButtons];
static bool button_state_[ButtonsPots::kNButtons];
static float pot_current_[ButtonsPots::kNPots];
static float pot_state_[ButtonsPots::kNPots];

MedianFilter<float> adc_filters[ButtonsPots::kNPots];
MedianFilter<bool> button_filters[ButtonsPots::kNButtons];


void ButtonsPots::Setup(bool button_isr)
{
    // Set GPIO buttons as read
    for (auto &gpio_idx : Buttons) {
        pinMode(gpio_idx, INPUT);
        // Set up ISR
        if (button_isr) {
            attachInterrupt(digitalPinToInterrupt(gpio_idx), ButtonISR, CHANGE);
        }
    }
    // Set analogue as ADC
    for (auto &gpio_idx : Pots) {
        pinMode(gpio_idx, INPUT);
    }

    // Reset states
    for (unsigned int n = 0; n < kNButtons; n++) {
        button_state_[n] = true;
    }
    for (unsigned int n = 0; n < kNPots; n++) {
        pot_state_[n] = 0;
    }
}

void ButtonsPots::Process(void)
{
    for (unsigned int n = 0; n < kNPots; n++) {
        pot_current_[n] = adc_filters[n].process(analogRead(Pots[n]));
        if (std::abs(pot_current_[n] - pot_state_[n]) > 20) {
            pot_state_[n] = pot_current_[n];
            // Pot has changed
            Serial.print("Pot ");
            Serial.print(n);
            Serial.print(" = ");
            Serial.println(pot_current_[n]);
            // Action
        }
    }

    for (unsigned int n = 0; n < kNButtons; n++) {
        button_current_[n] = button_filters[n].process(digitalRead(Buttons[n]));
        ProcessButton_(Buttons[n], n, button_current_[n]);
    }
}


void ButtonsPots::ProcessButton_(PinConfig pin_n,
                                 size_t n,
                                 bool button_current_state) {

    bool button_previous_state = button_state_[n];

    // Action
    if (button_current_state != button_previous_state) {
        button_state_[n] = button_current_state;

        Serial.print("GPIO ");
        Serial.print(pin_n);
        Serial.print(" state ");
        Serial.println(button_state_[n]);
        // Trigger action
        switch(pin_n) {
            // Buttons
            case button_Randomise:
            case button_ClearData:
            {
                if (!button_current_state) {
                    // Trigger button action
                    Serial.println("Press!");
                }
            } break;
            // Toggles
            case toggle_SaveData:
            case toggle_Training:
            {
                // Trigger toggle action
                Serial.print("Toggle ");
                Serial.println(button_state_[n]);
            } break;
            default: {}  // Unrecognised pin
        }
    }
}


void ButtonsPots::ButtonISR(void)
{
    for (unsigned int n = 0; n < kNButtons; n++) {

        // Debounce
        static unsigned long last_interrupt_time = 0;
        unsigned long interrupt_time = millis();
        if (interrupt_time > last_interrupt_time
                && interrupt_time - last_interrupt_time < 10) {
            last_interrupt_time = interrupt_time;
            return;  // Ignore bounce
        }
        last_interrupt_time = interrupt_time;

        PinConfig pin_n = Buttons[n];
        bool button_current_state = digitalRead(pin_n);
        
        ProcessButton_(pin_n, n, button_current_state);
    }
}
#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__

#if defined(__cplusplus) || defined(__XC__)
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/** (Deprecated) internal definition of number (to change precision) */
typedef float num_t;

/** Struct to exchange 3D joystick data */
typedef struct {
    float potX;
    float potY;
    float potRotate;
} ts_joystick_read;

/** Struct to exchange simplified MIDI data */
typedef struct {
    size_t note_number;
    float velocity;
} ts_midi_note;

/**
 * UI elements
 */

/** Index of pots in a 3D joystick */
typedef enum {
    joystick_potX,
    joystick_potY,
    joystick_potRotate,
    joystick_nPots
} te_joystick_pot;

/** Button indexes for the MEML interface (legacy) */
typedef enum {
    toggle_training,   // 0
    button_randomise,  // 1
    toggle_savedata,   // 2
    button_cleardata,  // 3
    button_clearmodel, // 4
    toggle_discretise, // 5
    toggle_complex,    // 6
    toggle_explmode,   // 7
    toggle_dataset,    // 8
    toggle_model,      // 9
    button_nButtons,
    button_zoommodeswitch
} te_button_idx;

/** Slider indexes for the MEML interface (legacy) */
typedef enum {
    slider_randomSpeed,
    slider_nIterations,
    slider_nSliders
} te_slider_idx;

/**
 * Behavioural elements/flags
 */

/** Neural network mode (training or inference) */
typedef enum {
    mode_inference,
    mode_training,
    mode_nModes
} te_nn_mode;


/** Exploration/zoom mode */
typedef enum {
    expl_mode_nnweights,  ///< Randomise the NN weights with a bit of Gaussian noise
    expl_mode_pretrain,  ///< Pretrain the network around a single point
    expl_mode_zoom,  ///< Restrict exploration movement only (zoom)
    expl_nModes
} te_expl_mode;

/** Info for web UI (legacy) */
typedef enum {
    ui_last_error,
    ui_nElements,
} te_ui_info;

/** MEML app IDs (legacy) */
typedef enum {
    app_id_unknown,  ///< Unknown/new app
    app_id_fmsynth,  ///< FM synth
    app_id_euclidean,  ///< Euclidean sequencer
    app_id_multifx,  ///< Multi-FX matrix processor
    app_id_machinelisten,  ///< Machine-listening FX processor
    app_nIDs
} te_app_id;

/** MIDI command type */
typedef enum {
    midi_noteon,
    midi_noteoff,
    midi_nCommands
} te_midi_command;

#define kMaxDatasets    4
#define kMaxModels      4

/**
 * State structure
 */


/**
 * Global app state type. Define only once
 * and use everywhere (search for gAppState).
 */
typedef struct {

    // 32-bit parameters
    uint32_t n_iterations;  ///< Maximum number of iterations in training
    float last_error;  ///< Last loss/error value when trained
    float exploration_range;  ///< Exploration/zoom range when training (0-1)

    // 8-bit parameters
    te_app_id app_id;  ///< App ID when needed
    uint8_t current_dataset;  ///< Current dataset index used (0-3 usually)
    uint8_t current_model;  ///< Current model index used (0-3 usually)
    te_nn_mode current_nn_mode;  ///< Current interaction mode (training/inference)
    te_expl_mode current_expl_mode;  ///< Current exploration mode (when zooming in/out)

} ts_app_state;


#if defined(__cplusplus) || defined(__XC__)
}
#endif

#endif  // __COMMON_DEFS_H__

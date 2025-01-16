#ifndef _COLLAB_HPP
#define _COLLAB_HPP
#include <Arduino.h>

#include <cstdint>

#include "maximilian.h"

#include <vector>
#include <array>
#include <cmath>

#include "OnePoleSmoother.hpp"

#include "../common/common_defs.h"

const size_t kN_synthparams = 5;
using synthparams_array = std::array<float, kN_synthparams>;
static constexpr size_t kN_notes = 4;

#endif  // _COLLAB_HPP
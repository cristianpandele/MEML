#ifndef __PICO_DEFS_HPP__
#define __PICO_DEFS_HPP__


#define AUDIO_FUNC(x)    __not_in_flash_func(x)
#define AUDIO_MEM    __not_in_flash("audio")


#endif  // __PICO_DEFS_HPP__

#ifndef CT_MEDIA_WINDOW_HPP
#define CT_MEDIA_WINDOW_HPP

#include <cstdint>
#include <SDL2/SDL.h>
#include "io/trace.hpp"

namespace ct {
  struct window_t {
  public:
    SDL_Window *p_sdl_win {};
    const char *p_title {};
    int32_t w {};
    int32_t h {};
    uint64_t *p_cpu_interval {};
  };

  typedef ct::window_t window_create_info;
}

namespace ct {
  ct::result media_create_window(
    ct::window_create_info *p_window_create_info,
    ct::window_t *p_window
  );

  void media_refresh_window(
    ct::window_t *p_window
  );
}

#endif
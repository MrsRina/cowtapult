#ifndef CT_WORLD_MANAGER_HPP
#define CT_WORLD_MANAGER_HPP

#include <SDL2/SDL.h>

namespace ct {
  class world_manager {
  public:
    void on_poll_event();
    void on_update();
    void on_render();
  };
}

#endif
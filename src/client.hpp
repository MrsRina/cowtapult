#ifndef CT_CLIENT_HPP
#define CT_CLIENT_HPP

#include <bicudo/bicudo.hpp>
#include "media/window.hpp"
#include "gpu/handler.hpp"
#include "world/world_manager.hpp"

namespace ct {
  extern struct application_t {
  public:
    bicudo::runtime bicudo_runtime {};
    ct::window_t window {};
    ct::world_manager world_manager {};
    ct::gpu_handler gpu_handler {};
    SDL_Event sdl_event {};
    bool running {};
    uint64_t fps {};
    uint64_t prev_fps_config {};
  } *p_app;

  void init();
  void quit();
  int32_t run();
}

#endif
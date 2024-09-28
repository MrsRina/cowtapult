#ifndef CT_GPU_HANDLER_HPP
#define CT_GPU_HANDLER_HPP

#include "opengl.hpp"
#include "io/memory.hpp"
#include "immediate.hpp"

namespace ct {
  class gpu_handler {
  public:
    static ct::flags media_window_flags;
  public:
    SDL_GLContext sdl_gl_context {};
    ct::immediate immediate {};
    bicudo::vec3 bg_color {};
  public:
    void init_sdl_attributes();
    void init_gl_context();
    void update_viewport();
  };
}

#endif
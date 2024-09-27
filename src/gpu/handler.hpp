#ifndef CT_GPU_HANDLER_HPP
#define CT_GPU_HANDLER_HPP

#include "opengl.hpp"
#include "io/memory.hpp"

namespace ct {
  class gpu_handler {
  public:
    static ct::flags media_window_flags;
  public:
    void init_sdl_attributes();
    void init_gl_context();
  };
}

#endif
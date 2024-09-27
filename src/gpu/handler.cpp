#include "handler.hpp"
#include "client.hpp"
#include <bicudo/util/math.hpp>

ct::flags ct::gpu_handler::media_window_flags {
  SDL_WINDOW_OPENGL
};

void ct::gpu_handler::init_sdl_attributes() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
}

void ct::gpu_handler::init_gl_context() {
  ct::window_t *p_window {&ct::p_app->window};
  p_window->sdl_gl_context = (
    SDL_GL_CreateContext(p_window->p_sdl_win)
  );

  glewInit();
}
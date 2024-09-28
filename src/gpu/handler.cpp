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
  bicudo::log() << "Initializing GL context, immediate graphics!";

  this->sdl_gl_context = (
    SDL_GL_CreateContext(ct::p_app->window.p_sdl_win)
  );

  glewInit();
  this->immediate.create();

  SDL_GL_SetSwapInterval(true); // vsync by default true
}

void ct::gpu_handler::update_viewport() {
  float w {static_cast<float>(ct::p_app->window.w)};
  float h {static_cast<float>(ct::p_app->window.h)};

  this->immediate.mat4x4_projection = bicudo::ortho(
    0.0f,
    w,
    h,
    0.0f
  );

  ct::p_app->camera.look_at -= ct::p_app->camera.placement.pos;

  bicudo::vec2 delta {
    (ct::p_app->camera.look_at / ct::p_app->camera.previous_zoom)
    +
    (ct::p_app->camera.placement.pos)
  };

  ct::p_app->camera.previous_zoom = ct::p_app->camera.zoom;
  ct::p_app->camera.placement.pos = (
    delta - (ct::p_app->camera.look_at / ct::p_app->camera.zoom)
  );

  ct::p_app->camera.look_at += ct::p_app->camera.placement.pos;

  this->immediate.mat4x4_projection = bicudo::scale(
    this->immediate.mat4x4_projection,
    {ct::p_app->camera.zoom, ct::p_app->camera.zoom, 1.0f}
  );

  glViewport(0.0f, 0.0f, w, h);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(this->bg_color.x, this->bg_color.y, this->bg_color.z, 1.0f);
}
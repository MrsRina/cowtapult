#include <cstdint>
#include <iostream>
#include "client.hpp"

ct::application_t *ct::p_app {};

int32_t main(int32_t, char**) {
  ct::application_t app {};
  ct::p_app = &app;

  ct::init();

  int32_t run {
    ct::run()
  };

  ct::quit();
  return run;
}

void ct::init() {
  ct::window_create_info win_create_info {
    .p_title = "🐄 Cowtapult 👽",
    .w = 800,
    .h = 600
  };

  ct::p_app->gpu_handler.init_sdl_attributes();
  ct::media_create_window(
    &win_create_info,
    &ct::p_app->window
  );

  ct::p_app->window.p_cpu_interval = &bicudo::cpu_interval_ms;
  ct::p_app->gpu_handler.init_gl_context();

  bicudo::init(
    &ct::p_app->bicudo_runtime
  );
}

void ct::quit() {
  bicudo::quit(
    &ct::p_app->bicudo_runtime
  );
}

int32_t ct::run() {
  ct::p_app->running  = true;
  while (ct::p_app->running) {
    while (SDL_PollEvent(&ct::p_app->sdl_event)) {
      if (ct::p_app->sdl_event.type == SDL_QUIT) {
        ct::p_app->running = false;
      }

      ct::p_app->world_manager.on_poll_event();
    }

    ct::p_app->world_manager.on_update();

    if (ct::p_app->prev_fps_config != ct::p_app->fps) {
      ct::p_app->prev_fps_config = ct::p_app->fps;
      bicudo::set_framerate(ct::p_app->fps);
    }

    ct::p_app->world_manager.on_render();

    ct::media_refresh_window(
      &ct::p_app->window
    );
  }

  return 0;
}
#include <cstdint>
#include <iostream>
#include <chrono>
#include "client.hpp"

#include <ekg/os/ekg_sdl.hpp>
#include <ekg/os/ekg_opengl.hpp>

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

  ct::p_app->fps = 144;
  ct::p_app->window.p_cpu_interval = &bicudo::cpu_interval_ms;
  ct::p_app->gpu_handler.init_gl_context();

  ct::p_app->bicudo_runtime = {
    .physics_runtime_type = bicudo::physics_runtime_type::CPU_SIDE
  };

  bicudo::init(
    &ct::p_app->bicudo_runtime
  );

  ekg::runtime_property ekg_runtime_property {
    .p_font_path = "./ComicMono.ttf",
    .p_font_path_emoji = "./twemoji.ttf",
    .p_gpu_api = new ekg::os::opengl(),
    .p_os_platform = new ekg::os::sdl(ct::p_app->window.p_sdl_win)
  };

  ekg::init(
    &ct::p_app->gui_manager.ekg_runtime,
    &ekg_runtime_property
  );

  ct::p_app->texture_manager.on_pre_init();
  ct::p_app->gui_manager.init();
  ct::p_app->texture_manager.on_init();
  ct::p_app->world_manager.on_load();
}

void ct::quit() {
  bicudo::quit(
    &ct::p_app->bicudo_runtime
  );

  ct::p_app->gui_manager.quit();
}

int32_t ct::run() {
  ct::p_app->running  = true;
  ekg::timing framerate_time_stamp {};
  bicudo::current_framerate = 60;

  double previous_ticks {static_cast<double>(SDL_GetTicks64() / 1000)};
  double current_ticks {};
  double frame_time {};
  double accumulator {};

  while (ct::p_app->running) {
    while (SDL_PollEvent(&ct::p_app->sdl_event)) {
      if (
          (ct::p_app->sdl_event.type == SDL_WINDOWEVENT)
          &&
          (ct::p_app->sdl_event.window.event == SDL_WINDOWEVENT_RESIZED)
      ) {
        ct::p_app->window.w = ct::p_app->sdl_event.window.data1;
        ct::p_app->window.h = ct::p_app->sdl_event.window.data2;

        ekg::ui::width = ct::p_app->window.w;
        ekg::ui::height = ct::p_app->window.h;
      }

      ct::p_app->gui_manager.on_poll_event();
      ekg::os::sdl_poll_event(ct::p_app->sdl_event);

      if (ct::p_app->sdl_event.type == SDL_QUIT) {
        ct::p_app->running = false;
      }

      ct::p_app->world_manager.on_poll_event();
    }

    /*
    current_ticks = static_cast<double>(SDL_GetTicks64()) / 1000;
    frame_time = current_ticks - previous_ticks;
    previous_ticks = current_ticks;

    accumulator += frame_time;
    while (accumulator >= bicudo::dt) {
      accumulator -= bicudo::dt;
    }*/

    ct::p_app->world_manager.on_update();
    ct::p_app->gui_manager.on_update();
    ekg::update();

    if (ct::p_app->prev_fps_config != ct::p_app->fps) {
      ct::p_app->prev_fps_config = ct::p_app->fps;
      bicudo::set_framerate(ct::p_app->fps);
    }

    if (ekg::reach(framerate_time_stamp, 1000) && ekg::reset(framerate_time_stamp)) {
      ekg::ui::dt = 1.0f / bicudo::current_framerate;
      bicudo::dt = ekg::ui::dt;
      bicudo::framerate = bicudo::current_framerate;
      bicudo::current_framerate = 0;
      ekg::ui::redraw = true;
    }

    ct::p_app->gpu_handler.update_viewport();
    ct::p_app->world_manager.on_render();
    ekg::render();

    ekg::log::flush();
    bicudo::log::flush();
    bicudo::current_framerate++;

    ct::media_refresh_window(
      &ct::p_app->window
    );
  }

  return 0;
}
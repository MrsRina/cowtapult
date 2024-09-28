#include "window.hpp"
#include "gpu/handler.hpp"

ct::result ct::media_create_window(
  ct::window_create_info *p_window_create_info,
  ct::window_t *p_window
) {
  p_window->p_sdl_win = SDL_CreateWindow(
    p_window_create_info->p_title,
    10,
    10,
    p_window_create_info->w,
    p_window_create_info->h,
    (
      ct::gpu_handler::media_window_flags
      |
      SDL_WINDOW_RESIZABLE
    )
  );

  SDL_SetWindowSize(
    p_window->p_sdl_win,
    --p_window_create_info->w,
    --p_window_create_info->h
  );

  SDL_SetWindowSize(
    p_window->p_sdl_win,
    ++p_window_create_info->w,
    ++p_window_create_info->h
  );

  p_window->w = p_window_create_info->w;
  p_window->h = p_window_create_info->h;

  return ct::results::SUCCESS;
}

void ct::media_refresh_window(
  ct::window_t *p_window
) {
  SDL_GL_SwapWindow(p_window->p_sdl_win);
  SDL_Delay(*p_window->p_cpu_interval);
}
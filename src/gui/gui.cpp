#include "gui.hpp"

#include <ekg/ekg.hpp>
#include "client.hpp"

/**
 * GUI loading
 **/

float ct::gui_loading::progress {};

void ct::gui_loading::on_open() {
  this->stack.tag = "loading-gui";
  this->stack.push();

  ekg::frame(
    "gui-frame",
    {
      .x = 0.0f,
      .y = 0.0f,
      .w = static_cast<float>(ct::p_app->window.w),
      .h = static_cast<float>(ct::p_app->window.h)
    }
  );

  ekg::label("Cowtapult v0.1.0!!", ekg::dock::fill)
    ->set_text_align(ekg::dock::center)  
    ->set_font_size(ekg::font::big)
    ->set_scaled_height(32);

  ekg::label("", ekg::dock::next | ekg::dock::fill)->set_scaled_height(12);
  ekg::label("", ekg::dock::next | ekg::dock::fill);

  ekg::frame("image", {.w = 256, .h = 256}, ekg::dock::none)
    ->set_layer(
      &ct::p_app->texture_manager.sampler(ct::p_app->texture_manager.get_sampler_id_by_tag("cow-loading")),
      ekg::layer::background
  );

  ekg::pop_group_parent();
  ekg::label("", ekg::dock::fill);

  ekg::slider<float>("gui-bar", ekg::dock::next | ekg::dock::fill)
    ->range<float>(0, 0.1f, 0.0f, 1.0f, 0)
    ->range<float>(0).f32.transfer_ownership(&ct::gui_loading::progress)
    ->set_text_align(ekg::dock::center);

  ekg::pop_group();
  this->stack.pop();
}

void ct::gui_loading::on_screen_change_size() {
  this->stack.find<ekg::ui::frame>("gui-frame")
    ->set_size(
      static_cast<float>(ct::p_app->window.w),
      static_cast<float>(ct::p_app->window.h)
    );
}

void ct::gui_loading::on_close() {
  this->stack.destroy();
}

/**
 * GUI main menu
 **/

void ct::gui_main_menu::on_open() {
  this->stack.tag = "main-menu-gui";
  this->stack.push();

  float w_div_2 {static_cast<float>(ct::p_app->window.w / 2)};
  float h_div_2 {static_cast<float>(ct::p_app->window.h / 2)};

  ekg::frame(
    "gui-frame",
    {
      .x = w_div_2 - (w_div_2 / 2.0f),
      .y = h_div_2 - (h_div_2 / 2.0f),
      .w = w_div_2,
      .h = h_div_2
    }
  );

  ekg::label("www", ekg::dock::fill | ekg::dock::next)->set_scaled_height(12);
  ekg::button("oi eu amo viver meow", ekg::dock::fill | ekg::dock::next)->set_text_align(ekg::dock::center);
  ekg::button("oi eu amo a maya", ekg::dock::fill | ekg::dock::next)->set_text_align(ekg::dock::center);
  ekg::button("potato vou te cozinhar", ekg::dock::fill | ekg::dock::next)->set_text_align(ekg::dock::center);
  ekg::button("booo", ekg::dock::fill | ekg::dock::next)->set_text_align(ekg::dock::center);

  ekg::pop_group();
  this->stack.pop();
}

void ct::gui_main_menu::on_screen_change_size() {
  float w_div_2 {static_cast<float>(ct::p_app->window.w / 2)};
  float h_div_2 {static_cast<float>(ct::p_app->window.h / 2)};

  this->stack.find<ekg::ui::frame>("gui-frame")
    ->set_size(w_div_2, h_div_2)
    ->set_pos(w_div_2 - (w_div_2 / 2.0f), h_div_2 - (h_div_2 / 2.0f));
}

void ct::gui_main_menu::on_close() {
  this->stack.destroy();
}
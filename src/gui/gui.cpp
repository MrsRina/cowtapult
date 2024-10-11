#include "gui.hpp"

#include <ekg/ekg.hpp>
#include "client.hpp"

void ct::gui_loading::on_open() {
  this->stack.tag = "gui-loading";
  this->stack.push();

  ekg::frame(
    "gui-loading-frame",
    {0, 0},
    {
      static_cast<float>(ekg::ui::width),
      static_cast<float>(ekg::ui::height)
    }
  );

  ekg::label("Cowtapult v0.1.0!!", ekg::dock::fill)
    ->set_text_align(ekg::dock::center)  
    ->set_font_size(ekg::font::big)
    ->set_scaled_height(32);

  ekg::label("", ekg::dock::next | ekg::dock::fill)->set_scaled_height(12);
  ekg::label("www", ekg::dock::next | ekg::dock::fill);
  ekg::label("wwww", ekg::dock::none);
  ekg::frame("image", {256, 256})
    ->set_layer(
      &ct::p_app->texture_manager.sampler(ct::p_app->texture_manager.get_sampler_id_by_tag("cow-loading")),
      ekg::layer::background
  );

  ekg::pop_group();

  this->stack.pop();
}

void ct::gui_loading::on_screen_change_size() {
  this->stack.find<ekg::ui::frame>("gui-loading-frame")
    ->set_size(
      static_cast<float>(ekg::ui::width),
      static_cast<float>(ekg::ui::height)
    );
}

void ct::gui_loading::on_close() {
  this->stack.destroy();
}
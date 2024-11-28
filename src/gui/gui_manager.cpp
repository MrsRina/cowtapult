#include "gui_manager.hpp"
#include "client.hpp"

void ct::gui_manager::close(std::string_view tag) {
  ct::gui *&p_gui {
    this->gui_map[tag]
  };

  if (p_gui == nullptr) {
    return;
  }

  p_gui->on_close();
  delete p_gui;
  p_gui = nullptr;

  this->current_open_gui = "null";
}

void ct::gui_manager::init() {
  ekg::input::bind("drag-object", "mouse-1");
  ekg::input::bind("drag-slingshot-object", "lshift+mouse-1");
  ekg::input::bind("drop-object", "mouse-up");
  ekg::input::bind("click-on-camera", "mouse-2");
  ekg::input::bind("drop-camera", "mouse-2-up");
  ekg::input::bind("zoom-camera", "mouse-wheel");
  ekg::input::bind("target-entity", "space-up");

  /*
  ekg::frame("oiii muuu", {20, static_cast<float>(ct::p_app->window.h) - 700}, {400, 400})
    ->set_resize(ekg::dock::left | ekg::dock::bottom | ekg::dock::right | ekg::dock::top)
    ->set_drag(ekg::dock::full);

  ekg::label("BG:", ekg::dock::next);
  ekg::slider<float>("bg-clear-color-ownership", ekg::dock::fill)
    ->set_bar_offset(4.0f)
    ->range<float>(0, 0.0f, 0.0f, 1.0f, 2)
    ->range<float>(0).f32.transfer_ownership(&ct::p_app->gpu_handler.bg_color.x)
    ->range<float>(1, 0.0f, 0.0f, 1.0f, 2)
    ->range<float>(1).f32.transfer_ownership(&ct::p_app->gpu_handler.bg_color.y)
    ->range<float>(2, 0.0f, 0.0f, 1.0f, 2)
    ->range<float>(2).f32.transfer_ownership(&ct::p_app->gpu_handler.bg_color.z)
    ->set_text_align(ekg::dock::center | ekg::dock::left);

  ekg::label("DT, Zoom:", ekg::dock::next);
  ekg::slider<float>("dt-ownership", ekg::dock::fill)
    ->set_bar_offset(4.0f)
    ->range<float>(1, 0.0f, 0.0f, 1.0f, 5)
    ->range<float>(1).f32.transfer_ownership(&bicudo::dt)
    ->range<float>(0, 0.0f, 0.0f, 10.0f, 5)
    ->range<float>(0).f32.transfer_ownership(&ct::p_app->camera.interpolated_zoom)
    ->set_text_align(ekg::dock::center | ekg::dock::left);

  ekg::label("FPS:", ekg::dock::next);
  ekg::slider<uint64_t>("framerate-ownership", ekg::dock::fill)
    ->range<uint64_t>(0, 0, 0, 1000)
    ->range<uint64_t>(0).u64.transfer_ownership(&bicudo::framerate)
    ->set_text_align(ekg::dock::center | ekg::dock::left);

  ekg::label("Gravity:", ekg::dock::next);
  ekg::slider<float>("gravity-ownership", ekg::dock::fill)
    ->range<float>(0, 9.0f, 0.0f, 20.0f)
    ->range<float>(0).f32.transfer_ownership(&ct::p_app->bicudo_runtime.gravity.y)
    ->set_text_align(ekg::dock::center | ekg::dock::left);

  ekg::label("Fract:", ekg::dock::next);
  ekg::slider<int32_t>("fract", ekg::dock::fill)
    ->range<int32_t>(0, 4, 0, 16)
    ->range<int32_t>(0).i32.transfer_ownership(&ct::p_app->world_manager.fract_amount)
    ->set_text_align(ekg::dock::center | ekg::dock::left);

  ekg::ui::label *p_position {ekg::label("", ekg::dock::next | ekg::dock::fill)};

  ekg::scrollbar("scrollbar-meow");
  ekg::pop_group();

  */

  this->open<ct::gui_loading>("loading-gui");
  ct::gui_loading::progress  = 0.0f;

  this->open<ct::gui_main_menu>("main-menu-gui");
}

void ct::gui_manager::on_poll_event() {
  ct::gui *p_gui {
    nullptr
  };

  if (
      (ct::p_app->sdl_event.type == SDL_WINDOWEVENT)
      &&
      (ct::p_app->sdl_event.window.event == SDL_WINDOWEVENT_RESIZED)
      &&
      this->current_open_gui != "null"
      &&
      (p_gui = this->gui_map[this->current_open_gui]) != nullptr
  ) {
    p_gui->on_screen_change_size();
  }

  ct::tools_pick_camera(
    &this->camera_pickup_info
  );

  ct::tools_pick_object_from_world(
    &this->object_pickup_info
  );

  if (!ct::p_app->world_manager.loaded_entity_list.empty() && ekg::input::action("target-entity")) {
    uint64_t size {
      ct::p_app->world_manager.loaded_entity_list.size()
    };

    uint64_t it {this->entity_target_sequence};
    uint64_t initial_it {this->entity_target_sequence};

    for (it = this->entity_target_sequence; it < size; it++) {
      ct::entity_base *&p_entity {
        ct::p_app->world_manager.loaded_entity_list.at(it)
      };

      if (p_entity->type == ct::entity_type::PLAYER) {
        this->entity_target_sequence = it;
        ct::p_app->camera.set_target(&p_entity->placement);
        this->entity_target_sequence++;

        break;
      }
    }

    if (this->entity_target_sequence == initial_it) {
      ct::p_app->camera.set_target(nullptr);
      this->entity_target_sequence = 0;
      return;
    }
  }
}

void ct::gui_manager::on_update() {
  ct::tools_update_picked_camera(
    &this->camera_pickup_info
  );

  ct::tools_update_picked_object(
    &this->object_pickup_info
  ); 
}

void ct::gui_manager::quit() {

}
#include "gui_manager.hpp"
#include "client.hpp"

void ct::gui_manager::init() {
  ekg::input::bind("click-on-object", "mouse-1");
  ekg::input::bind("drop-object", "mouse-1-up");
  ekg::input::bind("click-on-camera", "mouse-2");
  ekg::input::bind("drop-camera", "mouse-2-up");
  ekg::input::bind("zoom-camera", "mouse-wheel");
}

void ct::gui_manager::on_poll_event() {
  ct::tools_pick_camera(
    &this->camera_pickup_info
  );

  ct::tools_pick_object_from_world(
    &this->object_pickup_info
  );
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
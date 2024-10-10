#include "scene_manager.hpp"

void ct::scene_manager::open(ct::scene *p_scene) {
  if (
      (p_scene == nullptr && this->p_current_scene == nullptr)
      ||
      (this->p_current_scene == p_scene)
    ) {
    return;
  }

  if (this->p_current_scene) {
    this->p_current_scene = p_scene;
    this->p_current_scene->on_open();
  } else {
    this->p_current_scene->on_close();
    delete this->p_current_scene;
    this->p_current_scene = nullptr;

    if (p_scene != nullptr) {
      this->p_current_scene = p_scene;
      this->p_current_scene->on_open();
    }
  }
}

void ct::scene_manager::close() {
  this->open(nullptr);
}

void ct::scene_manager::on_update() {
  if (this->p_current_scene) {
    this->p_current_scene->on_update();
  }
}
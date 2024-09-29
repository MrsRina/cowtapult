#include "camera.hpp"
#include "client.hpp"

void ct::camera::set_target(bicudo::physics::placement *p_placement) {
  if (p_placement == nullptr || p_placement == &this->placement) {
    this->p_target_placement = nullptr;
    this->state = ct::camera_state::FREE;
    return;
  }

  this->p_target_placement = p_placement;
  this->state = ct::camera_state::TARGET;
  //this->placement.pos = (this->p_target_placement->pos + (this->p_target_placement->size / 2.0f)) - (this->placement.size / 2.0f);
}

void ct::camera::on_update() {
  this->placement.velocity += this->placement.acc * bicudo::dt;
  this->placement.pos += this->placement.velocity;
  this->placement.velocity = bicudo::lerp(this->placement.velocity, 0.0f, this->smooth + bicudo::dt);

  this->frustum.x = this->placement.pos.x;
  this->frustum.y = this->placement.pos.y;
  this->frustum.z = this->placement.size.x / this->zoom;
  this->frustum.w = this->placement.size.y / this->zoom;

  this->zoom = bicudo::lerp<float>(
    this->zoom,
    this->interpolated_zoom,
    0.2f
  );

  switch (this->state) {
  case ct::camera_state::FREE:
    this->look_at = (this->placement.pos + (this->placement.size / 2.0f));
    break;
  case ct::camera_state::TARGET:
    if (this->p_target_placement == nullptr) {
      return;
    }

    this->look_at = (this->placement.pos + (this->placement.size / 2.0f));
    this->placement.pos = (this->p_target_placement->pos + (((this->p_target_placement->size / 2.0f)))) - ((this->placement.size / 2.0f) / this->zoom);
    //this->placement.velocity = this->p_target_placement->velocity;

    break;
  }
}
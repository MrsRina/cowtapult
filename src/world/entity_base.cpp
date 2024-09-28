#include "entity_base.hpp"
#include "client.hpp"

void ct::entity_base::set_texuture(bicudo::id id) {
  if (id == CT_NO_TEXTURE) {
    this->texture_id = 0;
  }

  this->texture_id = ct::p_app->texture_manager.sampler(id).gl_id;
}

void ct::entity_base::on_update() {
  bicudo::update_position(
    &ct::p_app->bicudo_runtime,
    &this->placement
  );
}

void ct::entity_base::on_render() {
  ct::p_app->gpu_handler.immediate.draw(
    this->placement,
    {0.0f, 0.5f, 1.0f, 1.0f},
    this->texture_id
  );
}
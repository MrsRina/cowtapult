#include "entity_base.hpp"
#include "client.hpp"

void ct::entity_base::on_update() {
  bicudo::update_position(
    &ct::p_app->bicudo_runtime,
    &this->placement
  );
}

void ct::entity_base::on_render() {
  ct::p_app->gpu_handler.immediate.draw(
    this->placement,
    {0.0f, 0.5f, 1.0f, 1.0f}
  );
}
#include "entity_enemy.hpp"
#include "client.hpp"

void ct::entity_enemy::on_update() {
  ct::entity_base::on_update();
}

void ct::entity_enemy::on_render() {
  ct::p_app->gpu_handler.immediate.draw(
    this->placement,
    {1.0f, 0.5f, 1.0f, 1.0f},
    this->texture_id
  );
}
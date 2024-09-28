#include "world_manager.hpp"
#include "client.hpp"
#include <bicudo/bicudo.hpp>
#include "entity_player.hpp"

ct::entity_base *ct::world_manager::push_back_entity(
  ct::entity_base *p_entity_base
) {
  bicudo::insert(
    &ct::p_app->bicudo_runtime,
    &p_entity_base->placement
  );

  this->loaded_entity_list.push_back(p_entity_base);
  return p_entity_base;
}

void ct::world_manager::on_load() {
  ct::p_app->camera.set_target(nullptr);  

  ct::entity_player *p_cow {
    new ct::entity_player(
      {
        .p_tag = "vakinha",
        .mass = 2000.0f,
        .friction = 0.8f,
        .restitution = 0.2f,
        .pos = {20, 20},
        .size = {144, 144}
      }
    )
  };

  this->push_back_entity(p_cow);

  ct::entity_player *p_cow_2 {
    new ct::entity_player(
      {
        .p_tag = "gatinho",
        .mass = 20.0f,
        .friction = 0.8f,
        .restitution = 0.2f,
        .pos = {200, 20},
        .size = {400, 50}
      }
    )
  };

  this->push_back_entity(p_cow_2);

  ct::entity_base *p_terrain_bottom {
    new ct::entity_base(
      {
        .p_tag = "terrain-bottom",
        .mass = 0.0f,
        .friction = 0.8f,
        .restitution = 0.2f,
        .pos = {200, 800},
        .size = {1280, 50}
      }
    )
  };

  this->push_back_entity(p_terrain_bottom);

  ct::entity_base *p_terrain_top {
    new ct::entity_base(
      {
        .p_tag = "terrain-top",
        .mass = 0.0f,
        .friction = 0.8f,
        .restitution = 0.2f,
        .pos = {200, 200},
        .size = {1280, 50}
      }
    )
  };

  this->push_back_entity(p_terrain_top);

  ct::entity_base *p_terrain_left {
    new ct::entity_base(
      {
        .p_tag = "terrain-left",
        .mass = 0.0f,
        .friction = 0.8f,
        .restitution = 0.2f,
        .pos = {200, 200},
        .size = {50, 1280}
      }
    )
  };

  this->push_back_entity(p_terrain_left);

  ct::entity_base *p_terrain_right {
    new ct::entity_base(
      {
        .p_tag = "terrain-right",
        .mass = 0.0f,
        .friction = 0.8f,
        .restitution = 0.2f,
        .pos = {900, 200},
        .size = {50, 1280}
      }
    )
  };

  this->push_back_entity(p_terrain_right);
}

void ct::world_manager::on_poll_event() {
  if (
    (ct::p_app->sdl_event.type == SDL_WINDOWEVENT)
    &&
    (ct::p_app->sdl_event.window.event == SDL_WINDOWEVENT_RESIZED)
  ) {
    ct::p_app->camera.placement.size = {
      static_cast<float>(ct::p_app->window.w),
      static_cast<float>(ct::p_app->window.h)
    };
  }
}

void ct::world_manager::on_update() {
  for (ct::entity_base *&p_entity_base : this->loaded_entity_list) {
    p_entity_base->on_update();
  }

  ct::p_app->camera.on_update();
}

void ct::world_manager::on_render() {
  bicudo::vec4 rect {};
  ct::p_app->gpu_handler.immediate.invoke();

  for (ct::entity_base *&p_entity_base : this->loaded_entity_list) {
    rect.x = p_entity_base->placement.pos.x;
    rect.y = p_entity_base->placement.pos.y;
    rect.z = p_entity_base->placement.size.x;
    rect.w = p_entity_base->placement.size.y;

    if (!bicudo::vec4_collide_with_vec4(rect, ct::p_app->camera.frustum)) {
      continue;
    }

    p_entity_base->placement.pos -= ct::p_app->camera.placement.pos;
    p_entity_base->on_render();
    p_entity_base->placement.pos += ct::p_app->camera.placement.pos;
  }

  ct::p_app->gpu_handler.immediate.revoke();
}
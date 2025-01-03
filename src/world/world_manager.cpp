#include "world_manager.hpp"
#include "client.hpp"
#include "object.hpp"
#include <bicudo/bicudo.hpp>
#include "entity_player.hpp"

void ct::on_event_collision_pre_apply_forces(
  bicudo::physics::placement *&p_a,
  bicudo::physics::placement *&p_b
) {
  if (
      (bicudo::assert_float(p_a->mass, 0.0f))
      &&
      (bicudo::assert_float(p_b->mass, 0.0f))
    ) {
    return;
  }

  bicudo::physics::placement *p_fractured_placement {
    nullptr
  };

  float m1 {};
  float m2 {};
  float total_mass {p_a->mass + p_b->mass};

  bicudo::vec2 v1 {};
  bicudo::vec2 v2 {};
  float vdiff_sqrt {};

  float sv1 {};
  float sv2 {};

  float e1 {};
  float e2 {};
  float kinetic {};

  bicudo::vec2 c1 {};
  bicudo::vec2 c2 {};

  bicudo::vec2 start {ct::p_app->bicudo_runtime.collision_info.start * (p_b->mass / total_mass)};
  bicudo::vec2 end {ct::p_app->bicudo_runtime.collision_info.end * (p_a->mass / total_mass)};
  bicudo::vec2 p {start + end};

  c1.x = p_a->pos.x + (p_a->size.x / 2);
  c1.y = p_a->pos.y + (p_a->size.y / 2);
  c1 = p - c1;
    
  c2.x = p_b->pos.x + (p_b->size.x / 2);
  c2.y = p_b->pos.y + (p_b->size.y / 2);
  c2 = p - c2;

  v1 = (
    p_a->velocity + bicudo::vec2(-1.0f * p_a->angular_velocity * c1.y, p_a->angular_velocity * c1.x)
  );

  v2 = (
    p_b->velocity + bicudo::vec2(-1.0f * p_b->angular_velocity * c2.y, p_b->angular_velocity * c2.x)
  );

  m1 = p_a->mass;
  m2 = p_b->mass;

  bool which_check_to_destroy {};

  if (bicudo::assert_float(p_a->mass, 0.0f)) {
    which_check_to_destroy = 1;
  }

  if (bicudo::assert_float(p_b->mass, 0.0f)) {
    which_check_to_destroy = 0;
  }

  float f1 {
    (v1 * m1).magnitude()
  };

  float f2 {
    (v2 * m2).magnitude()
  };

  which_check_to_destroy = (
    which_check_to_destroy || (f1 > f2)
  );

  switch (which_check_to_destroy) {
  case 0:
    sv1 = v1.magnitude();
    sv2 = v2.magnitude();

    p_fractured_placement = p_a;
    break;
  case 1:
    m1 = p_b->mass;
    m2 = p_a->mass;

    // v2 > v1 then sv1 == v2
    sv1 = v2.magnitude();
    sv2 = v1.magnitude();

    p_fractured_placement = p_b;
    break;
  }

  e1 = m1 * sv1 * sv1;
  e2 = m2 * sv2 * sv2;

  vdiff_sqrt = (v2 - v1).magnitude();
  kinetic = e1 + e2;

  if (
      !(bicudo::assert_float(p_fractured_placement->mass, 0.0f))
      &&
      (!ekg_bitwise_contains(p_fractured_placement->flags, ct::collided_state::FRACTURED))
      &&
      (kinetic > p_fractured_placement->hardness)
    ) {
    // now it is able to sub-divide
    p_fractured_placement->flags = ct::collided_state::FRACTURED;
    ct::p_app->world_manager.in_world_event_poll.emplace(
      ct::event_t {
        .type = ct::event_type::FRACTURE,
        .p_data = p_fractured_placement
      }
    );
  }
}

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
  ct::p_app->bicudo_runtime.p_on_collision_pre_apply_forces = &ct::on_event_collision_pre_apply_forces;
  ct::p_app->camera.set_target(nullptr);
  ct::p_app->bicudo_runtime.gravity = {0.0f, 9.81f};
  ct::p_app->bicudo_runtime.solve_accurace = 0.4f;

  ct::entity_player *p_cow {
    new ct::entity_player(
      {
        .p_tag = "vakinha",
        .flags = ct::collided_state::FRACTURED,
        .mass = 5.0f,
        .friction = 0.6f,
        .restitution = 0.1f,
        .hardness = 0.0f,
        .pos = {200, 20},
        .size = {144, 144}
      }
    )
  };

  p_cow->pickup = ct::pickup_type::DRAG | ct::pickup_type::SLINGSHOT;
  this->push_back_entity(p_cow);

  ct::entity_base *p_bedrock {
    new ct::entity_base(
      {
        .p_tag = "bedrock",
        .mass = 0.0f,
        .friction = 0.8f,
        .restitution = 0.2f,
        .pos = {0, 0},
        .size = {CT_WORLD_LIMIT, CT_WORLD_LIMIT}
      }
    )
  };

  p_bedrock->placement.pos = (
    -(p_bedrock->placement.size / 2.0f)
  );

  p_bedrock->placement.pos.y = CT_WORLD_BEDROCK;
  this->push_back_entity(p_bedrock);
}

ct::entity_base *ct::world_manager::find_entity_by_id(
  bicudo::id id,
  uint64_t *p_index
) {
  for (uint64_t it {}; it < this->loaded_entity_list.size(); it++) {
    ct::entity_base *&p_entities {
      this->loaded_entity_list.at(it)
    };

    if (p_entities->placement.id == id) {
      if (p_index != nullptr) {
        *p_index = it;
      }

      return p_entities;
    }
  }

  return nullptr;
}

bool ct::world_manager::erase_entity(bicudo::id id) {
  for (uint64_t it {}; it < this->loaded_entity_list.size(); it++) {
    ct::entity_base *&p_entities {
      this->loaded_entity_list.at(it)
    };

    if (p_entities->placement.id == id) {
      CT_FREE_ENTITY(p_entities, it);
      return true;
    }
  }

  return false;
}

void ct::world_manager::process_world_events(
  ct::event_t *p_event
) {
  switch (p_event->type) {
    case ct::event_type::FRACTURE: {
      bicudo::physics::placement *p_fractured {
        (bicudo::physics::placement*) p_event->p_data
      };

      uint64_t index {};
      ct::entity_base *p_entity {
        this->find_entity_by_id(p_fractured->id, &index)
      };

      float mass_div_by_n {
        20.0f
      };

      bicudo::vec2 center {
        p_fractured->pos + (p_fractured->size / 2.0f) 
      };

      std::string tag {};
      tag += p_fractured->p_tag;
      tag += "-fracted";

      bicudo::physics::placement fract_placement {
        .p_tag = tag.c_str(),
        .flags = ct::collided_state::FRACTURED,
        .mass = mass_div_by_n,
        .friction = p_fractured->friction,
        .restitution = p_fractured->restitution,
        .hardness = p_fractured->hardness,
        .size = p_fractured->size / static_cast<float>(this->fract_amount),
        .velocity = p_fractured->velocity,
        .angle = 0.0f
      };

      int32_t row {};
      int32_t col {};

      for (int32_t it {}; it < (this->fract_amount * this->fract_amount); it++) {
        row = it / this->fract_amount;
        col = it % this->fract_amount;

        fract_placement.pos.x = p_fractured->pos.x + col * fract_placement.size.x;
        fract_placement.pos.y = p_fractured->pos.y + row * fract_placement.size.y;
        //fract_placement.pos = fract_placement.pos.rotate(fract_placement.angle, center);
        fract_placement.angular_acc = p_fractured->angular_acc;
        fract_placement.angular_velocity = p_fractured->angular_velocity;

        ct::entity_base *p_mini_fractured {
          new ct::entity_base(fract_placement)
        };

        p_mini_fractured->pickup = ct::pickup_type::DRAG;
        this->push_back_entity(p_mini_fractured);
      }

      CT_FREE_ENTITY(p_entity, index);
      break;
    }
  }
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
  bicudo::update_collisions(
    &ct::p_app->bicudo_runtime
  );

  while (!this->in_world_event_poll.empty()) {
    this->process_world_events(&this->in_world_event_poll.front());
    this->in_world_event_poll.pop();
  } 
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